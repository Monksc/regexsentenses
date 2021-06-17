%{
    #include "y.tab.h"
    int yylex();
%}

%%
"\\n"               { yylval.c = '\n'; return character; }
"\\t"               { yylval.c = '\t'; return character; }
"\\".               { yylval.c = yytext[1]; return character; }
"{"[1-9][0-9]*":"[1-9][0-9]*"}" { yylval.range[0] = atoi(yytext+1);
                        yylval.range[1] = atoi(index(yytext+1, ':')+1); 
                        return rangetoken; }
"{"[1-9][0-9]*"}"   { yylval.range[0] = atoi(yytext+1);
                        yylval.range[1] = yylval.range[0]; return rangetoken; }
"["                 { return leftbox; }
"]"                 { return rightbox; }
"("                 { return leftparanthesis; }
")"                 { return rightparanthesis; }
"*"                 { yylval.range[0] = 0;
                        yylval.range[1] = 255; return rangetoken; }
"+"                 { yylval.range[0] = 1;
                        yylval.range[1] = 255; return rangetoken; }
[.]                 { printf("MARsSH\n"); return dot; }
[-]                 { return yytext[0]; }
[\t\n]              ;
.                   { yylval.c = yytext[0]; return character;  }
%%

int yywrap(void) { return 1; }


