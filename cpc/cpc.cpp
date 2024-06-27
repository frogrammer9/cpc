#include <iostream>
#include <vector>
#include "lexer.hpp"

#ifdef _WIN32
#define CONSOLE_PAUSE_WINDOWS system("PAUSE")
#define SRC "../testfiles/test.txt"
#else
#define CONSOLE_PAUSE_WINDOWS 
#define SRC "testfiles/test.txt"
#endif // DEBUG

int main()
{
	std::vector<cpc::Token> tokens;
	try {
		tokens = cpc::Lexer::tokenise(SRC);
	}
	catch (cpc::LexerError& error) { error.log(); }
	for (auto a : tokens) {
		a.log();
	}

	CONSOLE_PAUSE_WINDOWS;
	return 0;
}
