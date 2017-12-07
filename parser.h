#ifndef PARSER_H
#define PARSER_H

#include "uthash.h"

typedef struct Field {
	char *key;
	int   val;
	UT_hash_handle hh;
} Field;

Field *collection = NULL;

void add_field(Field *f);
Field *get_field(char *key);
void delete_field(Field *f);

#endif
