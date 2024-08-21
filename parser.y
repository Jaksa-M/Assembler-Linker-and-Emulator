%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "definitions.h"

extern FILE *yyin;
extern int yylex();
void yyerror(const char *s);
int yyparse(void);
extern char *yytext;

extern void addToSymbolList(char* symbol);
extern void printSymbolList();
extern void clearSymbolList();
extern void addSymbolToWordList();
extern void addLiteralToWordList();
extern void printWordList();
extern void clearWordList();

//instruction functions:
extern void instruction_add_sub_mul_div_xchg(char* instr, int gprS, int gprD);
extern void instruction_and_or_xor_shl_shr(char* instr, int gprS, int gprD);
extern void instruction_not_push_pop(char* instr, int gpr);
extern void instruction_halt_int_iret_ret(char* instr);
extern void instruction_beq_bne_bgt(char* instr, int gpr1, int gpr2, char* what_op, int literal, char* symbol);
extern void instruction_call_jmp(char* instr, char* what_op, int literal, char* symbol);
extern void instruction_csrrd_csrrw(char* instr, int csr, int gpr);
extern void instruction_ld_st(char* instr, char* what_op, int gpr, int literal, char* symbol, int dollar_literal, char* dollar_symbol,
                                int reg, int mem_reg, int mem_reg_literal, int mem_reg_symbol);

//directives:
extern void directive_global();
extern void directive_extern();
extern void directive_section();
extern void directive_word();
extern void directive_skip(int literal);
extern void directive_ascii(const char* str);
extern void directive_equ();
extern void process_label(char* label);

//print functions:
extern void printSymbolTable();
extern void printFlinkTable();
extern void printRelocationTables();

//flink table functions:
extern void processFlinkTable();

//Elf function
extern void createELF();
extern void printMemoryMap();
%}

%union {
  int val1;
  int val2;
  char* val_str;
  int reg_num;
}

%token <val_str> SYMBOL
%token <val_str> DOLLAR_SYMBOL
%token <val_str> STRING 
%token <val_str> LABEL
%token <val1> LITERAL
%token <val1> DOLLAR_LITERAL
%token <reg_num> GPRX
%token <reg_num> CSRX
%token SECTION WORD SKIP ASCII EQU END COMMA COLON GLOBAL EXTERN
%token HALT INT IRET CALL RET JMP BEQ BNE BGT PUSH POP XCHG ADD SUB MUL DIV NOT AND OR XOR SHL SHR LD ST CSRRD CSRWR
%token LEFT_BRACKET RIGHT_BRACKET PLUS

%start program

%%

program: statements;

statements: /* empty */
          | statements statement;

statement: global_declaration
         | extern_declaration
         | section_declaration
         | word_declaration
         | skip_declaration
         | ascii_declaration
         | equ_declaration
         | label_definition
         | instruction
         | end_statement
         ;

global_declaration: GLOBAL SYMBOL_LIST { 
    printf("GLOBAL ");
    printSymbolList();
    directive_global();
    clearSymbolList();
};

extern_declaration: EXTERN SYMBOL_LIST {
    printf("EXTERN ");
    printSymbolList();
    directive_extern();
    clearSymbolList();
};

section_declaration: SECTION SYMBOL { printf("SECTION %s\n", $2); directive_section($2);};

word_declaration: WORD SYMBOL_LITERAL_LIST {
    printf("WORD ");
    printWordList();
    directive_word();
    clearWordList();
}

skip_declaration: SKIP LITERAL { printf("SKIP %d\n", $2); directive_skip($2);};

ascii_declaration: ASCII STRING { printf("ASCII %s\n", $2); directive_ascii($2);};

equ_declaration: EQU SYMBOL COMMA LITERAL { printf(".equ %d\n", $4); directive_equ();};

label_definition: LABEL { printf("LABEL: %s\n", $1); process_label($1);};

