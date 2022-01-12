// File name: SyntacticalAnalyzer.h
// Project: CS 460 Project 2 Spring 2021
// Author: Nicolas Marx, Diana Arce-Hernandez
// Date: 5/21/21
// Description: This file contains the SyntacticalAnalyzer class declaration.


#ifndef SYN_H
#define SYN_H

#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"

using namespace std;

class SyntacticalAnalyzer {
    public:
	SyntacticalAnalyzer (char * filename);
	~SyntacticalAnalyzer ();
    private:
	LexicalAnalyzer * lex;
	token_type token;
	ofstream p2file;
	int errors;
	void program();
	void more_defines();
	void define();
	void stmt_list();
	void stmt();
	void literal();
	void quoted_lit();
	void more_tokens();
	void param_list();
	void else_part();
	void stmt_pair();
	void stmt_pair_body();
	void assign_pair();
	void more_assigns();
	void action();
	void any_other_token();
};
	
#endif
