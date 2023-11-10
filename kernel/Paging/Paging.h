#include <stdint.h>
#include "PageFrameAllocator.h"

#define PML4E_ADDRESS_BITS

#define PRESENT   1
#define WRITEABLE 2
#define ADDRESS_BITS 0x000FFFFFFFFFF000

struct PageTable {
	uint64_t Entries[512];
} __attribute__((aligned(0x1000)));

struct Indexies {
	uint16_t PageOffset : 12;
	uint16_t PTE_i      :  9;
	uint16_t PDE_i      :  9;
	uint16_t PDPTE_i    :  9;
	uint16_t PML4_i     :  9;
} __attribute__((packed));

extern PageTable* GlobalPML4;
void SetPML4(PageTable* pml4);

/*
Virtual address is 48 bits
Physical address is 52 bits
*/
void MapMemory(void* physicalMemory, void* virtualMemory);

Indexies IndexMemory(uint64_t virtualMemory);