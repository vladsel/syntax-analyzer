#pragma once

#include <string>
#include <ostream>

using std::string;

class Lexeme { // attributes of one lexeme

public:
	Lexeme() = default;
	~Lexeme() = default;

	void printLexeme(std::ostream &out) const;

	unsigned short get_code() const;
	size_t get_row() const;
	size_t get_col() const;
	string get_name() const;

	void set_code(const unsigned short _code);
	void set_row(const size_t _row);
	void set_col(const size_t _col);
	void set_name(const string _name);

private:
	unsigned short code = 0;
	size_t row = 0;
	size_t col = 0;
	string name;
};