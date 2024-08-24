assembler_:
	flex lexer.l
	bison -d parser.y
	gcc -g -o assembler parser.tab.c lexer.c definitions.cpp -lfl -lstdc++

clean:
	rm -f assembler parser.tab.c parser.tab.h lexer.c lex.yy.c main.o math.o handler.o isr_timer.o isr_terminal.o isr_software.o

clean_build_run: clean assembler_
#	./assembler pixon_test.s -o elfoutput1.o
#	./assembler pixon_test2.s -o elfoutput2.o
	./assembler -o main.o main.s
#	./assembler -o math.o math.s
	./assembler -o handler.o handler.s
	./assembler -o isr_timer.o isr_timer.s
	./assembler -o isr_terminal.o isr_terminal.s
#	./assembler -o isr_software.o isr_software.s

.PHONY: assembler_ clean clean_build_run



linker_:
	gcc -g -o linker linker.cpp -lstdc++

clean_linker:
	rm -f linker program.o program.hex

clean_build_run_linker: clean_linker linker_
#	./linker -hex -o mem_content.hex -place=code2@0x4000F000 -place=code@0x40000000 elfoutput1.o elfoutput2.o

# TEST A:
#	./linker -relocatable -o program.o handler.o math.o main.o isr_terminal.o isr_timer.o isr_software.o
#	./linker -hex -place=my_code@0x40000000 -place=math@0xF0000000 -o program.hex handler.o math.o main.o isr_terminal.o isr_timer.o isr_software.o
# TEST B:
#	./linker -relocatable -o program.o main.o isr_terminal.o isr_timer.o handler.o
	./linker -hex -place=my_code@0x40000000 -o program.hex main.o isr_terminal.o isr_timer.o handler.o


emulator_:
	gcc -g -o emulator emulator.cpp -lstdc++

clean_emulator:
	rm -f emulator

clean_build_run_emulator: clean_emulator emulator_
#	./emulator mem_content.hex
	./emulator program.hex


full_clean: clean clean_linker clean_emulator

test_prepare: assembler_ linker_ emulator_

# popravljanje start.sh: chmod +x start.sh