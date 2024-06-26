#ifndef LEXER_HPP
#define LEXER_HPP

#include <filesystem>
#include <vector>
#include "langSpecs.hpp"

namespace cpc {

	struct LexerError : public std::exception {
		LexerError(std::pair<uint32_t, uint32_t> pos, std::string msg) : errorPos(pos), errorMsg(msg) {}
		std::pair<uint32_t, uint32_t> errorPos;
		std::string errorMsg;
		void log() { std::cerr << "Lexer: (" << errorPos.first << ", " << errorPos.second << ") : " << errorMsg << std::endl; }
	};

class Lexer
{
public:
	~Lexer();
	Lexer(const Lexer&) = delete;
	Lexer(Lexer&&) = delete;
	Lexer& operator=(const Lexer&) = delete;
	Lexer& operator=(Lexer&&) = delete;


	

	static std::vector<cpc::Token> tokenise(std::filesystem::path filepath);

private:
	Lexer();

};


#endif
}

