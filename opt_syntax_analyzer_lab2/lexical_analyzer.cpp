#include "lexical_analyzer.h"
#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;
using std::vector;
using std::pair;
using std::make_pair;

void Lexer::startLexer(const string file_name, std::ostream &out, const unsigned short tab) { // default tab = 4
	initCodesCounters();
	initAttributes();
	initKeyWords();
	initSeparators();
	setTab(tab);
	readInputSigFile(file_name);
	print_lexemeTable(out);
	print_errorLexemeTable(out);
}

void Lexer::print_lexemeTable(std::ostream &out) {
	if (!tables.lexemeTable.empty()) {
		out << std::setw(16) << "Line" << std::setw(16) << "Column"
			<< std::setw(16) << "Code" << std::setw(30) << "Token" << endl;
		out << "|-------------------------------------------------------------------------------|" << endl << endl;
		bool flag = false;
		for (const auto &it : tables.lexemeTable) {/////////////////////////////////////////////////////
			flag = false;
			for (const auto &itErr : tables.errorLexemeTable)
				if (it.get_name() == itErr.get_name()) {
					flag = true;
					break;
				}
			if (flag == false)
				it.printLexeme(out);
		}
		out << "\n|-------------------------------------------------------------------------------|" << endl << endl;
		out << "\n";
	}
	else if (tables.lexemeTable.empty() && tables.errorLexemeTable.empty())
		out << "\n Empty sig file" << endl;
	else
		out << std::setw(37) << "No lexemes to analyze\n" << endl;
}

void Lexer::print_errorLexemeTable(std::ostream &out) {
	if (!tables.errorLexemeTable.empty()) {
		for (const auto &it : tables.errorLexemeTable) {
			if (it.get_name()[0] == '(' && it.get_name()[1] == '*')
				out << std::setw(66) << "Lexical error: *) expected but end of file found:  " << it.get_name() << endl;
			else if (it.get_name().at(0) == '(' && it.get_name()[1] != '*')
				out << std::setw(67) << "Lexical error: **) expected but end of file found:  " << it.get_name() << endl;
			else
				out << std::setw(50) << "Lexical error: Illegal symbol(s):  " << it.get_name() << endl;
			out << std::setw(21) << "Line: " << it.get_row() << "; Column: "
				<< it.get_col() << "; Code: " << it.get_code() << ";\n" << endl;
		}
		out << "\n|-------------------------------------------------------------------------------|" << endl << endl;
		out << "\n\n";
	}
	else if (tables.lexemeTable.empty() && tables.errorLexemeTable.empty())
		out << "\n Empty sig file" << endl;
	else
		out << std::setw(43) << "No ERROR lexemes to analyze\n" << endl;
}

void Lexer::initCodesCounters() {
	keyWordsCounter = tables.minKeyWordsCounter;
	constCounter = tables.minConstCounter;
	identifiersCounter = tables.minIdentifiersCounter;
	separatorsCounter = tables.minSeparatorsCounter;
	errorCounter = tables.minErrorCounter;
	startErrorCounter = tables.minStartErrorCounter;
}

void Lexer::initAttributes() {
	for (unsigned short i = 0; i < 128; i++) {
		if (((i > 7) && (i < 14)) || (i == 32)) // space symbols = 0
			Attributes[i] = CODE_SPACE;
		else if ((i > 64) && (i < 91)) // identifiers and key words symbols = 1
			Attributes[i] = CODE_IDENTIFIER;
		else if ((i > 47) && (i < 58)) // number symbols = 2
			Attributes[i] = CODE_NUMBER;
		else if ((i == '*') || (i == '.') || (i == ':') || (i == ';') || (i == 47) || (i == 92))
			Attributes[i] = CODE_SEPARATOR; /* 47 = / , 92 = \, separator symbols = 3 */
		else if (i == '(')
			Attributes[i] = CODE_COMMENT; // start comment symbol = 4
		else
			Attributes[i] = CODE_ERROR; // error (inadmissible) symbols = 5
	}
}

void Lexer::initKeyWords() {
	if (tables.keyWordsTable.empty()) {
		tables.KEY_WORD_PROGRAM_CODE = keyWordsCounter;
		tables.keyWordsTable.push_back(make_pair("PROGRAM", keyWordsCounter++));
		tables.KEY_WORD_BEGIN_CODE = keyWordsCounter;
		tables.keyWordsTable.push_back(make_pair("BEGIN", keyWordsCounter++));
		tables.KEY_WORD_END_CODE = keyWordsCounter;
		tables.keyWordsTable.push_back(make_pair("END", keyWordsCounter++));
		tables.KEY_WORD_LOOP_CODE = keyWordsCounter;
		tables.keyWordsTable.push_back(make_pair("LOOP", keyWordsCounter++));
		tables.KEY_WORD_ENDLOOP_CODE = keyWordsCounter;
		tables.keyWordsTable.push_back(make_pair("ENDLOOP", keyWordsCounter++));
		tables.KEY_WORD_CASE_CODE = keyWordsCounter;
		tables.keyWordsTable.push_back(make_pair("CASE", keyWordsCounter++));
		tables.KEY_WORD_OF_CODE = keyWordsCounter;
		tables.keyWordsTable.push_back(make_pair("OF", keyWordsCounter++));
		tables.KEY_WORD_ENDCASE_CODE = keyWordsCounter;
		tables.keyWordsTable.push_back(make_pair("ENDCASE", keyWordsCounter++));
		tables.KEY_WORD_MOD_CODE = keyWordsCounter;
		tables.keyWordsTable.push_back(make_pair("MOD", keyWordsCounter++));
	}
}

