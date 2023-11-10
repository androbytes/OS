#include <stdint.h>

// In long mode, you use the TSS (mainly) to store the IST
struct TSS {
	uint32_t Reserved0;
	uint64_t RSP[3];
	uint64_t Reserved1;
	uint64_t IST[7];
	uint64_t Reserved2;
	uint16_t Reserved3;
	uint16_t IOPB;
} __attribute__((packed));

extern "C" void LoadTSS(void); // Load the TSS (the GDT has to have been loaded first)