instruction: HALT { printf("HALT\n"); instruction_halt_int_iret_ret("halt");}
           | INT { printf("INT\n"); instruction_halt_int_iret_ret("int");}
           | IRET { printf("IRET\n"); instruction_halt_int_iret_ret("iret");}
           | CALL SYMBOL { printf("CALL %s\n", $2); instruction_call_jmp("call", "symbol", 0, $2);}
           | CALL LITERAL { printf("CALL %d\n", $2); instruction_call_jmp("call", "literal", $2, "");}
           | RET { printf("RET\n"); instruction_halt_int_iret_ret("ret");}
           | JMP SYMBOL { printf("JMP %s\n", $2); instruction_call_jmp("jmp", "symbol", 0, $2);}
           | JMP LITERAL { printf("JMP %d\n", $2); instruction_call_jmp("jmp", "symbol", $2, "");}
           | BEQ GPRX COMMA GPRX COMMA SYMBOL { printf("BEQ %d, %d, %s\n", $2, $4, $6); instruction_beq_bne_bgt("beq", $2, $4, "symbol", 0, $6);}
           | BEQ GPRX COMMA GPRX COMMA LITERAL { printf("BEQ %d, %d, %d\n", $2, $4, $6); instruction_beq_bne_bgt("beq", $2, $4, "literal", $6, "");}
           | BNE GPRX COMMA GPRX COMMA SYMBOL { printf("BNE %d, %d, %s\n", $2, $4, $6); instruction_beq_bne_bgt("bne", $2, $4, "symbol", 0, $6);}
           | BNE GPRX COMMA GPRX COMMA LITERAL { printf("BNE %d, %d, %d\n", $2, $4, $6); instruction_beq_bne_bgt("bne", $2, $4, "literal", $6, "");}
           | BGT GPRX COMMA GPRX COMMA SYMBOL { printf("BGT %d, %d, %s\n", $2, $4, $6); instruction_beq_bne_bgt("bgt", $2, $4, "symbol", 0, $6);}
           | BGT GPRX COMMA GPRX COMMA LITERAL { printf("BGT %d, %d, %d\n", $2, $4, $6); instruction_beq_bne_bgt("bgt", $2, $4, "literal", $6, "");}
           | PUSH GPRX { printf("PUSH %d\n", $2); instruction_not_push_pop("push",$2);}
           | POP GPRX { printf("POP %d\n", $2); instruction_not_push_pop("pop",$2);}
           | XCHG GPRX COMMA GPRX { printf("XCHG %d, %d\n", $2, $4); instruction_add_sub_mul_div_xchg("xchg", $2, $4);}
           | ADD GPRX COMMA GPRX { printf("ADD %d, %d\n", $2, $4); instruction_add_sub_mul_div_xchg("add", $2, $4);}
           | SUB GPRX COMMA GPRX { printf("SUB %d, %d\n", $2, $4); instruction_add_sub_mul_div_xchg("sub", $2, $4);}
           | MUL GPRX COMMA GPRX { printf("MUL %d, %d\n", $2, $4); instruction_add_sub_mul_div_xchg("mul", $2, $4);}
           | DIV GPRX COMMA GPRX { printf("DIV %d, %d\n", $2, $4); instruction_add_sub_mul_div_xchg("div", $2, $4);}
           | NOT GPRX { printf("NOT %d\n", $2); instruction_not_push_pop("not",$2);}
           | AND GPRX COMMA GPRX { printf("AND %d, %d\n", $2, $4); instruction_and_or_xor_shl_shr("and", $2, $4);}
           | OR GPRX COMMA GPRX { printf("OR %d, %d\n", $2, $4); instruction_and_or_xor_shl_shr("or", $2, $4);}
           | XOR GPRX COMMA GPRX { printf("XOR %d, %d\n", $2, $4); instruction_and_or_xor_shl_shr("xor", $2, $4);}
           | SHL GPRX COMMA GPRX { printf("SHL %d, %d\n", $2, $4); instruction_and_or_xor_shl_shr("shl", $2, $4);}
           | SHR GPRX COMMA GPRX { printf("SHR %d, %d\n", $2, $4); instruction_and_or_xor_shl_shr("shr", $2, $4);}
           | LD DOLLAR_SYMBOL COMMA GPRX { printf("LD %s, %d\n", $2, $4); instruction_ld_st("ld","dollar_symbol",$4,0,"",0,$2,0,0,0,0);}
           | LD DOLLAR_LITERAL COMMA GPRX { printf("LD %d, %d\n", $2, $4); instruction_ld_st("ld","dollar_literal",$4,0,"",$2,"",0,0,0,0);}
           | LD SYMBOL COMMA GPRX { printf("LD %s, %d\n", $2, $4); instruction_ld_st("ld","symbol",$4,0,$2,0,"",0,0,0,0);}
           | LD LITERAL COMMA GPRX { printf("LD %d, %d\n", $2, $4); instruction_ld_st("ld","literal",$4,$2,"",0,"",0,0,0,0);}
           | LD GPRX COMMA GPRX { printf("LD %d, %d\n", $2, $4); instruction_ld_st("ld","reg",$4,0,"",0,"",$2,0,0,0);}
           | LD LEFT_BRACKET GPRX RIGHT_BRACKET COMMA GPRX { printf("LD [%d], %d\n", $3, $6); instruction_ld_st("ld","mem_reg",$6,0,"",0,"",$3,0,0,0);}
           | LD LEFT_BRACKET GPRX PLUS LITERAL RIGHT_BRACKET COMMA GPRX { printf("LD [%d + %d], %d\n", $3, $5, $8); instruction_ld_st("ld","mem_reg_literal",$8,$5,"",0,"",$3,0,0,0);}
           | LD LEFT_BRACKET GPRX PLUS SYMBOL RIGHT_BRACKET COMMA GPRX { printf("LD [%d + %s], %d\n", $3, $5, $8); instruction_ld_st("ld","mem_reg_symbol",$8,0,$5,0,"",$3,0,0,0);}
           | ST GPRX COMMA DOLLAR_SYMBOL { printf("ST %d, %s\n", $2, $4); instruction_ld_st("st","dollar_symbol",$2,0,"",0,$4,0,0,0,0);}
           | ST GPRX COMMA DOLLAR_LITERAL { printf("ST %d, %d\n", $2, $4); instruction_ld_st("st","dollar_literal",$2,0,"",$4,"",0,0,0,0);}
           | ST GPRX COMMA SYMBOL { printf("ST %d, %s\n", $2, $4); instruction_ld_st("st","symbol",$2,0,$4,0,"",0,0,0,0);}
           | ST GPRX COMMA LITERAL { printf("ST %d, %d\n", $2, $4); instruction_ld_st("st","literal",$2,$4,"",0,"",0,0,0,0);}
           | ST GPRX COMMA GPRX { printf("ST %d, %d\n", $2, $4); instruction_ld_st("st","reg",$2,0,"",0,"",$4,0,0,0);}
           | ST GPRX COMMA LEFT_BRACKET GPRX RIGHT_BRACKET { printf("ST %d, [%d]\n", $2, $5); instruction_ld_st("st","mem_reg",$2,0,"",0,"",$5,0,0,0);}
           | ST GPRX COMMA LEFT_BRACKET GPRX PLUS LITERAL RIGHT_BRACKET { printf("ST %d, [%d + %d]\n", $2, $5, $7); instruction_ld_st("st","mem_reg_literal",$2,$7,"",0,"",$5,0,0,0);}
           | ST GPRX COMMA LEFT_BRACKET GPRX PLUS SYMBOL RIGHT_BRACKET { printf("ST %d, [%d + %s]\n", $2, $5, $7); instruction_ld_st("st","mem_reg_symbol",$2,0,$7,0,"",$5,0,0,0);}
           | CSRRD CSRX COMMA GPRX { printf("CSRRD %d, %d\n", $2, $4); instruction_csrrd_csrrw("csrrd", $2, $4);}
           | CSRWR GPRX COMMA CSRX { printf("CSRWR %d, %d\n", $2, $4); instruction_csrrd_csrrw("csrrw", $4, $2);}
           ;

end_statement: END { 
    printf("END\n");
    printSymbolTable();
    //printFlinkTable();
    processFlinkTable();
    printRelocationTables();
    printMemoryMap();
    createELF();
    YYACCEPT; //This macro will make the parser immediately accept the input and stop further parsing
};

SYMBOL_LIST: SYMBOL_LIST COMMA SYMBOL { addToSymbolList($3); }
           | SYMBOL { /* process single symbol */ addToSymbolList($1); }
           ;
SYMBOL_LITERAL_LIST: SYMBOL_LITERAL_LIST COMMA SYMBOL { addSymbolToWordList($3); }
           | SYMBOL_LITERAL_LIST COMMA LITERAL { addLiteralToWordList($3); }
           | SYMBOL { /* process single symbol */ addSymbolToWordList($1); }
           | LITERAL { /* process single literal */ addLiteralToWordList($1); }
           ;
%%

void yyerror(const char *s) {
    fprintf(stderr, "Parser error: %s\n", s);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror("Error opening file");
        return 1;
    }

    yyin = f;
    yyparse();
    fclose(f);

    return 0;
}
