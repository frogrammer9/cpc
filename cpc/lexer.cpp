#include "lexer.hpp"
#include <cwctype>
#include <iostream>

//------------------------ DEBUG FUNCS
std::wostream& operator<<(std::wostream& os, const cpc::Token& tok) {
	switch(tok.type) {
		case cpc::ls::NONE:				os << "[NONE]"; break;
		case cpc::ls::INT_LITERAL:		os << "[INT LITERAL] : " << tok.value.value_or(L">invalid value<"); break;
		case cpc::ls::STRING_LITERAL:	os << "[STRING LITERAL] : " << tok.value.value_or(L">invalid value<"); break;
		case cpc::ls::IDENT:			os << "[IDENT LITERAL] : " << tok.value.value_or(L">invalid value<"); break;
		case cpc::ls::FLOAT_LITERAL:	os << "[FLOAT LITERAL] : " << tok.value.value_or(L">invalid value<"); break;
		default: {
			for(auto a : cpc::ls::symbols) {
				if(a.second == tok.type) { os << "[SYMBOL] : " << a.first; break; }
			}
			for(auto a : cpc::ls::keywords) {
				if(a.second == tok.type) { os << "[KEYWORD] : " << a.first; break; }
			}
		}
		break;
	}
	return os;
}
//------------------------ HELPER FUNCS
bool couldBeKey(const std::wstring& str, const std::map<std::wstring, cpc::ls::TokenType>& map) {
	for(const auto& elem : map)
		if(elem.first.starts_with(str) && str.size() < elem.first.size()) return true;
	return false;
}

