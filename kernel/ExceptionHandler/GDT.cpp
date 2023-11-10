#include "GDT.h"

//__attribute__((aligned(0x40)))
GDTEntries GlobalGDT = {
    {0xFFFF, 0, 0, 0x00, 0b11110000, 0},
    {0xFFFF, 0, 0, 0x9A, 0b11111010, 0},
    {0xFFFF, 0, 0, 0x92, 0b11111100, 0},
    {0xFFFF, 0, 0, 0xFA, 0b11111010, 0},
    {0xFFFF, 0, 0, 0xF2, 0b11111100, 0},
    {0, 0, 0, 0x89, 0x00, 0, 0, 0}
};

TSS GlobalTSS = {};

void GDT::Init(){
    uint64_t TSSAddress = (uint64_t)&GlobalTSS;
	GlobalGDT.TSS.Base0 = (uint16_t)TSSAddress;
	GlobalGDT.TSS.Base1 = (uint8_t)(TSSAddress >> 16);
	GlobalGDT.TSS.Base2 = (uint8_t)(TSSAddress >> 24);
	GlobalGDT.TSS.Base3 = (uint32_t)(TSSAddress >> 32);
	GlobalGDT.TSS.Limit = sizeof(TSS);
	
	GDTR gdtr{};
    gdtr.Size = sizeof(GDTEntries) - 1; // size - 1
    gdtr.Offset = (uint64_t)&GlobalGDT; // offset

    LoadGDT(&gdtr);
	LoadTSS();
}