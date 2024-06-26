#include <iostream>
#include <vector>
#include "lexer.hpp"

int main()
{
	std::vector<cpc::Token> tokens;
	try {
		tokens = cpc::Lexer::tokenise("C:/dev/cpc/test.txt");
	}
	catch (cpc::LexerError& error) { error.log(); }
	for (auto a : tokens) {
		a.log();
	}
	system("PAUSE");
	return 0;
}
