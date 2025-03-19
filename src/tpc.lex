%{
#include "tree.h"
#include "y.tab.h" 

int lineno = 1;
%}

%x COMMENT COMMENT_slash

%option noinput nounput

%%

[ \t]+        { /* Ignorer les espaces et les tabulations et les sauts de lignes */}
[\n]         {lineno++;}

"static"              { return STATIC; }  
"int"                 { yylval.ident = strdup(yytext); return TYPE;  }
"char"                { yylval.ident = strdup(yytext); return TYPE;  }
"void"                { yylval.ident = strdup(yytext); return VOID; }
"if"                  { return IF; }
"else"                { return ELSE; }
"while"               { return WHILE; }
"return"              { return RETURN; }

[a-zA-Z_][a-zA-Z_0-9]*  { yylval.ident = strdup(yytext); return IDENT; }
[0-9]+        { yylval.num = atoi(yytext); return NUM; }
'\\.'|'[^\\]'             { yylval.byte = yytext[1];     return CHARACTER; }

"=="            {yylval.ident = strdup(yytext); return EQ; }
"!="            {yylval.ident = strdup(yytext); return EQ; }
"<="|">="|"<"|">" {yylval.ident = strdup(yytext); return ORDER; }

"+"|"-"         {yylval.ident = strdup(yytext); return ADDSUB; }
"*"|"/"|"%"     {yylval.ident = strdup(yytext); return DIVSTAR; }

"&&"            {   return AND;}
"||"            {   return OR; }

"="             {   return '='; }
";"             {   return ';' ; }
","             {   return ','; }
"("             {   return '('; }
")"             {   return ')'; }
"{"             {   return '{'; }
"}"             {   return '}'; }
"!"             {   return '!' ; }

"//"            { BEGIN (COMMENT_slash) ;  }
<COMMENT_slash>. { /*ignrer toute la ligne */ }
<COMMENT_slash>\n { BEGIN (INITIAL); }
 

"/*"            { BEGIN (COMMENT); } /* Début commentaire multi-ligne */
<COMMENT>.      { /* Ignore les caractères dans les commentaires */ }
<COMMENT>\n     { 
                /* Ignore les sauts de ligne dans les commentaires */ 
                lineno++;
                }
<COMMENT>"*/"   { BEGIN (INITIAL); } /* Fin commentaire multi-ligne */

.               { fprintf(stderr, "Erreur lexicale : %s\n", yytext); return yytext[0]; }

%%
void yyerror(char * s){
    fprintf(stderr, "%s\n", s);
}

int yywrap(){
    return 1;
}
