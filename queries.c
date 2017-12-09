
#include "queries.h"

#define COUNT  0
#define QUERY  1
#define INSERT 2
#define SORT   3

Document *insert(Document *docs, char *d){
	docs = parseDoc(docs, d);
	return docs;
}

void query(char *d){
//	parseDoc(d);
}

void count(char *d){
//	parseDoc(d);
}

void sort(char *d) {
	char *token = strsep(&d, ",");
	printf("Sorting with these options: %s\n", token);
	printf("Remainder options: %s\n", d);
	if(d==NULL) {
//		No versions specified, print only the most recent version
		Document *tmpDb = NULL;
//		add
	}
}

Document *doQuery(Document *collection) {
	
	return collection;
}

Document *parseQueries(Document *collection) {
	char w[] = "final.sort([DocID],[vn])";
	char *queryDup = strdup(w);
	int selected=0;
	char delim[] = ".()";
	char *queryTmp = strsep(&queryDup, delim);
	if(!strcmp(queryTmp, "final")) {
		printf("Called on collection final\n"); }
	queryTmp = strsep(&queryDup, delim);
	if(!strcmp(queryTmp, "count")) { selected = COUNT; }
	if(!strcmp(queryTmp, "insert")) { selected = INSERT; }
	if(!strcmp(queryTmp, "sort")) { selected = SORT; }
	if(!strcmp(queryTmp, "query")) { selected = QUERY; }
	printf("Option selected: %d\n",selected);
	queryTmp = strsep(&queryDup, delim);
	if(!strcmp(queryTmp, "")) {
		printf("No options specified\n");
		if (selected == QUERY) {
				// should return all docs for the current version
		} else {
			printf("Malformed query: Must specify options\n");
		}
	} else {
		printf("Options specified: %s\n", queryTmp);
		switch(selected) {
			case COUNT:
				count(queryTmp);
				break;
			case INSERT:
				return insert(collection, queryTmp);
				break; 
			case SORT: 
				sort(queryTmp);
				break;
			case QUERY: 
				query(queryTmp);
				break;
		}
	}
//	printDocs();
	return collection;
}
