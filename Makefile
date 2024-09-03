assembler_:
	flex misc/lexer.l
	bison -d misc/parser.y
	gcc -g -o assembler parser.tab.c lexer.c src/assembler.cpp -lfl -lstdc++

clean:
	rm -f assembler parser.tab.c parser.tab.h lexer.c lex.yy.c lexer.h \
	tests/nivo-a/main.o tests/nivo-a/math.o tests/nivo-a/handler.o tests/nivo-a/isr_timer.o tests/nivo-a/isr_terminal.o tests/nivo-a/isr_software.o \
	tests/nivo-b/main.o tests/nivo-b/handler.o tests/nivo-b/isr_timer.o tests/nivo-b/isr_terminal.o \
	main.o math.o handler.o isr_timer.o isr_terminal.o isr_software.o

clean_build_run: clean assembler_
# TEST A:
#	./assembler -o math.o tests/nivo-a/math.s
#	./assembler -o isr_software.o tests/nivo-a/isr_software.s
#	./assembler -o main.o tests/nivo-a/main.s
#	./assembler -o handler.o tests/nivo-a/handler.s
#	./assembler -o isr_timer.o tests/nivo-a/isr_timer.s
#	./assembler -o isr_terminal.o tests/nivo-a/isr_terminal.s
# TEST B:
	./assembler -o main.o tests/nivo-b/main.s
	./assembler -o handler.o tests/nivo-b/handler.s
	./assembler -o isr_timer.o tests/nivo-b/isr_timer.s
	./assembler -o isr_terminal.o tests/nivo-b/isr_terminal.s

.PHONY: assembler_ clean clean_build_run

#--------------------------------------------------------------------------------------------------------------------------------
linker_:
	gcc -g -o linker src/linker.cpp -lstdc++

clean_linker:
	rm -f linker program.o program.hex tests/nivo-a/program.hex tests/nivo-b/program.hex

clean_build_run_linker: clean_linker linker_
# TEST A:
#	./linker -relocatable -o program.o handler.o math.o main.o isr_terminal.o isr_timer.o isr_software.o
#	./linker -hex -place=my_code@0x40000000 -place=math@0xF0000000 -o program.hex handler.o math.o main.o isr_terminal.o isr_timer.o isr_software.o
# TEST B:
	./linker -relocatable -o program.o main.o isr_terminal.o isr_timer.o handler.o
#	./linker -hex -place=my_code@0x40000000 -o program.hex main.o isr_terminal.o isr_timer.o handler.o

#---------------------------------------------------------------------------------------------------------------------------------
emulator_:
	gcc -g -o emulator src/emulator.cpp -lstdc++

clean_emulator:
	rm -f emulator

clean_build_run_emulator: clean_emulator emulator_
	./emulator program.hex


#---------------------------------------------------------------------------------------------------------------------------------
full_clean: clean clean_linker clean_emulator

test_prepare: assembler_ linker_ emulator_

# popravljanje start.sh: chmod +x start.sh