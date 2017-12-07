#ifndef PARSER_H
#define PARSER_H

#include "uthash.h"
#include <stdio.h>
#include "scanner.h"

typedef struct Field {
	char *key;
	int   val;
	UT_hash_handle hh;
} Field;

typedef struct Document {
	int id;
	Field *doc;
	UT_hash_handle hh;
} Document;

extern Document *docs;
extern Field *document;

void print_docs();
void add_field(char *, int);
Field *get_field(char *);
void delete_field(Field *);
Document *parse();

#endif
