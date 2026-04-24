# Psylang IR Ops

Format: `opcode arg1 arg2 res`

| Opcode | arg1 | arg2 | res | Meaning |
|---|---|---|---|---|
| `+` | lhs | rhs | temp | arithmetic add |
| `-` | lhs | rhs | temp | arithmetic sub |
| `*` | lhs | rhs | temp | arithmetic mul |
| `/` | lhs | rhs | temp | arithmetic div |
| `%` | lhs | rhs | temp | arithmetic mod |
| `<` | lhs | rhs | temp | compare less |
| `>` | lhs | rhs | temp | compare greater |
| `<=` | lhs | rhs | temp | compare less/equal |
| `>=` | lhs | rhs | temp | compare greater/equal |
| `==` | lhs | rhs | temp | compare equal |
| `!=` | lhs | rhs | temp | compare not equal |
| `&` | lhs | rhs | temp | bitwise and |
| `&&` | lhs | rhs | temp | logical and |
| `|` | lhs | rhs | temp | bitwise or |
| `||` | lhs | rhs | temp | logical or |
| `^` | lhs | rhs | temp | bitwise xor |
| `~` | value | `_` | temp | bitwise not |
| `<<` | lhs | rhs | temp | shift left |
| `>>` | lhs | rhs | temp | shift right |
| `!` | value | `_` | temp | logical not |
| `=` | value | `_` | target | assign |
| `LABEL` | `_` | `_` | label | label definition |
| `GOTO` | label | `_` | `_` | unconditional jump |
| `IF_FALSE` | cond | label | `_` | jump on false |
| `FUNC` | function name | arity | return type | function header |
| `FUNC_PARAM` | function name | `pos:param` | param type | formal parameter |
| `ENDFUNC` | `_` | `_` | `_` | function end |
| `PARAM` | value | `_` | `_` | call argument |
| `CALL` | callee | argc | temp | function call |
| `RETURN` | value | `_` | `_` | return |
| `ECHO` | value | `_` | `_` | print |
| `MEMBER_ACCESS` | type | member | temp | read member |
| `STORE_MEMBER` | type | member | value | write member |
| `INDEX_ACCESS` | array | index | temp | read index |
| `STORE_INDEX` | array | index | value | write index |
| `LIST_ELEM` | value | `_` | `_` | list element |
| `LIST_LITERAL` | element count | `_` | temp | list literal |
| `TYPE_BEGIN` | type name | `_` | `_` | type begin |
| `TYPE_FIELD` | type name | field name | `_` | type field |
| `TYPE_METHOD` | type name | method name | `_` | type method |
| `TYPE_END` | type name | `_` | `_` | type end |
