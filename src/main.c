#include "lexer.h"

#define $(a) #a "\n"

char *program = 
$(1.23 + 23434.2323 == 3)
$(algo = 23434.2323 == b_eta);


int
main()
{
        Tok *tokens = lexer(program);
        tokprint(tokens);
        tokfree(tokens);
        return 0;
}
