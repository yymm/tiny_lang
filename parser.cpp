#include "mpc/mpc.h"
#include "interp.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
    	fprintf(stderr, "usage of %s: file\n", argv[0]);
		exit(0);
	}
	mpc_parser_t *Ident = mpc_new("ident");
	mpc_parser_t *Value = mpc_new("value");
	mpc_parser_t *String = mpc_new("string");
	mpc_parser_t *Comment = mpc_new("comment");
	mpc_parser_t *Factor = mpc_new("factor");
	mpc_parser_t *Term = mpc_new("term");
	mpc_parser_t *Lexp = mpc_new("lexp");
	mpc_parser_t *Let = mpc_new("let");
	mpc_parser_t *Call = mpc_new("call");
	mpc_parser_t *Stmts = mpc_new("stmts");
	
	mpc_err_t* err = mpca_lang(MPCA_LANG_DEFAULT,
			" ident   : /[a-zA-Z][a-zA-Z0-9_]*/;                                          \n"
			" value   : /(-|+)?[0-9]+(\\.[0-9]*)?/;                                       \n"
			" string  : /\"(\\\\.|[^\"])*\"/;                                             \n"
			" comment : /#[^\n]*/;                                                        \n"
			" factor  : '(' <lexp> ')'                                                    \n"
			"         | <value>                                                           \n"
			"         | <ident>;                                                          \n"
			" term    : <factor> (('*' | '/' | '%') <factor>)*;                           \n"
			" lexp    : <term> (('+' | '-') <term>)*;                                     \n"
			" let     : <ident> '=' <lexp>;                                               \n"
			" call    : <ident> '(' (<lexp> | <string>)? (',' (<lexp> | <string>))* ')';  \n"
			" stmts   : (<let> | <call> | <comment>)*;                                    \n"
			, Ident, Value, String, Comment, Factor, Term, Lexp, Let, Call, Stmts);
	if (err != NULL)
	{
		mpc_err_print(err);
		mpc_err_delete(err);
		goto exit;
	}
	mpc_result_t result;
	if (!mpc_parse_contents(argv[1], Stmts, &result))
	{
		mpc_err_print(result.error);
		mpc_err_delete(result.error);
		goto exit;
	}
	mpc_ast_print((mpc_ast_t*)result.output);
	init_parser(); eval((mpc_ast_t*)result.output);
	mpc_ast_delete((mpc_ast_t*)result.output);
exit:
	mpc_cleanup(8, Ident, Value, String, Comment, Factor, Term, Lexp, Let, Call, Stmts);
	return 0;
}
