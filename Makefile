assembler_:
	flex lexer.l
	bison -d parser.y
	gcc -g -o assembler parser.tab.c lexer.c definitions.cpp -lfl -lstdc++

clean:
	rm -f assembler parser.tab.c parser.tab.h lexer.c lex.yy.c elfoutput.o

clean_build_run: clean assembler_
	./assembler pixon_test2.s

.PHONY: assembler_ clean clean_build_run



linker_:
	gcc -g -o linker linker.cpp -lstdc++

clean_linker:
	rm -f linker

clean_build_run_linker: clean_linker linker_
	./linker -hex -o mem_content.hex -place=code2@0x4000F000 -place=code@0x40000000 elfoutput1.o elfoutput2.o