#pragma once
#include <limine.h>
#include "../Utils/Utils.hpp"
#include "../Utils/memory.h"
#include "../Utils/PSF1.h"

class Framebuffer {
private:
	volatile limine_framebuffer_request* m_Request;
	limine_framebuffer* m_MainFramebuffer;
	PSF1Font* m_ConsoleFont;
public:
	Framebuffer(volatile limine_framebuffer_request* request, PSF1Font* font);
	
	void DrawLine(iVec2, iVec2, Color col = White);
	void FillRect(iVec2 position, iVec2 size, Color col = White);
	void Clear(Color col = White);
	void DrawCharacter(const char, uint32_t X, uint32_t Y, Color col = White);
	void DrawString(const char*, uint32_t X, uint32_t Y, Color col = White);
};