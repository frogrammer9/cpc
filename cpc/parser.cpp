#include "parser.hpp"
#include <functional>
#include <iostream>

typedef std::vector<cpc::Token>::iterator Tokenloc;
typedef std::unique_ptr<cpc::ExprNode> ExprNodePtr;
typedef std::unique_ptr<cpc::StmtNode> StmtNodePtr;

void expectToken(Tokenloc loc, cpc::ls::TokenType et) {
	if(loc->type != et) {
	std::wcerr << "Expected token: TODO (i dont wanna fuck with errors now)\n"; }
	exit(1);
}

enum BindingPower {
	def_bp, comma_bp, assigment_bp, logical_bp, relational_bp, additive_bp, multiplicative_bp, unary_bp, call_bp, member_bp, primary_bp
};

std::map<cpc::ls::TokenType, std::function<StmtNodePtr(Tokenloc)>> stmtLut;
std::map<cpc::ls::TokenType, std::function<ExprNodePtr(Tokenloc)>> nudLut;
std::map<cpc::ls::TokenType, std::function<ExprNodePtr(Tokenloc, ExprNodePtr, BindingPower)>> ledLut;
std::map<cpc::ls::TokenType, BindingPower> bpLut;

ExprNodePtr parseExpr(Tokenloc loc, BindingPower bp) {
	auto nudFun = nudLut.find(loc->type);
	if(nudFun == nudLut.end()) { std::wcerr << "NUD handler expected for token (errors TODO)\n"; exit(1); }
	auto left = nudFun->second(loc);
	while(bpLut.at(loc->type) > bp) {
		auto ledFun = ledLut.find(loc->type);
		if(ledFun == ledLut.end()) { std::wcerr << "LED handler expected for token (errors TODO)\n"; exit(1); }
		left = ledFun->second(loc, std::move(left), bp);
	}
	return left;
}

StmtNodePtr parseStmt(Tokenloc loc) {
	auto stmt_fun = stmtLut.find(loc->type);
	if(stmt_fun != stmtLut.end()) return stmt_fun->second(loc);
	ExprNodePtr expr = parseExpr(loc, def_bp);
	expectToken(loc, cpc::ls::SEMICOLON);
	return std::make_unique<cpc::ExprStmt>(std::move(expr));
}

ExprNodePtr parsePrimary(Tokenloc loc) {
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

ExprNodePtr parseBinop(Tokenloc loc, ExprNodePtr left, BindingPower bp) {
	auto oper = *loc++;
	auto right = parseExpr(loc, bp);
	return std::make_unique<cpc::BinExpr>(loc->type, std::move(left), std::move(right));
}

std::unique_ptr<cpc::BlockStmt> cpc::parse(const std::vector<cpc::Token>& tokens) {
	using namespace cpc;
	auto token = tokens.begin();

	std::unique_ptr<BlockStmt> stmt = std::make_unique<BlockStmt>();

	while(token != tokens.end()) {

	}
	
	return stmt;
}
