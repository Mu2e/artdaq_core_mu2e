#define BOOST_TEST_MODULE DTC_SubEvent_t
#include <boost/test/unit_test.hpp>

#include "TRACE/tracemf.h"
#define TRACE_NAME "DTC_SubEvent_t"

#include <fstream>

#include "artdaq-core-mu2e/Overlays/DTC_Packets/DTC_SubEvent.h"

BOOST_AUTO_TEST_SUITE(DTC_SubEvent_t)

BOOST_AUTO_TEST_CASE(Constructor)
{
	size_t size_bytes = sizeof(DTCLib::DTC_SubEventHeader);

	auto evt = std::make_unique<DTCLib::DTC_SubEvent>(size_bytes);

	DTCLib::DTC_SubEventHeader evtHdr;
	evtHdr.inclusive_subevent_byte_count = size_bytes;
	evtHdr.num_rocs = 0;
	evtHdr.event_tag_low = 1;
	evtHdr.event_tag_high = 0;
	memcpy(const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(evt->GetRawBufferPointer())), &evtHdr, sizeof(DTCLib::DTC_SubEventHeader));

	TLOG(TLVL_TRACE + 22) << "Calling SetupSubEvent";
	auto ok = evt->SetupSubEvent();
	BOOST_REQUIRE(ok);

	BOOST_REQUIRE_EQUAL(evt->GetDataBlockCount(), 0);
	BOOST_REQUIRE_EQUAL(evt->GetEventWindowTag(), DTCLib::DTC_EventWindowTag(1));
}

BOOST_AUTO_TEST_CASE(GoodBinaryFile)
{
	std::ifstream input("subevent_test.bin", std::ios::binary);
	std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});

	auto evt = std::make_unique<DTCLib::DTC_SubEvent>(buffer.data());
	auto ok = evt->SetupSubEvent();
	BOOST_REQUIRE(ok);
	BOOST_REQUIRE_EQUAL(evt->GetDataBlockCount(), 6);
	BOOST_REQUIRE_EQUAL(evt->GetEventWindowTag(), DTCLib::DTC_EventWindowTag(1));
}

BOOST_AUTO_TEST_CASE(BadBinaryFile_Short)
{
	std::ifstream input("subevent_test_short.bin", std::ios::binary);
	std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});

	auto evt = std::make_unique<DTCLib::DTC_SubEvent>(buffer.data());
	auto ok = evt->SetupSubEvent();
	BOOST_REQUIRE(!ok);
	BOOST_REQUIRE_EQUAL(evt->GetDataBlockCount(), 6);
	BOOST_REQUIRE_EQUAL(evt->GetEventWindowTag(), DTCLib::DTC_EventWindowTag(1));
}

BOOST_AUTO_TEST_CASE(BadBinaryFile_Long)
{
	std::ifstream input("subevent_test_long.bin", std::ios::binary);
	std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});

	auto evt = std::make_unique<DTCLib::DTC_SubEvent>(buffer.data());
	auto ok = evt->SetupSubEvent();
	BOOST_REQUIRE(!ok);
	BOOST_REQUIRE_EQUAL(evt->GetDataBlockCount(), 6);
	BOOST_REQUIRE_EQUAL(evt->GetEventWindowTag(), DTCLib::DTC_EventWindowTag(1));
}

BOOST_AUTO_TEST_SUITE_END()
