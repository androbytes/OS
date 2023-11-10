#include "Paging.h"
#include "PageFrameAllocator.h"
#include "Utils/memory.h"

// max size is 0x1000000000000 for virtual mem
// 0xF is 0b1111
PageTable* GlobalPML4;

void MapMemory(void* physicalMemory, void* virtualMemory) {
	Indexies indexies = IndexMemory((uint64_t)virtualMemory);
	if(GlobalPML4 == NULL) return;
	uint64_t pde = GlobalPML4->Entries[indexies.PML4_i];
	volatile PageTable* pdpt;
	if(!(pde & PRESENT)){
		pdpt = (PageTable*)GlobalPFA.AllocatePage();
		pdpt = (PageTable*)memset(pdpt, 0, 0x1000);
		pde = (ADDRESS_BITS & (uint64_t)pdpt) | PRESENT | WRITEABLE;
		GlobalPML4->Entries[indexies.PML4_i] = pde;
	} else {
		pdpt = (PageTable*)(pde & ADDRESS_BITS);
	}

	pde = pdpt->Entries[indexies.PDPTE_i];
	volatile PageTable* pd;
	if(!(pde & PRESENT)){
		pd = (PageTable*)GlobalPFA.AllocatePage();
		pd = (PageTable*)memset(pd, 0, 0x1000);
		pde = (ADDRESS_BITS & (uint64_t)pd) | PRESENT | WRITEABLE;
		pdpt->Entries[indexies.PDPTE_i] = pde;
	} else {
		pd = (PageTable*)(pde & ADDRESS_BITS);
	}

	pde = pd->Entries[indexies.PDE_i];
	volatile PageTable* pt;
	if(!(pde & PRESENT)){
		pt = (PageTable*)GlobalPFA.AllocatePage();
		pt = (PageTable*)memset(pt, 0, 0x1000);
		pde = (ADDRESS_BITS & (uint64_t)pt) | PRESENT | WRITEABLE;
		pd->Entries[indexies.PDE_i] = pde;
	} else {
		pt = (PageTable*)(pde & ADDRESS_BITS);
	}

	pt->Entries[indexies.PTE_i] = (ADDRESS_BITS & (uint64_t)physicalMemory) | PRESENT | WRITEABLE;
}

Indexies IndexMemory(uint64_t virtualMemory){
	Indexies indexies{};

	indexies.PageOffset = virtualMemory & 0xFFF; // Get first 12 bits
	virtualMemory >>= 12;                        // Shift to get rid of Page Offset
	indexies.PTE_i      = virtualMemory & 0x1FF; // Get first 9 bits
	virtualMemory >>= 9;                         // Shift to get rid of PTE index
	indexies.PDE_i      = virtualMemory & 0x1FF; // Get first 9 bits
	virtualMemory >>= 9;                         // Shift to get rid of PDE index
	indexies.PDPTE_i    = virtualMemory & 0x1FF; // Get first 9 bits
	virtualMemory >>= 9;                         // Shift to get rid of PDPTE index
	indexies.PML4_i     = virtualMemory & 0x1FF; // Get first 9 bits

	return indexies;
}

void SetPML4(PageTable* PML4){
	__asm__("mov %0, %%cr3" : : "r" (PML4));
}