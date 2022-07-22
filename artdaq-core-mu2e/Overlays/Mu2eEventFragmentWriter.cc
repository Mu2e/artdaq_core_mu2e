#include "artdaq-core-mu2e/Overlays/Mu2eEventFragmentWriter.hh"
#include "artdaq-core-mu2e/Overlays/FragmentType.hh"

mu2e::Mu2eEventFragmentWriter::Mu2eEventFragmentWriter(artdaq::Fragment& f, uint64_t eventWindowTag, uint8_t evbMode)
	: Mu2eEventFragment(f)
	, artdaq_Fragment_(f)
{
	artdaq_Fragment_.setUserType(mu2e::detail::FragmentType::MU2EEVENT);
	Metadata m;
	m.event_window_tag = eventWindowTag;
	m.evb_mode = evbMode;
	m.version = Mu2eEventFragment::CURRENT_VERSION;
	m.missing_data = false;
	m.has_index = true;
	m.tracker_block_count = 0;
	m.calorimeter_block_count = 0;
	m.index_offset = 0;
	artdaq_Fragment_.setMetadata<Metadata>(m);

	if (artdaq_Fragment_.size() !=
		artdaq::detail::RawFragmentHeader::num_words() +
			Metadata::size_bytes / sizeof(artdaq::RawDataType))
	{
		TLOG(TLVL_ERROR, "Mu2eEventFragmentWriter") << "Mu2eEventFragmentWriter: Raw artdaq::Fragment object size suggests it does not consist of its own header + the Mu2eEventFragment::Metadata object";
		TLOG(TLVL_ERROR, "Mu2eEventFragmentWriter") << "artdaq_Fragment size: " << artdaq_Fragment_.size() << ", Expected size: " << artdaq::detail::RawFragmentHeader::num_words() + Metadata::size_bytes / sizeof(artdaq::RawDataType);

		throw cet::exception("InvalidFragment") << "Mu2eEventFragmentWriter: Raw artdaq::Fragment object size suggests it does not consist of its own header + the Mu2eEventFragment::Metadata object";  // NOLINT(cert-err60-cpp)
	}

	artdaq_Fragment_.resize(1);
	*artdaq_Fragment_.dataBegin() = CONTAINER_MAGIC;
}

void mu2e::Mu2eEventFragmentWriter::addSpace_(size_t bytes)
{
	auto currSize = sizeof(artdaq::Fragment::value_type) * artdaq_Fragment_.dataSize();  // Resize takes into account header and metadata size
	artdaq_Fragment_.resizeBytesWithCushion(bytes + currSize, 1.3);
	reset_index_ptr_();  // Must reset index_ptr after resize operation!

	TLOG(TLVL_TRACE, "Mu2eEventFragmentWriter") << "dataEnd_ is now at " << static_cast<void*>(dataEnd_()) << " (oldSizeBytes/deltaBytes: " << currSize << "/" << bytes << ")";
}

