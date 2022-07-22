#include "mu2e-artdaq-core/Overlays/Mu2eEventHeader.hh"

mu2e::Mu2eEventHeader::Mu2eEventHeader(uint64_t timestamp, uint8_t mode)
	: EventWindowTag(timestamp), EventMode(mode)
{}