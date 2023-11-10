#include "PageFrameAllocator.h"
#include "Utils/memory.h"
//#include "buddy_alloc.h"

PFA GlobalPFA;

void PFA::Init(limine_memmap_response* response){
	m_MemMapResponse = response;
	void* LargestMemSegment = NULL;
	uint64_t LargestMemSize = 0; // In bytes

	for(uint64_t i = 0; i < m_MemMapResponse->entry_count; i++){
		limine_memmap_entry* entry = m_MemMapResponse->entries[i];
		if(entry->type != LIMINE_MEMMAP_USABLE){
			// continue, use another block of memory.
			continue;
		}
		if(entry->length > LargestMemSize){
			LargestMemSize = entry->length;
			LargestMemSegment = (void*)entry->base;
		}
	}

	m_BitmapSize = (LargestMemSize / BLOCK_SIZE * 8); // each block uses 1 bit (0 = free, 1 = used)
	m_BitmapByteSize = m_BitmapSize * BLOCK_SIZE; // block size is converted into bytes
	m_Bitmap = (uint8_t*)LargestMemSegment; // Where the bitmap is stored
	m_MemBase = (void*)((uint64_t)LargestMemSegment + m_BitmapByteSize); // Where you get pages
}

void* PFA::AllocatePage(){
	if(m_Bitmap == NULL) return nullptr;
	for(uint32_t i = LastMajor * 8; i < m_BitmapSize; i++){
		if(GetBitmapType(i) == BlockType::Free){
			SetBitmapType(BlockType::Used, i);
			LastMajor = i /  8;
			return (void*)((uint64_t)m_MemBase + (i * BLOCK_SIZE));
		}
	}
	return nullptr;
}

void PFA::FreePage(void* address){
	uint32_t index = ((uint64_t)address - (uint64_t)m_MemBase) / BLOCK_SIZE;
	if(GetBitmapType(index) != BlockType::Used) return; // trying to free a free block
	SetBitmapType(BlockType::Free, index);
	memset(address, 0, BLOCK_SIZE);
	LastMajor = index / 8;
}

void PFA::LockPage(void* address){
	uint32_t index = ((uint64_t)address - (uint64_t)m_MemBase) / BLOCK_SIZE;
	if(GetBitmapType(index) != BlockType::Free) return;
	SetBitmapType(BlockType::Used, index);
	memset(address, 0, BLOCK_SIZE);
}

void PFA::SetBitmapType(BlockType type, uint32_t index){
	if(index > m_BitmapSize) return;
	uint16_t major = index / 8;
	uint8_t minor = index % 8;
	*(m_Bitmap + major) |= type << minor;
}

BlockType PFA::GetBitmapType(uint32_t index){
	uint16_t major = index / 8;
	uint8_t minor = index % 8;
	return (BlockType)(*(m_Bitmap + major) >> minor & 1);
}