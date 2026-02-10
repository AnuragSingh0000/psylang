CC = gcc
TARGET = src/psy.out
LEX_SRC = src/psy.l
YACC_SRC = src/psy.y

GEN_SRCS = src/psy.tab.c src/lex.yy.c

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(GEN_SRCS)
	$(CC) $(GEN_SRCS) -o $(TARGET)

src/psy.tab.c src/psy.tab.h: $(YACC_SRC)
	cd src && bison -d psy.y

src/lex.yy.c: $(LEX_SRC) src/psy.tab.h
	cd src && flex psy.l

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(GEN_SRCS) src/psy.tab.h