void mu2e::Mu2eEventFragmentWriter::fill_event(std::vector<std::unique_ptr<DTCLib::DTC_Event>> const& data, artdaq::Fragment::timestamp_t fragment_timestamp)
{
	TLOG(TLVL_TRACE, "Mu2eEventFragmentWriter") << "Adding " << data.size() << " DTC_Events to Container";

	size_t total_size = 0;
	size_t trk_blocks = 0;
	size_t calo_blocks = 0;
	for (auto& d : data)
	{
		total_size += d->GetEventByteCount();
		trk_blocks += d->GetBlockCount(DTCLib::DTC_Subsystem_Tracker);
		calo_blocks += d->GetBlockCount(DTCLib::DTC_Subsystem_Calorimeter);
	}

	// Add space for headers and pad words
	auto extra_bytes = (trk_blocks + calo_blocks) * (sizeof(artdaq::detail::RawFragmentHeader) + sizeof(artdaq::RawDataType));
	auto index_size = sizeof(size_t) * (1 + trk_blocks + calo_blocks);
	auto fragment_payload_size = total_size + index_size + extra_bytes;

	TLOG(TLVL_TRACE, "Mu2eEventFragmentWriter") << "Total event size: " << total_size
												<< ", Tracker blocks: " << trk_blocks
												<< ", Calorimeter blocks: " << calo_blocks;
	TLOG(TLVL_TRACE, "Mu2eEventFragmentWriter") << "Payload Size is " << artdaq_Fragment_.dataSizeBytes()
												<< ", lastFragmentIndex is " << lastFragmentIndex()
												<< ", extra_bytes: " << extra_bytes
												<< ", index_size: " << index_size;

	if (artdaq_Fragment_.dataSizeBytes() < fragment_payload_size)
	{
		addSpace_(fragment_payload_size - artdaq_Fragment_.dataSizeBytes());
	}

	auto data_ptr = static_cast<artdaq::detail::RawFragmentHeader*>(dataEnd_());

	std::vector<DTCLib::DTC_Subsystem> subsystems{DTCLib::DTC_Subsystem_Tracker, DTCLib::DTC_Subsystem_Calorimeter};

	for (auto& subsystem : subsystems)
	{
		for (auto& evt : data)
		{
			auto subevts = evt->GetSubEvents();
			for (auto& subevt : subevts)
			{
				if (subevt.GetSubsystem() == subsystem)
				{
					for (size_t ii = 0; ii < subevt.GetDataBlockCount(); ++ii)
					{
						TLOG(TLVL_TRACE + 4) << "Getting first block header";
						auto blockStart = subevt.GetDataBlocks()[ii].blockPointer;
						auto headerPacket = subevt.GetDataBlocks()[ii].GetHeader();
						size_t byteCount = headerPacket->GetByteCount();

						TLOG(TLVL_TRACE + 4) << "Checking subsequent blocks to see if they are from the same ROC";
						while (ii < subevt.GetDataBlockCount() - 1)
						{
							try
							{
								auto newHeaderPacket = subevt.GetDataBlocks()[++ii].GetHeader();

								// Collapse multiple blocks from the same DTC/ROC into one Fragment
								if (newHeaderPacket->GetSubsystem() == subsystem && newHeaderPacket->GetID() == headerPacket->GetID() && newHeaderPacket->GetLinkID() == headerPacket->GetLinkID() && newHeaderPacket->GetHopCount() == headerPacket->GetHopCount())
								{
									TLOG(TLVL_TRACE + 4) << "Adding " << newHeaderPacket->GetByteCount() << " bytes to current block size ("
														 << byteCount << "), as this block is from the same ROC as previous";
									byteCount += newHeaderPacket->GetByteCount();
								}
								else
								{
									--ii;
									break;
								}
							}
							catch (...)
							{
								TLOG(TLVL_ERROR) << "There may be data corruption in the Fragment. Aborting search for same-ROC blocks";
								break;
							}
						}

						// Use byte count to calculate how many words the current data
						// block should occupy in the new fragment.
						auto const wordCount = byteCount / sizeof(artdaq::RawDataType);
						auto const fragmentSize = (byteCount % sizeof(artdaq::RawDataType) == 0) ? wordCount : wordCount + 1;

						TLOG(TLVL_TRACE + 3) << "Creating Fragment with size " << fragmentSize << " and adding to container";

						data_ptr->version = artdaq::detail::RawFragmentHeader::CurrentVersion;
						data_ptr->word_count = fragmentSize + artdaq::detail::RawFragmentHeader::num_words();
						data_ptr->sequence_id = fragment_timestamp;
						data_ptr->fragment_id = headerPacket->GetEVBMode();
						data_ptr->timestamp = fragment_timestamp;
						data_ptr->type = subsystem == DTCLib::DTC_Subsystem_Tracker ? mu2e::detail::FragmentType::TRK : mu2e::detail::FragmentType::CAL;
						data_ptr->touch();
						data_ptr->metadata_word_count = 0;

						memcpy(data_ptr + 1, blockStart, byteCount);

						data_ptr = reinterpret_cast<artdaq::detail::RawFragmentHeader*>(reinterpret_cast<uint8_t*>(data_ptr + 1) + fragmentSize * sizeof(artdaq::RawDataType));
						if (subsystem == DTCLib::DTC_Subsystem_Tracker)
						{
							metadata()->tracker_block_count++;
						}
						else
						{
							metadata()->calorimeter_block_count++;
						}
					}
				}
			}
		}
	}

	metadata()->has_index = 0;

	auto index = create_index_();
	metadata()->index_offset = index[block_count() - 1];  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
	TLOG(TLVL_TRACE + 3) << "Index offset is now " << metadata()->index_offset;
	memcpy(dataBegin_() + metadata()->index_offset, index, sizeof(size_t) * (block_count() + 1));  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)

	metadata()->has_index = 1;
	reset_index_ptr_();
}
