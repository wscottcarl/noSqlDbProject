// Concerns: where is whitespace possible; is final.query([]) a thing
// TODO:sort

#include "queries.h"

#define COUNT  0
#define QUERY  1
#define INSERT 2
#define SORT   3

int flag = 1;

Document *insert(Document *docs, char *d){
	docs = parseDoc(docs, d);
	return docs;
}

void query(Document *docs, char *d){

	Document *tmpDb;
	Field *vn, *id;
	tmpDb = NULL;

	if(!strcmp(d,"") || !strcmp(d, " ")) {
// 	()
//	Returns all documents for current version
	Document *s;
	char *keyVn = "vn", *keyId = "DocID";
		for(s=docs;s!=NULL;s=s->hh.next) {
			vn = getField(s->doc, keyVn);
			id = getField(s->doc, keyId);
			if(getDocVersion(docs, id->val)-1 == vn->val) {
// 			This doc should be kept in tmpDb
				tmpDb = addDocById(tmpDb, s->id, s->doc);
			}
		}
		return;
	}
	char *options = strdup(d);
	char *tmp = strsep(&options, "[");
	char *condition = strsep(&options, "]");
	tmp = strsep(&options, "[");
	char *field = strsep(&options, "]");
	tmp = strsep(&options, "[");
	char *version = strsep(&options, "]");
//	printf("Condition: %s\nField: %s\nVersion: %s\n\n", condition, field, version);


	tmpDb = filterCondition(docs, condition);
	tmpDb = filterVersions(tmpDb, version);
	tmpDb = filterProject(tmpDb, field);
	//printDocs(tmpDb);
	printDocsToFile(tmpDb);
}

Document *filterProject(Document *docs, char *fields){
	
	Document *tmpDb=NULL, *s;
	if(!strcmp(fields,"") || !strcmp(fields, " ")) {
//	No projection, show all fields
		tmpDb = docs;
	} else {
		Field *doc;
		for(s=docs;s!=NULL;s=s->hh.next) {
			doc = getFilteredDoc(s->doc,fields);
			if(doc != NULL) { tmpDb = addDocById(tmpDb, s->id, doc); }
			doc=NULL;
		}
	}
	return tmpDb;
}

Field *getFilteredDoc(Field *source, char *field) {

	Field *sink, *tmpF;
	sink = NULL;
	char *fields = strdup(field);
	char *tmp = strsep(&fields, ",");
	while(tmp!=NULL) {
		tmpF = getField(source,tmp);
		if( tmpF !=NULL){
			sink = addField(sink, tmpF->key, tmpF->val);
		}
		tmp = strsep(&fields, ",");
		if(tmp!=NULL) { tmp++; }
	}
	free(fields);
	return sink;
}

Document *filterCondition(Document *docs, char *condition) {
	
	Document* s;
	Document* tmpV=docs;
	Document* tmpC=NULL;
	char *tmp;

// tmpV should now only contain the appropriate versions
	if(!strcmp(condition,"") || !strcmp(condition, " ")) {
//	No conditions set, include all docs
		tmpC = docs;
	} else {
// 	Conditions present. Split by commas
		tmp = strsep(&condition, ",");
		while(tmp!=NULL) {
			tmpC = NULL;
			for(s=tmpV;s!=NULL;s=s->hh.next) {
				if(checkCondition(tmp, s->doc)) {
					tmpC = addDocById(tmpC, s->id, s->doc);
				}
			}
			tmpV = tmpC;
			tmp = strsep(&condition, ",");
			if(tmp!=NULL) { tmp++; }
		}
	}
	return tmpC;
}

Document *filterVersions(Document *docs, char *version) {

	Document *s;
	Document *tmpV = NULL;
	Field *vn, *id;
	char *keyVn = "vn";
	char *keyId = "DocID";

	if (version==NULL) {
//	Filter out all of the documents that aren't the most recent version
		for(s=docs;s!=NULL;s=s->hh.next) {
			vn = getField(s->doc, keyVn);
			id = getField(s->doc, keyId);
			if(getDocVersion(docs, id->val)-1 == vn->val) {
// 			This doc should be kept in tmpV
				tmpV = addDocById(tmpV, s->id, s->doc);
			}
		}
	} else if(!strcmp(version,"") || !strcmp(version, " ")) {
//	Include ALL versions
		tmpV = docs;
	} else {
//	Include N most recent versions
		for(s=docs;s!=NULL;s=s->hh.next) {
			vn = getField(s->doc, keyVn);
			id = getField(s->doc, keyId);
			if(getDocVersion(docs, id->val)-atoi(version)-1 < vn->val) {
// 			This doc should be kept in tmpV
				tmpV = addDocById(tmpV, s->id, s->doc);
			}
		}
	}
	return tmpV;
}


int handleNotEq(Field *doc, char *field, char *val) { 
	if(getField(doc,field) == NULL) { return 0; }
	if(getField(doc,field)->val != atoi(val)) { return 1; }
	return 0; 
}
int handleLtEq(Field *doc, char *field, char *val) { 
	if(getField(doc,field) == NULL) { return 0; }
	if(getField(doc,field)->val <= atoi(val)) { return 1; }
	return 0; 
}
int handleLt(Field *doc, char *field, char *val) { 
	if(getField(doc,field) == NULL) { return 0; }
	if(getField(doc,field)->val < atoi(val)) { return 1; }
	return 0; 
}
int handleGtEq(Field *doc, char *field, char *val) { 
	if(getField(doc,field) == NULL) { return 0; }
	if(getField(doc,field)->val >= atoi(val)) { return 1; }
	return 0;
}
int handleGt(Field *doc, char *field, char *val) { 
	if(getField(doc,field) == NULL) { return 0; }
	if(getField(doc,field)->val > atoi(val)) { return 1; }
	return 0;
}
int handleEq(Field *doc, char *field, char *val) { 
	if(getField(doc,field) == NULL) { return 0; }
	if(getField(doc,field)->val==atoi(val)) { return 1; }
	return 0;
}

