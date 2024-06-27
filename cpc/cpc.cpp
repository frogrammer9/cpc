#include <iostream>
#include <vector>
#include "lexer.hpp"

#ifdef _WIN32
#define CONSOLE_PAUSE_WINDOWS system(PAUSE)
#else
#define CONSOLE_PAUSE_WINDOWS 
#endif // DEBUG

int main()
{
	std::vector<cpc::Token> tokens;
	try {
		tokens = cpc::Lexer::tokenise("testfiles/test.txt");
	}
	catch (cpc::LexerError& error) { error.log(); }
	for (auto a : tokens) {
		a.log();
	}

	CONSOLE_PAUSE_WINDOWS;
	return 0;
}
