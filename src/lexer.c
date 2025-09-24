#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

Tok *
tnew(Lex l)
{
        Tok *t = calloc(1, sizeof(Tok));
        t->lexeme = l;
        return t;
}

#define tokadd(l, n)                    \
        {                               \
                cur->next = tnew((l));  \
                cur = cur->next;        \
                cur->line = (line);     \
                cur->offset = (offset); \
                sptr += (n);            \
                break;                  \
        }

char *
get_id(char *sptr)
{
        int len;
        char *id;
        char *c = sptr;
        if (*c != '_' && !isalpha(*c)) return NULL;
        ++c;
        while (*c == '_' || isalnum(*c))
                ++c;

        len = c - sptr;
        id = malloc(len + 1);
        memcpy(id, sptr, len);
        id[len] = 0;
        return id;
}

double
get_number(char **psptr)
{
        /* [0-9]+("." [0-9]+)? */
        double d = 0;
        double dec = 1;

        while (isdigit(**psptr)) {
                d *= 10;
                d += **psptr - '0';
                ++*psptr;
        }

        if (**psptr != '.' || !isdigit((*psptr)[1])) return d;

        ++*psptr; // consume dot
        while (isdigit(**psptr)) {
                dec *= 10;
                d += (**psptr - '0') / dec;
                ++*psptr;
        }

        return d;
}

Tok *
lexer(char *str)
{
        char *sptr = str;
        Tok *cur;
        Tok *start;
        int line = 1;
        char *lineptr = sptr;
        int offset;
        start = cur = tnew(UNKNOWN);

        // clang-format off
repeat:
        offset = sptr - lineptr + 1;
        switch (*sptr) {
        case '(':                       tokadd(LPARENT,         1);
        case ')':                       tokadd(RPARENT,         1);
        case '[':                       tokadd(LBRACE,          1);
        case ']':                       tokadd(RBRACE,          1);
        case '{':                       tokadd(LBRACKET,        1);
        case '}':                       tokadd(RBRACKET,        1);
        case '#':                       tokadd(HASH,            1);
        case ',':                       tokadd(COMMA,           1);
        case '.':                       tokadd(DOT,             1);
        case '-': if (sptr[1] == '-')   tokadd(MINUS_MINUS,     2);
                                        tokadd(MINUS,           1);
        case '+': if (sptr[1] == '+')   tokadd(PLUS_PLUS,       2);
                                        tokadd(PLUS,            1);
        case ';':                       tokadd(SEMICOLON,       1);
        case ':': if (sptr[1] == ':')   tokadd(COLON_COLON,     2);
                                        tokadd(COLON,           1);
        case '/':                       tokadd(SLASH,           1);
        case '*':                       tokadd(STAR,            1);
        case '!': if (sptr[1] == '=')   tokadd(BANG_EQUAL,      2);
                                        tokadd(BANG,            1);
        case '=': if (sptr[1] == '=')   tokadd(EQUAL_EQUAL,     2);
                                        tokadd(EQUAL,           1);
        case '>': if (sptr[1] == '=')   tokadd(GREATER_EQUAL,   2);
                  if (sptr[1] == '>')   tokadd(SHIFT_RIGHT,     2);
                                        tokadd(GREATER,         1);
        case '<': if (sptr[1] == '=')   tokadd(LESS_EQUAL,      2);
                  if (sptr[1] == '<')   tokadd(SHIFT_LEFT,      2);
                                        tokadd(LESS,            1);
        case '&': if (sptr[1] == '&')   tokadd(AND,             2);
                                        tokadd(BITWISE_AND,     1);
        case '|': if (sptr[1] == '=')   tokadd(OR,              2);
                                        tokadd(BITWISE_OR,      1);
        case '@':                       tokadd(AT,              1);
        case '^':                       tokadd(BITWISE_XOR,     1);
        case '~':                       tokadd(BITWISE_NOT,     1);
        case EOF:                       tokadd(END_OF_FILE,     1);
        // clang-format on
        case '\n':
                ++line;
                ++sptr;
                lineptr = sptr;
                break;
        default:
                if (isspace(*sptr)) {
                        while (isspace(*sptr))
                                ++sptr;
                        break;
                }

                if (isdigit(*sptr)) {
                        cur->next = tnew(NUMBER);
                        cur = cur->next;
                        cur->offset = sptr - lineptr + 1;
                        cur->line = line;
                        cur->as.num = get_number(&sptr);
                        break;
                }

                char *id;
                if ((id = get_id(sptr))) {
                        cur->next = tnew(IDENTIFIER);
                        cur = cur->next;
                        cur->offset = sptr - lineptr + 1;
                        cur->line = line;
                        cur->as.text = id;
                        sptr += strlen(id);
                        break;
                }

                printf("Unknown char: %c\n", *sptr);
                ++sptr;
                break;
        }

        /* Just to avoid an identation level */
        if (*sptr) goto repeat;

        cur = start->next;
        free(start);
        return cur;
}

