# Psylang

psylang is a custom programming language. Currently the lexical analyzer has been implemented.

## Project Structure

- src - Lexer (psy.l) and parser (psy.y) specifications along with generated C code.
- tests - Sample .psy files for testing language features.
- lexer_results - Output from running the lexer on test files.

## Build and Usage

To compile the project:
```bash
make
```
This produces the src/psy.out executable.

To run a specific file:
```bash
make run < INPUT_FILE
```

To run all tests and update lexer_results/:
```bash
make run-tests
```

## Error Handling

The lexer reports common issues like invalid identifiers (e.g. starting with a digit), unknown tokens, and unterminated strings, providing the line number and a description on stderr.

