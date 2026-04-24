#ifndef DEFS_H
#define DEFS_H

#include <stdbool.h>
#include <stdio.h>

typedef struct Node Node;
typedef struct Inst Inst;

#define SYMBOL_INDEX_UNRESOLVED (-1)

typedef enum NodeType
{
    NODE_PROGRAM,
    NODE_TYPE_DECL,
    NODE_FUN_DECL,
    NODE_FUNCTION,
    NODE_TYPED_VAR,
    NODE_VAR_TYPE,
    NODE_VAR_DECL,
    NODE_EXPR_STMT,
    NODE_FOR_STMT,
    NODE_IF_STMT,
    NODE_ECHO_STMT,
    NODE_PRIMARY,
    NODE_UNARY_OP,
    NODE_BINARY_OP,
    NODE_TERNARY_OP,
    NODE_ASSIGN_OP,
    NODE_WHILE_STMT,
    NODE_DO_WHILE_STMT, 
    NODE_BLOCK,
    NODE_CONTINUE,
    NODE_BREAK,
    NODE_RETURN,
    NODE_CALL,
    NODE_MEMBER_ACCESS,
    NODE_INDEX_ACCESS,
    NODE_POSTFIX_OP,
    NODE_LIST_LITERAL,
    NODE_LAMBDA
} NodeType;

typedef enum VarTypeKind
{
    VAR_TYPE_INT,
    VAR_TYPE_FLOAT,
    VAR_TYPE_STRING,
    VAR_TYPE_BOOL,
    VAR_TYPE_DYN,
    VAR_TYPE_LIST,
    VAR_TYPE_CUSTOM,
    VAR_TYPE_NULL
} VarTypeKind;

typedef enum PrimaryNodeType
{
    PRIMARY_INT,
    PRIMARY_FLOAT,
    PRIMARY_STRING,
    PRIMARY_BOOL,
    PRIMARY_NULL,
    PRIMARY_IDENTIFIER
} PrimaryNodeType;

typedef enum OperatorType
{
    OP_PLUS,
    OP_MINUS,
    OP_STAR,
    OP_SLASH,
    OP_PERC,
    OP_LESS,
    OP_GREATER,
    OP_LESS_EQ,
    OP_GREATER_EQ,
    OP_BANG_EQ,
    OP_EQ_EQ,
    OP_AND,
    OP_DOUBLE_AND,
    OP_PIPE,
    OP_DOUBLE_PIPE,
    OP_TILDE,
    OP_XOR,
    OP_SHIFT_LEFT,
    OP_SHIFT_RIGHT,
    OP_EQ,
    OP_PLUS_EQ,
    OP_MINUS_EQ,
    OP_STAR_EQ,
    OP_SLASH_EQ,
    OP_PERC_EQ,
    OP_PLUS_PLUS,
    OP_MINUS_MINUS,
    OP_BANG
} OperatorType;

typedef enum IrOp
{
    IR_LABEL,
    IR_GOTO,
    IR_IF_FALSE,
    IR_FUNC,
    IR_ENDFUNC,
    IR_FUNC_PARAM,
    IR_ECHO,
    IR_RETURN,
    IR_PARAM,
    IR_CALL,
    IR_MEMBER_ACCESS,
    IR_INDEX_ACCESS,
    IR_STORE_MEMBER,
    IR_STORE_INDEX,
    IR_LIST_ELEM,
    IR_LIST_LITERAL,
    IR_TYPE_BEGIN,
    IR_TYPE_FIELD,
    IR_TYPE_END,
    IR_ALLOC
} IrOp;

struct Inst
{
    int is_special;
    union
    {
        OperatorType op;
        IrOp special;
    } as;
    char *arg1;
    char *arg2;
    char *res;
    struct Inst *next;
};

typedef struct PrimaryNode
{
    PrimaryNodeType type;
    union
    {
        int intval;
        float floatval;
        char *stringval;
        bool boolval;
    } val;
} PrimaryNode;

typedef struct ProgramNode
{
    Node *declarations;
} ProgramNode;

