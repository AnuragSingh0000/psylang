CC = gcc
TARGET = src/psy.out
LEX_SRC = src/psy.l
YACC_SRC = src/psy.y

GEN_SRCS = src/psy.tab.c src/lex.yy.c

TEST_DIR = tests
OUT_DIR = lexer_results

.PHONY: all run clean run-lexer-tests

all: $(TARGET)

$(TARGET): $(GEN_SRCS)
	$(CC) $(GEN_SRCS) -o $(TARGET)

src/psy.tab.c src/psy.tab.h: $(YACC_SRC)
	cd src && bison -d psy.y

src/lex.yy.c: $(LEX_SRC) src/psy.tab.h
	cd src && flex psy.l

run: $(TARGET)
	./$(TARGET)

run-lexer-tests: $(TARGET)
	@mkdir -p $(OUT_DIR)/output $(OUT_DIR)/expected
	@echo "Running tests from $(TEST_DIR) and saving results in $(OUT_DIR)/output (comparing with $(OUT_DIR)/expected)"
	@total=0; passed=0; \
	for f in $(TEST_DIR)/*.psy; do \
		if [ -f "$$f" ]; then \
			total=$$((total+1)); \
			name=$$(basename "$$f"); \
			echo -n "Running $$name... "; \
			./$(TARGET) < "$$f" > "$(OUT_DIR)/output/$$name.out" 2>&1; \
			if [ -f "$(OUT_DIR)/expected/$$name.out" ]; then \
				if cmp -s "$(OUT_DIR)/expected/$$name.out" "$(OUT_DIR)/output/$$name.out"; then \
					echo "PASS"; \
					passed=$$((passed+1)); \
				else \
					echo "FAIL"; \
					echo "  Differences:"; \
					diff -u "$(OUT_DIR)/expected/$$name.out" "$(OUT_DIR)/output/$$name.out" || true; \
				fi; \
			else \
				echo "NO EXPECTED (created output only)"; \
			fi; \
		fi; \
	done; \
	echo "$$passed/$$total tests passed."; \
	if [ "$$passed" -ne "$$total" ]; then exit 1; fi

clean:
	rm -f $(TARGET) $(GEN_SRCS) src/psy.tab.h
