%{
/* ===== C DECLARATIONS ===== */
/// This code is written by Kylie Roenigk and Angie Bragg
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include "tree_builder.tab.hpp"

using namespace std;

extern int yylex();
extern char *yytext;
FILE* out = stdout;

int linecount = 1;

void Addline(const char* text)
{
	for (int i = 0; text[i] != '\0'; i++) {
		if (text[i] == '\n') {
			linecount++;
		}
	}
}

int line_num(){
	return linecount;
}
%}

%x	COMMENT

%%

\n+       {   Addline(yytext);    }

[ \t\f\r]+		{	/* ignore */	}

"//"[^\n\r]*	{	/* ignore */	}

"/*"		{	BEGIN(COMMENT);		}

<COMMENT>"*/"	{	BEGIN(INITIAL);	}

<COMMENT>\n		{	Addline(yytext);	}

<COMMENT>\r		{	/* ignore */	}

<COMMENT>.		{	/* ignore */		}

\#[^\n\r]*     {		/* ignore */ 	}

"buildnode"			{	
						return TKBUILDNODE;	
					}

"name" 				{
						yylval.s_val = strdup("name");
    					return TKNAME;
					}

"weight" 			{
						yylval.s_val = strdup("weight");
    					return TKWEIGHT;
					}

"isachildof" 		{
						yylval.s_val = strdup("isachildof");
    					return TKISACHILDOF;
					}

"for"       {   
				return TKFOR;   
			}

"in"        {   
				return TKIN;    	
			}

"print"		{	
				return TKPRINT;		
			}

[0-9]+          {   
					yylval.s_val = strdup(yytext);
					return TKINTLITERAL; 
				}

\"([^\"\\]|\\.)*\"          {   
								yylval.s_val = strdup(yytext);
								return TKSTRINGLITERAL;     
							}

[a-zA-Z0-9_]*[a-zA-Z_][a-zA-Z0-9_]*     {   
											yylval.s_val = strdup(yytext);
											return TKVARIABLE;   
										}

","			{	
				return yytext[0];		
			}

[=+\-*/%()\[\];:{}]		{	
						fprintf(out, "%s\n", yytext);
						return yytext[0];	
					}

.					{ fprintf(out, "Unexpected character: %s\n", yytext); }

%%
int yywrap(void){
	return 1;
}
