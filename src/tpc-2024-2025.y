%{
#include <string.h>
#include "tree.h"
#include "compilateur.h"

extern FILE *yyin;
extern int lineno;  

void yyerror(char * s);
int yylex(void);
Node *root;
%}

%union {
    int num;         /* Pour les nombres */
    char byte;        /* Pour les caractères */
    char *ident;     /* Pour les identifiants */
    Node *node;      /* Pour les noeuds */
}

%token <ident> IDENT STATIC VOID IF ELSE WHILE RETURN
%token <byte> CHARACTER 
%token <ident> ADDSUB DIVSTAR
%token <ident> TYPE
%token <num> NUM
%token <ident> ORDER EQ OR AND
%token '{' '}' '(' ')' '!' ';' ','

%type <node> Prog DeclVarsGlobal DeclVars Declarateurs DeclFonct DeclFoncts EnTeteFonct Parametres ListTypVar Corps
%type <node> SuiteInstr Instr Exp TB FB M E T F Arguments ListExp 

%nonassoc THEN  
%nonassoc ELSE

%start Prog
%%
Prog:
    DeclVarsGlobal DeclFoncts{    
        root = makeNode("Program");
        Node * funs = makeNode("Functions");
        Node * global = $1;
        if (global->firstChild) addChild(root, $1);
        else { deleteTree(global);}
        addChild(root, funs);
        addChild(funs, $2);
    }
    ;

DeclVarsGlobal:
       DeclVarsGlobal TYPE Declarateurs ';' {
           $$ = $1;
           Node * t = makeNode($2);
           addChild($$, t);
           addChild(t, $3);
       }
    |
       { $$ = makeNode("Global"); }
    ;

    
DeclVars:
       DeclVars TYPE Declarateurs ';'{
            $$ = $1;
            Node * t = makeNode($2);
            addChild($$, t);
            addChild(t, $3);
       }
    |  DeclVars STATIC TYPE Declarateurs ';'{
            $$ = $1;
            Node * s = makeNode("Static");
            Node * t = makeNode($3);
            addChild($$, s);
            addChild(s, t);
            addChild(t, $4);
       }
    |
       { 
        $$ = makeNode("Vars");
       }
    ;

Declarateurs:
       Declarateurs ',' IDENT{
            $$ = makeNode($3);
            addSibling($$, $1);
       }
    |  IDENT {$$ = makeNode($1);}
    ;

DeclFoncts:
       DeclFoncts DeclFonct{
            $$ = $1;
            addSibling($$, $2);
       }
    |  DeclFonct { $$ = $1; }
    ;

DeclFonct:
       EnTeteFonct Corps {
            $$ = makeNode("Function");
            addChild($$, $1);
            addChild($$, $2);
       }
    ;

EnTeteFonct:
       TYPE IDENT '(' Parametres ')'{
            $$ = makeNode("Head");
            addChild($$, makeNode($1));
            addChild($$, makeNode($2));
            Node * p = makeNode("Parameter");
            addChild($$, p);
            addChild(p, $4);
       }
    |  VOID IDENT '(' Parametres ')'{
            $$ = makeNode("Head");
            addChild($$, makeNode($1));
            addChild($$, makeNode($2));
            Node * p = makeNode("Parameter");
            addChild($$, p);
            addChild(p, $4);
        }
    ;

Parametres:
       VOID { $$ = makeNode($1); }
    |  ListTypVar { $$ = $1; }
    ;

ListTypVar:
       ListTypVar ',' TYPE IDENT{
            $$ = $1;
            Node * t = makeNode($3);
            addSibling($$, t);
            addChild(t, makeNode($4));
        }
    |  TYPE IDENT{
            $$ = makeNode($1);
            addChild($$, makeNode($2));
        }
    ;

Corps: '{' DeclVars SuiteInstr '}'{
            $$ = makeNode("Body");
            addChild($$, $2);
            addChild($$, $3);
        }
    ;

SuiteInstr:
       SuiteInstr Instr{
            $$ = $1;
            addChild($$, $2);
        }
    | { $$ = makeNode("Suite_instr"); }
    ;

Instr:
       IDENT '=' Exp ';' {
            $$ = makeNode("Eq");
            addChild($$, makeNode($1));
            addChild($$, $3);
       }
    |  IF '(' Exp ')' Instr  %prec THEN {
            $$ =  makeNode("If");
            addChild($$, $3);
            Node * other = makeNode("Then");
            addSibling($$, other);
            addChild(other, $5);
       }
    |  IF '(' Exp ')' Instr ELSE Instr {
            $$ = makeNode("If");
            addChild($$, $3);
            addChild($$, $5);
            Node * other = makeNode("Else");
            addSibling($$, other);
            addChild(other, $7);
       }
    |  WHILE '(' Exp ')' Instr {
            $$ = makeNode("While");
            addChild($$, $3);
            addChild($$, $5);
       }
    |  IDENT '(' Arguments  ')' ';' {
            $$ = makeNode($1);
            addChild($$, $3);
       }
    |  RETURN Exp ';' {
            $$ = makeNode("Return");
            addChild($$, $2);
       }
    |  RETURN ';' { $$ =  makeNode("Return"); }
    |  '{' SuiteInstr '}' { $$ = $2; }
    |  ';' {$$ = NULL;}
    ;
  
