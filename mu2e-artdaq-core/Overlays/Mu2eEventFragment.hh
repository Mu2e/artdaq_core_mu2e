#ifndef artdaq_core_Data_Mu2eEventFragment_hh
#define artdaq_core_Data_Mu2eEventFragment_hh

#include <memory>
#include "artdaq-core/Data/Fragment.hh"
#include "cetlib_except/exception.h"

//#include <ostream>
//#include <vector>

// Implementation of "Mu2eEventFragment", an artdaq::Fragment overlay class

namespace mu2e {
class Mu2eEventFragment;
}

/**
 * \brief The artdaq::Mu2eEventFragment class represents a Fragment which contains other Fragments
 */
class mu2e::Mu2eEventFragment
{
public:
	/// The current version of the Mu2eEventFragmentHeader
	static constexpr uint8_t CURRENT_VERSION = 1;
	/// Marker word used in index
	static constexpr size_t CONTAINER_MAGIC = 0x00BADDEED5B1BEE5;

	/**
	 * \brief Contains the information necessary for retrieving Fragment objects from the Mu2eEventFragment
	 */
	struct Metadata
	{
		uint64_t event_window_tag : 48;
		uint64_t evb_mode : 8;
		uint64_t version : 4;       ///< Version number of Mu2eEventFragment
		uint64_t missing_data : 1;  ///< Flag if the Mu2eEventFragment knows that it is missing data
		uint64_t has_index : 1;     ///< Whether the Mu2eEventFragment has an index at the end of the payload
		uint64_t unused_flag1 : 1;  ///< Unused
		uint64_t unused_flag2 : 1;  ///< Unused

		uint32_t tracker_block_count;
		uint32_t calorimeter_block_count;

		uint64_t index_offset;  ///< Index starts this many bytes after the beginning of the payload (is also the total size of contained Fragments)

		/// Size of the Metadata object
		static size_t const size_bytes = 24ul;
	};
	static_assert(sizeof(Metadata) == Metadata::size_bytes, "Mu2eEventFragment::Metadata size changed");

	/**
	 * \param f The Fragment object to use for data storage
	 * 
	 * The constructor simply sets its const private member "artdaq_Fragment_"
	 * to refer to the artdaq::Fragment object
	*/
	explicit Mu2eEventFragment(artdaq::Fragment const& f)
		: artdaq_Fragment_(f), index_ptr_(nullptr), index_ptr_owner_(nullptr) {}

	virtual ~Mu2eEventFragment()
	{
	}

	/**
	 * \brief const getter function for the Metadata
	 * \return const pointer to the Metadata
	 */
	Metadata const* metadata() const
	{
		return artdaq_Fragment_.metadata<Metadata>();
	}

	size_t tracker_block_count() const { return metadata()->tracker_block_count; }
	size_t calorimeter_block_count() const { return metadata()->calorimeter_block_count; }
	size_t block_count() const { return tracker_block_count() + calorimeter_block_count(); }

	/**
	 * \brief Gets the flag if the Mu2eEventFragment knows that it is missing data
	 * \return The flag if the Mu2eEventFragment knows that it is missing data
	 */
	bool missing_data() const { return static_cast<bool>(metadata()->missing_data); }

	/**
	 * \brief Gets the start of the data
	 * \return Pointer to the first Fragment in the Mu2eEventFragment
	 */
	void const* dataBegin() const
	{
		return reinterpret_cast<void const*>(&*artdaq_Fragment_.dataBegin());  // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
	}

	/**
	 * \brief Gets the last Fragment in the Mu2eEventFragment
	 * \return Pointer to the last Fragment in the Mu2eEventFragment
	 */
	void const* dataEnd() const
	{
		return reinterpret_cast<void const*>(reinterpret_cast<uint8_t const*>(dataBegin()) + lastFragmentIndex());  // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast,cppcoreguidelines-pro-bounds-pointer-arithmetic)
	}

