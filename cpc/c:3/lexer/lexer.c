#include "lexer.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void printToken(const Token* tok) {
	const char* names[] = {
		"[KEYWORD] - for", "[KEYWORD] - while", "[KEYWORD] - do", "[KEYWORD] - int", "[KEYWORD] - i8", "[KEYWORD] - i16", "[KEYWORD] - i32", "[KEYWORD] - i64",
		"[KEYWORD] - u8", "[KEYWORD] - u16", "[KEYWORD] - u32", "[KEYWORD] - u64", "[KEYWORD] - f32", "[KEYWORD] - f64", "[KEYWORD] - nullptr",
		"[KEYWORD] - return", "[KEYWORD] - if", "[KEYWORD] - else", "[KEYWORD] - auto", "[KEYWORD] - break", "[KEYWORD] - case", "[KEYWORD] - switch",
		"[KEYWORD] - const", "[KEYWORD] - default", 	"[KEYWORD] - continue", "[KEYWORD] - enum", "[KEYWORD] - static", "[KEYWORD] - struct",
		"[KEYWORD] - class", "[KEYWORD] - union", "[KEYWORD] - void", "[KEYWORD] - sizeof", "[SYMBOL] - (", "[SYMBOL] - )", "[SYMBOL] - [", "[SYMBOL] - ]",
		"[SYMBOL] - {",
		"[SYMBOL] - }", "[SYMBOL] - +", "[SYMBOL] - -", "[SYMBOL] - *", "[SYMBOL] - %", "[SYMBOL] - /", "[SYMBOL] - //", "[SYMBOL] - /*", "[SYMBOL] - */",
		"[SYMBOL] - ++", "[SYMBOL] - --", "[SYMBOL] - ->", "[SYMBOL] - .", "[SYMBOL] - ,", "[SYMBOL] - ;", "[SYMBOL] - :", "[SYMBOL] - =", "[SYMBOL] - +=",
		"[SYMBOL] - -=", "[SYMBOL] - *=", "[SYMBOL] - /=", "[SYMBOL] - %=", "[SYMBOL] - |=", "[SYMBOL] - &=", "[SYMBOL] - ^=", "[SYMBOL] - <<=",
		"[SYMBOL] - >>=","[SYMBOL] - ~", "[SYMBOL] - &", "[SYMBOL] - !", "[SYMBOL] - &&", "[SYMBOL] - ||", "[SYMBOL] - ^^", "[SYMBOL] - ?", "[SYMBOL] - ==",
		"[SYMBOL] - !=", "[SYMBOL] - <", "[SYMBOL] - >", "[SYMBOL] - <=", "[SYMBOL] - >=" 
	};
	switch(tok->type) {
		case UINT_LITERAL:
		printf("[UINT LITERAL] : %lu\n", tok->value.ui);
		break;
		case INT_LITERAL:
		printf("[INT LITERAL] : %ld\n", tok->value.ui);
		break;
		case FLOAT_LITERAL:
		printf("[FLOAT LITERAL] : %f\n", tok->value.f);
		break;
		case STRING_LITERAL:
		printf("[STRING LITERAL] : %s\n", tok->value.s.str);
		break;
		case IDENT:
		printf("[IDENT] : %s\n", tok->value.s.str);
		break;
		default:
		printf("%s\n", names[tok->type]);
	}
}

void tkl_addToken(TokenList* list, Token* token) {
	if(list->size == list->reserved) {
		Token* newArr = (Token*)calloc(list->reserved + 10, sizeof(Token));
		if(list->list) { 
			memcpy(newArr, list->list, list->size * sizeof(Token));
			free(list->list);
		}
		list->reserved += 10;
		list->list = newArr;
	}
	list->list[list->size++] = *token;
}
void tkl_destroy(TokenList* list) {
	if(list->list) free(list->list);
}

#define CASE(t1, t2) case t1: type = t2; break
#define NASCASE default: state = complete; continue
#define SSC_CASE(t1, t2) case t1: hstr_addc(&buffer, t2); state = stringLiteral; break
#define KW_CASE_I(t1, t2) if(strcmp(buffer.str, t1) == 0) tok.type = t2
#define KW_CASE(t1, t2) else if(strcmp(buffer.str, t1) == 0) tok.type = t2

