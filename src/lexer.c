/* This file is part of Lu programming language
 *
 * Copyright (C) 2025  Hugo Coto Flórez
 *
 * Lu is free software; you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or any later version.
 *
 * Lu is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY of FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Lu. If not, see <https://www.gnu.org/licenses/>
 *
 * For questions or support, contact: hugo.coto@member.fsf.org
 */

#include "lexer.h"
#include "common.h"
#include "report.h"

const char *
lex_repr(Lex l)
{
        return l.repr;
}

Tok *
new_tok()
{
        return calloc(1, sizeof(Tok));
}

Tok *
get_tok(char *c)
{
}

Tok *
lexer(char *src)
{
        char *srcptr = src;
        Tok *ctok = new_tok();
        while (*srcptr && *srcptr != EOF) {
                get_tok(srcptr);
                ctok = ctok->next;
        }
        return ctok;
}

char *
tok_repr(Tok *t)
{
        return t->repr;
}

void
add_lex(Lex l)
{
        report("Adding lexeme: `%s` as `%s`", l.lex, l.repr);
}

#define LEX(l, n) add_lex((Lex) { .lex = (l),   \
                                  .repr = (#n), \
                                  .id = __COUNTER__ });

void
load_lexemes(void)
{
/*   */ #include "lexemes"
}
