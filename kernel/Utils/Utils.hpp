#pragma once
#include <stdint.h>
#include <stddef.h>

#define White Color(255, 255, 255)

class iVec2 {
public:
	static iVec2 ZERO;

	int X;
	int Y;

	iVec2() = default;
	iVec2(int X, int Y)
		: X(X), Y(Y) {}

	iVec2& operator-(const iVec2 & other) {
		//if (this == &other) return iVec2::ZERO;
		this->X -= other.X;
		this->Y -= other.Y;
		return *this;
	}

	friend iVec2& operator+=(iVec2& lhs, const iVec2& rhs) {
		lhs.X += rhs.X;
		lhs.Y += rhs.Y;
		return lhs;
	}

	friend iVec2& operator+(iVec2& lhs, const iVec2& rhs) {
		lhs += rhs;
		return lhs;
	}
};

inline iVec2 iVec2::ZERO = iVec2(0, 0);

class Color {
public:
	uint8_t r, g, b;

	Color(uint8_t r, uint8_t g, uint8_t b)
		: r(r), g(g), b(b) {}

	operator uint32_t() {
		return ((uint32_t)(r & 0xff) << 16) + ((uint32_t)(g & 0xff) << 8) + (uint32_t)(b & 0xff);
	}
};

inline uint64_t strlen(const char* str) {
    uint64_t size = 0;
    while (*str != NULL) {
        size++;
        *str++;
    }
    return size;
}


inline bool strcmp(const char* first, const char* second){
	if(first[0] != second[0]) return false;
	size_t firstSize = strlen(first);
	if(firstSize != strlen(second)) return false;
	for(size_t i = 0; i < firstSize; i++)
		if(first[i] != second[i]) return false;
	return true;
}

inline bool endswith(const char *str, const char *end)
{
    const char *_str = str;
    const char *_end = end;

    while (*str != 0)
        str++;
    str--;

    while (*end != 0)
        end++;
    end--;

    while (true)
    {
        if (*str != *end)
            return false;

        str--;
        end--;

        if (end == _end || (str == _str && end == _end))
            return true;

        if (str == _str)
            return false;
    }

    return true;
}

inline char str[100]; // used in all to*** functions
inline char* tostr(uint64_t num){
	if(num == 0){
		str[0] = '0';
		return str;
	}

    uint8_t size;
    uint64_t sizeTest = num;
    while (sizeTest / 10 > 0){
        sizeTest /= 10;
        size++;
    }

	uint8_t index;
	while(num != 0){
		str[size - index] = (char)(48 + (num % 10));
		num /= 10;
		index++;
	}
	return str;
}

inline char* tohex(uint64_t num){
    uint64_t* valPtr = &num;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 8 * 2 - 1;
    for (uint8_t i = 0; i < size; i++){
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        str[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        str[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }
    str[size + 1] = 0;
    return str;
}