#pragma once
#include "limine.h"
#include <stdint.h>

#define MM_TO_INDEX(major, minor) (minor + (major * 8))

// Block size is in bytes
#define BLOCK_SIZE 0x1000

enum BlockType {
	Free,
	Used // could be reserved or locked
};

class PFA {
private:
	void* m_MemBase;
	uint8_t* m_Bitmap;
	uint32_t m_BitmapSize; // Size in blocks
	uint64_t m_BitmapByteSize; // Size of bitmap in bytes

	limine_memmap_response* m_MemMapResponse;

	void SetBitmapType(BlockType, uint32_t);
	BlockType GetBitmapType(uint32_t);

	uint16_t LastMajor = 0;
public:
	void Init(limine_memmap_response*);

	void* AllocatePage();
	void FreePage(void*);
	void LockPage(void*);

	uint64_t GetBitmapSize() { return m_BitmapSize; }
};

extern PFA GlobalPFA;