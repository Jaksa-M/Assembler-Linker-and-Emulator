# # Define variables
# LEXER = lexer.l
# PARSER = parser.y
# EXEC = parser
# SRCS = parser.tab.c lexer.c definitions.cpp
# HDRS = parser.tab.h lexer.h definitons.h
# OBJS = parser.tab.o lexer.o definitions.o

# # Default target
# all: $(EXEC)

# # Build the lexer
# lexer.c: $(LEXER)
# 	flex $(LEXER)

# # Build the parser
# parser.tab.c parser.tab.h: $(PARSER)
# 	bison -d $(PARSER)

# # Compile the object files
# $(OBJS): $(SRCS) $(HDRS)
# 	gcc -c parser.tab.c -o parser.tab.o
# 	gcc -c lexer.c -o lexer.o
# 	g++ -c defintions.cpp -o definitions.o

# # Link the object files to create the executable
# $(EXEC): $(OBJS)
# 	gcc $(OBJS) -o $(EXEC) -lfl

# # Clean up the generated files
# clean:
# 	rm -f $(EXEC) $(SRCS) $(HDRS) $(OBJS)

# # Run the parser with the input file
# run: $(EXEC)
# 	./$(EXEC) sample.asm
	
# # Clean, make, and run
# clean_run:
# 	make clean
# 	make all
# 	make run

# .PHONY: all clean run clean_run

assembler_:
	flex lexer.l
	bison -d parser.y
	gcc -g -o assembler parser.tab.c lexer.c definitions.cpp -lfl -lstdc++

clean:
	rm -f assembler parser.tab.c parser.tab.h lexer.c lex.yy.c

clean_build_run: clean assembler_
	./assembler pixon_test.s

.PHONY: assembler_ clean clean_build_run