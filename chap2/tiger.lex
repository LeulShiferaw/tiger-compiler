%{
#include <string.h>
#include "util.h"
#include "tokens.h"
#include "errormsg.h"

int charPos=1;

int yywrap(void)
{
 charPos=1;
 return 1;
}


void adjust(void)
{
 EM_tokPos=charPos;
 charPos+=yyleng;
}

%}

letter  [a-zA-Z]
digits  [0-9]+
%Start START_STATE IN_STRING IN_COMMENT
%%
<START_STATE>\n	        {adjust(); EM_newline(); continue;}
<START_STATE>\r	        {adjust(); EM_newline(); continue;}
<START_STATE>\r\n	    {adjust(); EM_newline(); continue;}
<START_STATE>[ \t]	    {adjust(); continue;}

<START_STATE>for  	    {adjust(); return FOR;}
<START_STATE>while       {adjust(); return WHILE;}
<START_STATE>to          {adjust(); return TO;}
<START_STATE>break       {adjust(); return BREAK;}
<START_STATE>let         {adjust(); return LET;}
<START_STATE>in          {adjust(); return IN;}
<START_STATE>end         {adjust(); return END;}
<START_STATE>function    {adjust(); return FUNCTION;}
<START_STATE>var         {adjust(); return VAR;}
<START_STATE>type        {adjust(); return TYPE;}
<START_STATE>array       {adjust(); return ARRAY;}
<START_STATE>if          {adjust(); return IF;}
<START_STATE>then        {adjust(); return THEN;}
<START_STATE>else        {adjust(); return ELSE;}
<START_STATE>do          {adjust(); return DO;}
<START_STATE>of          {adjust(); return OF;}
<START_STATE>nil         {adjust(); return NIL;}

<START_STATE>","	            {adjust(); return COMMA;}
<START_STATE>";"             {adjust(); return SEMICOLON;}
<START_STATE>":"             {adjust(); return COLON;}
<START_STATE>"("             {adjust(); return LPAREN;}
<START_STATE>")"             {adjust(); return RPAREN;}
<START_STATE>"["             {adjust(); return LBRACK;}
<START_STATE>"]"             {adjust(); return RBRACK;}
<START_STATE>"{"             {adjust(); return LBRACE;}
<START_STATE>"}"             {adjust(); return RBRACE;}
<START_STATE>"."             {adjust(); return DOT;}
<START_STATE>"+"             {adjust(); return PLUS;}
<START_STATE>"-"             {adjust(); return MINUS;}
<START_STATE>"*"             {adjust(); return TIMES;}
<START_STATE>"/"             {adjust(); return DIVIDE;}
<START_STATE>"="             {adjust(); return EQ;}
<START_STATE>"<>"            {adjust(); return NEQ;}
<START_STATE>"<"             {adjust(); return LT;}
<START_STATE>">"             {adjust(); return GT;}
<START_STATE>">="            {adjust(); return GE;}
<START_STATE>"<="            {adjust(); return LE;}
<START_STATE>"&"             {adjust(); return AND;}
<START_STATE>"|"             {adjust(); return OR;}
<START_STATE>":="            {adjust(); return ASSIGN;}

<START_STATE>[a-zA-Z][a-zA-Z0-9_]*  {adjust(); yylval.sval = yytext; return ID;}
<START_STATE>[0-9]+	        {adjust(); yylval.ival=atoi(yytext); return INT;}

<START_STATE>"/*"           {adjust(); BEGIN IN_COMMENT; }
<IN_COMMENT>.               {adjust(); continue;}
<IN_COMMENT>"*/"            {adjust(); BEGIN START_STATE; }

<START_STATE>\"             {adjust(); BEGIN IN_STRING; yylval.sval = String("");}
<IN_STRING>\"               {adjust(); BEGIN START_STATE; return STRING;}
<IN_STRING>.                {adjust(); strcat(yylval.sval, String(yytext));}

<START_STATE>.	            {adjust(); EM_error(EM_tokPos,"illegal token");}
.	                        {adjust(); BEGIN START_STATE; yyless(0);}