	std::pair<void const*, size_t> atPtr(size_t index) const
	{
		if (index >= block_count() || block_count() == 0)
		{
			throw cet::exception("ArgumentOutOfRange") << "Buffer overrun detected! Mu2eEventFragment::atPtr was asked for a non-existent Fragment!";  // NOLINT(cert-err60-cpp)
		}

		return std::make_pair(static_cast<uint8_t const*>(dataBegin()) + fragmentIndex(index) + sizeof(artdaq::detail::RawFragmentHeader, 
			fragSize(index) - sizeof(artdaq::detail::RawFragmentHeader); // Skip past header
	}

	std::pair<void const*, size_t> trackerAtPtr(size_t trkIndex) const
	{
		if (index >= tracker_block_count())
		{
			throw cet::exception("ArgumentOutOfRange") << "Buffer overrun detected! Mu2eEventFragment::trackerAtPtr was asked for a non-existent Fragment!";  // NOLINT(cert-err60-cpp)
		}
		return atPtr(index);
	}

	std::pair<void const*, size_t> calorimeterAtPtr(size_t caloIndex) const
	{
		if (index >= calorimeter_block_count())
		{
			throw cet::exception("ArgumentOutOfRange") << "Buffer overrun detected! Mu2eEventFragment::calorimeterAtPtr was asked for a non-existent Fragment!";  // NOLINT(cert-err60-cpp)
		}
		return atPtr(index + metadata()->tracker_block_count);
	}

	/**
	 * \brief Gets a specific Fragment from the Mu2eEventFragment
	 * \param index The Fragment index to return
	 * \return Pointer to the specified Fragment in the Mu2eEventFragment
	 * \exception cet::exception if the index is out-of-range
	 */
	artdaq::FragmentPtr at(size_t index) const
	{
		if (index >= block_count() || block_count() == 0)
		{
			throw cet::exception("ArgumentOutOfRange") << "Buffer overrun detected! Mu2eEventFragment::at was asked for a non-existent Fragment!";  // NOLINT(cert-err60-cpp)
		}

		artdaq::FragmentPtr frag(nullptr);
		auto size = fragSize(index);
		if (size < sizeof(artdaq::RawDataType) * artdaq::detail::RawFragmentHeader::num_words())
		{
			TLOG(TLVL_WARNING, "Mu2eEventFragment") << "Contained Fragment is below minimum size! Reported Data and Metadata sizes will be incorrect!";
			frag = std::make_unique<artdaq::Fragment>();
		}
		else
		{
			// Subtract RawFragmentHeader::num_words here as Fragment consturctor will allocate n + detail::RawFragmentHeader::num_words(), and we want fragSize to be allocated.
			frag = std::make_unique<artdaq::Fragment>((fragSize(index)) / sizeof(artdaq::RawDataType) - artdaq::detail::RawFragmentHeader::num_words());
		}
		memcpy(frag->headerAddress(), reinterpret_cast<uint8_t const*>(dataBegin()) + fragmentIndex(index), fragSize(index));  // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast,cppcoreguidelines-pro-bounds-pointer-arithmetic)
		return frag;
	}

	/**
	 * \brief Gets a specific Fragment from the Mu2eEventFragment
	 * \param index The Fragment index to return
	 * \return Pointer to the specified Fragment in the Mu2eEventFragment
	 * \exception cet::exception if the index is out-of-range
	 */
	artdaq::FragmentPtr trackerAt(size_t index) const
	{
		if (index >= tracker_block_count())
		{
			throw cet::exception("ArgumentOutOfRange") << "Buffer overrun detected! Mu2eEventFragment::trackerAt was asked for a non-existent Fragment!";  // NOLINT(cert-err60-cpp)
		}
		return at(index);
	}
	/**
	 * \brief Gets a specific Fragment from the Mu2eEventFragment
	 * \param index The Fragment index to return
	 * \return Pointer to the specified Fragment in the Mu2eEventFragment
	 * \exception cet::exception if the index is out-of-range
	 */
	artdaq::FragmentPtr calorimeterAt(size_t index) const
	{
		if (index >= calorimeter_block_count())
		{
			throw cet::exception("ArgumentOutOfRange") << "Buffer overrun detected! Mu2eEventFragment::calorimeterAt was asked for a non-existent Fragment!";  // NOLINT(cert-err60-cpp)
		}
		return at(index + metadata()->tracker_block_count);
	}
	/**
	 * \brief Gets the size of the Fragment at the specified location in the Mu2eEventFragment, in bytes
	 * \param index The Fragment index
	 * \return The size of the Fragment at the specified location in the Mu2eEventFragment, in bytes
	 * \exception cet::exception if the index is out-of-range
	 */
	size_t fragSize(size_t index) const
	{
		if (index >= block_count() || block_count() == 0)
		{
			throw cet::exception("ArgumentOutOfRange") << "Buffer overrun detected! Mu2eEventFragment::fragSize was asked for a non-existent Fragment!";  // NOLINT(cert-err60-cpp)
		}
		auto end = fragmentIndex(index + 1);
		if (index == 0) return end;
		return end - fragmentIndex(index);
	}

	/**
	 * \brief Alias to Mu2eEventFragment::at()
	 * \param index The Fragment index to return
	 * \return Pointer to the specified Fragment in the Mu2eEventFragment
	 * \exception cet::exception if the index is out-of-range
	 */
	artdaq::FragmentPtr operator[](size_t index) const
	{
		return this->at(index);
	}

	/**
	 * \brief Get the offset of a Fragment within the Mu2eEventFragment
	 * \param index The Fragment index
	 * \return The offset of the requested Fragment within the Mu2eEventFragment
	 * \exception cet::exception if the index is out-of-range
	 */
	size_t fragmentIndex(size_t index) const
	{
		if (index > block_count())
		{
			throw cet::exception("ArgumentOutOfRange") << "Buffer overrun detected! Mu2eEventFragment::fragmentIndex was asked for a non-existent Fragment!";  // NOLINT(cert-err60-cpp)
		}
		if (index == 0) { return 0; }

		auto index_ptr = get_index_();

		return index_ptr[index - 1];  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
	}

	/**
	 * \brief Returns the offset of the last Fragment in the Mu2eEventFragment
	 * \return The offset of the last Fragment in the Mu2eEventFragment
	 */
	size_t lastFragmentIndex() const
	{
		return fragmentIndex(block_count());
	}

protected:
	/**
	 * \brief Create an index for the currently-contained Fragments
	 * \return Array of block_count size_t words containing index
	 */
	const size_t* create_index_() const
	{
		TLOG(TLVL_TRACE + 10, "Mu2eEventFragment") << "Creating new index for Mu2eEventFragment";
		index_ptr_owner_ = std::make_unique<std::vector<size_t>>(block_count() + 1);

		auto current = reinterpret_cast<uint8_t const*>(artdaq_Fragment_.dataBegin());  // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
		size_t offset = 0;
		for (size_t ii = 0; ii < block_count(); ++ii)
		{
			auto this_size = reinterpret_cast<const artdaq::detail::RawFragmentHeader*>(current)->word_count * sizeof(artdaq::RawDataType);  // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
			//TLOG(TLVL_TRACE + 10, "Mu2eEventFragment") << "Block " << ii << " / " << block_count() << ": Size=" << this_size << " bytes";
			offset += this_size;
			index_ptr_owner_->at(ii) = offset;
			current += this_size;  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		}
		index_ptr_owner_->at(block_count()) = CONTAINER_MAGIC;
		return &index_ptr_owner_->at(0);
	}

	/**
	 * \brief Reset the index pointer to a newly-created index
	 */
	void reset_index_ptr_() const
	{
		TLOG(TLVL_TRACE + 11, "Mu2eEventFragment") << "Request to reset index_ptr recieved. has_index=" << metadata()->has_index << ", Check word = " << std::hex
												   << *(reinterpret_cast<size_t const*>(artdaq_Fragment_.dataBeginBytes() + metadata()->index_offset) + block_count());    // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast,cppcoreguidelines-pro-bounds-pointer-arithmetic)
		if (metadata()->has_index && *(reinterpret_cast<size_t const*>(artdaq_Fragment_.dataBeginBytes() + metadata()->index_offset) + block_count()) == CONTAINER_MAGIC)  // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast,cppcoreguidelines-pro-bounds-pointer-arithmetic)
		{
			TLOG(TLVL_TRACE + 11, "Mu2eEventFragment") << "Setting index_ptr to found valid index";
			index_ptr_ = reinterpret_cast<size_t const*>(artdaq_Fragment_.dataBeginBytes() + metadata()->index_offset);  // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast,cppcoreguidelines-pro-bounds-pointer-arithmetic)
		}
		else
		{
			TLOG(TLVL_ERROR, "Mu2eEventFragment") << "Index invalid or not found!";
			throw cet::exception("InvalidIndex") << "Index invalid or not found!";
		}
	}

	/**
	 * \brief Get a pointer to the index
	 * \return pointer to size_t array of Fragment offsets in payload, terminating with CONTAINER_MAGIC
	 */
	const size_t* get_index_() const
	{
		if (index_ptr_ != nullptr) return index_ptr_;

		reset_index_ptr_();

		return index_ptr_;
	}

private:
	Mu2eEventFragment(Mu2eEventFragment const&) = delete;             // Mu2eEventFragments should definitely not be copied
	Mu2eEventFragment(Mu2eEventFragment&&) = delete;                  // Mu2eEventFragments should not be moved, only the underlying Fragment
	Mu2eEventFragment& operator=(Mu2eEventFragment const&) = delete;  // Mu2eEventFragments should definitely not be copied
	Mu2eEventFragment& operator=(Mu2eEventFragment&&) = delete;       // Mu2eEventFragments should not be moved, only the underlying Fragment

	artdaq::Fragment const& artdaq_Fragment_;

	mutable const size_t* index_ptr_;
	mutable std::unique_ptr<std::vector<size_t>> index_ptr_owner_;
};

#endif /* artdaq_core_Data_Mu2eEventFragment_hh */
