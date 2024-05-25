#include "syntax_analyzer.h"

#include <iostream>
#include <iomanip>

using namespace std;

void Parser::startParser(const std::string file_name, std::ostream &out) {
	if (tables.errorLexemeTable.empty())
		ErrorStr =+ "\nLexical error: No ERROR lexemes to analyze\n";
	else {
		for (const auto &itErr : tables.errorLexemeTable) {
			ErrorStr += "Lexical error on Line ";
			ErrorStr += to_string(itErr.get_row());
			ErrorStr += " Column ";
			ErrorStr += to_string(itErr.get_col());
			ErrorStr += ": Illegal symbol(s): ";
			ErrorStr += itErr.get_name();
			ErrorStr += "\n";
			ErrorCounter++;
		}
	}
	ErrorStr += "\n";
	start();
	if (ErrorCounter == 0) {
		//cout << "\nSyntax analyzer completed successfully" << endl;
	}
}

void Parser::DeleteTree(Node *Root) {
	Node *Temp = nullptr;
	while (Root != nullptr) {
		if (Root->Down != nullptr)
			DeleteTree(Root->Down);
		Temp = Root;
		Root = Root->Next;
		RemoveNode(Temp);
	}
}

bool Parser::start() {
	Tree = InitTree();
	return program(Tree);
}

bool Parser::program(Node *NodePtr) {
	Node *N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<program>", false);
	N = N->Down;
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	if (Lexems[Pos].get_code() == tables.KEY_WORD_PROGRAM_CODE) {
		N->Down = AddNode(N, Lexems[Pos].get_code(), Lexems[Pos].get_name(), Lexems[Pos].get_row(), Lexems[Pos].get_col(), "", true);
		N = N->Down;
		Pos++;
		if (Pos >= Lexems.size()) {
			Errors(1, Pos);
			return false;
		}
		N->Next = AddNode(N, -1, "", -1, -1, "<procedure-identifier>", false);
		N = N->Next;
		if (identifier(N)) {
			Pos++;
			if (Pos >= Lexems.size()) {
				Errors(1, Pos);
				return false;
			}
			if (Lexems[Pos].get_code() == tables.SEPARATOR_SEMICOLON_CODE) {
				N->Next = AddNode(N, Lexems[Pos].get_code(), Lexems[Pos].get_name(), Lexems[Pos].get_row(), Lexems[Pos].get_col(), "", true);
				Pos++;
				N = N->Next;
				N->Next = AddNode(N, -1, "", -1, -1, "<block>", false);
				N = N->Next;
				if (Pos >= Lexems.size()) {
					Errors(1, Pos);
					return false;
				}
				if (block(N)) {
					Pos++;
					if (Pos >= Lexems.size()) {
						Errors(1, Pos);
						return false;
					}
					if ((Pos < Lexems.size()) && (Lexems[Pos].get_code() == tables.SEPARATOR_DOT_CODE)) {
						N->Next = AddNode(N, Lexems[Pos].get_code(), Lexems[Pos].get_name(), Lexems[Pos].get_row(), Lexems[Pos].get_col(), "", true);
						Pos++;
						if (Pos >= Lexems.size()) {
							//Errors(1, Pos);
							return true;
						}
						else
							program(N->Next);
					} else {
						Errors(3, Pos);
						return false;
					}
				} else {
					return false;
				}
			} else {
				Errors(2, Pos);
				return false;
			}
		} else {
			Errors(5, Pos);
			return false;
		}
	} else {
		Errors(4, Pos);
		return false;
	}
}

bool Parser::block(Node *NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node *N = NodePtr;
	if (Lexems[Pos].get_code() == tables.KEY_WORD_BEGIN_CODE) {
		N->Down = AddNode(N, Lexems[Pos].get_code(), Lexems[Pos].get_name(), Lexems[Pos].get_row(), Lexems[Pos].get_col(), "", true);
		N = N->Down;
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<statements-list>", false);
		N = N->Next;
		if (Pos >= Lexems.size()) {
			Errors(1, Pos);
			return false;
		}
		if (statement_list(N)) {
			Pos++;
			if (Pos >= Lexems.size()) {
				Errors(1, Pos);
				return false;
			}
			if (Lexems[Pos].get_code() == tables.KEY_WORD_END_CODE) {
				N->Next = AddNode(N, Lexems[Pos].get_code(), Lexems[Pos].get_name(), Lexems[Pos].get_row(), Lexems[Pos].get_col(), "", true);
				return true;
			} else {
				if (err == false)
					Errors(7, Pos);
				return false;
			}
		} else {
			return false;
		}
	} else {
		Errors(6, Pos);
		return false;
	}
}

