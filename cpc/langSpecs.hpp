#ifndef LANGSPECS_HPP
#define LANGSPECS_HPP

#include <array>
#include <cstdint>
#include <string>
#include <iostream>
#include <utility>

namespace cpc {

	struct Token {
		enum class TokenType {
			none,
			//operators 
			OP_empty, OP_plus, OP_minus, OP_star, OP_slash, OP_ampersand, OP_less, OP_greater, OP_dot, OP_comma, OP_assign, OP_equals, OP_diffrent, 
			OP_lessEqual, OP_greaterEqual, OP_increment, OP_decrement,
			OP_and, OP_logicAnd, OP_or, OP_logicOr, OP_xor, OP_logicXor, OP_exclemation, OP_plusAssign, OP_minusAssing, OP_starAssign, OP_slashAssign,
			//keywords
			KW_empty, KW_int8, KW_int16, KW_int32, KW_int64, KW_uint8, KW_uint16, KW_uint32, KW_uint64, KW_float, KW_double, KW_bool, KW_for, KW_while, 
			KW_if, KW_else, KW_char, KW_static, KW_const,
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
				std::cout << "[None (This is bad)]\n"; break;
			case TokenType::intLiteral:
				std::cout << "[Int Literal] : " << intVal << "\n"; break;
			case TokenType::uintLiteral:
				std::cout << "[Uint Literal] : " << uintVal << "\n"; break;
			case TokenType::OP_increment:
				std::cout << "[Operator increment (++)]" << "\n"; break;
			case TokenType::OP_plus:
				std::cout << "[Operator plus (+)]" << "\n"; break;
			case TokenType::OP_minus:
				std::cout << "[Operator minus (-)]" << "\n"; break;
			case TokenType::OP_star:
				std::cout << "[Operator star (*)]" << "\n"; break;
			case TokenType::OP_slash:
				std::cout << "[Operator slash (/)]" << "\n"; break;
			case TokenType::OP_assign:
				std::cout << "[Operator assign (=)]" << "\n"; break;
			case TokenType::OP_ampersand:
				std::cout << "[Operator ampersand (&)]" << "\n"; break;
			case TokenType::OP_equals:
				std::cout << "[Operator equal (==)]" << "\n"; break;
			case TokenType::OP_diffrent:
				std::cout << "[Operator diffrent (!=)]" << "\n"; break;
			case TokenType::OP_less:
				std::cout << "[Operator less (<)]" << "\n"; break;
			case TokenType::OP_greater:
				std::cout << "[Operator greater (>)]" << "\n"; break;
			case TokenType::OP_lessEqual:
				std::cout << "[Operator less or equal (<=)]" << "\n"; break;
			case TokenType::OP_greaterEqual:
				std::cout << "[Operator greater or equal (>=)]" << "\n"; break;
			case TokenType::OP_decrement:
				std::cout << "[Operator decrement (--)]" << "\n"; break;
			case TokenType::OP_and:
				std::cout << "[Operator and (&)]" << "\n"; break;
			case TokenType::OP_logicAnd:
				std::cout << "[Operator logic and (&&)]" << "\n"; break;
			case TokenType::OP_or:
				std::cout << "[Operator or (|)]" << "\n"; break;
			case TokenType::OP_logicOr:
				std::cout << "[Operator logic or (||)]" << "\n"; break;
			case TokenType::OP_xor:
				std::cout << "[Operator xor (^)]" << "\n"; break;
			case TokenType::OP_logicXor:
				std::cout << "[Operator logic xor (^^)]" << "\n"; break;
			case TokenType::OP_dot:
				std::cout << "[Operator dot (.)]" << "\n"; break;
			case TokenType::OP_comma:
				std::cout << "[Operator comma (,)]" << "\n"; break;
			case TokenType::OP_plusAssign:
				std::cout << "[Operator add and assign (+=)]" << "\n"; break;
			case TokenType::OP_minusAssing:
				std::cout << "[Operator subtract and assin(-=)]" << "\n"; break;
			case TokenType::OP_starAssign:
				std::cout << "[Operator multiply and assign (*=)]" << "\n"; break;
			case TokenType::OP_slashAssign:
				std::cout << "[Operator divide and assign (/=)]" << "\n"; break;
			default:
				std::cout << "Not yet implemented (" << (int)type << ")\n";
			}
		}
	};

	static auto Keywords = std::to_array<std::pair<const wchar_t*, cpc::Token::TokenType>>({
	std::make_pair(L"int",		cpc::Token::TokenType::KW_int32 ), 
	std::make_pair(L"int8",		cpc::Token::TokenType::KW_int8), 
	std::make_pair(L"int16",	cpc::Token::TokenType::KW_int16),
	std::make_pair(L"int32",	cpc::Token::TokenType::KW_int32),
	std::make_pair(L"int64",	cpc::Token::TokenType::KW_int64), 
	std::make_pair(L"float",	cpc::Token::TokenType::KW_float), 
	std::make_pair(L"double",	cpc::Token::TokenType::KW_double), 
	std::make_pair(L"bool",		cpc::Token::TokenType::KW_bool), 
	std::make_pair(L"char",		cpc::Token::TokenType::KW_char), 
	std::make_pair(L"uint",		cpc::Token::TokenType::KW_uint32), 
	std::make_pair(L"uint8",	cpc::Token::TokenType::KW_uint8), 
	std::make_pair(L"uint16",	cpc::Token::TokenType::KW_uint16), 
	std::make_pair(L"uint32",	cpc::Token::TokenType::KW_uint32), 
	std::make_pair(L"uint64",	cpc::Token::TokenType::KW_uint64), 
	std::make_pair(L"const",	cpc::Token::TokenType::KW_const),
	std::make_pair(L"for",		cpc::Token::TokenType::KW_for), 
	std::make_pair(L"while",	cpc::Token::TokenType::KW_while), 
	std::make_pair(L"if",		cpc::Token::TokenType::KW_if), 
	std::make_pair(L"else",		cpc::Token::TokenType::KW_else), 
	std::make_pair(L"static",	cpc::Token::TokenType::KW_static), 
	std::make_pair(L"class",	cpc::Token::TokenType::KW_class)
	});

	static constexpr auto Operators = std::to_array<std::pair<const wchar_t*, cpc::Token::TokenType>> ({
	std::make_pair(L"+",	cpc::Token::TokenType::OP_plus),
	std::make_pair(L"-",	cpc::Token::TokenType::OP_minus),
	std::make_pair(L"++",	cpc::Token::TokenType::OP_increment),
	std::make_pair(L"--",	cpc::Token::TokenType::OP_decrement),
	std::make_pair(L"=",	cpc::Token::TokenType::OP_assign),
	std::make_pair(L"+=",	cpc::Token::TokenType::OP_plusAssign),
	std::make_pair(L"-=",	cpc::Token::TokenType::OP_minusAssing),
	std::make_pair(L"*",	cpc::Token::TokenType::OP_star),
	std::make_pair(L"*=",	cpc::Token::TokenType::OP_starAssign),
	std::make_pair(L"/",	cpc::Token::TokenType::OP_slash),
	std::make_pair(L"/=",	cpc::Token::TokenType::OP_slashAssign),
	std::make_pair(L"==",	cpc::Token::TokenType::OP_equals),
	std::make_pair(L"!=",	cpc::Token::TokenType::OP_diffrent),
	std::make_pair(L">=",	cpc::Token::TokenType::OP_greaterEqual),
	std::make_pair(L"<=",	cpc::Token::TokenType::OP_lessEqual),
	std::make_pair(L"<",	cpc::Token::TokenType::OP_less),
	std::make_pair(L">",	cpc::Token::TokenType::OP_greater),
	std::make_pair(L",",	cpc::Token::TokenType::OP_comma),
	std::make_pair(L".",	cpc::Token::TokenType::OP_dot),
	std::make_pair(L"&",	cpc::Token::TokenType::OP_and),
	std::make_pair(L"|",	cpc::Token::TokenType::OP_or),
	std::make_pair(L"^",	cpc::Token::TokenType::OP_xor),
	std::make_pair(L"&&",	cpc::Token::TokenType::OP_logicAnd),
	std::make_pair(L"||",	cpc::Token::TokenType::OP_logicOr),
	std::make_pair(L"^^",	cpc::Token::TokenType::OP_logicXor),
	std::make_pair(L"&",	cpc::Token::TokenType::OP_ampersand),
	});
}

#endif 
