#include "syntax_analyzer.h"

#include <iostream>

int main() {

	try {
		std::ofstream file_out1;
		file_out1.open("generated_lexemes1.txt", std::ios::out);
		if (!file_out1.is_open())
			throw std::runtime_error("Impossible to open the generated_lexemes1 file_out");

		std::ofstream file_out2;
		file_out2.open("generated_lexemes2.txt", std::ios::out);
		if (!file_out2.is_open())
			throw std::runtime_error("Impossible to open the generated_lexemes2 file_out");

		std::ofstream file_out3;
		file_out3.open("generated_lexemes3.txt", std::ios::out);
		if (!file_out3.is_open())
			throw std::runtime_error("Impossible to open the generated_lexemes3 file_out");

		std::ofstream file_out4;
		file_out4.open("generated_lexemes4.txt", std::ios::out);
		if (!file_out4.is_open())
			throw std::runtime_error("Impossible to open the generated_lexemes4 file_out");

		std::ofstream file_out5;
		file_out5.open("generated_lexemes5.txt", std::ios::out);
		if (!file_out5.is_open())
			throw std::runtime_error("Impossible to open the generated_lexemes5 file_out");

		std::ofstream file_out6;
		file_out6.open("generated_lexemes6.txt", std::ios::out);
		if (!file_out6.is_open())
			throw std::runtime_error("Impossible to open the generated_lexemes6 file_out");

		std::ofstream file_out7;
		file_out7.open("generated_lexemes7.txt", std::ios::out);
		if (!file_out7.is_open())
			throw std::runtime_error("Impossible to open the generated_lexemes7 file_out");

		std::ofstream file_out8;
		file_out8.open("generated_lexemes8.txt", std::ios::out);
		if (!file_out8.is_open())
			throw std::runtime_error("Impossible to open the generated_lexemes8 file_out");

		std::ofstream file_out9;
		file_out9.open("generated_lexemes9.txt", std::ios::out);
		if (!file_out9.is_open())
			throw std::runtime_error("Impossible to open the generated_lexemes9 file_out");


		Tables aTables, bTables, cTables, dTables, eTables, fTables, gTables, hTables, iTables;

		Lexer a(aTables);
		a.startLexer("test1", file_out1);

		Lexer b(bTables);
		b.startLexer("test2", file_out2);

		Lexer c(cTables);
		c.startLexer("test3", file_out3);

		Lexer d(dTables);
		d.startLexer("test4", file_out4);

		Lexer e(eTables);
		e.startLexer("test5", file_out5);

		Lexer f(fTables);
		f.startLexer("test6", file_out6);

		Lexer g(gTables);
		g.startLexer("test7", file_out7);

		Lexer h(hTables);
		h.startLexer("test8", file_out8);

		Lexer i(iTables);
		i.startLexer("test9", file_out9);


		Parser aP(aTables);
		aP.startParser("test1", std::cout);
		aP.TreeListing("test1");

		Parser bP(bTables);
		bP.startParser("test2", std::cout);
		bP.TreeListing("test2");

		Parser cP(cTables);
		cP.startParser("test3", std::cout);
		cP.TreeListing("test3");

		Parser dP(dTables);
		dP.startParser("test4", std::cout);
		dP.TreeListing("test4");

		Parser eP(eTables);
		eP.startParser("test5", std::cout);
		eP.TreeListing("test5");

		Parser fP(fTables);
		fP.startParser("test6", std::cout);
		fP.TreeListing("test6");

		Parser gP(gTables);
		gP.startParser("test7", std::cout);
		gP.TreeListing("test7");

		Parser hP(hTables);
		hP.startParser("test8", std::cout);
		hP.TreeListing("test8");

		Parser iP(iTables);
		iP.startParser("test9", std::cout);
		iP.TreeListing("test9");


		std::cout << "\nALL INFORMATION WRITEN TO FILES" << std::endl;
	} catch (std::exception &ex) {
		std::cout << "\n" << ex.what() << std::endl;
	} catch (...) {
		exit(1);
	}

	return 0;
}