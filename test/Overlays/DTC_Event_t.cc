#define BOOST_TEST_MODULE DTC_Event_t
#include <boost/test/unit_test.hpp>

#include "TRACE/tracemf.h"
#define TRACE_NAME "DTC_Event_t"

#include <fstream>

#include "artdaq-core-mu2e/Overlays/DTC_Packets/DTC_Event.h"

BOOST_AUTO_TEST_SUITE(DTC_Event_t)

BOOST_AUTO_TEST_CASE(Constructor)
{
	size_t size_bytes = sizeof(DTCLib::DTC_EventHeader);

	auto evt = std::make_unique<DTCLib::DTC_Event>(size_bytes);

	DTCLib::DTC_EventHeader evtHdr;
	evtHdr.inclusive_event_byte_count = size_bytes;
	evtHdr.num_dtcs = 0;
	evtHdr.event_tag_low = 1;
	evtHdr.event_tag_high = 0;
	memcpy(const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(evt->GetRawBufferPointer())), &evtHdr, sizeof(DTCLib::DTC_EventHeader));

	TLOG(TLVL_TRACE + 22) << "Calling SetupEvent";
	auto ok = evt->SetupEvent();
	BOOST_REQUIRE(ok);

	BOOST_REQUIRE_EQUAL(evt->GetSubEventCount(), 0);
	BOOST_REQUIRE_EQUAL(evt->GetEventWindowTag(), DTCLib::DTC_EventWindowTag(1));
}

BOOST_AUTO_TEST_CASE(GoodBinaryFile)
{
	std::ifstream input("event_test.bin", std::ios::binary);
	std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});

	auto evt = std::make_unique<DTCLib::DTC_Event>(buffer.data());
	auto ok = evt->SetupEvent();
	BOOST_REQUIRE(ok);
	BOOST_REQUIRE_EQUAL(evt->GetSubEventCount(), 3);
	BOOST_REQUIRE_EQUAL(evt->GetEventWindowTag(), DTCLib::DTC_EventWindowTag(1));

    auto subevt = evt->GetSubEvent(0);
	BOOST_REQUIRE_EQUAL(subevt->GetDataBlockCount(), 6);
	BOOST_REQUIRE_EQUAL(subevt->GetEventWindowTag(), DTCLib::DTC_EventWindowTag(1));
	BOOST_REQUIRE_EQUAL(subevt->GetDTCID(), 0);

}

BOOST_AUTO_TEST_CASE(BadBinaryFile_Short)
{
	std::ifstream input("event_test_short.bin", std::ios::binary);
	std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});

	auto evt = std::make_unique<DTCLib::DTC_Event>(buffer.data());
	auto ok = evt->SetupEvent();
	BOOST_REQUIRE(!ok);
	BOOST_REQUIRE_EQUAL(evt->GetSubEventCount(), 3);
	BOOST_REQUIRE_EQUAL(evt->GetEventWindowTag(), DTCLib::DTC_EventWindowTag(1));
}

BOOST_AUTO_TEST_CASE(BadBinaryFile_Long)
{
	std::ifstream input("event_test_long.bin", std::ios::binary);
	std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});

	auto evt = std::make_unique<DTCLib::DTC_Event>(buffer.data());
	auto ok = evt->SetupEvent();
	BOOST_REQUIRE(!ok);
	BOOST_REQUIRE_EQUAL(evt->GetSubEventCount(), 3);
	BOOST_REQUIRE_EQUAL(evt->GetEventWindowTag(), DTCLib::DTC_EventWindowTag(1));
}

BOOST_AUTO_TEST_SUITE_END()
