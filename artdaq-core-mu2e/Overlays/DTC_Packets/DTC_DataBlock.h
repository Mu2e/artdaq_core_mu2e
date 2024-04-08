#ifndef artdaq_core_mu2e_Overlays_DTC_Packets_DTC_DataBlock_h
#define artdaq_core_mu2e_Overlays_DTC_Packets_DTC_DataBlock_h

#include "artdaq-core-mu2e/Overlays/DTC_Packets/DTC_DataHeaderPacket.h"
#include "artdaq-core-mu2e/Overlays/DTC_Packets/DTC_DataPacket.h"

#include <cassert>
#include <cstdint>
#include <memory>
#include <vector>

namespace DTCLib {

/// <summary>
/// A Data Block object (DataHeader packet plus associated Data Packets)
/// Constructed as a pointer to a region of memory
/// </summary>
struct DTC_DataBlock
{
	std::shared_ptr<std::vector<uint8_t>> allocBytes{nullptr};  ///< Used if the block owns its memory
	const void* blockPointer{nullptr};                          ///< Pointer to DataBlock in Memory
	size_t byteSize{0};                                         ///< Size of DataBlock
	mutable std::shared_ptr<DTC_DataHeaderPacket> hdr{nullptr};

	/**
	 * @brief Create a DTC_DataBlock using a pointer to a memory location containing a Data Block
	 * @param ptr Pointer to Data Block
	 * 
	 * WARNING: This function assumes that the pointer is pointing to a valid DTC_DataHeaderPacket!
	*/
	DTC_DataBlock(const void* ptr)
		: blockPointer(ptr)
	{
		DTC_DataPacket pkt(ptr);
		DTC_DataHeaderPacket hdr(pkt);
		byteSize = hdr.GetByteCount();
	}

	/// <summary>
	/// Create a DTC_DataBlock pointing to the given location in memory with the given size
	/// </summary>
	/// <param name="ptr">Pointer to DataBlock in memory</param>
	/// <param name="sz">Size of DataBlock</param>
	DTC_DataBlock(const void* ptr, size_t sz)
		: blockPointer(ptr), byteSize(sz) {}

	DTC_DataBlock(size_t sz)
		: allocBytes(new std::vector<uint8_t>(sz)), blockPointer(allocBytes->data()), byteSize(sz)
	{
	}

	inline std::shared_ptr<DTC_DataHeaderPacket> GetHeader() const
	{
		assert(byteSize >= 16);
		if (hdr) return hdr;
		hdr = std::make_shared<DTC_DataHeaderPacket>(DTC_DataPacket(blockPointer));
		return hdr;
	}

	inline const void* GetRawBufferPointer() const
	{
		assert(byteSize >= 16);
		return static_cast<const void*>(reinterpret_cast<const uint8_t*>(blockPointer));
	}
	inline const void* GetData() const
	{
		assert(byteSize > 16);
		return static_cast<const void*>(reinterpret_cast<const uint8_t*>(blockPointer) + 16);
	}
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Packets_DTC_DataBlock_h
