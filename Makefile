CC = gcc
PARSER_TARGET = src/psy.out
PRINT_TOKENS_TARGET = src/psy_printtokens.out
PRINT_RDT_TARGET = src/psy_printRDT.out

LEX_SRC = src/psy.l
PARSER_YACC = src/psy.y
PRINT_TOKENS_YACC = src/psy_printtokens.y
PRINT_RDT_YACC = src/psy_printRDT.y

PARSER_GEN_C = src/psy.tab.c
PARSER_GEN_H = src/psy.tab.h
PRINT_TOKENS_GEN_C = src/psy_printtokens.tab.c
PRINT_TOKENS_GEN_H = src/psy_printtokens.tab.h
PRINT_RDT_GEN_C = src/psy_printRDT.tab.c
PRINT_RDT_GEN_H = src/psy_printRDT.tab.h

SEMANTIC_C = src/semantic.c
IR_C = src/ir_gen.c src/ir_dump.c

LEX_C = src/lex.yy.c

PARSER_SRCS = $(PARSER_GEN_C) $(LEX_C) $(SEMANTIC_C) $(IR_C)
PRINT_TOKENS_SRCS = $(PRINT_TOKENS_GEN_C) $(LEX_C)
PRINT_RDT_SRCS = $(PRINT_RDT_GEN_C) $(LEX_C)

TEST_DIR = tests
RESULTS_DIR = results

.PHONY: all clean run-parser run-print-rdt run-print-tokens test-ast-printer test-ir-dump test-print-tokens test-print-rdt test-all

all: $(PARSER_TARGET) $(PRINT_TOKENS_TARGET) $(PRINT_RDT_TARGET)

$(PARSER_TARGET): $(PARSER_SRCS)
	$(CC) $(PARSER_SRCS) -o $(PARSER_TARGET)

$(PRINT_TOKENS_TARGET): $(PRINT_TOKENS_SRCS)
	$(CC) $(PRINT_TOKENS_SRCS) -o $(PRINT_TOKENS_TARGET)

$(PRINT_RDT_TARGET): $(PRINT_RDT_SRCS)
	$(CC) $(PRINT_RDT_SRCS) -o $(PRINT_RDT_TARGET)

$(PARSER_GEN_C) $(PARSER_GEN_H): $(PARSER_YACC)
	cd src && bison -d psy.y

$(PRINT_TOKENS_GEN_C) $(PRINT_TOKENS_GEN_H): $(PRINT_TOKENS_YACC)
	cd src && bison -d psy_printtokens.y -o psy_printtokens.tab.c

$(PRINT_RDT_GEN_C) $(PRINT_RDT_GEN_H): $(PRINT_RDT_YACC)
	cd src && bison -d psy_printRDT.y -o psy_printRDT.tab.c

$(LEX_C): $(LEX_SRC) $(PARSER_GEN_H)
	cd src && flex psy.l

run-parser: $(PARSER_TARGET)
	@if [ -z "$(FILE)" ]; then echo "Usage: make run-parser FILE=path/to/file.psy"; exit 1; fi
	./$(PARSER_TARGET) "$(FILE)"

run-print-rdt: $(PRINT_RDT_TARGET)
	@if [ -z "$(FILE)" ]; then echo "Usage: make run-print-rdt FILE=path/to/file.psy"; exit 1; fi
	./$(PRINT_RDT_TARGET) "$(FILE)"

run-print-tokens: $(PRINT_TOKENS_TARGET)
	./$(PRINT_TOKENS_TARGET)

define RUN_TESTS
    @total=0; passed=0; \
    echo "Running tests using $(1) into $(2)..."; \
    for f in $$(find $(TEST_DIR) -type f -name '*.psy' | sort); do \
       rel_path=$${f#$(TEST_DIR)/}; \
       rel_dir=$$(dirname "$$rel_path"); \
       name=$$(basename "$$f"); \
       out_dir="$(2)/output/$$rel_dir"; \
       exp_dir="$(2)/expected/$$rel_dir"; \
       mkdir -p "$$out_dir" "$$exp_dir"; \
       echo -n "  Running $$rel_path... "; \
	$(4) ./$(1) $(3) > "$$out_dir/$$name.out" 2>&1; \
       if [ -f "$$exp_dir/$$name.out" ]; then \
           total=$$((total+1)); \
          if diff -u --strip-trailing-cr "$$exp_dir/$$name.out" "$$out_dir/$$name.out" > /dev/null; then \
             echo "PASS"; \
             passed=$$((passed+1)); \
          else \
             echo "FAIL"; \
             echo "    Differences:"; \
             diff -u --strip-trailing-cr "$$exp_dir/$$name.out" "$$out_dir/$$name.out" || true; \
          fi; \
       else \
          echo "NO EXPECTED (created output only)"; \
       fi; \
    done; \
    echo "$$passed/$$total tests passed for $(1)."; \
    if [ "$$passed" -ne "$$total" ] && [ "$$total" -gt 0 ]; then exit 1; fi
endef

test-ast-printer: $(PARSER_TARGET)
	$(call RUN_TESTS,$(PARSER_TARGET),$(RESULTS_DIR)/print-ast,"$$f",PSY_DEBUG=1)

test-ir-dump: $(PARSER_TARGET)
	$(call RUN_TESTS,$(PARSER_TARGET),$(RESULTS_DIR)/print-ir,"$$f",PSY_DEBUG=1 DUMP_IR=1 DUMP_IR_ONLY=1)

test-print-tokens: $(PRINT_TOKENS_TARGET)
	$(call RUN_TESTS,$(PRINT_TOKENS_TARGET),$(RESULTS_DIR)/print-tokens,< "$$f",PSY_DEBUG=1)

test-print-rdt: $(PRINT_RDT_TARGET)
	$(call RUN_TESTS,$(PRINT_RDT_TARGET),$(RESULTS_DIR)/print-rdt,"$$f",PSY_DEBUG=1)

test-all:
	@failed=0; \
	$(MAKE) test-ast-printer || failed=1; \
	$(MAKE) test-ir-dump || failed=1; \
	$(MAKE) test-print-rdt || failed=1; \
	$(MAKE) test-print-tokens || failed=1; \
	if [ "$$failed" -ne 0 ]; then \
		printf "\033[1;31mSome test suites failed.\033[0m\n"; \
		exit 1; \
	else \
		printf "\033[1;32mAll test suites passed.\033[0m\n"; \
	fi

clean:
	rm -f $(PARSER_TARGET) $(PRINT_TOKENS_TARGET) $(PRINT_RDT_TARGET) \
	      $(PARSER_GEN_C) $(PARSER_GEN_H) $(PRINT_TOKENS_GEN_C) \
	      $(PRINT_TOKENS_GEN_H) $(LEX_C) $(PRINT_RDT_GEN_C) $(PRINT_RDT_GEN_H)
