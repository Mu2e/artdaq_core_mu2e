#ifndef mu2e_artdaq_core_Overlays_DTCFragment_hh
#define mu2e_artdaq_core_Overlays_DTCFragment_hh

#include "artdaq-core/Data/Fragment.hh"
//#include "artdaq/DAQdata/features.hh"
#include "cetlib_except/exception.h"

#include <ostream>
#include <vector>

// Implementation of "DTCFragment", an artdaq::Fragment overlay class
// used for pedagogical purposes

namespace mu2e {
class DTCFragment;

typedef uint8_t packet_t[16];
// Let the "<<" operator dump the DTCFragment's data to stdout
std::ostream &operator<<(std::ostream &, DTCFragment const &);
}  // namespace mu2e

class mu2e::DTCFragment
{
public:
	// The DTCFragment represents its data through the adc_t type, which
	// is a typedef of an 16-member uint8_t array.

	// typedef uint8_t packet_t[16];

	// The "Header" struct contains "metadata" specific to the fragment
	// which is not hardware-related

	// Header::data_t -- not to be confused with Metadata::data_t ! --
	// describes the standard size of a data type not just for the
	// header data, but ALSO the physics data beyond it; the size of the
	// header in units of Header::data_t is given by "size_words", and
	// the size of the fragment beyond the header in units of
	// Header::data_t is given by "event_size"
	struct Header
	{
		typedef uint8_t data_t;
		typedef uint16_t data_size_t;
		typedef uint64_t timestamp_t;

		// Word 0
		data_size_t BlockByteCount;
		// Word 1
		data_t SubsystemID : 4;
		data_t PacketType : 4;
		data_t ROCID : 3;
		data_t unused1 : 4;
		data_t Valid : 1;
		// Word 2
		data_size_t PacketCount : 11;
		data_size_t unused2 : 5;
		// Word 3-5
		data_size_t TimestampLow;
		data_size_t TimestampMed;
		data_size_t TimestampHigh;
		// Word 6
		data_t Status;
		data_t FormatVersion;
		// Word 7
		data_t DTCID;
		data_t EVBMode;

		static size_t const size_words = 16ul;
	};

	static_assert(sizeof(Header) == Header::size_words * sizeof(Header::data_t), "DTCFragment::Header size changed");

	// The constructor simply sets its const private member "artdaq_Fragment_"
	// to refer to the artdaq::Fragment object

	DTCFragment(artdaq::Fragment const &f)
		: artdaq_Fragment_(f) {}

	// const getter functions for the data in the header
	Header::data_size_t hdr_byte_count() const { return header_()->BlockByteCount; }
	Header::data_t hdr_subsystem_id() const { return header_()->SubsystemID; }
	Header::data_t hdr_packet_type() const { return header_()->PacketType; }
	Header::data_t hdr_roc_id() const { return header_()->ROCID; }
	bool hdr_is_valid() const { return header_()->Valid; }
	Header::data_t hdr_status() const { return header_()->Status; }
	Header::data_t hdr_data_format_version() const { return header_()->FormatVersion; }
	Header::data_t hdr_dtc_id() const { return header_()->DTCID; }
	Header::data_t hdr_evb_mode() const { return header_()->EVBMode; }
	Header::data_size_t hdr_packet_count() const { return header_()->PacketCount; }
	Header::timestamp_t hdr_timestamp() const
	{
		return static_cast<Header::timestamp_t>(header_()->TimestampLow) 
			+ (static_cast<Header::timestamp_t>(header_()->TimestampMed) << 16) 
			+ (static_cast<Header::timestamp_t>(header_()->TimestampHigh) << 32);
	}

	static constexpr size_t hdr_size_words() { return Header::size_words; }

	size_t dataSize() const { return hdr_packet_count() * words_per_packet_(); }

	// Start of the DTC packets, returned as a pointer to the packet type
	packet_t const *dataBegin() const { return reinterpret_cast<packet_t const *>(header_() + 1); }

	// End of the DTC packets, returned as a pointer to the packet type
	packet_t const *dataEnd() const { return dataBegin() + hdr_packet_count(); }

protected:
	// Functions to translate between size (in bytes) of a DTC packet, size of
	// this fragment overlay's concept of a unit of data (i.e.,
	// Header::data_t) and size of an artdaq::Fragment's concept of a
	// unit of data (the artdaq::Fragment::value_type).

	static constexpr size_t words_per_packet_() { return sizeof(packet_t) / sizeof(Header::data_t); }

	static constexpr size_t words_per_frag_word_()
	{
		return sizeof(artdaq::Fragment::value_type) / sizeof(Header::data_t);
	}

	// header_() simply takes the address of the start of this overlay's
	// data (i.e., where the DTCFragment::Header object begins) and
	// casts it as a pointer to DTCFragment::Header

	Header const *header_() const
	{
		return reinterpret_cast<DTCFragment::Header const *>(&*artdaq_Fragment_.dataBegin());
	}

private:
	artdaq::Fragment const &artdaq_Fragment_;
};

#endif /* mu2e_artdaq_Overlays_DTCFragment_hh */
