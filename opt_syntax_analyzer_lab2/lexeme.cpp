#include "lexeme.h"

#include <iostream>
#include <iomanip>

void Lexeme::printLexeme(std::ostream &out) const {
	out << std::setw(16) << row << std::setw(16) << col
		<< std::setw(16) << code << std::setw(30) << name << std::endl;
}

unsigned short Lexeme::get_code() const {
	return code;
}

size_t Lexeme::get_row() const {
	return row;
}

size_t Lexeme::get_col() const {
	return col;
}

string Lexeme::get_name() const {
	return name;
}

void Lexeme::set_code(const unsigned short _code) {
	if (_code >= 0)
		code = _code;
	else
		throw std::range_error("\nerror code <= 0\n");
}

void Lexeme::set_row(const size_t _row) {
	if (_row >= 0)
		row = _row;
	else
		throw std::range_error("\nerror row <= 0\n");
}

void Lexeme::set_col(const size_t _col) {
	if (_col >= 0)
		col = _col;
	else
		throw std::range_error("\nerror col <= 0\n");
}

void Lexeme::set_name(const string _name) {
	if (!_name.empty())
		name = _name;
}
