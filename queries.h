#ifndef QUERIES_H
#define QUERIES_H

#include "string.h"
#include "parser.h"
#include <ctype.h>

Document *insert(Document *, char*);
void sort(Document *, char*);
void query(Document *, char*);
void count(Document *, char*);
Document *parseQueries(Document *);
int checkCondition(char*, Field*);
int isFieldSingle(char*);
Document *filterCondition(Document *, char*);
Document *filterVersions( Document *, char*);
Document *filterProject(  Document *, char*);
Field    *getFilteredDoc( Field *, char*);
Document *filterBadFields(Document *, char*);

#endif
