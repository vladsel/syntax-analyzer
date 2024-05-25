#pragma once

#include <fstream>
#include "tables.h"

class Lexer { // lexical analyzer

public:
	explicit Lexer(Tables &_tables) : tables(_tables) { }
	~Lexer() = default;
	void startLexer(const std::string file_name, std::ostream &out, const unsigned short tab = 4);
	void print_lexemeTable(std::ostream &out);
	void print_errorLexemeTable(std::ostream &out);
	void setTab(unsigned short tab);

private:
	void initCodesCounters();
	void initAttributes();
	void initKeyWords();
	void initSeparators();
	void readInputSigFile(const std::string file_name);
	void countCurrentPos(const char &character);
	void push_back_token(const unsigned short &code, const size_t &row,
						 const size_t &col, const std::string &name);
	void compare_push_back_lexeme_to_tables(std::vector <std::pair<std::string, unsigned short>> &codeNameTable,
											std::string &bufToken, unsigned short &codeNameCounter);

	void INP(const char &character); // entering the next character
	void letter_process(); // processing identifier token = 1
	void digit_process(); // processing number token = 2
	void delimiter_process(); // processing separator token = 3
	void comment_process(); // processing and deleting comments = 4
	void space_process(); // skipping the next spcae character = 0
	void error_process(const std::string &bufTokenFromAnotherCategory = ""); // errors processing = 5

	unsigned short searchTokenFromTable(const std::vector <std::pair<std::string, unsigned short>> &infoTable,
										const std::string &bufToken) const;
	
	unsigned short Attributes[128] = {}; // array for ASCII code symbols

	Tables &tables; // All tables with lexemes are located in tables.h

	unsigned short keyWordsCounter = 0;
	unsigned short constCounter = 0;
	unsigned short identifiersCounter = 0;
	unsigned short separatorsCounter = 0;
	unsigned short errorCounter = 0;
	unsigned short startErrorCounter = 0;

	size_t currentRow = 1;
	size_t currentCol = 1;

	unsigned short tabulation = 0;

	std::ifstream sig_file;

	// working temporary variables
	char temp = 0;							// for every character from file
	size_t startRow = 1;					// for calculating token positions
	size_t startCol = 1;					// for calculating token positions
	Lexeme tokenInfo;						// for pushing tokens into lexemeTable
	unsigned short lexemeCodeFromTable = 0; // for cheking lexemes in tables

	enum AttributesCode {
		CODE_SPACE,				// space symbols = 0
		CODE_IDENTIFIER,		// identifiers and key words symbols = 1
		CODE_NUMBER,			// number symbols = 2
		CODE_SEPARATOR,			// separator symbols = 3
		CODE_COMMENT,			// start comment symbol = 4
		CODE_ERROR				// error (inadmissible) symbols = 5
	};
};