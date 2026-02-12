CC = gcc
TARGET = src/psy.out
LEX_SRC = src/psy.l
YACC_SRC = src/psy.y

GEN_SRCS = src/psy.tab.c src/lex.yy.c

TEST_DIR = tests
OUT_DIR = lexer_results

.PHONY: all run clean run-tests

all: $(TARGET)

$(TARGET): $(GEN_SRCS)
	$(CC) $(GEN_SRCS) -o $(TARGET)

src/psy.tab.c src/psy.tab.h: $(YACC_SRC)
	cd src && bison -d psy.y

src/lex.yy.c: $(LEX_SRC) src/psy.tab.h
	cd src && flex psy.l

run: $(TARGET)
	./$(TARGET)

run-tests: $(TARGET)
	@mkdir -p $(OUT_DIR)
	@for f in $(TEST_DIR)/*; do \
		if [ -f "$$f" ]; then \
			echo "Running $$f..."; \
			./$(TARGET) < "$$f" > "$(OUT_DIR)/$$(basename $$f).out" 2>&1; \
		fi; \
	done

clean:
	rm -f $(TARGET) $(GEN_SRCS) src/psy.tab.h