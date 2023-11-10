#include <limine.h>
#include "Utils/memory.h"
#include "ExceptionHandler/GDT.h"
#include "Rendering/Framebuffer.h"
#include "Utils/ModuleManager.h"
#include "Paging/Paging.h"

#define CHECK_REQUEST(request, countName) if(request.response == NULL || request.response->countName < 1) hcf();

static volatile limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

static volatile limine_module_request module_request = {
	.id = LIMINE_MODULE_REQUEST,
	.revision = 0
};

static volatile limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

static volatile limine_kernel_address_request kernel_address_request = {
	.id = LIMINE_KERNEL_ADDRESS_REQUEST,
	.revision = 0
};

// Halt and catch fire function.
static void hcf(void) {
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}

extern char _KernelEnd[];

extern "C" void _start() {
    CHECK_REQUEST(framebuffer_request, framebuffer_count);
	CHECK_REQUEST(module_request, module_count);
	CHECK_REQUEST(memmap_request, entry_count);
	if(kernel_address_request.response == NULL) hcf();
	
	GlobalFileManager = ModuleManager(&module_request);

	GDT GlobalDescriptorTable = GDT();
    GlobalDescriptorTable.Init();

	PSF1Font font{};
	if(!LoadFont(&font, "zap-light16.psf")) hcf();

	GlobalPFA = PFA();
	GlobalPFA.Init(memmap_request.response);

    Framebuffer framebuffer = Framebuffer(&framebuffer_request, &font);
    framebuffer.DrawLine(iVec2(0, 0), iVec2(12, 100));
    framebuffer.FillRect(iVec2(600, 600), iVec2(100, 100));

	GlobalPML4 = (PageTable*)GlobalPFA.AllocatePage();
	memset(GlobalPML4, 0, 0x1000);
	limine_kernel_address_response* kernel_address = kernel_address_request.response;

	//void* memory = GlobalPFA.AllocatePage();
	//framebuffer.DrawString(tohex((uint64_t)memory), 10, 10);
	//framebuffer.DrawString(tohex((uint64_t)GlobalPFA.AllocatePage()), 10, 26);
	uint64_t _KernelStart = kernel_address->virtual_base;
	for(uint64_t i = 0; i < (uint64_t)_KernelEnd - _KernelStart; i+=4096){
		void* physAddress = (void*)(kernel_address->physical_base + i);
		void* virtAddress = (void*)(_KernelStart + i);
		MapMemory(physAddress, virtAddress);
	}

	/*limine_framebuffer* res = framebuffer_request.response->framebuffers[0];
	for(uint64_t i = 0; i < (res->pitch * res->height) + 0x1000; i += 4096){
		void* address = (void*)((uint64_t)res->address + i);
		MapMemory(address, address);
	}*/

	/*for(size_t i = 0; i < GetMemorySize(memmap_request.response); i += BLOCK_SIZE){
		MapMemory((void*)i, (void*)i);
	}*/

	//MapMemory(GlobalPFA.AllocatePage(), (void*)0x1000);

	SetPML4(GlobalPML4);

	//void* mymemaddr = (void*)0x1000;
	//*(uint8_t*)mymemaddr = 69;

	//framebuffer.DrawString(tostr(*(uint8_t*)mymemaddr), 10, 50);
	framebuffer.Clear();

    hcf();
}
