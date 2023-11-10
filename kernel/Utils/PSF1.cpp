#include "PSF1.h"
#include "ModuleManager.h"
#include <stddef.h>

bool LoadFont(PSF1Font* font, const char* path){
	limine_file* file = GlobalFileManager.GetFile(path);
	if(!file) return false;
	font->Header = (PSF1Header*)file->address;
	if(font->Header->MagicBytes[0] != MAGIC_BYTE_0) return false;
	if(font->Header->MagicBytes[1] != MAGIC_BYTE_1) return false;
	font->GlyphData = (void*)((uint64_t)file->address + sizeof(PSF1Header));
	if(font->GlyphData == NULL) return false;
	return true;
}