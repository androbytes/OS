#include "Framebuffer.h"

Framebuffer::Framebuffer(volatile limine_framebuffer_request* request, PSF1Font* font) {
	m_Request = request;
	m_MainFramebuffer = request->response->framebuffers[0];
	m_ConsoleFont = font;
}

void Framebuffer::DrawLine(iVec2 p0, iVec2 p1, Color color) {
	iVec2 d = p1 - p0;
	if(d.X > d.Y){
		int yi = 1;
		if(d.Y < 0){
			yi = -1;
			d.Y = -d.Y;
		}
		int D = 2 * d.Y - d.X;
		int y = p0.Y;

		for (int x = p0.X; x < p1.X; x++) {
			uint32_t* ptr = static_cast<uint32_t*>(m_MainFramebuffer->address);
			ptr[y * (m_MainFramebuffer->pitch / 4) + x] = color;
			if (D > 0) {
				y += yi;
				D += 2 * (d.Y - d.X);
			} else
				D += 2 * d.Y;
		}
	} else {
		int xi = 1;
		if(d.X < 0){
			xi = -1;
			d.X = -d.X;
		}
		int D = (2 * d.X) - d.Y;
		int x = p0.X;

		for (int y = p0.Y; y < p1.Y; y++) {
			uint32_t* ptr = static_cast<uint32_t*>(m_MainFramebuffer->address);
			ptr[y * (m_MainFramebuffer->pitch / 4) + x] = (uint32_t)color;
			if (D > 0) {
				x += xi;
				D += 2 * (d.X - d.Y);
			} else
				D += 2 * d.X;
		}
	}
}

void Framebuffer::FillRect(iVec2 position, iVec2 size, Color color){
	uint32_t* vram = static_cast<uint32_t*>(m_MainFramebuffer->address);
	for(int x = 0; x < size.X; x++){
		for(int y = 0; y < size.Y; y++)
			vram[(y + position.X)] = (uint32_t)color;
		vram+=m_MainFramebuffer->pitch/4;
	}
}

void Framebuffer::Clear(Color color) {
	memset(m_MainFramebuffer->address, color, static_cast<size_t>(m_MainFramebuffer->height * m_MainFramebuffer->pitch));
}

void Framebuffer::DrawCharacter(const char character, uint32_t x, uint32_t y, Color color){
	uint32_t* vram = (uint32_t*)m_MainFramebuffer->address;
	char* charPtr = (char*)m_ConsoleFont->GlyphData + (character * m_ConsoleFont->Header->GlyphSize);
	for(unsigned long i = y; i < y + 16; i++){
		for(unsigned long j = x; j < x + 8; j++){
			if((*charPtr & (0b10000000 >> (j - x))) > 0)
				*(uint32_t*)(vram + j + (i * (m_MainFramebuffer->pitch / 4))) = color;
		}
		charPtr++;
	}
}

void Framebuffer::DrawString(const char* string, uint32_t X, uint32_t Y, Color color){
	uint32_t CurrentX = X, CurrentY = Y;
	while(*string != NULL){
		DrawCharacter(*string, CurrentX, CurrentY, color);
		CurrentX += 8;
		if(CurrentX > m_MainFramebuffer->width){
			CurrentX = 0;
			CurrentY += 16;
		}
		string++;
	}
}