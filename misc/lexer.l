%{
#include "parser.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
int yylex(void);

int hexStringToInt(const char* hexStr) { //parameter can be in hex format or decimal, it will return decimal number
  int result = 0;
  int isHex = 0;
  const char* p = hexStr;

  // Skip optional "0x" or "0X" prefix
  if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) {
    isHex = 1;
    p += 2;
  }
  
  // Process each character in the string
  while (*p) {
    char ch = *p;
    int value;

    if(isHex == 1){
      if (isdigit(ch)) {
        value = ch - '0';
      } else if (isalpha(ch)) {
        value = tolower(ch) - 'a' + 10;
      } else {
        fprintf(stderr, "Error: Invalid hexadecimal character '%c'\n", ch);
      }

      if (value < 0 || value > 15) {
        fprintf(stderr, "Error: Invalid hexadecimal character '%c'\n", ch);
      }

      result = result * 16 + value;
      p++;
    }
    else {
      if (isdigit(ch)) {
        value = ch - '0';
        result = result * 10 + value;
      } else {
        fprintf(stderr, "Error: Invalid decimal character '%c'\n", ch);
      }
    }
  }
  return result;
}
%}
%option outfile="lexer.c" header-file="lexer.h"
%option noyywrap

%%

[ \r\n\t]+                  { /* skip whitespace */ }
"#"[^\n\r]*                 { /* skip comments */ }
","                         { return COMMA; }
":"                         { return COLON; }
".global"                   { return GLOBAL; }
".extern"                   { return EXTERN; }
".section"                  { return SECTION; }
".word"                     { return WORD; }
".skip"                     { return SKIP; }
".ascii"                    { return ASCII; }
".equ"                      { return EQU; }
".end"                      { return END; }
"halt"                      { return HALT; }
"int"                       { return INT; }
"iret"                      { return IRET; }
"call"                      { return CALL; }
"ret"                       { return RET; }
"jmp"                       { return JMP; }
"beq"                       { return BEQ; }
"bne"                       { return BNE; }
"bgt"                       { return BGT; }
"push"                      { return PUSH; }
"pop"                       { return POP; }
"xchg"                      { return XCHG; }
"add"                       { return ADD; }
"sub"                       { return SUB; }
"mul"                       { return MUL; }
"div"                       { return DIV; }
"not"                       { return NOT; }
"and"                       { return AND; }
"or"                        { return OR; }
"xor"                       { return XOR; }
"shl"                       { return SHL; }
"shr"                       { return SHR; }
"ld"                        { return LD; }
"st"                        { return ST; }
"csrrd"                     { return CSRRD; }
"csrwr"                     { return CSRWR; }
"+"                         { return PLUS; }
\$[A-Za-z_][A-Za-z0-9_]*    { yylval.val_str = strdup(yytext + 1); return DOLLAR_SYMBOL; }
\$0x[0-9a-fA-Fx]+           { yylval.val1 = hexStringToInt(yytext + 1); return DOLLAR_LITERAL; }
\$[0-9]+                    { yylval.val1 = atoi(yytext + 1); return DOLLAR_LITERAL; }
[A-Za-z_][A-Za-z0-9_]*:     { yylval.val_str = strdup(yytext); yylval.val_str[strlen(yylval.val_str) - 1] = '\0'; return LABEL; }
[A-Za-z_][A-Za-z0-9_]*      { yylval.val_str = strdup(yytext); return SYMBOL; }
0x[0-9a-fA-F]+              { yylval.val1 = hexStringToInt(yytext); return LITERAL; } // hex literal
[0-9]+                      { yylval.val1 = atoi(yytext); return LITERAL; } // decimal literal

"%"("r0"|"r1"|"r2"|"r3"|"r4"|"r5"|"r6"|"r7"|"r8"|"r9"|"r10"|"r11"|"r12"|"r13"|"r14"|"r15"|"sp"|"pc") {
  if (strcmp(yytext, "%sp") == 0) {yylval.reg_num = 14;} 
  else if (strcmp(yytext, "%pc") == 0) {yylval.reg_num = 15;} 
  else {yylval.reg_num = atoi(&yytext[2]);}
  return GPRX; 
}
"%"("status"|"handler"|"cause") { 
  if (strcmp(yytext, "%status") == 0) {yylval.reg_num = 0;} 
  else if (strcmp(yytext, "%handler") == 0) {yylval.reg_num = 1;} 
  else if (strcmp(yytext, "%cause") == 0) {yylval.reg_num = 2;}
  return CSRX;
}
\"([^"]|\\.)*\"           { yylval.val_str = strdup(yytext); return STRING; }
"["                       { return LEFT_BRACKET; }
"]"                       { return RIGHT_BRACKET; }
%%