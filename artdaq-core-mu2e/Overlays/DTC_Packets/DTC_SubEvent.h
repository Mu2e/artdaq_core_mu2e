#ifndef artdaq_core_mu2e_Overlays_DTC_Packets_DTC_SubEvent_h
#define artdaq_core_mu2e_Overlays_DTC_Packets_DTC_SubEvent_h

#include "artdaq-core-mu2e/Overlays/DTC_Packets/DTC_DataBlock.h"
#include "artdaq-core-mu2e/Overlays/DTC_Packets/DTC_SubEventHeader.h"

#include "artdaq-core-mu2e/Overlays/DTC_Types/DTC_EventMode.h"
#include "artdaq-core-mu2e/Overlays/DTC_Types/DTC_EventWindowTag.h"
#include "artdaq-core-mu2e/Overlays/DTC_Types/DTC_Subsystem.h"

#include <cstdint>
#include <vector>
#include <array>

namespace DTCLib {

class DTC_SubEvent
{
public:
	static const uint8_t REQUIRED_SUBEVENT_FORMAT_VERSION;
	
	/// <summary>
	/// Construct a DTC_SubEvent using a pointer to data. Flag will be set that the packet is read-only.
	/// </summary>
	/// <param name="ptr">Pointer to data</param>
	explicit DTC_SubEvent(const void* data);
	explicit DTC_SubEvent(size_t data_size);

	DTC_SubEvent()
		: header_(), data_blocks_(), buffer_ptr_(nullptr) {}


	void SetupSubEvent();
	size_t GetSubEventByteCount() const { return header_.inclusive_subevent_byte_count; }

	DTC_EventWindowTag GetEventWindowTag() const;
	void SetEventWindowTag(DTC_EventWindowTag const& tag);
	void SetEventMode(DTC_EventMode const& mode);
	uint8_t GetDTCID() const;
	const void* GetRawBufferPointer() const { return buffer_ptr_; }

	std::vector<DTC_DataBlock> const& GetDataBlocks() const
	{
		return data_blocks_;
	}
	size_t GetDataBlockCount() const { return data_blocks_.size(); }
	const DTC_DataBlock* GetDataBlock(size_t idx) const
	{
		if (idx >= data_blocks_.size()) throw std::out_of_range("Index " + std::to_string(idx) + " is out of range (max: " + std::to_string(data_blocks_.size() - 1) + ")");
		return &data_blocks_[idx];
	}
	void AddDataBlock(DTC_DataBlock blk)
	{
		auto block_id = blk.GetHeader()->GetLinkID();
		auto insert_iter = data_blocks_.begin();
        while (insert_iter != data_blocks_.end()) {
			if (block_id < insert_iter->GetHeader()->GetLinkID()) break;
			++insert_iter;
        }
		data_blocks_.insert(insert_iter, blk);
		header_.num_rocs++;
		UpdateHeader();
	}

	DTC_Subsystem GetSubsystem(DTC_Link_ID link = DTC_Link_0) const {
		switch(link){
			case DTC_Link_0: return static_cast<DTC_Subsystem>(header_.link0_subsystem); break;
			case DTC_Link_1: return static_cast<DTC_Subsystem>(header_.link1_subsystem); break;
			case DTC_Link_2: return static_cast<DTC_Subsystem>(header_.link2_subsystem); break;
			case DTC_Link_3: return static_cast<DTC_Subsystem>(header_.link3_subsystem); break;
			case DTC_Link_4: return static_cast<DTC_Subsystem>(header_.link4_subsystem); break;
			case DTC_Link_5: return static_cast<DTC_Subsystem>(header_.link5_subsystem); break;
			default: return static_cast<DTC_Subsystem>(0);
		}
	}
	bool HasSubsystem(DTC_Subsystem subsys) const {
		if (static_cast<DTC_Subsystem>(header_.link0_subsystem) == subsys) return true;
		if (static_cast<DTC_Subsystem>(header_.link1_subsystem) == subsys) return true;
		if (static_cast<DTC_Subsystem>(header_.link2_subsystem) == subsys) return true;
		if (static_cast<DTC_Subsystem>(header_.link3_subsystem) == subsys) return true;
		if (static_cast<DTC_Subsystem>(header_.link4_subsystem) == subsys) return true;
		if (static_cast<DTC_Subsystem>(header_.link5_subsystem) == subsys) return true;
		return false;
	}
	void SetDTCMAC(uint8_t mac) {
		header_.dtc_mac = mac;
	}
	void SetSourceDTC(uint8_t id, DTC_Subsystem subsystem = DTC_Subsystem_Other){
		std::array<DTC_Subsystem, 6> subsystems;
		subsystems.fill(subsystem); //Use same subsystem for all six links
		SetSourceDTC(id,subsystems);
	}
	void SetSourceDTC(uint8_t id, std::array<DTC_Subsystem, 6> subsystems){
		header_.source_dtc_id = id;
		header_.link0_subsystem = static_cast<uint8_t>(subsystems[0]);
		header_.link1_subsystem = static_cast<uint8_t>(subsystems[1]);
		header_.link2_subsystem = static_cast<uint8_t>(subsystems[2]);
		header_.link3_subsystem = static_cast<uint8_t>(subsystems[3]);
		header_.link4_subsystem = static_cast<uint8_t>(subsystems[4]);
		header_.link5_subsystem = static_cast<uint8_t>(subsystems[5]);
	}
	const DTC_SubEventHeader* GetHeader() const { return &header_; }
	void UpdateHeader();

private:
	std::shared_ptr<std::vector<uint8_t>> allocBytes{nullptr};  ///< Used if the block owns its memory
	DTC_SubEventHeader header_;
	std::vector<DTC_DataBlock> data_blocks_;
	const void* buffer_ptr_;
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Packets_DTC_SubEvent_h
