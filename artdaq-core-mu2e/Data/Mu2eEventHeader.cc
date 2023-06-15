#include "artdaq-core-mu2e/Data/Mu2eEventHeader.hh"

mu2e::Mu2eEventHeader::Mu2eEventHeader(uint64_t timestamp, uint8_t mode)
	: EventWindowTag(timestamp), EventMode(mode)
{}
