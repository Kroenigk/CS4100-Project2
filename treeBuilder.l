%{
/* ===== C DECLARATIONS ===== */
/// This code is written by Kylie Roenigk and Angie Bragg
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>

using namespace std;

#define TKBUILDNODE 001
#define TKFOR 002
#define TKPRINT 003
#define TKNAME 004
#define TKWEIGHT 005
#define TKISACHILDOF 006
#define TKIN 007
#define TKSTRINGLITERAL 010
#define TKINTLITERAL 011
#define TKVARIABLE 012
#define TKSYMBOL 013
#define TKCOMMA 014
#define TKUNMATCHED 015



extern "C" int yylex();
extern char *yytext;
FILE* out = NULL;

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
						fprintf(out, "TKBUILDNODE\n");
						return TKBUILDNODE;	
					}

"name" 				{
    					fprintf(out, "TKNAME\n");
    					return TKNAME;
					}

"weight" 			{
    					fprintf(out, "TKWEIGHT\n");
    					return TKWEIGHT;
					}

"isachildof" 		{
    					fprintf(out, "TKISACHILDOF\n");
    					return TKISACHILDOF;
					}

"for"       {   
				fprintf(out, "TKFOR\n");
				return TKFOR;   
			}

"in"        {   
				fprintf(out, "TKIN\n");
				return TKIN;    	
			}

"print"		{	
				fprintf(out, "TKPRINT\n");
				return TKPRINT;		
			}

[0-9]+          {   
					fprintf(out, "TKINTLITERAL(%s)\n", yytext);
					return TKINTLITERAL; 
				}

\"([^\"\\]|\\.)*\"          {   
								fprintf(out, "TKSTRINGLITERAL(%s)\n", yytext);
								return TKSTRINGLITERAL;     
							}

[a-zA-Z0-9_]*[a-zA-Z_][a-zA-Z0-9_]*     {   
											fprintf(out, "TKVARIABLE(%s)\n", yytext);
											return TKVARIABLE;   
										}

","			{	
				fprintf(out, "TKCOMMA\n");
				return TKCOMMA;		
			}

[=+()\[\];:{}]		{	
						fprintf(out, "%s\n", yytext);
						return yytext[0];	
					}

.					{ fprintf(out, "Unexpected character: %s\n", yytext); }

%%
int yywrap(void){
	return 1;
}

int main(){
	out = fopen("scanner_out.txt", "w");
	if(!out){
		perror("scanner_out.txt");
        return 1;
	}
	int token;
	token = yylex();
	while(token){
		token = yylex();
	}

	fprintf(out, "\n");
	fclose(out);
    return 0;
}
