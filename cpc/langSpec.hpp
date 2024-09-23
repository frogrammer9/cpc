#ifndef CPC_LANGSPEC
#define CPC_LANGSPEC

#include <map>
#include <string>
namespace cpc::ls {

enum TokenType {
	FOR, WHILE, DO, I8, I16, I32, I64, U8, U16, U32, U64, F32, F64, BOOL, TRUE, FALSE, NULLPTR, RETURN, IF, ELSE, AUTO, BREAK, CASE, SWITCH, CONST, DEFAULT, 
	CONTINUE, ENUM, STATIC, STRUCT, CLASS, UNION, VOID, SIZEOF, 
	//symbols
	OPEN_PARENTHESES, CLOSE_PARENTHESES, OPEN_BRACKETS, CLOSE_BRACKETS, OPEN_BRACES, CLOSE_BRACES, PLUS, DASH, STAR, PERCENT,
	SLASH, INCREMENT, DECREMENT, ARROW, DOT, COMMA, SEMICOLON, COLON, ASSIGMENT, PLUS_ASSIGN, MINUS_ASSIGN, 
	MULTIPLY_ASSIGN, DEVIDE_ASSIGN, MODULO_ASSIGN, OR_ASSIGN, AND_ASSIGN, XOR_ASSIGN, SHL_ASSIGN, SHR_ASSIGN, TYLDA, VBAR, ANDPERCENT, CARET, EXCLEMATION, 
	LOGIC_AND, LOGIC_OR, LOGIC_XOR, QUESTION, EQUAL, DIFFERENT, LESS, GREATER, LESS_OR_EQUAL, GREATER_OR_EQUAL, SHL, SHR,
	COMMENT_LINE, COMMENT_BLOCK_BEGIN, COMMENT_BLOCK_END,
	//LITERALS
	INT_LITERAL, FLOAT_LITERAL, STRING_LITERAL, IDENT,
	NONE,
};

const inline static std::wstring comment_line_symbol = L"//";
const inline static std::wstring comment_block_begin_symbol = L"/*";
const inline static std::wstring comment_block_end_symbol = L"*/";

const inline static std::map<std::wstring, TokenType> keywords = {
	{L"for",		TokenType::FOR},		{L"while",		TokenType::WHILE},		{L"do",			TokenType::DO},
	{L"int",		TokenType::I32}, 		{L"float",		TokenType::F32}, 		{L"i8",			TokenType::I8},
	{L"i16", 		TokenType::I16}, 		{L"i32", 		TokenType::I32}, 		{L"i64", 		TokenType::I64},
	{L"u8", 		TokenType::U8}, 		{L"u16", 		TokenType::U16}, 		{L"u32", 		TokenType::U32},
	{L"u64", 		TokenType::U64}, 		{L"f32", 		TokenType::F32}, 		{L"f64", 		TokenType::F64},
	{L"bool", 		TokenType::BOOL}, 		{L"true", 		TokenType::TRUE}, 		{L"false", 		TokenType::FALSE},
	{L"nullptr", 	TokenType::NULLPTR}, 	{L"return", 	TokenType::RETURN}, 	{L"if", 		TokenType::IF},
	{L"else", 		TokenType::ELSE},		{L"auto", 		TokenType::AUTO}, 		{L"break", 		TokenType::BREAK},
	{L"case", 		TokenType::CASE}, 		{L"switch", 	TokenType::SWITCH}, 	{L"const", 		TokenType::CONST},
	{L"default", 	TokenType::DEFAULT}, 	{L"continue", 	TokenType::CONTINUE}, 	{L"enum", 		TokenType::ENUM},
	{L"static", 	TokenType::STATIC}, 	{L"struct", 	TokenType::STRUCT}, 	{L"class", 		TokenType::CLASS},
	{L"union", 		TokenType::UNION},		{L"void", 		TokenType::VOID},		{L"sizeof", 	TokenType::SIZEOF},
};

const inline static std::map<std::wstring, TokenType> symbols = {
	{L"(",		TokenType::OPEN_PARENTHESES},	{L")",		TokenType::CLOSE_PARENTHESES},	{L"{",		TokenType::OPEN_BRACES},
	{L"}",		TokenType::CLOSE_BRACES}, 		{L"[",		TokenType::OPEN_BRACKETS}, 		{L"]",		TokenType::CLOSE_BRACKETS},
	{L"+", 		TokenType::PLUS},				{L"-", 		TokenType::DASH},				{L"*", 		TokenType::STAR},
	{L"%", 		TokenType::PERCENT},			{L"/", 		TokenType::DASH},				{L"++",		TokenType::INCREMENT},
	{L"--", 	TokenType::DECREMENT},			{L"->", 	TokenType::ARROW},				{L".", 		TokenType::DOT},
	{L",",		TokenType::COMMA},				{L";",		TokenType::SEMICOLON},			{L":", 		TokenType::COLON},
	{L"=", 		TokenType::ASSIGMENT},			{L"+=", 	TokenType::PLUS_ASSIGN}, 		{L"-=",		TokenType::MINUS_ASSIGN},
	{L"*=", 	TokenType::MULTIPLY_ASSIGN}, 	{L"/=", 	TokenType::DEVIDE_ASSIGN}, 		{L"%=",		TokenType::MODULO_ASSIGN},
	{L"|=", 	TokenType::OR_ASSIGN},			{L"&=", 	TokenType::AND_ASSIGN}, 		{L"^=",		TokenType::XOR_ASSIGN},
	{L"<<=", 	TokenType::SHL_ASSIGN},			{L">>=", 	TokenType::SHR_ASSIGN}, 		{L"~", 		TokenType::TYLDA},
	{L"|", 		TokenType::VBAR},				{L"&", 		TokenType::ANDPERCENT},			{L"^",		TokenType::CARET},
	{L"!", 		TokenType::EXCLEMATION},		{L"&&", 	TokenType::LOGIC_AND},			{L"||", 	TokenType::LOGIC_OR},
	{L"^^", 	TokenType::LOGIC_XOR},			{L"?", 		TokenType::QUESTION},			{L"==", 	TokenType::EQUAL},
	{L"!=", 	TokenType::DIFFERENT},			{L"<", 		TokenType::LESS},				{L">",		TokenType::GREATER},
	{L"<=", 	TokenType::LESS_OR_EQUAL},		{L">=", 	TokenType::GREATER},			{L"<<", 	TokenType::SHL},
	{L">>", 	TokenType::SHR},
	{comment_line_symbol,			TokenType::COMMENT_LINE},
	{comment_block_begin_symbol, 	TokenType::COMMENT_BLOCK_BEGIN},
	{comment_block_end_symbol,	 	TokenType::COMMENT_BLOCK_END},
};

}

#endif