void Lexer::initSeparators() {
	if (tables.separatorsTable.empty()) {
		tables.SEPARATOR_MULTIPLIER_CODE = separatorsCounter;
		tables.separatorsTable.push_back(make_pair("*", separatorsCounter++));
		tables.SEPARATOR_DOT_CODE = separatorsCounter;
		tables.separatorsTable.push_back(make_pair(".", separatorsCounter++));
		tables.SEPARATOR_COLON_CODE = separatorsCounter;
		tables.separatorsTable.push_back(make_pair(":", separatorsCounter++));
		tables.SEPARATOR_SEMICOLON_CODE = separatorsCounter;
		tables.separatorsTable.push_back(make_pair(";", separatorsCounter++));
		tables.SEPARATOR_LEFT_CARRIAGE_CODE = separatorsCounter;
		tables.separatorsTable.push_back(make_pair(std::string(1, static_cast<char>(47)), separatorsCounter++));
		tables.SEPARATOR_RIGHT_CARRIAGE_CODE = separatorsCounter;
		tables.separatorsTable.push_back(make_pair(std::string(1, static_cast<char>(92)), separatorsCounter++));
	}
}

void Lexer::setTab(unsigned short tab) {
	if (tab >= 0 && tab <= 8)
		tabulation = tab;
	else
		throw std::exception("\nInvalid tabulation\n");
}

void Lexer::readInputSigFile(const string file_name) {
	try {
		if (file_name.empty())
			throw std::invalid_argument("error file_name");

		auto n = file_name.rfind(".sig");

		if (n == string::npos)
			sig_file.open(file_name + ".sig", std::ios::in);
		else
			sig_file.open(file_name, std::ios::in);
		if (!sig_file.is_open())
			throw std::runtime_error("Impossible to open the sig_file");
	}
	catch (const std::exception &ex) {
		cout << "\n" << ex.what() << endl;
		return;
	}

	temp = sig_file.get();
	while (true) {

		if (sig_file.eof())
			return;

		INP(temp);
	}

	sig_file.close();
}

void Lexer::countCurrentPos(const char &character) {
	if (character == '\t') {
		switch ((currentCol - 1) % tabulation) {
			case 0:
				currentCol += tabulation;
				break;
			case 1:
				currentCol += tabulation - 1;
				break;
			case 2:
				currentCol += tabulation - 2;
				break;
			case 3:
				currentCol += tabulation - 3;
				break;
			case 4:
				currentCol += tabulation - 4;
				break;
			case 5:
				currentCol += tabulation - 5;
				break;
			case 6:
				currentCol += tabulation - 6;
				break;
			case 7:
				currentCol += tabulation - 7;
				break;
		}
	}	
	else if (character != '\n')
		currentCol++;
	else {
		currentCol = 1;
		currentRow++;
	}
}

void Lexer::push_back_token(const unsigned short &code, const size_t &row, const size_t &col, const string &name) {
	tokenInfo.set_code(code);
	tokenInfo.set_row(row);
	tokenInfo.set_col(col);
	tokenInfo.set_name(name);
	if (code >= startErrorCounter) {
		tables.lexemeTable.push_back(tokenInfo); ///////////////////////////////////////////////////////////////
		tables.errorLexemeTable.push_back(tokenInfo);
	}
	else
		tables.lexemeTable.push_back(tokenInfo);
}

void Lexer::compare_push_back_lexeme_to_tables(vector <pair<string, unsigned short>> &codeNameTable,
											   string &bufToken, unsigned short &codeNameCounter) {
	if (!bufToken.empty()) {
		lexemeCodeFromTable = searchTokenFromTable(codeNameTable, bufToken);
		if (lexemeCodeFromTable == 0) {
			push_back_token(codeNameCounter, currentRow, startCol, bufToken);
			codeNameTable.push_back(make_pair(bufToken, codeNameCounter));
			codeNameCounter++;
		} else
			push_back_token(lexemeCodeFromTable, currentRow, startCol, bufToken);
	}
}

void Lexer::INP(const char &character) {
	try {
		switch (Attributes[character]) {
			case CODE_SPACE:				// space symbols = 0
				space_process();
				break;
			case CODE_IDENTIFIER:			// identifiers and key words symbols = 1
				letter_process();
				break;
			case CODE_NUMBER:				// number symbols = 2
				digit_process();
				break;
			case CODE_SEPARATOR:			// separator symbols = 3
				delimiter_process();
				break;
			case CODE_COMMENT:				// start comment symbol = 4
				comment_process();
				break;
			case CODE_ERROR:				// error (inadmissible) symbols = 5
				error_process();
				break;
			default:
				if (!sig_file.eof())
					throw std::runtime_error("Incorrect INP switch case error");
		}
	}
	catch (const std::runtime_error &ex) {
		cout << "\n" << ex.what() << endl;
	}
}

