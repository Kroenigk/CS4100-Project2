%{
/* ===== C DECLARATIONS ===== */
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <string.h>

using namespace std;

#define TKBUILDNODE 
#define TKFOR 
#define TKPRINT
#define TKNAME
#define TKWEIGHT
#define TKISCHILDOF
#define TKIN
#define TKSTRINGLITERAL
#define TKINTLITERAL
#define TKVARIABLE 



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
