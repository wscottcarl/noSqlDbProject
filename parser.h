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

void printDocsToFile(Document *);
void printDocsToFileSysid(Document *);
void printDocs(Document *);
Field *addFieldNoOrder(Field*,char *, int);
Field *addField(Field*,char *, int);
Field *getField(Field *,char *);
void deleteField(Field*, Field *);
Document *parse();
int getDocVersion(Document *,int);
int getRecentVersion(Document *);
void cleanCollection(Document *);
Document *parseDoc(Document *,char *);
Document *addDocById(Document *, int, Field *);
Document *addDocByIdSorted(Document *, int, Field *);
//void sortDb(Document *);
void sortFields(Field *);
int sortDocs(Document *, Document *);
int orderFields(Field *, Field *);
int alphabet(char *, char *);

#endif
