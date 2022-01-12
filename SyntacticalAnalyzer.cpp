// File name: SyntacticalAnalyzer.cpp
// Project: CS 460 Project 2 Spring 2021
// Author: Nicolas Marx, Diana Arce-Hernandez
// Date: 5/21/21
// Description: This file contains the SyntacticalAnalyzer
//              public and private function declarations.


#include <iostream>
#include <iomanip>
#include <fstream>
#include "SyntacticalAnalyzer.h"

using namespace std;

SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename){
	lex = new LexicalAnalyzer(filename);
	string name = filename;
	if (name.length() > 3 and name.substr(name.length()-3, 3) == ".ss"){
		p2file.open(name.substr(0, name.length()-3) + ".p2");
		if (p2file.fail()){
			cout << "File " << filename << " not found\n";
			exit (2);
		}
	} else {cout << "Invalid file extension; must be '.ss'\n";exit (1);}
	token_type t;
	errors = 0;
	token = lex->GetToken();
	program();
}
SyntacticalAnalyzer::~SyntacticalAnalyzer(){
	delete lex;
	p2file.close();
}
void SyntacticalAnalyzer::program(){
	stringstream message;
	if (token == ERROR_T || token == LAMBDA){
		errors++;
		message << "'" << lex->GetLexeme() << "' unexpected";
		lex->ReportError(message.str());
		token = lex->GetToken();
	} else {
		lex->debug << "Using Rule 1\n";
		p2file << "Using Rule 1\n";
		if (token != LPAREN_T){
		// 1.<program> -> LPAREN_T <define> LPAREN_T <more_defines> EOF_T
			errors++;
			message << "'" << lex->GetTokenName(LPAREN_T);
			message << "' expected";
			lex->ReportError(message.str());
		} token = lex->GetToken();
		define();
		if (token != LPAREN_T){
			errors++;
			message << "'" << lex->GetTokenName(LPAREN_T);
			message << "' is expected";
			lex->ReportError(message.str());
			stringstream message;
			errors++;
			message << "'" << lex->GetLexeme() << "' unexpected";
			lex->ReportError(message.str());
			token = lex->GetToken();
			// since this was two errors, forcibly remove one token
		} token = lex->GetToken();
		// while (!(token != LPAREN_T && token != EOF_T && token != IDENT_T && token != DEFINE_T)){token = lex->GetToken();}
		more_defines();
		if (token != EOF_T){
			errors++;
			message << "'" << lex->GetTokenName(EOF_T);
			message << "' expected";
			lex->ReportError(message.str());
		}//else{program done}
	}
}
void SyntacticalAnalyzer::more_defines(){
	stringstream message;
	if (token == LPAREN_T){
		token = lex->GetToken();
		while (token == LPAREN_T){
			token = lex->GetToken();
			errors++;
			message << "'" << lex->GetLexeme() << "' unexpected";
			lex->ReportError(message.str());
			token = lex->GetToken();
		}
	} else if (token == IDENT_T){
		// 3.<more_defines> -> IDENT_T <stmt_list> RPAREN_T
		lex->debug << "Using Rule 3\n";
		p2file << "Using Rule 3\n";
		token = lex->GetToken();
		stmt_list();
		if (token != RPAREN_T){ 
			errors++;
			message << "'" << lex->GetTokenName(RPAREN_T);
			message << "' expected";
			lex->ReportError(message.str());
		} token = lex->GetToken();
	} else { // 2.<more_defines> -> <define> LPAREN_T <more_defines>
		if (token == DEFINE_T){
			lex->debug << "Using Rule 2\n";
			p2file << "Using Rule 2\n";
			define();
			if (token != LPAREN_T){
				errors++;
				message << "'" << lex->GetTokenName(LPAREN_T);
				message << "' expected";
				lex->ReportError(message.str());
			} token = lex->GetToken();
			more_defines();
		}
	}
}
void SyntacticalAnalyzer::define(){
	stringstream message;
	// 4.<define> -> DEFINE_T LPAREN_T IDENT_T <param_list> RPAREN_T <stmt> <stmt_list> RPAREN_T
	lex->debug << "Using Rule 4\n";
	p2file << "Using Rule 4\n";
	if (token != DEFINE_T){
		errors++;
		message << "'" << lex->GetTokenName(DEFINE_T);
		message << "' expected";
		lex->ReportError(message.str());
	} token = lex->GetToken();
	if (token != LPAREN_T){
		errors++;
		message << "'" << lex->GetTokenName(LPAREN_T);
		message << "' expected";
		lex->ReportError(message.str());
	} token = lex->GetToken();
	if (token != IDENT_T){
		errors++;
		message << "'" << lex->GetTokenName(IDENT_T);
		message << "' expected";
		lex->ReportError(message.str());
	} token = lex->GetToken();
	param_list();
	if (token != RPAREN_T){
		errors++;
		message << "'" << lex->GetTokenName(RPAREN_T);
		message << "' expected";
		lex->ReportError(message.str());
	} token = lex->GetToken();
	stmt();
	stmt_list();
	if (token != RPAREN_T){
		errors++;
		message << "'" << lex->GetTokenName(RPAREN_T);
		message << "' expected";
		lex->ReportError(message.str());
	} token = lex->GetToken();
}
void SyntacticalAnalyzer::stmt_list(){
	if (token == RPAREN_T){ // 6.<stmt_list> -> λ
		// RPAREN_T denotes the end of the list.
		lex->debug << "Using Rule 6\n";
		p2file << "Using Rule 6\n";
		// token = lex->GetToken();
		// we don't get a new token since none should have been used
	} else {  // 5.<stmt_list> -> <stmt> <stmt_list>
		if (token == IDENT_T || token == LPAREN_T || token == NUMLIT_T
		 || token == STRLIT_T || token == SQUOTE_T){
			lex->debug << "Using Rule 5\n";
			p2file << "Using Rule 5\n";
		}
		stmt();
		stmt_list();
	}
}
void SyntacticalAnalyzer::stmt(){
	stringstream message;
	if (token == IDENT_T){ // 8.<stmt> -> IDENT_T
		lex->debug << "Using Rule 8\n";
		p2file << "Using Rule 8\n";
		token = lex->GetToken();
	} else if (token == LPAREN_T) {
		// 9.<stmt> -> LPAREN_T <action> RPAREN_T
		lex->debug << "Using Rule 9\n";
		p2file << "Using Rule 9\n";
		token = lex->GetToken();
		action();
		if (token != RPAREN_T){
			errors++;
			message << "'" << lex->GetTokenName(RPAREN_T);
			message << "' expected";
			lex->ReportError(message.str());
		} token = lex->GetToken();
	} else { // 7.<stmt> -> <literal>
		if (token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T){
			lex->debug << "Using Rule 7\n";
			p2file << "Using Rule 7\n";
		}
		literal();
	}
}
void SyntacticalAnalyzer::literal(){
	stringstream message;
	if (token == NUMLIT_T){ // 10.<literal> -> NUMLIT_T
		lex->debug << "Using Rule 10\n";
		p2file << "Using Rule 10\n";
		token = lex->GetToken();
	} else if (token == STRLIT_T){ // 11.<literal> -> STRLIT_T
		lex->debug << "Using Rule 11\n";
		p2file << "Using Rule 11\n";
		token = lex->GetToken();
	} else if (token == SQUOTE_T){ // 12.<literal> -> SQUOTE_T <quoted_lit>
		lex->debug << "Using Rule 12\n";
		p2file << "Using Rule 12\n";
		token = lex->GetToken();
		quoted_lit();
	} else { errors++;
		message << "'" << lex->GetLexeme() << "' unexpected";
		lex->ReportError(message.str());
		token = lex->GetToken();
	}
}
void SyntacticalAnalyzer::quoted_lit(){
	// 13.<quoted_lit> -> <any_other_token>
	lex->debug << "Using Rule 13\n";
	p2file << "Using Rule 13\n";
	any_other_token();
}
void SyntacticalAnalyzer::more_tokens(){
	if (token == RPAREN_T){ // 15.<more_tokens> -> λ
		// RPAREN_T denotes the end of the list.
		lex->debug << "Using Rule 15\n";
		p2file << "Using Rule 15\n";
		// token = lex->GetToken();
		// we don't get a new token since none should have been used
	} else { // 14.<more_tokens> -> <any_other_token> <more_tokens>
		lex->debug << "Using Rule 14\n";
		p2file << "Using Rule 14\n";
		any_other_token();
		more_tokens();
	}
}
void SyntacticalAnalyzer::param_list(){
	stringstream message;
	if (token == RPAREN_T){ // 17.<param_list> -> λ 
		// RPAREN_T denotes the end of the list.
		lex->debug << "Using Rule 17\n";
		p2file << "Using Rule 17\n";
		// token = lex->GetToken();
		// we don't get a new token since none should have been used
	} else if (token == IDENT_T){
		// 16.<param_list> -> IDENT_T <param_list>
		lex->debug << "Using Rule 16\n";
		p2file << "Using Rule 16\n";
		token = lex->GetToken();
		param_list();
	} else { errors++;
		message << "'" << lex->GetLexeme() << "' unexpected";
		lex->ReportError(message.str());
	}
}
void SyntacticalAnalyzer::else_part(){
	if (token == RPAREN_T){ // 19.<else_part> -> λ
		// RPAREN_T denotes the end of the list.
		lex->debug << "Using Rule 19\n";
		p2file << "Using Rule 19\n";
		// token = lex->GetToken();
		// we don't get a new token since none should have been used
	} else { // 18.<else_part> -> <stmt>
		lex->debug << "Using Rule 18\n";
		p2file << "Using Rule 18\n";
		stmt();
	}
}
void SyntacticalAnalyzer::stmt_pair(){
	stringstream message;
	if (token == RPAREN_T){ // 21.<stmt_pair> -> λ
		// RPAREN_T denotes the end of the list.
		lex->debug << "Using Rule 21\n";
		p2file << "Using Rule 21\n";
		// token = lex->GetToken();
		// we don't get a new token since none should have been used
	} else if (token == LPAREN_T){
		// 20.<stmt_pair> -> LPAREN_T <stmt_pair_body>
		lex->debug << "Using Rule 20\n";
		p2file << "Using Rule 20\n";
		token = lex->GetToken();
		stmt_pair_body();
	} else { errors++;
		message << "'" << lex->GetLexeme() << "' unexpected";
		lex->ReportError(message.str());
	}
}
void SyntacticalAnalyzer::stmt_pair_body(){
	stringstream message;
	if (token == ELSE_T){ // 23.<stmt_pair_body> -> ELSE_T <stmt> RPAREN_T
		lex->debug << "Using Rule 23\n";
		p2file << "Using Rule 23\n";
		token = lex->GetToken();
		stmt();
		if (token != RPAREN_T){
			errors++;
			message << "'" << lex->GetTokenName(RPAREN_T);
			message << "' expected";
			lex->ReportError(message.str());
		}
		token = lex->GetToken();
	}
	else { // 22.<stmt_pair_body> -> <stmt> <stmt> RPAREN_T <stmt_pair>
		lex->debug << "Using Rule 22\n";
		p2file << "Using Rule 22\n";
		stmt();
		stmt();
		if (token != RPAREN_T){
			errors++;
			message << "'" << lex->GetTokenName(RPAREN_T);
			message << "' expected";
			lex->ReportError(message.str());
		}
		token = lex->GetToken();
		stmt_pair();
	}
}
void SyntacticalAnalyzer::assign_pair(){
	// 24.<assign_pair> -> LBRACK_T IDENT_T <stmt> RBRACK_T
	stringstream message;
	lex->debug << "Using Rule 24\n";
	p2file << "Using Rule 24\n";
	if (token == LPAREN_T){token = lex->GetToken();}
	if (token != LBRACK_T){
		errors++;
		message << "'" << lex->GetTokenName(LBRACK_T);
		message << "' expected";
		lex->ReportError(message.str());
	} token = lex->GetToken();
	if (token != IDENT_T){
		errors++;
		message << "'" << lex->GetTokenName(IDENT_T);
		message << "' expected";
		lex->ReportError(message.str());
	} token = lex->GetToken();
	stmt();
	if (token != RBRACK_T){
		errors++;
		message << "'" << lex->GetTokenName(RBRACK_T);
		message << "' expected";
		lex->ReportError(message.str());
	} token = lex->GetToken();
}
void SyntacticalAnalyzer::more_assigns(){
	if (token == RPAREN_T){ // 26.<more_assigns> -> λ
		lex->debug << "Using Rule 26\n";
		p2file << "Using Rule 26\n";
		// token = lex->GetToken();
		// we don't get a new token since none should have been used
	} else { // 25.<more_assigns> -> <assign_pair> <more_assigns>
		lex->debug << "Using Rule 25\n";
		p2file << "Using Rule 25\n";
		assign_pair();
		more_assigns();
	}
}
void SyntacticalAnalyzer::action(){
	stringstream message;
	if (token == IF_T){ // 27.<action> -> IF_T <stmt> <stmt> <else_part>
		lex->debug << "Using Rule 27\n";
		p2file << "Using Rule 27\n";
		token = lex->GetToken();
		stmt();
		stmt();
		else_part();
	} else if (token == COND_T){
		// 28.<action> -> COND_T LPAREN_T <stmt_pair_body>
		lex->debug << "Using Rule 28\n";
		p2file << "Using Rule 28\n";
		token = lex->GetToken();
		if (token != LPAREN_T){
			errors++;
			message << "'" << lex->GetTokenName(LPAREN_T);
			message << "' expected";
			lex->ReportError(message.str());
		} token = lex->GetToken();
		stmt_pair_body();
	} else if (token == LET_T){
// 29.<action> -> LET_T LPAREN_T <more_assigns> RPAREN_T <stmt> <stmt_list>
		lex->debug << "Using Rule 29\n";
		p2file << "Using Rule 29\n";
		token = lex->GetToken();
		if (token != LPAREN_T){
			errors++;
			message << "'" << lex->GetTokenName(LPAREN_T);
			message << "' expected";
			lex->ReportError(message.str());
		} token = lex->GetToken();
		more_assigns();
		if (token != RPAREN_T){
			errors++;
			message << "'" << lex->GetTokenName(RPAREN_T);
			message << "' expected";
			lex->ReportError(message.str());
		} token = lex->GetToken();
		stmt();
		stmt_list();
	} else if (token == LISTOP1_T // 30.<action> -> LISTOP1_T <stmt>
		|| token == NOT_T // 34.<action> -> NOT_T <stmt>
		|| token == NUMBERP_T // 35.<action> -> NUMBERP_T <stmt>
		|| token == LISTP_T // 36.<action> -> LISTP_T <stmt>
		|| token == ZEROP_T // 37.<action> -> ZEROP_T <stmt>
		|| token == NULLP_T // 38.<action> -> NULLP_T <stmt>
		|| token == STRINGP_T // 39.<action> -> STRINGP_T <stmt>
		|| token == ROUND_T // 45.<action> -> ROUND_T <stmt>
		|| token == DISPLAY_T // 52.<action> -> DISPLAY_T <stmt>
	){	if (token == LISTOP1_T){
			lex->debug << "Using Rule 30\n";
			p2file << "Using Rule 30\n";
		} else if (token == NOT_T){
			lex->debug << "Using Rule 34\n";
			p2file << "Using Rule 34\n";
		} else if (token == NUMBERP_T){
			lex->debug << "Using Rule 35\n";
			p2file << "Using Rule 35\n";
		} else if (token == LISTP_T){
			lex->debug << "Using Rule 36\n";
			p2file << "Using Rule 36\n";
		} else if (token == ZEROP_T){
			lex->debug << "Using Rule 37\n";
			p2file << "Using Rule 37\n";
		} else if (token == NULLP_T){
			lex->debug << "Using Rule 38\n";
			p2file << "Using Rule 38\n";
		} else if (token == STRINGP_T){
			lex->debug << "Using Rule 39\n";
			p2file << "Using Rule 39\n";
		} else if (token == ROUND_T){
			lex->debug << "Using Rule 45\n";
			p2file << "Using Rule 45\n";
		} else if (token == DISPLAY_T){
			lex->debug << "Using Rule 52\n";
			p2file << "Using Rule 52\n";
		}
		token = lex->GetToken();
		stmt();
	} else if (token == LISTOP2_T // 31.<action> -> LISTOP2_T <stmt> <stmt>
		|| token == MODULO_T // 44.<action> -> MODULO_T <stmt> <stmt>
	){	if (token == LISTOP2_T){
			lex->debug << "Using Rule 31\n";
			p2file << "Using Rule 31\n";
		} else if (token == MODULO_T){
			lex->debug << "Using Rule 44\n";
			p2file << "Using Rule 44\n";
		}
		token = lex->GetToken();
		stmt();
		stmt();
	} else if (token == AND_T // 32.<action> -> AND_T <stmt_list>
		|| token == OR_T // 33.<action> -> OR_T <stmt_list>
		|| token == PLUS_T // 40.<action> -> PLUS_T <stmt_list>
		|| token == MULT_T // 43.<action> -> MULT_T <stmt_list>
		|| token == EQUALTO_T // 46.<action> -> EQUALTO_T <stmt_list>
		|| token == GT_T // 47.<action> -> GT_T <stmt_list>
		|| token == LT_T // 48.<action> -> LT_T <stmt_list>
		|| token == GTE_T // 49.<action> -> GTE_T <stmt_list>
		|| token == LTE_T // 50.<action> -> LTE_T <stmt_list>
		|| token == IDENT_T // 51.<action> -> IDENT_T <stmt_list>
	){	if (token == AND_T){
			lex->debug << "Using Rule 32\n";
			p2file << "Using Rule 32\n";
		} else if (token == OR_T){
			lex->debug << "Using Rule 33\n";
			p2file << "Using Rule 33\n";
		} else if (token == PLUS_T){
			lex->debug << "Using Rule 40\n";
			p2file << "Using Rule 40\n";
		} else if (token == MULT_T){
			lex->debug << "Using Rule 43\n";
			p2file << "Using Rule 43\n";
		} else if (token == EQUALTO_T){
			lex->debug << "Using Rule 46\n";
			p2file << "Using Rule 46\n";
		} else if (token == GT_T){
			lex->debug << "Using Rule 47\n";
			p2file << "Using Rule 47\n";
		} else if (token == LT_T){
			lex->debug << "Using Rule 48\n";
			p2file << "Using Rule 48\n";
		} else if (token == GTE_T){
			lex->debug << "Using Rule 49\n";
			p2file << "Using Rule 49\n";
		} else if (token == LTE_T){
			lex->debug << "Using Rule 50\n";
			p2file << "Using Rule 50\n";
		} else if (token == IDENT_T){
			lex->debug << "Using Rule 51\n";
			p2file << "Using Rule 51\n";
		}
		token = lex->GetToken();
		stmt_list();
	} else if (token == MINUS_T //41.<action> -> MINUS_T <stmt> <stmt_list>
		|| token == DIV_T // 42.<action> -> DIV_T <stmt> <stmt_list>
	){	if (token == MINUS_T){
			lex->debug << "Using Rule 41\n";
			p2file << "Using Rule 41\n";
		} else if (token == DIV_T){
			lex->debug << "Using Rule 42\n";
			p2file << "Using Rule 42\n";
		}
		token = lex->GetToken();
		stmt();
		stmt_list();
	} else if (token == NEWLINE_T){ // 53.<action> -> NEWLINE_T
		lex->debug << "Using Rule 53\n";
		p2file << "Using Rule 53\n";
		token = lex->GetToken();
	} else { errors++;
		message << "'" << lex->GetLexeme() << "' unexpected";
		lex->ReportError(message.str());
		token = lex->GetToken();
		action();
	}
}
void SyntacticalAnalyzer::any_other_token(){
	stringstream message;
	if (token == LPAREN_T){
		// 54.<any_other_token> -> LPAREN_T <more_tokens> RPAREN_T
		lex->debug << "Using Rule 54\n";
		p2file << "Using Rule 54\n";
		token = lex->GetToken();
		more_tokens();
		if (token != RPAREN_T){
			errors++;
			message << "'" << lex->GetTokenName(RPAREN_T);
			message << "' expected";
			lex->ReportError(message.str());
		} token = lex->GetToken();
	} else if (token == IDENT_T // 55.<any_other_token> -> IDENT_T
		|| token == NUMLIT_T // 56.<any_other_token> -> NUMLIT_T
		|| token == STRLIT_T // 57.<any_other_token> -> STRLIT_T
		|| token == IF_T // 58.<any_other_token> -> IF_T
		|| token == DISPLAY_T // 59.<any_other_token> -> DISPLAY_T
		|| token == NEWLINE_T // 60.<any_other_token> -> NEWLINE_T
		|| token == LISTOP1_T // 61.<any_other_token> -> LISTOP1_T
		|| token == LISTOP2_T // 62.<any_other_token> -> LISTOP2_T
		|| token == AND_T // 63.<any_other_token> -> AND_T
		|| token == OR_T // 64.<any_other_token> -> OR_T
		|| token == NOT_T // 65.<any_other_token> -> NOT_T
		|| token == DEFINE_T // 66.<any_other_token> -> DEFINE_T
		|| token == LET_T // 67.<any_other_token> -> LET_T
		|| token == NUMBERP_T // 68.<any_other_token> -> NUMBERP_T
		|| token == LISTP_T // 69.<any_other_token> -> LISTP_T
		|| token == ZEROP_T // 70.<any_other_token> -> ZEROP_T
		|| token == NULLP_T // 71.<any_other_token> -> NULLP_T
		|| token == STRINGP_T // 72.<any_other_token> -> STRINGP_T
		|| token == PLUS_T // 73.<any_other_token> -> PLUS_T
		|| token == MINUS_T // 74.<any_other_token> -> MINUS_T
		|| token == DIV_T // 75.<any_other_token> -> DIV_T
		|| token == MULT_T // 76.<any_other_token> -> MULT_T
		|| token == MODULO_T // 77.<any_other_token> -> MODULO_T
		|| token == ROUND_T // 78.<any_other_token> -> ROUND_T
		|| token == EQUALTO_T // 79.<any_other_token> -> EQUALTO_T
		|| token == GT_T // 80.<any_other_token> -> GT_T
		|| token == LT_T // 81.<any_other_token> -> LT_T
		|| token == GTE_T // 82.<any_other_token> -> GTE_T
		|| token == LTE_T // 83.<any_other_token> -> LTE_T
		|| token == COND_T // 85.<any_other_token> -> COND_T
		|| token == ELSE_T // 86.<any_other_token> -> ELSE_T
	){	if (token == IDENT_T){
			lex->debug << "Using Rule 55\n";
			p2file << "Using Rule 55\n";
		} else if (token == NUMLIT_T){
			lex->debug << "Using Rule 56\n";
			p2file << "Using Rule 56\n";
		} else if (token == STRLIT_T){
			lex->debug << "Using Rule 57\n";
			p2file << "Using Rule 57\n";
		} else if (token == IF_T){
			lex->debug << "Using Rule 58\n";
			p2file << "Using Rule 58\n";
		} else if (token == DISPLAY_T){
			lex->debug << "Using Rule 59\n";
			p2file << "Using Rule 59\n";
		} else if (token == NEWLINE_T){
			lex->debug << "Using Rule 60\n";
			p2file << "Using Rule 60\n";
		} else if (token == LISTOP1_T){
			lex->debug << "Using Rule 61\n";
			p2file << "Using Rule 61\n";
		} else if (token == LISTOP2_T){
			lex->debug << "Using Rule 62\n";
			p2file << "Using Rule 62\n";
		} else if (token == AND_T){
			lex->debug << "Using Rule 63\n";
			p2file << "Using Rule 63\n";
		} else if (token == OR_T){
			lex->debug << "Using Rule 64\n";
			p2file << "Using Rule 64\n";
		} else if (token == NOT_T){
			lex->debug << "Using Rule 65\n";
			p2file << "Using Rule 65\n";
		} else if (token == DEFINE_T){
			lex->debug << "Using Rule 66\n";
			p2file << "Using Rule 66\n";
		} else if (token == LET_T){
			lex->debug << "Using Rule 67\n";
			p2file << "Using Rule 67\n";
		} else if (token == NUMBERP_T){
			lex->debug << "Using Rule 68\n";
			p2file << "Using Rule 68\n";
		} else if (token == LISTP_T){
			lex->debug << "Using Rule 69\n";
			p2file << "Using Rule 69\n";
		} else if ( token == ZEROP_T){
			lex->debug << "Using Rule 70\n";
			p2file << "Using Rule 70\n";
		} else if (token == NULLP_T){
			lex->debug << "Using Rule 71\n";
			p2file << "Using Rule 71\n";
		} else if (token == STRINGP_T){
			lex->debug << "Using Rule 72\n";
			p2file << "Using Rule 72\n";
		} else if (token == PLUS_T){
			lex->debug << "Using Rule 73\n";
			p2file << "Using Rule 73\n";
		} else if (token == MINUS_T){
			lex->debug << "Using Rule 74\n";
			p2file << "Using Rule 74\n";
		} else if (token == DIV_T){
			lex->debug << "Using Rule 75\n";
			p2file << "Using Rule 75\n";
		} else if (token == MULT_T){
			lex->debug << "Using Rule 76\n";
			p2file << "Using Rule 76\n";
		} else if (token == MODULO_T){
			lex->debug << "Using Rule 77\n";
			p2file << "Using Rule 77\n";
		} else if (token == ROUND_T){
			lex->debug << "Using Rule 78\n";
			p2file << "Using Rule 78\n";
		} else if (token == EQUALTO_T){
			lex->debug << "Using Rule 79\n";
			p2file << "Using Rule 79\n";
		} else if (token == GT_T){
			lex->debug << "Using Rule 80\n";
			p2file << "Using Rule 80\n";
		} else if (token == LT_T){
			lex->debug << "Using Rule 81\n";
			p2file << "Using Rule 81\n";
		} else if (token == GTE_T){
			lex->debug << "Using Rule 82\n";
			p2file << "Using Rule 82\n";
		} else if (token == LTE_T){
			lex->debug << "Using Rule 83\n";
			p2file << "Using Rule 83\n";
		} else if (token == COND_T){
			lex->debug << "Using Rule 85\n";
			p2file << "Using Rule 85\n";
		} else if (token == ELSE_T){
			lex->debug << "Using Rule 86\n";
			p2file << "Using Rule 86\n";
		}
		token = lex->GetToken();
	} else if (token == SQUOTE_T){
		// 84.<any_other_token> -> SQUOTE_T <any_other_token>
		lex->debug << "Using Rule 84\n";
		p2file << "Using Rule 84\n";
		token = lex->GetToken();
		any_other_token();
	} else { errors++;
		message << "'" << lex->GetLexeme() << "' unexpected";
		lex->ReportError(message.str());
	}
}