//------------------------ FUNCS
std::vector<cpc::Token> cpc::tokenise(const std::wstring& code) {
	std::vector<Token> tokensOut;

	enum State {
		start,
		base_select, zero_ignore, minus_analizer, dot_analizer,
		int10_literal, int16_literal, int2_literal, float_literal, string_literal, char_literal,
		ident, string_special_char, string_special_char_num,
		symbol, complete,
		comment_line, comment_block
	} state = start;
	
	std::wstring buffer;
	ls::TokenType type = ls::NONE;

	std::wstring auxBuffer;
	size_t inx = 0;
	size_t lastSymbolInx = 0;

	while(inx < code.size()) {
		switch(state) {
			case start:
				if(std::iswspace(code[inx])) inx++;
				else if(code[inx] == '"') {state = string_literal; inx++;}
				else if(code[inx] == '-') {state = minus_analizer; inx++;}
				else if(code[inx] == '.') {state = dot_analizer; inx++;}
				else if(code[inx] == '0') {state = base_select; inx++;}
				else if(std::iswdigit(code[inx])) state = int10_literal;
				else if(std::iswpunct(code[inx])) state = symbol;
				else if(std::iswalpha(code[inx])) state = ident;
				else { std::cerr << "Huh? state none.\n"; exit(1); }
			break;
			case base_select:
				if(code[inx] == '0') {state = zero_ignore; inx++;}
				else if(code[inx] == '.') {state = float_literal; buffer += '.'; inx++;}
				else if(code[inx] == 'x') {state = int16_literal; inx++;}
				else if(code[inx] == 'b') {state = int2_literal; inx++;}
				else if(std::iswdigit(code[inx])) {state = int10_literal; inx++;}
				else {state = complete; type = ls::INT_LITERAL; buffer += L"d0";}
			break;
			case zero_ignore:
				if(code[inx] == '0') inx++;
				else if(code[inx] == '.') {state = float_literal; buffer += '.'; inx++;}
				else if(std::iswdigit(code[inx])) {state = int10_literal; inx++;}
				else {state = complete; type = ls::INT_LITERAL; buffer += L"d0"; inx++;}
			break;
			case minus_analizer:
				buffer += '-';
				if(code[inx] == '0') {state = base_select; inx++; }
				else if(code[inx] == '.') {state = float_literal; buffer += '.'; inx++;}
				else if(std::iswdigit(code[inx])) state = int10_literal;
				else {buffer.clear(); inx--; state = symbol;}
			break;
			case dot_analizer:
				buffer += '.';
				if(std::iswdigit(code[inx])) state = float_literal;
				else state = symbol;
			break;
				//ints are coded bnnn... where n are digits of the int and b is the base (d-10, x-16, b-2)
			case int10_literal:
				if(buffer[0] != L'd') buffer = L'd' + buffer;
				if(!std::iswdigit(code[inx])) {state = complete; type = ls::INT_LITERAL; continue;}
				buffer += code[inx++];
			break;
			case int16_literal:
				if(buffer[0] != L'x') buffer = L'x' + buffer;
				if(!std::iswxdigit(code[inx])) {state = complete; type = ls::INT_LITERAL; continue;}
				buffer += code[inx++];
			break;
			case int2_literal:
				if(buffer[0] != L'b') buffer = L'b' + buffer;
				if(code[inx] != '1' && code[inx] != '0') {state = complete; type = ls::INT_LITERAL; continue;}
				buffer += code[inx++];
			break;
			case float_literal:
				if(!std::iswdigit(code[inx])) {state = complete; continue;}
				buffer += code[inx++];
			break;
			case string_literal:
				if(code[inx] == '"') {state = complete; type = ls::STRING_LITERAL; inx++; continue;}
				if(code[inx] == '\\') {state = string_special_char; inx++; continue;}
				buffer += code[inx++];
			break;
			case char_literal:
				if(std::iswalnum(code[inx])) {buffer += code[inx]; }
			break;
			case ident:
				if(!std::iswalnum(code[inx])) {state = complete; type = ls::IDENT; continue;}
				buffer += code[inx++];
			break;
			case string_special_char:
				switch(code[inx]) {
					case '\"': buffer += '\"'; break;
					case '\'': buffer += '\''; break;
					case '\\': buffer += '\\'; break;
					case 'a': buffer += '\a'; break;
					case 'b': buffer += '\b'; break;
					case 'e': buffer += '\e'; break;
					case 'f': buffer += '\f'; break;
					case 'n': buffer += '\n'; break;
					case 'r': buffer += '\r'; break;
					case 't': buffer += '\t'; break;
					case 'v': buffer += '\v'; break; 
					default: 
						if(std::iswxdigit(code[inx])) { state = string_special_char_num; continue; }
						inx--; 
					break;
				}
				inx++; 
				state = string_literal;
			break;
			case string_special_char_num:
				if(!std::iswxdigit(code[inx]) || auxBuffer.size() == 4) {
					auto c = static_cast<wchar_t>(std::stoi(auxBuffer, nullptr, 16));
					buffer += c;
					auxBuffer.clear(); state = string_literal; continue;
				}
				auxBuffer += code[inx++];
			break;
			case symbol: {
				if(!std::iswpunct(code[inx])) {state = complete; continue;}
				std::wstring lookupStr = buffer + code[inx];
				auto itr = ls::symbols.find(lookupStr);
				if(itr != ls::symbols.end()) {type = itr->second; lastSymbolInx = inx;} //Going backwards in time is problably a bad idea
				if(!couldBeKey(lookupStr, ls::symbols)) { state = complete; inx = lastSymbolInx + 1; continue; }
				else buffer += code[inx++];
			}
			break;
			case complete: {
				if(type == ls::TokenType::COMMENT_LINE) {buffer.clear(); type = ls::NONE; state = comment_line; continue;}
				if(type == ls::TokenType::COMMENT_BLOCK_BEGIN) {buffer.clear(); type = ls::NONE; state = comment_block; continue;}
				std::map<std::wstring, ls::TokenType>::const_iterator mIt;
				if(type == ls::IDENT && (mIt = ls::keywords.find(buffer)) != ls::keywords.end()) tokensOut.emplace_back(mIt->second); 
				else if(ls::symbols.find(buffer) != ls::symbols.end()) tokensOut.emplace_back(type);
				else tokensOut.emplace_back(type, buffer);
				state = start;
				buffer.clear();
				type = ls::NONE;
			}
			break;
			case comment_line: if(code[inx] == '\n' || code[inx] == '\r') state = start; inx++; break;
			case comment_block: 
				if(buffer.size() < ls::comment_block_end_symbol.size()) buffer += code[inx++];
				else {
					std::wmemmove(&buffer[0], &buffer[1], buffer.size() - 1);
					buffer[buffer.size() - 1] = code[inx++];
					if(buffer == ls::comment_block_end_symbol) {state = start; buffer.clear();}
				}
			break;
		}
	}

	return tokensOut;
}
