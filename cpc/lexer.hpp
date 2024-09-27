#ifndef CPC_LEXER
#define CPC_LEXER

#include <optional>
#include <string>
#include <vector>
#include "langSpec.hpp"

namespace cpc {

struct Token {
	Token(ls::TokenType t, const std::wstring val) : type(t), value(val) {}
	Token(ls::TokenType t) : type(t), value(std::nullopt) {}

	ls::TokenType type;
	std::optional<std::wstring> value;
};

std::vector<Token> tokenise(const std::wstring& code); 

}
std::wostream& operator<<(std::wostream& os, const cpc::Token& tok);
std::wostream& operator<<(std::wostream& os, const cpc::ls::TokenType& type);

#endif