void Lexer::letter_process() { // identifiers and key words symbols = 1
	string bufToken;
	startCol = currentCol;
	bool flag = true;

	while (1) {
		bufToken.push_back(temp);
		countCurrentPos(temp);
		temp = sig_file.get();
		if (Attributes[temp] == CODE_SPACE || Attributes[temp] == CODE_SEPARATOR
			|| Attributes[temp] == CODE_COMMENT || sig_file.eof())
			break;
		if (flag == true)
			if (Attributes[temp] != CODE_IDENTIFIER && Attributes[temp] != CODE_NUMBER)
				flag = false;
	}

	if (flag == true) {
		lexemeCodeFromTable = searchTokenFromTable(tables.keyWordsTable, bufToken);
		if (lexemeCodeFromTable == 0)
			compare_push_back_lexeme_to_tables(tables.identifiersTable, bufToken, identifiersCounter);
		else
			push_back_token(lexemeCodeFromTable, currentRow, startCol, bufToken);
		bufToken.clear();
		//INP(temp);
	} else
		error_process(bufToken);
}

void Lexer::digit_process() { // number symbols = 2
	string bufToken;
	startCol = currentCol;
	bool flag = true;

	while (1) {
		bufToken.push_back(temp);
		countCurrentPos(temp);
		temp = sig_file.get();
		if (Attributes[temp] == CODE_SPACE || Attributes[temp] == CODE_SEPARATOR
			|| Attributes[temp] == CODE_COMMENT || sig_file.eof())
			break;
		if (flag == true)
			if (Attributes[temp] != CODE_NUMBER)
				flag = false;
	}

	if (flag == true) {
		compare_push_back_lexeme_to_tables(tables.const_unsignedIntTable, bufToken, constCounter);
		bufToken.clear();
		//INP(temp);
	}
	else
		error_process(bufToken);
}

void Lexer::delimiter_process() { // separator symbols = 3
	string bufToken;
	startCol = currentCol;
	bufToken.push_back(temp);
	countCurrentPos(temp);
	temp = sig_file.get();
	compare_push_back_lexeme_to_tables(tables.separatorsTable, bufToken, separatorsCounter);
	bufToken.clear();
	//INP(temp);
}

void Lexer::comment_process() { // start comment symbol = 4
	startCol = currentCol;
	countCurrentPos(temp);
	temp = sig_file.get();
	bool endCom = false;
	if (temp == '*') {
		string bufToken("(");
		while (1) {
			bufToken.push_back(temp);
			countCurrentPos(temp);
			temp = sig_file.get();
			if (temp == EOF) {
				error_process(bufToken);
				return;
			}
			else if (temp == '*')
				while (1) {
					bufToken.push_back(temp);
					countCurrentPos(temp);
					temp = sig_file.get();
					if (temp == EOF) {
						error_process(bufToken);
						return;
					}
					else if (temp == ')') {
						countCurrentPos(temp);
						temp = sig_file.get();
						bufToken.clear();
						endCom = true;
						break;
					}
					else if (temp != '*')
						break;
				}
			if (endCom)
				break;
		}
	}
	if (endCom)
		return;
		//INP(temp);
	else if (temp == EOF)
		error_process("(");
	else {
		countCurrentPos(temp);
		temp = sig_file.get();
		error_process("(");
	}
}

void Lexer::space_process() { // space symbols = 0
	while (1) {
		countCurrentPos(temp);
		temp = sig_file.get();
		if (Attributes[temp] != CODE_SPACE || sig_file.eof())
			break;
	}
	//INP(temp);
}

void Lexer::error_process(const string &bufTokenFromAnotherCategory) { // error (inadmissible) symbols = 5
	bool flag = true;
	if (!bufTokenFromAnotherCategory.empty())
		flag = false;

	string bufToken;
	if (flag == false)
		bufToken = bufTokenFromAnotherCategory;

	if (flag == true) {
		startCol = currentCol;
		while (1) {
			bufToken.push_back(temp);
			countCurrentPos(temp);
			temp = sig_file.get();
			if (Attributes[temp] == CODE_SPACE || Attributes[temp] == CODE_SEPARATOR
				|| Attributes[temp] == CODE_COMMENT || sig_file.eof())
				break;
		}
	}

	compare_push_back_lexeme_to_tables(tables.errorTokenTable, bufToken, errorCounter);
	bufToken.clear();
	//INP(temp);
}

unsigned short Lexer::searchTokenFromTable(const vector<pair<string, unsigned short>> &infoTable,
										   const string &bufToken) const {
	if (!bufToken.empty()) {
		for (const auto &it : infoTable)
			if (bufToken == it.first)
				return it.second;
	}
	return 0;
} // return 0 = searching failed, return > 0 = number was found