typedef struct TypeDeclNode
{
    char *name;
    Node *fields;
} TypeDeclNode;

typedef struct FunctionNode
{
    Node *parameters;
    Node *returnType;
    Node *body;
} FunctionNode;

typedef struct FunDeclNode
{
    char *name;
    Node *function;
} FunDeclNode;

typedef struct TypedVarNode // used for function parameter type declarations. like x : int 
{
    char *name;
    Node *varType;
} TypedVarNode;

typedef struct VarTypeNode { // used to represent actual types like int, float etc. The listSize field is there because a type can be list[5] too. customTypeName is present for user defined types.
    VarTypeKind kind;
    int listSize;
    char *customTypeName;
} VarTypeNode;

typedef struct VarDeclNode
{
    Node *varType;
    char *name;
    Node *initializer;
} VarDeclNode;

typedef struct ExprStmtNode
{
    Node *expr;
} ExprStmtNode;

typedef struct ForStmtNode
{
    Node *init;
    Node *condition;
    Node *increment;
    Node *body;
} ForStmtNode;

typedef struct IfStmtNode
{
    Node *condition;
    Node *thenBranch;
    Node *elseBranch;
} IfStmtNode;

typedef struct EchoStmtNode
{
    Node *expr;
} EchoStmtNode;

typedef struct UnaryOp
{
    OperatorType op;
    Node *expr;
} UnaryOp;

typedef struct BinaryOp
{
    OperatorType op;
    Node *left;
    Node *right;
} BinaryOp;

typedef struct TernaryOp
{
    Node *condition;
    Node *ifBranch;
    Node *elseBranch;
} TernaryOp;

typedef struct AssignOp
{
    OperatorType op;
    Node *left;
    Node *right;
} AssignOp;

typedef struct WhileStmtNode
{
    Node *condition;
    Node *body;
} WhileStmtNode;

typedef struct DoWhileStmtNode
{
    Node *body;
    Node *condition;
} DoWhileStmtNode;

typedef struct BlockNode
{
    Node *declarations;
} BlockNode;

typedef struct ContinueNode
{
} ContinueNode;

typedef struct BreakNode
{
} BreakNode;

typedef struct ReturnNode
{
    Node *expr;
} ReturnNode;

typedef struct CallNode
{
    Node *callee;
    Node *arguments;
} CallNode;

typedef struct MemberAccessNode
{
    Node *object;
    char *member;
} MemberAccessNode;

typedef struct IndexAccessNode
{
    Node *object;
    Node *index;
} IndexAccessNode;

typedef struct PostfixOpNode
{
    OperatorType op;
    Node *expr;
} PostfixOpNode;

typedef struct ListNode
{
    Node *elements;
} ListNode;

typedef struct LambdaNode
{
    Node *function;
} LambdaNode;

struct Node
{
    NodeType type;
    int line;
    int pos;
    int symbol_table_index;
    Node *next; // for linked list of declarations and statements
    union
    {
        ProgramNode program;
        TypeDeclNode typeDecl;
        FunDeclNode funDecl;
        FunctionNode function;
        TypedVarNode typedVar;
        VarTypeNode varType;
        VarDeclNode varDecl;    
        ExprStmtNode exprStmt;
        ForStmtNode forStmt;
        IfStmtNode ifStmt;
        EchoStmtNode echoStmt;
        PrimaryNode primary;
        UnaryOp unary;
        BinaryOp binary;
        TernaryOp ternary;
        AssignOp assign;
        WhileStmtNode whileStmt;
        DoWhileStmtNode doWhileStmt;
        BlockNode block;
        ContinueNode continueStmt;
        BreakNode breakStmt;
        ReturnNode returnStmt;
        CallNode call;
        MemberAccessNode memberAccess;
        IndexAccessNode indexAccess;
        PostfixOpNode postfix;
        ListNode list;
        LambdaNode lambda;
    } as;
};

void generateIr(const Node *ast_root);
bool irHasError(void);
Inst *irGetHead(void);
void irDump(FILE *out);

#endif