/*******************************************************************************
* Assignment: Project 1 - Lexical Analyzer for Scheme to C++ Translator        *
* File: LexicalAnalyzer.h                                                      *
*                                                                              *
* Description: This file contains the implementation of Project 1              *
*******************************************************************************/

#ifndef LEX_H
#define LEX_H

#include <iostream>
#include <fstream>
#include <map>

using namespace std;

/*******************************************************************************
* Type: token_type                                                             *
*                                                                              *
* Description: The token_type enumerated type is used to identify the tokens   *
*              associated with the lexemes scanned from an input file.         *
*******************************************************************************/

enum token_type { NONE=-1,
	LAMBDA, IDENT_T, NUMLIT_T, STRLIT_T, IF_T, COND_T, DISPLAY_T, NEWLINE_T,
	LISTOP1_T, LISTOP2_T, AND_T, OR_T, NOT_T, DEFINE_T, LET_T, NUMBERP_T,
	LISTP_T, ZEROP_T, NULLP_T, STRINGP_T, MODULO_T, ROUND_T, ELSE_T, PLUS_T,
	MINUS_T, DIV_T, MULT_T, EQUALTO_T, GT_T, LT_T, GTE_T, LTE_T, LPAREN_T,
	RPAREN_T, LBRACK_T, RBRACK_T, SQUOTE_T, ERROR_T, EOF_T, MAX_TOKENS
};

/*******************************************************************************
* Class: Lexical_Analyzer                                                      *
*                                                                              *
* Description: This class is designed to                                       *
*******************************************************************************/

class LexicalAnalyzer 
{
    public:
	LexicalAnalyzer (char * filename);
	~LexicalAnalyzer ();
	token_type GetToken ();
	string GetTokenName (token_type t) const;
	string GetLexeme () const;
	void ReportError (const string & msg);
	ofstream debug;
    private:
	ifstream input;
	ofstream listing;
	ofstream p1file;
	string line;
	int linenum;
	int pos;
	string lexeme;
	int error_type;
	int errors;
	map <string, token_type> symbols;
};
	
#endif
