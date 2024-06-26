#ifndef LANGSPECS_HPP
#define LANGSPECS_HPP

#include <array>
#include <string>
#include <iostream>

namespace cpc {

	struct Token {
		enum class TokenType {
			none,
			//operators 
			OP_plus, OP_minus, OP_star, OP_slash, OP_ampersand, OP_less, OP_greater, OP_dot, OP_comma, OP_assign, OP_equals, OP_diffrent, OP_lessEqual, OP_greaterEqual,
			OP_and, OP_doubleAnd, OP_or, OP_doubleOr, OP_xor, OP_doubleXor, OP_exclemation, 
			//keywords
			KW_int8, KW_int16, KW_int32, KW_int64, KW_uint8, KW_uint16, KW_uint32, KW_uint64, KW_float, KW_double, KW_bool, KW_for, KW_while, KW_if, KW_else, KW_char, KW_static,
			KW_class, 
			//internal
			//todo implement better literal type system
			stringLiteral, intLiteral, uintLiteral, floatLiteral,
		};
		TokenType type;
		union {;
			int64_t intVal;
			uint64_t uintVal;
			double floatVal;
		};
		std::wstring stringVal;
		void log() const {
			switch (type) {
			case TokenType::none:
				std::cout << "[None (This is bad)]\n";
				break;
			case TokenType::intLiteral:
				std::cout << "[Int Literal] : " << intVal << "\n";
				break;
			case TokenType::uintLiteral:
				std::cout << "[Uint Literal] : " << uintVal << "\n";
				break;
			default:
				std::cout << "Not yet implemented\n";
			}
		}
	};

	constexpr std::array<const char*, 21> Keywords = {
		"int", "int8", "int16", "int32", "int64", "float", "double", "bool", "char", "uint", "uint8", "uint16", "uint32", "uint64", "const"
		"for", "while", "if", "else", "static"
		"class"
	};
	constexpr std::array<const char*, 21> Operators = {
		"+", "-", "++", "--", "=", "+=", "-=", "*", "*=", "/", "/="
	};
}

#endif 