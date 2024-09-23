#include "lexer.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char** argv) {
	
	std::wifstream file(argv[1]);
	file.imbue(std::locale("en_US.UTF-8"));
	if(!file.is_open()) { std::wcout << "Failed to open file\n"; exit(1); }
	std::wstringstream filebuffer;
	filebuffer << file.rdbuf();

	std::wstring code = filebuffer.str();

	auto toks = cpc::tokenise(code);

	for(auto a : toks) {
		std::wcout << a << "\n";
	}

	return 0;
}
