#ifndef CPC_LEXER
#define CPC_LEXER

#include <stdint.h>
#include "../aux/hstring.h"

typedef enum {
	//keywords
	FOR, WHILE, DO, INT, I8, I16, I32, I64, U8, U16, U32, U64, F32, F64, NULLPTR, RETURN, IF, ELSE, AUTO, BREAK, CASE, SWITCH, CONST, DEFAULT, 
	CONTINUE, ENUM, STATIC, STRUCT, CLASS, UNION, VOID, SIZEOF, 
	//symbols
	OPEN_PARENTHESES, CLOSE_PARENTHESES, OPEN_BRACKETS, CLOSE_BRACKETS, OPEN_BRACES, CLOSE_BRACES, PLUS, DASH, STAR, PERCENT,
	SLASH, INCREMENT, DECREMENT, ARROW, DOT, COMMA, SEMICOLON, COLON, ASSIGMENT, PLUS_ASSIGN, MINUS_ASSIGN, 
	MULTIPLY_ASSIGN, DEVIDE_ASSIGN, MODULO_ASSIGN, OR_ASSIGN, AND_ASSIGN, XOR_ASSIGN, SHL_ASSIGN, SHR_ASSIGN, TYLDA, VBAR, ANDPERCENT, CARET, EXCLEMATION, 
	LOGIC_AND, LOGIC_OR, LOGIC_XOR, QUESTION, EQUAL, DIFFERENT, LESS, GREATER, LESS_OR_EQUAL, GREATER_OR_EQUAL, SHL, SHR,
	//LITERALS
	UINT_LITERAL, INT_LITERAL, FLOAT_LITERAL, IDENT, STRING_LITERAL,
	NONE
} TokenType;


typedef struct {
	TokenType type;

	union {
		uint64_t ui;
		int64_t i;
		double f;
		hstring s;
	} value;
} Token;

void printToken(const Token* tok);

typedef struct {
	uint64_t size;
	uint64_t reserved;
	Token* list;
} TokenList;

void tkl_addToken(TokenList* list, Token* token);
void tkl_destroy(TokenList* list);

TokenList cpc_tokenise(const char* code);

#endif
