#include "hstring.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void hstr_create(hstring* str) {
	str->str = NULL;
	str->size = 0;
	str->reserved = 0;
}
void hstr_set(hstring* str, char* text) {
	uint64_t size = strlen(text);
	str->size = size;
	str->reserved = size;
	char* newArr = calloc(size + 1, sizeof(char));
	strcpy(newArr, text);
	if(str->str) free(str->str);
	str->str = newArr;
}
void hstr_destroy(hstring* str) {
	if(str->str) free(str->str);
	str->str = NULL;
	str->size = 0;
	str->reserved = 0;
}
void hstr_addc(hstring* str, char newChar) {
	if(str->reserved < str->size + 2) hstr_reserve(str, 1);
	str->str[str->size] = newChar;
	str->str[++str->size] = '\0';
}
void hstr_reserve(hstring* str, uint64_t size) {
	char* newArr = (char*)calloc(str->reserved + size, sizeof(char));
	if(str->str != NULL) { 
		strcpy(newArr, str->str);
		free(str->str);
	}
	str->str = newArr;
	str->reserved += size;
}
void hstr_copy(hstring* str, hstring* source) {
	if(str->str) free(str->str);
	char* newArr = (char*)calloc(source->size + 1, sizeof(char));
	strcpy(newArr, source->str);
	str->size = source->size;
	str->reserved = str->size + 1;
	str->str = newArr;
}
void hstr_clear(hstring* str) {
	str->size = 0;
	if(str->str) memset((void*)str->str, 0, str->reserved);
}
