# Psylang

psylang is a custom programming language. Currently the lexical analyzer has been implemented.

## Project Structure

- `src` - Lexer (psy.l) and parser (psy.y) specifications along with generated C code.
- `tests` - Sample .psy files for testing language features.
- `lexer_results` - Output and Expected output from running the lexer on test files.

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

To run all tests:
```bash
make run-lexer-tests
```

This updates `lexer_results/output` and compares it with `lexer_results/expected` and reports the number of passing tests.

## Error Handling

The lexer reports common issues like invalid identifiers (e.g. starting with a digit), unknown tokens, and unterminated strings, providing the line number and a description on stderr. All these scenarios can be found in the tests. (Note the error might appear at the top of the files in the output, this is because stderr is redirected first to the file and then stdout is redirected to the file). If you check by running `make run < INPUT_FILE` in the terminal without redirecting the output to a file, the error message is printed in the end itself.

<img width="714" height="245" alt="image" src="https://github.com/user-attachments/assets/78abc718-1b58-4ee3-84a4-41c308a6765a" />
