#include <stdint.h>
#include <stddef.h>
#include <limine.h>

#define CHECK_REQUEST(request, countName) if(request.response == NULL || request.response->countName < 1) hcf();

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

static volatile struct limine_terminal_request terminal_request = {
    .id = LIMINE_TERMINAL_REQUEST,
    .revision = 0
};

// Halt and catch fire function.
static void hcf(void) {
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}

void _start(void) {
    CHECK_REQUEST(framebuffer_request, framebuffer_count);
    CHECK_REQUEST(terminal_request, terminal_count);

    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];
    struct limine_terminal* terminal = terminal_request.response->terminals[0];

    // Note: we assume the framebuffer model is RGB with 32-bit pixels.
    for (size_t i = 0; i < 100; i++) {
        uint32_t *fb_ptr = framebuffer->address;
        fb_ptr[i * (framebuffer->pitch / 4) + i] = 0xffffff;
    }

    terminal_request.response->write(terminal, "hi");
    
    hcf();
}
