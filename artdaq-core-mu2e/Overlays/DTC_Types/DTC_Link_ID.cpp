#include "artdaq-core-mu2e/Overlays/DTC_Types/DTC_Link_ID.h"

#include "TRACE/tracemf.h"


namespace DTCLib {

inline TraceStreamer& operator<<(TraceStreamer& ts, DTC_Link_ID const& link)
{
	return ts << static_cast<unsigned int>(link);
}

}  // namespace DTCLib

