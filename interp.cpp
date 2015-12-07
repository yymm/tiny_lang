#include "interp.h"
#include <stdio.h>
#include <stdlib.h>
#include "klib/khash.h"

#define is_a(t, a) (strstr(t->tag, a) != NULL)

// typedef struct T
// {
// 	int t;	// type
// 	union _v
// 	{
// 		double d;
// 		double *a;
// 	} v;
// } T;
// 
// #define TYPE_NULL 0
// #define TYPE_DOUBLE 1
// #define TYPE_ARRAY 2

KHASH_MAP_INIT_STR(dhash, double)
khash_t(dhash) *h;

void init_parser()
{
	h = kh_init(dhash);
}

double eval(mpc_ast_t *t)
{
	int i = 0;
	if (is_a(t, "comment"))
	{
		return 0.0;
	}
	if (is_a(t, "value"))
	{
		return atof(t->contents);
	}
	if (is_a(t, "ident"))
	{
		khint_t k = kh_get(dhash, h, t->contents);
		if (k == kh_end(h))
		{
			return 0.0;
		}
		return kh_value(h, k);
	}
	if (is_a(t, "factor"))
	{
		return eval(t->children[1]);
	}
	if (is_a(t, "lexp") || is_a(t, "term"))
	{
		double lhs = eval(t->children[0]);
		for (i = 1; i < t->children_num; i += 2)
		{
			char* op = t->children[i]->contents;
			double rhs = eval(t->children[i+1]);
			if (strcmp(op, "+") == 0) { lhs += rhs;           }
			if (strcmp(op, "-") == 0) { lhs -= rhs;           }
			if (strcmp(op, "*") == 0) { lhs *= rhs;           }
			if (strcmp(op, "/") == 0) { lhs /= rhs;           }
			if (strcmp(op, "%") == 0) { lhs = fmod(lhs, rhs); }
		}
		return lhs;
	}
	if (is_a(t, "let"))
	{
		int ret = 0;
		khint_t k = kh_put(dhash, h, t->children[0]->contents, &ret);
		double v = eval(t->children[2]);
		kh_value(h, k) = v;
		return v;
	}
	if (is_a(t, "call"))
	{
		if (!strcmp(t->children[0]->contents, "print"))
		{
			for(i = 2; i < t->children_num; i += 2)
			{
				if (is_a(t->children[i], "string"))
				{
					char* s = t->children[i]->contents;
					printf("%s", s);
				}
				else
				{
					double v = eval(t->children[i]);
					printf("%f", v);
				}
			}
			printf("\n");
		}
		else
		{
			printf("Unknown function...\n");
		}
		return 0.0;
	}
	for(i = 0; i < t->children_num; i++)
	{
		eval(t->children[i]);
	}
	return 0.0;
}