bool Parser::statement_list(Node *NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node *N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<statement>", false);
	if (statement(N->Down)) {
		N = N->Down;
		N->Next = AddNode(N, -1, "", -1, -1, "<statements-list>", false);
		N = N->Next;
		Pos++;
		if (statement_list(N)) {
			return true;
		} else {
			return false;
		}
	} else {
		N->Down = AddNode(N, -1, "", -1, -1, "<empty>", false);
		Pos--;
		return true;
	}
}


bool Parser::statement(Node *NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node *N = NodePtr;
	if (Lexems[Pos].get_code() == tables.KEY_WORD_LOOP_CODE) {
		N->Down = AddNode(N, Lexems[Pos].get_code(), Lexems[Pos].get_name(), Lexems[Pos].get_row(), Lexems[Pos].get_col(), "", true);
		N = N->Down;
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<statements-list>", false);
		N = N->Next;
		if (statement_list(N)) {
			Pos++;
			if (Pos >= Lexems.size()) {
				Errors(1, Pos);
				return false;
			}
			if (Lexems[Pos].get_code() == tables.KEY_WORD_ENDLOOP_CODE) {
				N->Next = AddNode(N, Lexems[Pos].get_code(), Lexems[Pos].get_name(), Lexems[Pos].get_row(), Lexems[Pos].get_col(), "", true);
				N = N->Next;
				Pos++;
				if (Pos >= Lexems.size()) {
					Errors(1, Pos);
					return false;
				}
				if (Lexems[Pos].get_code() == tables.SEPARATOR_SEMICOLON_CODE) {
					N->Next = AddNode(N, Lexems[Pos].get_code(), Lexems[Pos].get_name(), Lexems[Pos].get_row(), Lexems[Pos].get_col(), "", true);
					return true;
				} else {
					Errors(2, Pos);
					return false;
				}
			} else {
				Errors(16, Pos);
				return false;
			}
		} else {
			return false;
		}
	}
	else {
		if (Lexems[Pos].get_code() == tables.KEY_WORD_CASE_CODE) {
			N->Down = AddNode(N, Lexems[Pos].get_code(), Lexems[Pos].get_name(), Lexems[Pos].get_row(), Lexems[Pos].get_col(), "", true);
			N = N->Down;
			Pos++;
			N->Next = AddNode(N, -1, "", -1, -1, "<expression>", false);
			N = N->Next;
			if (expression(N)) {
				Pos++;
				if (Pos >= Lexems.size()) {
					Errors(1, Pos);
					return false;
				}
				if (Lexems[Pos].get_code() == tables.KEY_WORD_OF_CODE) {
					N->Next = AddNode(N, Lexems[Pos].get_code(), Lexems[Pos].get_name(), Lexems[Pos].get_row(), Lexems[Pos].get_col(), "", true);
					N = N->Next;
					Pos++;
					N->Next = AddNode(N, -1, "", -1, -1, "<alternatives-list>", false);
					N = N->Next;
					if (alternatives_list(N)) {
						Pos++;
						if (Pos >= Lexems.size()) {
							Errors(1, Pos);
							return false;
						}
						if (Lexems[Pos].get_code() == tables.KEY_WORD_ENDCASE_CODE) {
							N->Next = AddNode(N, Lexems[Pos].get_code(), Lexems[Pos].get_name(), Lexems[Pos].get_row(), Lexems[Pos].get_col(), "", true);
							N = N->Next;
							Pos++;
							if (Pos >= Lexems.size()) {
								Errors(1, Pos);
								return false;
							}
							if (Lexems[Pos].get_code() == tables.SEPARATOR_SEMICOLON_CODE) {
								N->Next = AddNode(N, Lexems[Pos].get_code(), Lexems[Pos].get_name(), Lexems[Pos].get_row(), Lexems[Pos].get_col(), "", true);
								return true;
							} else {
								Errors(2, Pos);
								return false;
							}
						} else {
							Errors(13, Pos);
							return false;
						}
					} else {
						return false;
					}
				} else {
					Errors(15, Pos);
					return false;
				}
			} else {
				return false;
			}
		} else {
			err = true;
			if (Lexems[Pos - 1].get_code() != tables.SEPARATOR_SEMICOLON_CODE && Lexems[Pos - 1].get_code() != tables.KEY_WORD_LOOP_CODE && Lexems[Pos - 1].get_code() != tables.SEPARATOR_LEFT_CARRIAGE_CODE)
				Errors(12, Pos);
			return false;
		}
	}
}

