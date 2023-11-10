#pragma once
#include "TSS.h"

struct GDTR {
	uint16_t Size;
    uint64_t Offset;
} __attribute__((packed));

struct GDTEntry {
    uint16_t Limit0; // ignored
    uint16_t Base0; // ignored
    uint8_t Base1; // ignored
    uint8_t AccessByte;
    uint8_t LimitFlags;
    uint8_t Base2;
} __attribute__((packed));

// In 64-bit mode, TSS stores the IST(Interrupt Stack Table)
struct TSSEntry {
	uint16_t Limit;
	uint16_t Base0;
	uint8_t Base1;
	uint8_t AccessByte;
	uint8_t LimitFlags;
	uint8_t Base2;
	uint32_t Base3;
	uint32_t Reserved;
} __attribute__((packed));

struct GDTEntries {
    GDTEntry NullDescriptor;
    GDTEntry KMCS; // Kernel Mode Code Segment
    GDTEntry KMDS; // Kernel Mode Data Segment
    GDTEntry UMCS; // User Mode Code Segmnet
    GDTEntry UMDS; // User Mode Data Segment
    TSSEntry TSS;  // Task State Segment (in long mode it is used for other things)
} __attribute__((packed)); //__attribute__((aligned(0x40))); // AMD Specs say to align by a quad-word

extern GDTEntries GlobalGDT;
extern "C" void LoadGDT(GDTR* gdtr);

extern TSS GlobalTSS;

// Access Byte is comprised of: (stolen from OSDev)
//(7) P: Present bit. Allows an entry to refer to a valid segment. Must be set (1) for any valid segment.
//(6 / 5) DPL: Descriptor privilege level field. Contains the CPU Privilege level of the segment. 0 = highest privilege (kernel), 3 = lowest privilege (user applications).
//(4) S: Descriptor type bit. If clear (0) the descriptor defines a system segment (eg. a Task State Segment). If set (1) it defines a code or data segment.
//(3) E: Executable bit. If clear (0) the descriptor defines a data segment. If set (1) it defines a code segment which can be executed from.
//(2) DC: Direction bit/Conforming bit.
//  For data selectors: Direction bit. If clear (0) the segment grows up. If set (1) the segment grows down, ie. the Offset has to be greater than the Limit.
//  For code selectors: Conforming bit.
//  If clear (0) code in this segment can only be executed from the ring set in DPL.
//  If set (1) code in this segment can be executed from an equal or lower privilege level. For example, code in ring 3 can far-jump to conforming code in a ring 2 segment. The DPL field represent the highest privilege level that is allowed to execute the segment. For example, code in ring 0 cannot far-jump to a conforming code segment where DPL is 2, while code in ring 2 and 3 can. Note that the privilege level remains the same, ie. a far-jump from ring 3 to a segment with a DPL of 2 remains in ring 3 after the jump.
//(1) RW: Readable bit/Writable bit.
//  For code segments: Readable bit. If clear (0), read access for this segment is not allowed. If set (1) read access is allowed. Write access is never allowed for code segments.
//  For data segments: Writeable bit. If clear (0), write access for this segment is not allowed. If set (1) write access is allowed. Read access is always allowed for data segments.
//(0) A: Accessed bit. The CPU will set it when the segment is accessed unless set to 1 in advance. This means that in case the GDT descriptor is stored in read only pages and this bit is set to 0, the CPU trying to set this bit will trigger a page fault. Best left set to 1 unless otherwise needed.

// Flags (upper 4 bits of LimitFlags)
// (3) G: Granularity flag, indicates the size the Limit value is scaled by. If clear (0), the Limit is in 1 Byte blocks (byte granularity). If set (1), the Limit is in 4 KiB blocks (page granularity).
// (2) DB: Size flag. If clear (0), the descriptor defines a 16-bit protected mode segment. If set (1) it defines a 32-bit protected mode segment. A GDT can have both 16-bit and 32-bit selectors at once.
// (1) L: Long-mode code flag. If set (1), the descriptor defines a 64-bit code segment. When set, DB should always be clear. For any other type of segment (other code types or any data segment), it should be clear (0).
// (0) Reserved

class GDT {
public:
    void Init();
};