#ifndef LEXER_H_
#define LEXER_H_

typedef enum {
        LPARENT,
        RPARENT,
        LBRACE,
        RBRACE,
        LBRACKET,
        RBRACKET,
        HASH,
        COMMA,
        DOT,
        MINUS,
        PLUS,
        SEMICOLON,
        COLON,
        COLON_COLON,
        SLASH,
        STAR,
        BANG,
        BANG_EQUAL,
        EQUAL,
        EQUAL_EQUAL,
        GREATER,
        GREATER_EQUAL,
        LESS,
        LESS_EQUAL,
        IDENTIFIER,
        STRING,
        NUMBER,
        AND,
        CLASS,
        ELSE,
        FALSE,
        FUNCTION,
        VAR,
        FOR,
        IF,
        NIL,
        OR,
        AT,
        EXTERN,
        RETURN,
        TRUE,
        WHILE,
        END_OF_FILE,
        BITWISE_AND,
        BITWISE_OR,
        BITWISE_XOR,
        BITWISE_NOT,
        PLUS_PLUS,
        MINUS_MINUS,
        SHIFT_LEFT,
        SHIFT_RIGHT,
        UNKNOWN,
} Lex;

typedef struct Tok {
        Lex lexeme;
        union {
                char *text;
                double num;
        } as;
        int line, offset;
        struct Tok *next;
} Tok;

Tok *lexer(char *str);
void tokprint(Tok *);
void tokfree(Tok *);
const char *get_lex_repr(Lex l);

#endif