bool Parser::expression(Node *NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node *N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<multiplier>", false);
	N = N->Down;
	if (multiplier(N)) {
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<multipliers-list>", false);
		N = N->Next;
		if (Pos >= Lexems.size()) {
			Errors(1, Pos);
			return false;
		}
		if (multipliers_list(N)) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool Parser::identifier(Node *NodePtr) {
	Node *N = NodePtr;
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	if ((Lexems[Pos].get_code() >= IdentMin) && (Lexems[Pos].get_code() <= IdentMax)) {
		N->Down = AddNode(N, -1, "", -1, -1, "<identifier>", false);
		N = N->Down;
		N->Down = AddNode(N, Lexems[Pos].get_code(), Lexems[Pos].get_name(), Lexems[Pos].get_row(), Lexems[Pos].get_col(), "", true);
		return true;
	} else {
		return false;
	}
}

bool Parser::alternatives_list(Node *NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node *N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<alternative>", false);
	N = N->Down;
	if (alternative(N)) {
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<alternatives-list>", false);
		N = N->Next;
		if (Pos >= Lexems.size()) {
			Errors(1, Pos);
			return false;
		}
		if (alternatives_list(N)) {
			return true;
		} else {
			return false;
		}
	} else {
		N->Down = AddNode(N, -1, "", -1, -1, "<empty>", false);
		Pos--;
		return true;
	}
}

bool Parser::multiplier(Node *NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node *N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<variable-identifier>", false);
	if (identifier(N->Down)) {
		N = N->Down;
		return true;
	} else if (integer(N)) {
		return true;
	} else {
		if (Lexems[Pos-1].get_code() != tables.KEY_WORD_OF_CODE && Lexems[Pos - 1].get_code() != tables.SEPARATOR_RIGHT_CARRIAGE_CODE)
			Errors(8, Pos);
		return false;
	}
}

bool Parser::multipliers_list(Node *NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node *N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<multiplication-instruction>", false);
	N = N->Down;
	if (multiplication_instruction(N)) {
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<multiplier>", false);
		N = N->Next;
		if (Pos >= Lexems.size()) {
			Errors(1, Pos);
			return false;
		}
		if (multiplier(N)) {
			err1 = true;
			Pos++;
			N->Next = AddNode(N, -1, "", -1, -1, "<multipliers-list>", false);
			N = N->Next;
			if (Pos >= Lexems.size()) {
				Errors(1, Pos);
				return false;
			}
			if (multipliers_list(N)) {
				err1 = true;
				return true;
			} else {
				return false;
			}
		} else {
			err = true;
			return false;
		}
	} else {
		err1 = false;
		N->Down = AddNode(N, -1, "", -1, -1, "<empty>", false);
		Pos--;
		return true;
	}
}

bool Parser::alternative(Node *NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node *N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<expression>", false);
	N = N->Down;
	if (expression(N)) {
		Pos++;
		if (Pos >= Lexems.size()) {
			Errors(1, Pos);
			return false;
		}
		if (Lexems[Pos].get_code() == tables.SEPARATOR_COLON_CODE) {
			N->Next = AddNode(N, Lexems[Pos].get_code(), Lexems[Pos].get_name(), Lexems[Pos].get_row(), Lexems[Pos].get_col(), "", true);
			N = N->Next;
			Pos++;
			if (Pos >= Lexems.size()) {
				Errors(1, Pos);
				return false;
			}
			if (Lexems[Pos].get_code() == tables.SEPARATOR_LEFT_CARRIAGE_CODE) {
				N->Down = AddNode(N, Lexems[Pos].get_code(), Lexems[Pos].get_name(), Lexems[Pos].get_row(), Lexems[Pos].get_col(), "", true);
				N = N->Down;
				Pos++;
				N->Next = AddNode(N, -1, "", -1, -1, "<statements-list>", false);
				N = N->Next;
				if (Pos >= Lexems.size()) {
					Errors(1, Pos);
					return false;
				}
				if (statement_list(N)) {
					Pos++;
					if (Pos >= Lexems.size()) {
						Errors(1, Pos);
						return false;
					}
					if (Lexems[Pos].get_code() == tables.SEPARATOR_RIGHT_CARRIAGE_CODE) {
						N->Next = AddNode(N, Lexems[Pos].get_code(), Lexems[Pos].get_name(), Lexems[Pos].get_row(), Lexems[Pos].get_col(), "", true);
						if (Pos >= Lexems.size()) {
							Errors(1, Pos);
							return false;
						}
						return true;
					}
					else {
						Errors(11, Pos);
						return false;
					}
				}
				else {
					return false;
				}
			}
			else {
				Errors(11, Pos);
				return false;
			}
		} else {
			Errors(2, Pos);
			return false;
		}
	}
	else {
		return false;
	}
}

bool Parser::multiplication_instruction(Node *NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node *N = NodePtr;
	if (Lexems[Pos].get_code() == tables.SEPARATOR_MULTIPLIER_CODE || Lexems[Pos].get_code() == tables.SEPARATOR_LEFT_CARRIAGE_CODE || Lexems[Pos].get_code() == tables.KEY_WORD_MOD_CODE) {
		N->Down = AddNode(N, Lexems[Pos].get_code(), Lexems[Pos].get_name(), Lexems[Pos].get_row(), Lexems[Pos].get_col(), "", true);
		N = N->Down;
		return true;
	} else {
		if (err1 == false && Lexems[Pos].get_code() != tables.KEY_WORD_OF_CODE && Lexems[Pos].get_code() != tables.SEPARATOR_COLON_CODE)
			Errors(9, Pos);
		return false;
	}
}

bool Parser::integer(Node *NodePtr) {
	Node *N = NodePtr;
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	if ((Lexems[Pos].get_code() >= DigitMin) && (Lexems[Pos].get_code() <= DigitMax)) {
		N->Down = AddNode(N, -1, "", -1, -1, "<unsigned-integer>", false);
		N = N->Down;
		N->Down = AddNode(N, Lexems[Pos].get_code(), Lexems[Pos].get_name(), Lexems[Pos].get_row(), Lexems[Pos].get_col(), "", true);
		N = N->Down;
		return true;
	} else {
		return false;
	}
}

void Parser::Errors(int p, int pos) {
	switch (p) {
		case 1:
			//cout << "Syntactic error: Unexpected end of the file!" << endl;
			ErrorStr += "Syntactic error: Unexpected end of the file! On line ";
			ErrorStr += to_string(Lexems[pos - 1].get_row());
			ErrorStr += " Column ";
			ErrorStr += to_string(Lexems[pos - 1].get_col());
			ErrorCounter++;
			break;
		case 2:
			//cout << "Syntactic error on Line " << Lexems[pos].get_row() << " Column " << Lexems[pos].get_col() << ": \";\" is passed" << endl;
			ErrorStr += "Syntactic error on Line ";
			ErrorStr += to_string(Lexems[pos].get_row());
			ErrorStr += " Column ";
			ErrorStr += to_string(Lexems[pos].get_col());
			ErrorStr += ": \";\" is passed\n";
			ErrorCounter++;
			break;
		case 3:
			//cout << "Syntactic error on Line " << Lexems[pos].get_row() << " Column " << Lexems[pos].get_col() << ": \".\" is passed after the \"END\"" << endl;
			ErrorStr += "Syntactic error on Line ";
			ErrorStr += to_string(Lexems[pos].get_row());
			ErrorStr += " Column ";
			ErrorStr += to_string(Lexems[pos].get_col());
			ErrorStr += " \".\" is passed after the \"END\"\n";
			ErrorCounter++;
			break;
		case 4:
			//cout << "Syntactic error on Line " << Lexems[pos].get_row() << " Column " << Lexems[pos].get_col() << ": Keyword \"PROGRAM\" is passed" << endl;
			ErrorStr += "Syntactic error on Line ";
			ErrorStr += to_string(Lexems[pos].get_row());
			ErrorStr += " Column ";
			ErrorStr += to_string(Lexems[pos].get_col());
			ErrorStr += ": Keyword \"PROGRAM\" is passed\n";
			ErrorCounter++;
			break;
		case 5:
			//cout << "Syntactic error on Line " << Lexems[pos].get_row() << " Column " << Lexems[pos].get_col() << ": Must be procedure identifier" << endl;
			ErrorStr += "Syntactic error on Line ";
			ErrorStr += to_string(Lexems[pos].get_row());
			ErrorStr += " Column ";
			ErrorStr += to_string(Lexems[pos].get_col());
			ErrorStr += ": Must be procedure identifier\n";
			ErrorCounter++;
			break;
		case 6:
			//cout << "Syntactic error on Line " << Lexems[pos].get_row() << " Column " << Lexems[pos].get_col() << ": Keyword \"BEGIN\" is passed" << endl;
			ErrorStr += "Syntactic error on Line ";
			ErrorStr += to_string(Lexems[pos].get_row());
			ErrorStr += " Column ";
			ErrorStr += to_string(Lexems[pos].get_col());
			ErrorStr += ": Keyword \"BEGIN\" is passed\n";
			ErrorCounter++;
			break;
		case 7:
			//cout << "Syntactic error on Line " << Lexems[pos].get_row() << " Column " << Lexems[pos].get_col() << ": Keyword \"END\" is passed" << endl;
			ErrorStr += "Syntactic error on Line ";
			ErrorStr += to_string(Lexems[pos].get_row());
			ErrorStr += " Column ";
			ErrorStr += to_string(Lexems[pos].get_col());
			ErrorStr += ": Keyword \"END\" is passed\n";
			ErrorCounter++;
			break;
		case 8:
			//cout << "Syntactic error on Line " << Lexems[pos].get_row() << " Column " << Lexems[pos].get_col() << ": Must be constant or identifier" << endl;
			ErrorStr += "Syntactic error on Line ";
			ErrorStr += to_string(Lexems[pos].get_row());
			ErrorStr += " Column ";
			ErrorStr += to_string(Lexems[pos].get_col());
			ErrorStr += ": Must be constant or identifier\n";
			ErrorCounter++;
			break;
		case 9:
			//cout << "Syntactic error on Line " << Lexems[pos].get_row() << " Column " << Lexems[pos].get_col() << ": Must be multiplication instruction" << endl;
			ErrorStr += "Syntactic error on Line ";
			ErrorStr += to_string(Lexems[pos].get_row());
			ErrorStr += " Column ";
			ErrorStr += to_string(Lexems[pos].get_col());
			ErrorStr += ": Must be multiplication instruction\n";
			ErrorCounter++;
			break;
		case 11:
			//cout << "Syntactic error on Line " << Lexems[pos].get_row() << " Column " << Lexems[pos].get_col() << ": \"\\ or /\" is passed after the \"OF :\"" << endl;
			ErrorStr += "Syntactic error on Line ";
			ErrorStr += to_string(Lexems[pos].get_row());
			ErrorStr += " Column ";
			ErrorStr += to_string(Lexems[pos].get_col());
			ErrorStr += ": \"\\ or /\" is passed after the \"OF :\"\n";
			ErrorCounter++;
			break;
		case 12:
			//cout << "Syntactic error on Line " << Lexems[pos].get_row() << " Column " << Lexems[pos].get_col() << ": Keyword \"LOOP or CASE\" is passed" << endl;
			ErrorStr += "Syntactic error on Line ";
			ErrorStr += to_string(Lexems[pos].get_row());
			ErrorStr += " Column ";
			ErrorStr += to_string(Lexems[pos].get_col());
			ErrorStr += ": Keyword \"LOOP or CASE\" is passed\n";
			ErrorCounter++;
			break;
		case 13:
			//cout << "Syntactic error on Line " << Lexems[pos].get_row() << " Column " << Lexems[pos].get_col() << ": Keyword \"ENDCASE\" is passed" << endl;
			ErrorStr += "Syntactic error on Line ";
			ErrorStr += to_string(Lexems[pos].get_row());
			ErrorStr += " Column ";
			ErrorStr += to_string(Lexems[pos].get_col());
			ErrorStr += ": Keyword \"ENDCASE\" is passed\n";
			ErrorCounter++;
			break;
		case 14:
			//cout << "Syntactic error on Line " << Lexems[pos].get_row() << " Column " << Lexems[pos].get_col() << ": Keyword \"CASE\" is passed" << endl;
			ErrorStr += "Syntactic error on Line ";
			ErrorStr += to_string(Lexems[pos].get_row());
			ErrorStr += " Column ";
			ErrorStr += to_string(Lexems[pos].get_col());
			ErrorStr += ": Keyword \"CASE\" is passed\n";
			ErrorCounter++;
			break;
		case 15:
			//cout << "Syntactic error on Line " << Lexems[pos].get_row() << " Column " << Lexems[pos].get_col() << ": Keyword \"OF\" is passed" << endl;
			ErrorStr += "Syntactic error on Line ";
			ErrorStr += to_string(Lexems[pos].get_row());
			ErrorStr += " Column ";
			ErrorStr += to_string(Lexems[pos].get_col());
			ErrorStr += ": Keyword \"OF\" is passed\n";
			ErrorCounter++;
			break;
		case 16:
			//cout << "Syntactic error on Line " << Lexems[pos].get_row() << " Column " << Lexems[pos].get_col() << ": Keyword \"ENDLOOP\" is passed" << endl;
			ErrorStr += "Syntactic error on Line ";
			ErrorStr += to_string(Lexems[pos].get_row());
			ErrorStr += " Column ";
			ErrorStr += to_string(Lexems[pos].get_col());
			ErrorStr += ": Keyword \"ENDLOOP\" is passed\n";
			ErrorCounter++;
			break;
	}
}

Node *Parser::InitTree() {
	Node *Help = new Node;
	Help->Code = -1;
	Help->Column = -1;
	Help->Down = NULL;
	Help->IsTerminal = false;
	Help->Line = -1;
	Help->Name = "";
	Help->Next = NULL;
	Help->Func = "<signal-program>";
	return Help;
}

Node *Parser::AddNode(Node *Nd, int code, string name, int line, int column, string Func, bool isterminal) {
	Node *Help = new Node;
	Help->Code = code;
	Help->Column = column;
	Help->Down = NULL;
	Help->IsTerminal = isterminal;
	Help->Line = line;
	Help->Name = name;
	Help->Next = NULL;
	Help->Func = Func;
	return Help;
}

Node *Parser::RemoveNode(Node *N) {
	if (N != nullptr) 
		delete(N);
	return nullptr;
}

void Parser::WriteTree(Node *Root, string sps) {
	while (Root != NULL) {
		if (Root->IsTerminal) {
			TreeStr += sps + to_string(Root->Code) + "   " + Root->Name + "\n";
		} else {
			TreeStr += sps + Root->Func + "\n";
		}
		if (Root->Down != NULL) {
			for (int i = 0; i < 4; i++) {
				sps.push_back(' ');
			}
			WriteTree(Root->Down, sps);
			for (int i = 0; i < 4; i++) {
				sps.pop_back();
			}
		}
		Root = Root->Next;
	}
}

void Parser::TreeListing(string File) {
	TreeStr = "";
	File += "_generated_parser.txt";
	ofstream f;
	f.open(File);
	if (ErrorCounter > 0) {
		WriteTree(Tree, "");
		f << TreeStr << endl;
		f << ErrorStr << endl;
	} else {
		WriteTree(Tree, "");
		f << TreeStr << endl;
	}
}
