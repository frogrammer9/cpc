#ifndef CPC_PARSER
#define CPC_PARSER

#include "langSpec.hpp"
#include "lexer.hpp"
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace cpc {

typedef int Type; //TODO: implement

struct ExprNode {
	virtual ~ExprNode() = default;
	virtual void print() = 0;
};

struct NumExpr : public ExprNode {
	NumExpr(const std::wstring& val);
	std::variant<int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t, float, double> value;
	enum Type { i8, i16, i32, i64, u8, u16, u32, u64, f32, f64 } type;
	inline void print() override {

	}
};

struct IntLiteralExpr : public ExprNode {
	inline IntLiteralExpr(const std::wstring& valstr) {
		wchar_t base = valstr[0];
		std::wstring str = valstr.substr(1, valstr.size());
		switch(base) {
			case 'd': val = std::stoull(valstr.substr(1, valstr.size()), nullptr, 10); break;
			case 'x': val = std::stoull(valstr.substr(1, valstr.size()), nullptr, 16); break;
			case 'b': val = std::stoull(valstr.substr(1, valstr.size()), nullptr,  2); break;
		}
	}
	uint64_t val;
	inline void print() override { std::wcout << L"IntLit: " << val << L"\n"; }
};

struct FloatLiteralExpr : public ExprNode {
	FloatLiteralExpr(const std::wstring& valstr) : val(std::stod(valstr)) {}
	double val;
	inline void print() override { std::wcout << L"FloatLit: " << val; }
};

struct StringLiteralExpr : public ExprNode {
	StringLiteralExpr(const std::wstring& s) : str(s) {}
	std::wstring str;
	inline void print() override { std::wcout << L"StrLit: " << str; }
};

struct IdentExpr: public ExprNode {
	IdentExpr(const std::wstring& s) : str(s) {}
	std::wstring str;
	inline void print() override { std::wcout << L"Ident: " << str; }
};

struct VarExpr : public ExprNode {
	VarExpr(const std::wstring& n) : name(n) {}
	Type type;
	std::wstring name;
	inline void print() override { std::wcout << L"Var: " << name; }
};

struct BinExpr : public ExprNode {
	inline BinExpr(ls::TokenType oper, std::unique_ptr<ExprNode> L, std::unique_ptr<ExprNode> R) {
		switch(oper) {
			//No this is not readable but I will surely remember what is going on here, surely
			case cpc::ls::PLUS: op = cpc::BinExpr::add; break; case cpc::ls::DASH: op = cpc::BinExpr::sub; break;
			case cpc::ls::STAR: op = cpc::BinExpr::mul; break; case cpc::ls::SLASH: op = cpc::BinExpr::div; break;
			case cpc::ls::PERCENT: op = cpc::BinExpr::modulo; break; case cpc::ls::ANDPERCENT: op = cpc::BinExpr::bitAnd; break;
			case cpc::ls::VBAR: op = cpc::BinExpr::bitOr; break; case cpc::ls::CARET: op = cpc::BinExpr::bitXor; break;
			case cpc::ls::SHL: op = cpc::BinExpr::shl; break; case cpc::ls::SHR: op = cpc::BinExpr::shr; break;
			case cpc::ls::ASSIGMENT: op = cpc::BinExpr::assign; break; case cpc::ls::PLUS_ASSIGN: op = cpc::BinExpr::addAssign; break;
			case cpc::ls::MINUS_ASSIGN: op = cpc::BinExpr::subAssign; break; case cpc::ls::MULTIPLY_ASSIGN: op = cpc::BinExpr::mulAssign; break;
			case cpc::ls::DEVIDE_ASSIGN: op = cpc::BinExpr::divAssign; break; case cpc::ls::MODULO_ASSIGN: op = cpc::BinExpr::moduloAssign; break;
			case cpc::ls::AND_ASSIGN: op = cpc::BinExpr::andAssign; break; case cpc::ls::OR_ASSIGN: op = cpc::BinExpr::orAssign; break;
			case cpc::ls::XOR_ASSIGN: op = cpc::BinExpr::xorAssign; break; case cpc::ls::SHL_ASSIGN: op = cpc::BinExpr::shlAssign; break;
			case cpc::ls::SHR_ASSIGN: op = cpc::BinExpr::shrAssign; break; case cpc::ls::LOGIC_AND: op = cpc::BinExpr::logicAnd; break;
			case cpc::ls::LOGIC_OR: op = cpc::BinExpr::logicOr; break; case cpc::ls::LOGIC_XOR: op = cpc::BinExpr::logicXor; break;
			case cpc::ls::EQUAL: op = cpc::BinExpr::equal; break; case cpc::ls::DIFFERENT: op = cpc::BinExpr::different; break;
			case cpc::ls::LESS: op = cpc::BinExpr::less; break; case cpc::ls::LESS_OR_EQUAL: op = cpc::BinExpr::lessOrEqual; break;
			case cpc::ls::GREATER: op = cpc::BinExpr::greater; break; case cpc::ls::GREATER_OR_EQUAL: op = cpc::BinExpr::greaterOrEqual; break;
			default: std::wcerr << "Invalid operator (errors TODO)\n"; exit(1); break;
		}
	}
	enum Op {
		add, sub, mul, div, modulo,
		bitAnd, bitOr, bitXor, shl, shr,
		assign, addAssign, subAssign, mulAssign, divAssign, moduloAssign, andAssign, orAssign, xorAssign, shlAssign, shrAssign,
		logicAnd, logicOr, logicXor,
		equal, different, less, lessOrEqual, greater, greaterOrEqual,
	} op;
	std::unique_ptr<ExprNode> LHS, RHS;
	inline void print() override {
		std::wstring syms[] = {L"+", L"-", L"*", L"/", L"%", L"&", L"|", L"^", L"<<", L">>", L"=", L"+=", L"-=", L"*=", L"/=", L"%=", L"&=", L"|=", L"^=", 
		L"<<=", L">>=", L"&&", L"||", L"^^", L"==", L"!=", L"<", L"<=", L">", L">="};
		std::wcout << L"Binop: (\n";
		std::wcout << L"("; LHS->print(); std::wcout << L")\n";
		std::wcout << "\"" << syms[op] << "\"";
		std::wcout << L"("; RHS->print(); std::wcout << L")\n";
		std::wcout << L")\n";
	}
};

struct CondExpr : public ExprNode {
	CondExpr(std::unique_ptr<ExprNode> c, std::unique_ptr<ExprNode> t, std::unique_ptr<ExprNode> e)
	: cond(std::move(c)), then(std::move(t)), elze(std::move(e)) {}
	std::unique_ptr<ExprNode> cond;
	std::unique_ptr<ExprNode> then;
	std::unique_ptr<ExprNode> elze; //??
	inline void print() override {

	}
};

struct LoopExpr : public ExprNode {
	LoopExpr(std::unique_ptr<ExprNode> c, std::unique_ptr<ExprNode> l) : cond(std::move(c)), loop(std::move(l)) {}
	std::unique_ptr<ExprNode> cond;
	std::unique_ptr<ExprNode> loop;
	inline void print() override {

	}
};

struct CallExpr : public ExprNode {
	CallExpr(const std::wstring& callee, std::vector<std::unique_ptr<ExprNode>> args) : callee(std::move(callee)), args(std::move(args)) {}
	std::wstring callee;
	std::vector<std::unique_ptr<ExprNode>> args;
	inline void print() override {

	}
};

struct PrototypeExpr : public ExprNode {
	PrototypeExpr(const std::wstring& callee, std::vector<VarExpr> args) : callee(std::move(callee)), args(std::move(args)) {}
	Type type;
	std::wstring callee;
	std::vector<VarExpr> args;
	inline void print() override {

	}
};

struct FunctionExpr : public ExprNode {
	FunctionExpr(std::unique_ptr<PrototypeExpr> pe, std::unique_ptr<ExprNode> body) : proto(std::move(pe)), body(std::move(body)) {}
	std::unique_ptr<PrototypeExpr> proto;
	std::unique_ptr<ExprNode> body;
	inline void print() override {

	}
};

struct StmtNode {
	virtual ~StmtNode() = default;
	virtual void print() = 0;
};

struct ExprStmt : public StmtNode {
	ExprStmt(std::unique_ptr<ExprNode> e) : expr(std::move(e)) {}
	std::unique_ptr<ExprNode> expr;
	inline void print() override { expr->print(); }
};

struct BlockStmt : public StmtNode {
	BlockStmt() = default;
	BlockStmt(std::vector<std::unique_ptr<StmtNode>> stmts) : statements(std::move(stmts)) {}
	std::vector<std::unique_ptr<StmtNode>> statements;
	inline void print() override {
		for(auto& ptr : statements) { ptr->print(); }
	}
};

std::unique_ptr<BlockStmt> parse(std::vector<Token>& tokens);

}
#endif //!CPC_PARSER