Exp :  Exp OR TB {
            $$ = makeNode("Or");
            addChild($$, $1);
            addChild($$, $3);
        }
    |  TB { $$ = $1; }
    ;

TB  :  TB AND FB {
            $$ = makeNode("And");
            addChild($$, $1);
            addChild($$, $3);
        }
    |  FB { $$ = $1; }
    ;

FB  :  FB EQ M {
            $$ = makeNode("Eq");
            $$->value = $2;
            addChild($$, $1);
            addChild($$, $3);
        }
    |  M { $$ = $1; }
    ;

M   :  M ORDER E {
            $$ = makeNode("Order");
            $$->value = $2;
            addChild($$, $1);
            addChild($$, $3);
        }
    |  E { $$ = $1; }
    ;

E   :  E ADDSUB T {
            $$ = makeNode("AddSub");
            $$->value = $2;
            addChild($$, $1);
            addChild($$, $3);
        }
    |  T { $$ = $1; }
    ;    

T   :  T DIVSTAR F {
            $$ = makeNode("DivStar");
            $$->value = $2;
            addChild($$, $1);
            addChild($$, $3);
        }
    |  F { $$ = $1; }
    ;

F   :  ADDSUB F {
            $$ = makeNode("AddSub");
            $$->value = $1;
            addChild($$, $2);
        }
    |  '!' F { $$ = $2; }
    |  '(' Exp ')' { $$ = $2; }
    |  NUM { 
            char str[50];
            sprintf(str, "%d", $1);
            $$ = makeNode(str);
            }
    |  CHARACTER { 
                char str[50];
                sprintf(str, "'%c'", $1);
                $$ = makeNode(str); 
                }
    |  IDENT { $$ = makeNode($1); }
    |  IDENT '(' Arguments  ')'{
            $$ = makeNode($1);
            addChild($$, $3);
        }
    ;

Arguments:
       ListExp {
            $$ = makeNode("Args");
            addChild($$, $1);
        }
    | { $$ = NULL; }
    ;

ListExp:
       ListExp ',' Exp {
        $$ = $3;
        addSibling($$, $1);
       }
    |  Exp {$$ = $1;}
    ;

%%
void print_help(char * prog){
  printf("Commande :\n");
  printf("  %s [OPTIONS] [FICHIER]\n", prog);
  printf("Options : \n");
  printf("  -t, --tree    Affiche l'arbre abstrai du fichier\n");
  printf("  -h, --help    Affiche cette aide\n");
}

int option (char * arg){
  if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0){ return 1; }
  else if (strcmp(arg, "-t") == 0 || strcmp(arg, "--tree") == 0) { return 2; }
  return 0;  
}


int main(int argc, char * argv[]) {

    int opt = 3;
      
    if (argc == 3){
        yyin = fopen(argv[2], "r");

        if(!yyin){
            fprintf(stderr, "Le fichier n'existe pas !\n");
            return 1;
        }

        opt = option(argv[1]);

        switch (opt){
        case 0:
            fprintf(stderr, "L'option %s n'est pas reconnue !\n", argv[1]);
            return 1;
        case 1:
            print_help(argv[0]);
            return 0;
        default:
        break;
        }

    } else if (argc == 2 ){
        opt = option(argv[1]);
        switch (opt){

        case 0:{
            yyin = fopen(argv[1], "r");
            if(!yyin){
                fprintf(stderr, "Le fichier n'existe pas !\n");
                return 1;
            }
            break;

        }
            
        case 1:
            print_help(argv[0]);
            return 0;
        default:
        break;
        }
    }
    else if( argc > 3){
        fprintf(stderr,"Le nombre d'Arguments est incorrect\n");
        return 1;
    }
    int parseResult  = yyparse();

    if (yyin != NULL){
        fclose(yyin);
    }

    if (parseResult == 0) {

        printf("Analyse réussie.\n");
        translate(root);

        if (opt == 2){
            printTree(root);
            deleteTree(root);
            
        }        
        
    } else 
        printf("Erreur pendant l'analyse à la ligne %d, code de retour : %d\n",lineno, parseResult);

    return parseResult ; 
}
