#include "parser.hpp"
#include <functional>
#include <iostream>

enum BindingPower { def_bp, comma_bp, assigment_bp, logical_bp, relational_bp, additive_bp, multiplicative_bp, unary_bp, call_bp, member_bp, primary_bp };

typedef std::vector<cpc::Token>::iterator Tokenloc; //PERF: maybe this should be a const iterator
typedef std::unique_ptr<cpc::ExprNode> ExprNodePtr;
typedef std::unique_ptr<cpc::StmtNode> StmtNodePtr;

typedef std::function<ExprNodePtr(Tokenloc&, ExprNodePtr, BindingPower)> led_fn;
typedef std::function<ExprNodePtr(Tokenloc&)> nud_fn;
typedef std::function<StmtNodePtr(Tokenloc&)> stmt_fn;


void expectToken(Tokenloc& loc, cpc::ls::TokenType et) {
	if(loc++->type != et) {
		std::wcerr << "Expected token: " << et << " Got token: " << *loc << "\n"; 
		exit(1);
	}
}

std::map<cpc::ls::TokenType, nud_fn> nudLut;
std::map<cpc::ls::TokenType, led_fn> ledLut;
std::map<cpc::ls::TokenType, stmt_fn> stmtLut;
std::map<cpc::ls::TokenType, BindingPower> bpLut;

ExprNodePtr parseExpr(Tokenloc& loc, BindingPower bp) {
	auto nudFun = nudLut.find(loc->type);
	if(nudFun == nudLut.end()) { std::wcerr << "NUD handler expected for token " << nudFun->first << "\n"; exit(1); }
	ExprNodePtr left = nudFun->second(loc);

	for (auto bpV = bpLut.find(loc->type); ((bpV == bpLut.end()) ? def_bp : bpV->second) > bp; bpV = bpLut.find(loc->type)) {
		auto ledFun = ledLut.find(loc->type);
		if(ledFun == ledLut.end()) { std::wcerr << "LED handler expected for token " << ledFun->first << "\n"; exit(1); }
		left = std::move(ledFun->second(loc, std::move(left), ((bpV == bpLut.end()) ? def_bp : bpV->second)));
	}
	return left;
}

StmtNodePtr parseStmt(Tokenloc& loc) {
	auto stmt_fun = stmtLut.find(loc->type);
	if(stmt_fun != stmtLut.end()) return stmt_fun->second(loc);
	ExprNodePtr expr = parseExpr(loc, def_bp);
	expectToken(loc, cpc::ls::SEMICOLON);
	return std::make_unique<cpc::ExprStmt>(std::move(expr));
}

ExprNodePtr parsePrimary(Tokenloc& loc) {
	switch(loc->type) {
		case cpc::ls::INT_LITERAL:
			return std::make_unique<cpc::IntLiteralExpr>(loc++->value.value());
		break;
		case cpc::ls::FLOAT_LITERAL:
			return std::make_unique<cpc::FloatLiteralExpr>(loc++->value.value());
		break;
		case cpc::ls::STRING_LITERAL:
			return std::make_unique<cpc::StringLiteralExpr>(loc++->value.value());
		break;
		case cpc::ls::IDENT:
			return std::make_unique<cpc::IdentExpr>(loc++->value.value());
		break;
		default:
			std::wcerr << "Expected a primitve (errors are todo)\n";
			exit(1);
		break;
	}
}

ExprNodePtr parseBinop(Tokenloc& loc, ExprNodePtr left, BindingPower bp) {
	auto oper = loc++->type;
	auto right = parseExpr(loc, bp);
	return std::make_unique<cpc::BinExpr>(oper, std::move(left), std::move(right));
}

void led(cpc::ls::TokenType type, BindingPower bp, led_fn fn) { bpLut[type] = bp; ledLut[type] = fn; }
void nud(cpc::ls::TokenType type, BindingPower bp, nud_fn fn) { bpLut[type] = bp; nudLut[type] = fn; }
void stmt(cpc::ls::TokenType type, BindingPower bp, stmt_fn fn) { bpLut[type] = bp; stmtLut[type] = fn; }

std::unique_ptr<cpc::BlockStmt> cpc::parse(std::vector<cpc::Token>& tokens) {
	using namespace cpc;
	using namespace cpc::ls;

	led(TokenType::LOGIC_AND, logical_bp, parseBinop);
	led(TokenType::LOGIC_OR, logical_bp, parseBinop);
	led(TokenType::LOGIC_XOR, logical_bp, parseBinop);

	led(TokenType::EQUAL, relational_bp, parseBinop);
	led(TokenType::DIFFERENT, relational_bp, parseBinop);
	led(TokenType::LESS, relational_bp, parseBinop);
	led(TokenType::LESS_OR_EQUAL, relational_bp, parseBinop);
	led(TokenType::GREATER, relational_bp, parseBinop);
	led(TokenType::GREATER_OR_EQUAL, relational_bp, parseBinop);

	led(TokenType::PLUS, additive_bp, parseBinop);
	led(TokenType::DASH, additive_bp, parseBinop);

	led(TokenType::STAR, multiplicative_bp, parseBinop);
	led(TokenType::SLASH, multiplicative_bp, parseBinop);
	led(TokenType::PERCENT, multiplicative_bp, parseBinop);

	nud(TokenType::INT_LITERAL, def_bp, parsePrimary);
	nud(TokenType::FLOAT_LITERAL, def_bp, parsePrimary);
	nud(TokenType::STRING_LITERAL, def_bp, parsePrimary);
	nud(TokenType::IDENT, def_bp, parsePrimary);

	Tokenloc token = tokens.begin();

	std::vector<std::unique_ptr<StmtNode>> body;

	while(token != tokens.end()) {
		body.emplace_back(std::move(parseStmt(token)));
	}
	
	return std::make_unique<BlockStmt>(std::move(body));
}
