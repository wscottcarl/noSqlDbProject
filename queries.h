#ifndef QUERIES_H
#define QUERIES_H

#include "string.h"
#include "parser.h"

Document *insert(Document *, char*);
void sort(char*);
void query(char*);
void count(char*);
Document *doQuery(Document *);
Document *parseQueries(Document *);

#endif