TokenList cpc_tokenise(const char* code) {
	enum State {
		none, minusAnalizer, symbol, uintLiteral, intLiteral, floatLiteral, stringLiteral, stringSpecialChar, ident, complete, 
		commentLine, commentBlock, commentBlockExit
	} state = none;
	char prev;
	Token tok;
	tok.type = NONE;
	TokenType type;
	TokenList tokensOut;
	tokensOut.size = 0;
	tokensOut.reserved = 0;
	tokensOut.list = NULL;
	hstring buffer;
	hstr_create(&buffer);

	for(char prev = '\0'; (prev = *code);) {
		switch(state) {
			case none:
				if(isspace(*code)) code++;
				else if(isalpha(*code)) state = ident;
				else if(*code == '\"') { state = stringLiteral; code++; }
				else if(isdigit(*code)) { state = uintLiteral; }
				else if(*code == '-') { state = minusAnalizer; code++; }
				else if(ispunct(*code)) state = symbol;
			break;
			case minusAnalizer:
				if(isdigit(*code)) { state = intLiteral; hstr_addc(&buffer, '-'); continue; }
				type = DASH;
				if(ispunct(*code)) state = symbol;
				else state = complete;
			break;
			case symbol:
				if(isdigit(*code) && type == DASH) {
					hstr_addc(&buffer, '-');
					state = intLiteral;
					continue;
				}
				if(type == SLASH && *code == '/') { state = commentLine; type = NONE; continue; }
				if(type == SLASH && *code == '*') { state = commentBlock; type = NONE; continue; }
				switch(*code) {
					case '+': // + ++
						switch(type) {
							CASE(NONE, PLUS);
							CASE(PLUS, INCREMENT);
							NASCASE;
						} break;
					case '-': // - --
						switch(type) {
							CASE(NONE, DASH);
							CASE(DASH, DECREMENT);
							NASCASE;
						} break;
					case '=':
						switch(type) { // = += -= *= %= /= |= &= ^= <<= >>= == != <= >=
							CASE(NONE, ASSIGMENT);
							CASE(PLUS, PLUS_ASSIGN);
							CASE(DASH, MINUS_ASSIGN);
							CASE(STAR, MULTIPLY_ASSIGN);
							CASE(SLASH, DEVIDE_ASSIGN);
							CASE(VBAR, OR_ASSIGN);
							CASE(ANDPERCENT, AND_ASSIGN);
							CASE(CARET, XOR_ASSIGN);
							CASE(SHL, SHL_ASSIGN);
							CASE(SHR, SHR_ASSIGN);
							CASE(ASSIGMENT, EQUAL);
							CASE(EXCLEMATION, DIFFERENT);
							CASE(LESS, LESS_OR_EQUAL);
							CASE(GREATER, GREATER_OR_EQUAL);
							NASCASE;
						} break;
					case '<': // < <<
						switch(type) {
							CASE(NONE, LESS);
							CASE(LESS, SHL);
							NASCASE;
						} break;
					case '>': // > >> ->
						switch(type) {
							CASE(NONE, GREATER);
							CASE(GREATER, SHR);
							CASE(DASH, ARROW);
							NASCASE;
						} break;
					case '|': // | ||
						switch(type) {
							CASE(NONE, VBAR);
							CASE(VBAR, LOGIC_OR);
							NASCASE;
						} break;
					case '&': // & &&
						switch(type) {
							CASE(NONE, ANDPERCENT);
							CASE(ANDPERCENT, LOGIC_AND);
							NASCASE;
						} break;
					case '^': // ^ ^^
						switch(type) {
							CASE(NONE, CARET);
							CASE(CARET, LOGIC_XOR);
							NASCASE;
						} break;
					CASE('!', EXCLEMATION);
					CASE('~', TYLDA);
					CASE('*', STAR);
					CASE('/', SLASH);
					CASE('%', PERCENT);
					CASE('?', QUESTION);
					CASE(':', COLON);
					CASE(';', SEMICOLON);
					CASE('(', OPEN_PARENTHESES);
					CASE(')', CLOSE_PARENTHESES);
					CASE('{', OPEN_BRACES);
					CASE('}', CLOSE_BRACES);
					CASE('[', OPEN_BRACKETS);
					CASE(']', CLOSE_BRACKETS);
					CASE('.', DOT);
					CASE(',', COMMA);
					NASCASE;
				}
				code++;
			break;
			case uintLiteral:
				if(*code == '.') { state = floatLiteral; hstr_addc(&buffer, '.'); code++; continue; }
				if(!isxdigit(*code)) { state = complete; type = UINT_LITERAL; continue; }
				hstr_addc(&buffer, *code);
				code++;
			break;
			case intLiteral:
				if(*code == '.') { state = floatLiteral; hstr_addc(&buffer, '.'); code++; continue; }
				if(!isxdigit(*code)) { state = complete; type = INT_LITERAL; continue; }
				hstr_addc(&buffer, *code);
				code++;
			break;
			case floatLiteral:
				if(!isdigit(*code)) { state = complete; type = FLOAT_LITERAL; continue; }
				hstr_addc(&buffer, *code);
				code++;
			break;
			case stringLiteral:
				if(*code == '\"') { state = complete; code++; type = STRING_LITERAL; continue; }
				if(*code == '\\') { state = stringSpecialChar; code++; continue; }
				hstr_addc(&buffer, *code);
				code++;
			break;
			case stringSpecialChar:
				switch(*code) {
					SSC_CASE('\"', '\"');
					SSC_CASE('\'', '\'');
					SSC_CASE('\\', '\\');
					SSC_CASE('a', '\a');
					SSC_CASE('b', '\b');
					SSC_CASE('e', '\e');
					SSC_CASE('f', '\f');
					SSC_CASE('n', '\n');
					SSC_CASE('r', '\r');
					SSC_CASE('t', '\t');
					SSC_CASE('v', '\v');
					default:
						//TODO: error
					break;
				}
			break;
			case ident:
				if(!isalpha(*code) && !isdigit(*code)) { type = IDENT; state = complete; continue; }
				hstr_addc(&buffer, *code);
				code++;
			break;
			case complete:
				tok.type = type;
				switch(type) {
					case UINT_LITERAL:
						tok.value.ui = strtoull(buffer.str, NULL, 10);
					break;
					case INT_LITERAL:
						tok.value.i = strtoll(buffer.str, NULL, 10);
					break;
					case FLOAT_LITERAL:
						tok.value.f = strtod(buffer.str, NULL);
					break;
					case STRING_LITERAL: 
						hstr_create(&tok.value.s);
						hstr_copy(&tok.value.s, &buffer);
					break;
					case IDENT:
						KW_CASE_I("for", FOR);
						KW_CASE("while", WHILE);
						KW_CASE("do", DO);
						KW_CASE("int", INT);
						KW_CASE("i8", I8);
						KW_CASE("i16", I16);
						KW_CASE("i32", I32);
						KW_CASE("i64", I64);
						KW_CASE("u8", U8);
						KW_CASE("u16", U16);
						KW_CASE("u32", U32);
						KW_CASE("u64", U64);
						KW_CASE("f32", F32);
						KW_CASE("f64", F64);
						KW_CASE("nullptr", NULLPTR);
						KW_CASE("return", RETURN);
						KW_CASE("if", IF);
						KW_CASE("else", ELSE);
						KW_CASE("auto", AUTO);
						KW_CASE("break", BREAK);
						KW_CASE("case", CASE);
						KW_CASE("switch", SWITCH);
						KW_CASE("const", CONST);
						KW_CASE("default", DEFAULT);
						KW_CASE("continue", CONTINUE);
						KW_CASE("enum", ENUM);
						KW_CASE("static", STATIC);
						KW_CASE("struct", STRUCT);
						KW_CASE("class", CLASS);
						KW_CASE("union", UNION);
						KW_CASE("void", VOID);
						KW_CASE("sizeof", SIZEOF);
					else {
						hstr_create(&tok.value.s);
						hstr_copy(&tok.value.s, &buffer);
						tok.type = IDENT;
					}
					break;
					default:
						tok.value.ui = 0;
					break;
				}
				tkl_addToken(&tokensOut, &tok);
				hstr_clear(&buffer);
				type = NONE;
				state = none;
			break;
			case commentLine:
				if(*code == '\n' || *code == '\r') state = none;
			break;
			case commentBlock:
				if(*code == '*') state = commentBlockExit;
			break;
			case commentBlockExit:
				if(*code == '/') { state = none; continue; }
				state = commentBlock;
			break;
		}
	}
	return tokensOut;
}
