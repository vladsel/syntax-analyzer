#pragma once

#include "lexeme.h"
#include <vector>

using std::vector;
using std::pair;

class Tables {
public:
	Tables() = default;
	~Tables() = default;
	friend class Lexer;
	friend class Parser;

private:
	vector <Lexeme> lexemeTable;		 // information about lexemes (row, col, code, name)
	vector <Lexeme> errorLexemeTable;	 // information about ERROR lexemes (row, col, code, name)

	vector <pair<string, unsigned short>> keyWordsTable;
	vector <pair<string, unsigned short>> const_unsignedIntTable;
	vector <pair<string, unsigned short>> identifiersTable;
	vector <pair<string, unsigned short>> errorTokenTable;
	vector <pair<string, unsigned short>> separatorsTable;

	unsigned short minKeyWordsCounter = 401; // lexeme codes
	unsigned short minConstCounter = 501;
	unsigned short minIdentifiersCounter = 1001;
	unsigned short minSeparatorsCounter = 1;
	unsigned short minErrorCounter = 5001;
	unsigned short minStartErrorCounter = 5001;

	unsigned short KEY_WORD_PROGRAM_CODE = 0;
	unsigned short KEY_WORD_BEGIN_CODE = 0;
	unsigned short KEY_WORD_END_CODE = 0;
	unsigned short KEY_WORD_LOOP_CODE = 0;
	unsigned short KEY_WORD_ENDLOOP_CODE = 0;
	unsigned short KEY_WORD_CASE_CODE = 0;
	unsigned short KEY_WORD_OF_CODE = 0;
	unsigned short KEY_WORD_ENDCASE_CODE = 0;
	unsigned short KEY_WORD_MOD_CODE = 0;

	unsigned short SEPARATOR_COLON_CODE = 0;
	unsigned short SEPARATOR_MULTIPLIER_CODE = 0;
	unsigned short SEPARATOR_SEMICOLON_CODE = 0;
	unsigned short SEPARATOR_LEFT_CARRIAGE_CODE = 0;
	unsigned short SEPARATOR_RIGHT_CARRIAGE_CODE = 0;
	unsigned short SEPARATOR_DOT_CODE = 0;
};
