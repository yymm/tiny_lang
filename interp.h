#ifndef __INTERP_H
#define __INTERP_H

#include "mpc/mpc.h"

void init_parser();
double eval(mpc_ast_t *ast);

#endif
