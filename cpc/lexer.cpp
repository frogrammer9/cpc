#include "lexer.hpp"
#include "langSpecs.hpp"
#include <climits>
#include <fstream>
#include <cwctype>
#include <iostream>
#include <istream>
#include <string>

#define ERROR_POS getPos(i, llv)

cpc::Token::TokenType isKeyword(std::wstring str) {
	for(auto a : cpc::Keywords) {
		if(a.first == str) return a.second;
	}
	return cpc::Token::TokenType::none;
}

cpc::Token::TokenType isOperator(std::wstring str) {
	for(auto a : cpc::Operators) {
		if(a.first == str) return a.second;
	}
	return cpc::Token::TokenType::none;
}

std::vector<uint32_t> getLineLengts(std::wstringstream& ss) {
	std::vector<uint32_t> charCounts;
	std::wstring line;
	while (std::getline(ss, line)) {
		int charCount = line.size();
		charCounts.push_back(charCount);
	}
	return charCounts;
}

std::pair<uint32_t, uint32_t> getPos(uint32_t pos, const std::vector<uint32_t>& llv) {
	for (int i = 0; i < llv.size(); i++) {
		if (pos > llv.at(i) + 1) pos -= llv.at(i) + 1;
		else return { i + 1, pos + 1 };
	}
	return {UINT_MAX, UINT_MAX};
}

std::vector<cpc::Token> cpc::Lexer::tokenise(std::filesystem::path filepath)
{
	std::vector<cpc::Token> out;
	std::wifstream infile(filepath, std::ios::in);
	if (!infile.is_open()) std::wcerr << "Couldn't open file: " << filepath.c_str() << "\n";
	std::wstringstream dataStream;
	dataStream << infile.rdbuf();
	std::wstring data = dataStream.str();
	// line counts (for errors)
	auto llv = getLineLengts(dataStream);
	// state machine
	cpc::Token::TokenType type = cpc::Token::TokenType::none;
	int numbase;
	bool canSelectBase;
	bool firstLoop = true;

	std::wstring stmOut;
	for (uint64_t i = 0; i < data.size(); i++) {
		for (wchar_t curr = data[i], prev = 0; i < data.size(); prev = (i != 0) ? data[i-1] : 0 , curr = data[i]) {
			switch (type) {
			case cpc::Token::TokenType::none:
				numbase = 10;
				canSelectBase = true;
				stmOut.clear();
				if (std::iswspace(curr)) i++;
				else if (prev == '-' && std::iswdigit(curr)) { type = cpc::Token::TokenType::intLiteral; stmOut += prev, stmOut += curr, i++; }
				else if (std::iswpunct(prev)) type = Token::TokenType::OP_empty;
				else if (std::iswdigit(curr)) type = cpc::Token::TokenType::intLiteral;
				else if (curr == '.') { type = cpc::Token::TokenType::floatLiteral; stmOut += curr, i++; }
				else if (std::iswalpha(curr)) type = cpc::Token::TokenType::stringLiteral;
				else type = cpc::Token::TokenType::none, /*temp*/ i++;
				continue;
			//Literals
			case cpc::Token::TokenType::intLiteral:
				if (std::iswspace(curr)) {
					cpc::Token tok;
					tok.type = (stmOut.find(L"-") != std::wstring::npos) ? cpc::Token::TokenType::intLiteral : cpc::Token::TokenType::uintLiteral;
					tok.intVal = std::stol(stmOut, nullptr, numbase);
					out.push_back(tok);
					i++;
					type = cpc::Token::TokenType::none;
					continue;
				}
				if (curr == '0' && !iswdigit(prev)) { i++; continue; }
				if (prev == '0' && canSelectBase) {
					switch (curr) {
					case 'x':
						numbase = 16;
						break;
					case 'o':
						numbase = 8;
						break;
					case 'b':
						numbase = 2;
						break;
					default:
						numbase = 10;
					}
					i++;
					canSelectBase = false;
					continue;
				}
				canSelectBase = false;
				if (curr == '.') {
					if (numbase == 10)
					{
						type = cpc::Token::TokenType::floatLiteral; stmOut += curr; i++; continue;
					}
					else throw cpc::LexerError(ERROR_POS, "floats only in base 10");
				}
				if (std::iswxdigit(curr) && numbase == 16) { 
					stmOut += curr; i++;
				}	
				else if (std::iswdigit(curr)) { 
					if (numbase == 8 && curr >= '8') throw cpc::LexerError(ERROR_POS, "for base 8 use numbers 0-7");
					else if (numbase == 2 && curr >= '2') throw cpc::LexerError(ERROR_POS, "for base 2 use numbers 0-1");
					else stmOut += curr, i++;
				}
				else throw cpc::LexerError(ERROR_POS, "NaN inside a NLiteral");
				continue;
			case cpc::Token::TokenType::floatLiteral:

				continue;
			case cpc::Token::TokenType::stringLiteral:

				continue;
			case cpc::Token::TokenType::OP_empty:
				std::wstring op = stmOut + prev;
				auto opData1 = isOperator(op);
				if(opData1 != cpc::Token::TokenType::none) { stmOut = op; i++; }
				auto opData2 = isOperator(op + curr);
				if(opData2 == cpc::Token::TokenType::none) {
					cpc::Token tok;
					tok.type = opData1;
					out.push_back(tok);
					type = Token::TokenType::none;
				}
			continue; 
			}
		}
	}
	cpc::Token tok;
	tok.type = type;
	switch (type) {
	case cpc::Token::TokenType::intLiteral:
	case cpc::Token::TokenType::uintLiteral:
		tok.intVal = std::stol(stmOut, nullptr, numbase);
		break;
	case cpc::Token::TokenType::floatLiteral:
		tok.intVal = std::stod(stmOut, nullptr);
		break;
	case cpc::Token::TokenType::none:
		infile.close();
		return out;
		break;
	}
	out.push_back(tok);
	infile.close();
	return out;
}
