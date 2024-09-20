#include "lexer.hpp"
#include <iostream>

int main(int argc, char** argv) {

	const wchar_t* code = L"int a = 12; a++; ";
	auto toks = cpc::tokenise(code);
	for(auto a : toks) {
		std::wcout << a << "\n";
	}

	return 0;
}
