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

extern int sysId;
extern char *sortField;

void printDocs(Document *);
Field *addField(Field*,char *, int);
Field *getField(Field *,char *);
void deleteField(Field*, Field *);
Document *parse();
int getDocVersion(Document *,int);
int getRecentVersion(Document *);
void cleanCollection(Document *);
Document *parseDoc(Document *,char *);

#endif