int checkCondition(char *txt, Field *doc) {
// example: DocID>2
	int i = 0;
	char *p = txt;
	char *tmp = strdup(txt);
	char *field = strsep(&tmp, "<>=");
	while(*p) {
//	Loop using p to find first operator
		switch(p[0]){
			case '<':
				*p++;
				if(p[0] == '>') { p++; i = handleNotEq(doc, field, p); }
				else if (p[0] == '=') { p++; i = handleLtEq(doc, field, p); }
				else { i = handleLt(doc, field, p); }
				break;
			case '>':
				*p++;
				if(p[0] == '=') { p++; i = handleGtEq(doc, field, p); }
				else { i = handleGt(doc, field, p); }
				break;
			case '=':
				*p++;
				i = handleEq(doc, field, p);
				break;
		}
		*p++;
	}
	return i;
}

void count(Document *docs, char *d){
	FILE *fp = fopen("wscarl.txt","a");
	Document *tmpDb = NULL;
	char *tmp = strsep(&d, "[");
	char *fields = strsep(&d, "]");
	tmp = strsep(&d, "[");
	char *version = strsep(&d, "]");
	
	if(!isFieldSingle(fields)) { 
		fprintf(fp, "Error: too many fields in count operation. Only 1 allowed\n");
		return;
	}

	tmpDb = filterVersions(docs, version);
	tmpDb = filterProject(tmpDb, fields);
	
	unsigned int t = HASH_COUNT(tmpDb);

	fprintf(fp, "count_%s: %u\n", fields, t);
	fclose(fp);
}

int isFieldSingle(char *fields) {
// checks whether only one field name is listed for the count operation

	char *fCopy = strdup(fields);
	char *tmp = strsep(&fCopy, ",");
//	tmp = strsep(&fCopy, ",");
	if(fCopy == NULL) { 
		free(fCopy);
		return 1; 
	}
	else { 
		free(fCopy);
		return 0; 
	}
}

void sort(Document *docs, char *d) {

	Document *tmpDb = NULL;
	char *tmp = strsep(&d, "[");
	char *fields = strsep(&d, "]");
	tmp = strsep(&d, "[");
	char *version = strsep(&d, "]");
	
	if(!isFieldSingle(fields)) { 
		printf("Error: too many fields in sort operation. Only 1 allowed\n");
		return;
	}

	tmpDb = filterBadFields(docs, fields);
	tmpDb = filterVersions(tmpDb, version);
	sortDb(tmpDb, fields);
	printDocsToFile(tmpDb);
	char *sysId = "sysid";
	sortDb(docs, sysId);
}

Document *filterBadFields(Document *docs, char *field) {
	
	Document* s;
	Document* tmpC=NULL;

	for(s=docs;s!=NULL;s=s->hh.next) {
//	Loop through all docs
		if(getField(s->doc,field)!=NULL) {
//		Check if doc contains field
//		If yes, add that doc to tmpC. If no, ignore
			tmpC = addDocById(tmpC, s->id, s->doc);
		}
	}
	return tmpC;
}

Document *parseQueries(Document *collection) {
//	Need to open file and call queries on a per-line basis
	FILE *fp = fopen("queries.txt","r");
	FILE *fpo;
	char *line = readLine(fp);

	while(!feof(fp)){ 	
//	Stores info related to parsing the query
		fpo = fopen("wscarl.txt","a");
		fprintf(fpo,"%s\n" ,line);
		fclose(fpo);
		char *queryDup = strdup(line);
		int selected=0;
		char delim[] = ".()";

//	Parse out the collection
		char *queryTmp = strsep(&queryDup, delim);
		if(!strcmp(queryTmp, "final")) {
			printf("Called on collection final\n"); }

// 	Parse out what function is declared
		queryTmp = strsep(&queryDup, delim);
		if(!strcmp(queryTmp, "count")) { selected = COUNT; }
		if(!strcmp(queryTmp, "insert")) { selected = INSERT; }
		if(!strcmp(queryTmp, "sort")) { selected = SORT; }
		if(!strcmp(queryTmp, "query")) { selected = QUERY; }
		printf("Option selected: %d\n",selected);

// 	Parse out the options associated with that function, then call the appropriate function
		queryTmp = strsep(&queryDup, delim);
		if(!strcmp(queryTmp, "")) {
			printf("No options specified\n");
			if (selected == QUERY) {
				query(collection, queryTmp);
			} else {
				printf("Malformed query: Must specify options\n");
			}
		} else {
			printf("Options specified: %s\n", queryTmp);
			switch(selected) {
				case COUNT:
					count(collection, queryTmp);
					break;
				case INSERT:
					return insert(collection, queryTmp);
					break; 
				case SORT: 
					sort(collection, queryTmp);
					break;
				case QUERY: 
					query(collection, queryTmp);
					break;
			}
		}
		line = readLine(fp);
	}
	fclose(fp);
	return collection;
}
