#include "lexer/lexer.h"
#include <stdio.h>
#include <stdlib.h>

char* readfile(const char* filename) {
	FILE* f = fopen(filename, "rb");
	if(f == NULL) { printf("failed to open file\n"); return NULL; }
	fseek(f, 0, SEEK_END);
	uint64_t fsize = ftell(f);
	rewind(f);
	char* buffer = (char*)calloc(fsize + 1, sizeof(char));
	(void)fread(buffer, sizeof(char), fsize, f);
	buffer[fsize] = 0;
	fclose(f);
	return buffer;
}

int main(int argc, char* argv[]) {

	char* code = readfile("test.cp");
	if(code == NULL) exit(1);
	printf("This is the code: %s", code);
	TokenList tl = cpc_tokenise(code);

	for(int i = 0; i < tl.size; ++i) {
		printToken(&tl.list[i]);
	}

	return 0;
}