const char *lex_repr_lookup[] = {
        [LPARENT] = "LPARENT",
        [RPARENT] = "RPARENT",
        [LBRACE] = "LBRACE",
        [RBRACE] = "RBRACE",
        [LBRACKET] = "LBRACKET",
        [RBRACKET] = "RBRACKET",
        [HASH] = "HASH",
        [COMMA] = "COMMA",
        [DOT] = "DOT",
        [MINUS] = "MINUS",
        [PLUS] = "PLUS",
        [SEMICOLON] = "SEMICOLON",
        [COLON] = "COLON",
        [COLON_COLON] = "COLON_COLON",
        [SLASH] = "SLASH",
        [STAR] = "STAR",
        [BANG] = "BANG",
        [BANG_EQUAL] = "BANG_EQUAL",
        [EQUAL] = "EQUAL",
        [EQUAL_EQUAL] = "EQUAL_EQUAL",
        [GREATER] = "GREATER",
        [GREATER_EQUAL] = "GREATER_EQUAL",
        [LESS] = "LESS",
        [LESS_EQUAL] = "LESS_EQUAL",
        [IDENTIFIER] = "IDENTIFIER",
        [STRING] = "STRING",
        [NUMBER] = "NUMBER",
        [AND] = "AND",
        [CLASS] = "CLASS",
        [ELSE] = "ELSE",
        [FALSE] = "FALSE",
        [FUNCTION] = "FUNCTION",
        [VAR] = "VAR",
        [FOR] = "FOR",
        [IF] = "IF",
        [NIL] = "NIL",
        [OR] = "OR",
        [AT] = "AT",
        [EXTERN] = "EXTERN",
        [RETURN] = "RETURN",
        [TRUE] = "TRUE",
        [WHILE] = "WHILE",
        [END_OF_FILE] = "END_OF_FILE",
        [BITWISE_AND] = "BITWISE_AND",
        [BITWISE_OR] = "BITWISE_OR",
        [BITWISE_XOR] = "BITWISE_XOR",
        [BITWISE_NOT] = "BITWISE_NOT",
        [PLUS_PLUS] = "PLUS_PLUS",
        [MINUS_MINUS] = "MINUS_MINUS",
        [SHIFT_LEFT] = "SHIFT_LEFT",
        [SHIFT_RIGHT] = "SHIFT_RIGHT",
        [UNKNOWN] = "UNKNOWN",
};

const char *
get_lex_repr(Lex l)
{
        return lex_repr_lookup[l];
}

void
tokprint(Tok *t)
{
        if (t == NULL) return;
        switch (t->lexeme) {
        case NUMBER:
                printf("TOK at %d:%d: %s %g\n", t->line, t->offset, get_lex_repr(t->lexeme), t->as.num);
                break;

        case IDENTIFIER:
                printf("TOK at %d:%d: %s %s\n", t->line, t->offset, get_lex_repr(t->lexeme), t->as.text);
                break;

        default:
                printf("TOK at %d:%d: %s\n", t->line, t->offset, get_lex_repr(t->lexeme));
                break;
        }
        tokprint(t->next);
}

void
tokfree(Tok *t)
{
        if (t == NULL) return;
        switch (t->lexeme) {
        case IDENTIFIER:
                free(t->as.text);
                break;
        default:
                break;
        }
        tokfree(t->next);
        free(t);
}
