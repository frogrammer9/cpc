#ifndef CPC_HSTRING
#define CPC_HSTRING

#include <stdint.h>

typedef struct {
	uint64_t size;
	uint64_t reserved;
	char* str;
} hstring;

void hstr_create(hstring* str);
void hstr_set(hstring* str, char* text);
void hstr_destroy(hstring* str);
void hstr_addc(hstring* str, char newChar);
void hstr_reserve(hstring* str, uint64_t size);
void hstr_copy(hstring* str, hstring* source);
void hstr_clear(hstring* str);

#endif
