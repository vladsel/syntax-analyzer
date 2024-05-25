#pragma once

#include "lexical_analyzer.h"

// tree node
struct Node {
	int Code = -1;
	std::string Name;
	int Line = -1;
	int Column = -1;
	std::string Func;
	bool IsTerminal = false;
	Node *Next = nullptr;
	Node *Down = nullptr;
};

class Parser { // syntax analyzer

public:
	explicit Parser(Tables &_tables) : tables(_tables) { }
	~Parser() { DeleteTree(Tree); }

	void startParser(const std::string file_name, std::ostream &out);

	Node *InitTree(); //init tree
	Node *AddNode(Node *Nd, int code, std::string Name, int line, int column, std::string Func, bool isterminal);
	Node *RemoveNode(Node *N);
	void WriteTree(Node *Root, std::string sps); // output tree
	void TreeListing(std::string File); // output tree to file
	void Errors(int p, int pos); // output errors
	void DeleteTree(Node *Root);

	// functions to manipulate with parser
	bool start();
	bool program(Node *NodePtr);
	bool block(Node *NodePtr);
	bool statement_list(Node *NodePtr);
	bool statement(Node *NodePtr);
	bool expression(Node *NodePtr);
	bool integer(Node *NodePtr);
	bool identifier(Node *NodePtr);
	bool alternatives_list(Node *NodePtr);
	bool multiplier(Node *NodePtr);
	bool multipliers_list(Node *NodePtr);
	bool alternative(Node *NodePtr);
	bool multiplication_instruction(Node *NodePtr);

private:
	Tables &tables;

	Node *Tree = nullptr; // tree root
	vector<Lexeme> Lexems = tables.lexemeTable; // information about lexemes (row, col, code, name)

	size_t Pos = 0; //current lexeme
	unsigned short IdentMin = tables.minIdentifiersCounter;
	unsigned short IdentMax = tables.minErrorCounter - 1;
	unsigned short DigitMin = tables.minConstCounter;
	unsigned short DigitMax = tables.minIdentifiersCounter - 1;

	std::string TreeStr = "";
	std::string ErrorStr = "";

	bool err = false;
	bool err1 = false;
	int ErrorCounter = 0; // error counter
};