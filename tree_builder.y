%start start_var
%left '*' '/' '%'
%left '+' '-'

%{
#include <map>
#include <string>
#include "parse_tree.h"

using namespace std;

%}

%union {
  char* s_val;
  integer_expression *int_ptr;
  string_expression *str_ptr;
  statement *s_ptr;
  compound_statement *c_ptr;
  string_list_node* str_list;
}

%{
extern int yylex();
extern void yyerror(char *String);
extern FILE *yyin;
extern FILE *out;

#include <iostream>
using namespace std;

%}
%token <s_val> TKBUILDNODE TKFOR TKPRINT TKNAME TKWEIGHT TKISACHILDOF TKIN TKSTRINGLITERAL TKINTLITERAL TKVARIABLE
%type <int_ptr> integer_expression
%type <str_ptr> string_expression
%type <s_ptr> statement for_statement int_assignment_statement string_assignment_statement print_statement build_node_statement name_assignment is_child_of_assignment weight_assignment
%type <c_ptr> prog start_var
%type <str_list> list string_list
%%

start_var: prog {
  map<string, int> int_sym_tab;
  map<string, string> str_sym_tab;
  $$ = $1;
  $1->evaluate_statement(int_sym_tab, str_sym_tab);
}

prog: statement prog { $$ = new compound_statement($1, $2); }
  | %empty { $$ = NULL; }
;

statement: for_statement { $$ = $1; }
  | int_assignment_statement { $$ = $1; }
  | string_assignment_statement { $$ = $1; }
  | print_statement { $$ = $1; }
  | build_node_statement { $$ = $1; }
  | name_assignment { $$ = $1; }
  | is_child_of_assignment { $$ = $1; }
  | weight_assignment { $$ = $1; }
;

print_statement: TKPRINT '(' string_expression ')' ';' { $$ = new print_statement($3); }
;

build_node_statement: TKBUILDNODE '{' prog '}' ';' { $$ = new build_node_statement($3); }
;

int_assignment_statement: TKVARIABLE '=' integer_expression ';' { $$ = new assignment_statement($1, $3); }
;

string_assignment_statement: TKVARIABLE '=' string_expression ';' { $$ = new assignment_statement($1, $3); }
;

name_assignment: TKNAME '=' string_expression ';' { $$ = new assignment_statement($1, $3); }
;

weight_assignment: TKWEIGHT '=' integer_expression ';' { $$ = new assignment_statement($1, $3); }
;

is_child_of_assignment: TKISACHILDOF '=' string_expression ';' { $$ = new assignment_statement($1, $3); }
;

list: '[' string_list ']' { $$ = $2; }
;
string_list: TKSTRINGLITERAL { $$ = new string_list_node($1, nullptr);}
  | TKSTRINGLITERAL ',' string_list { $$ = new string_list_node($1, $3);}
;

for_statement: TKFOR TKVARIABLE TKIN '[' TKINTLITERAL ':' TKINTLITERAL ']' '{' prog '}' ';' { $$ = new for_statement($2, atoi($5), atoi($7), $10); }
  | TKFOR TKVARIABLE TKIN list '{' prog '}' ';' { $$ = new for_statement($2, $4, $6);}
;

integer_expression: TKINTLITERAL { $$ = new int_constant(atoi($1)); }
  | TKVARIABLE { $$ = new variable($1); }
  | '-' integer_expression { $$ = new neg_constant($2); }
  | integer_expression '+' integer_expression { $$ = new plus_expr($1, $3); }
  | integer_expression '-' integer_expression { $$ = new minus_expr($1, $3); }
  | integer_expression '*' integer_expression { $$ = new mult_expr($1, $3); }
  | integer_expression '/' integer_expression { $$ = new div_expr($1, $3); }
  | integer_expression '%' integer_expression { $$ = new mod_expr($1, $3); }
  | '(' integer_expression ')' { $$ = $2; }
;

string_expression: TKSTRINGLITERAL { $$ = new str_lit_expr($1); }
  | TKVARIABLE '+' string_expression { $$ = new str_str_expr(new str_var_expr($1), $3); }
  | TKVARIABLE '+' integer_expression { $$ = new str_int_expr(new str_var_expr($1), $3); }
  | string_expression '+' string_expression { $$ = new str_str_expr($1, $3); }
  | string_expression '+' integer_expression { $$ = new str_int_expr($1, $3); }
  | integer_expression '+' string_expression { $$ = new int_str_expr($1, $3); }
  | TKVARIABLE { $$ = new str_var_expr($1); }
;
%%
#include "lex.yy.c"

void yyerror(char *error_string)
{
  cout << "Error : " << error_string << "  on line " << line_num() << endl;
  exit(-1);
}


int parse_input_file(const char *path) {
  FILE *input_file = stdin;
  if (path != NULL) {
    input_file = fopen(path, "r");
    if (input_file == NULL) {
      cerr << "Could not open input file: " << path << endl;
      return 1;
    }
  }

  yyin = input_file;
  out = fopen("/dev/null", "w");

  yyparse();

  if (out != NULL) {
    fclose(out);
  }
  if (path != NULL && input_file != NULL) {
    fclose(input_file);
  }
  return 0;
}
