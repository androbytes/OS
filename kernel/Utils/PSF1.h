#define MAGIC_BYTE_0 0x36
#define MAGIC_BYTE_1 0x04

struct PSF1Header {
	unsigned char MagicBytes[2];
	unsigned char FontMode;
	unsigned char GlyphSize;
};

struct PSF1Font {
	PSF1Header* Header;
	void* GlyphData;
};

bool LoadFont(PSF1Font* font, const char*);