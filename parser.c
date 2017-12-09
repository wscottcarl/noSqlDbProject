#include "parser.h"

//Field *document = NULL;
//Document *docs = NULL;
int sysId=1;

Field *addField(Field *document, char *key, int v) {
	Field *f;
	
	HASH_FIND_STR(document, key, f);
	if(f==NULL) {
		f = (Field*) malloc(sizeof(Field));
		f->key = key;
		f->val = v;
		HASH_ADD_STR(document, key, f);
	}
//	printf("AddField args - key:%s, val:%d\n", key, v);
//	printf("AddField f - f->key:%s, f->val:%d\n", f->key, f->val);
//	printf("AddField document - d->key:%s, d->val:%d\n", document->key, document->val);
	return document;
}

Field *getField(Field *document, char *key) {
	Field *f;
	HASH_FIND_STR(document, key, f);
	return f;
}

void deleteField(Field *document, Field *f) {
	HASH_DEL(document, f);
	free(f);
}

Document *addDocById(Document *docs, int id, Field *doc) {
	Document *d;

	HASH_FIND_INT(docs, &id, d);
	if(d==NULL) {
		d = (Document*) malloc(sizeof(Document));
		d->id = id;
		d->doc = doc;
		HASH_ADD_INT(docs, id, d);
	}
	return docs;
}

Document *getDoc(Document *docs, int *id) {
	Document *d;
	HASH_FIND_INT(docs, id, d);
	return d;
}

int getDocVersion(Document *docs, int docId) {
	Document *d, *tmp;
	Field *f;
	int count=1;
	char *key = "DocID";
	HASH_ITER(hh, docs, d, tmp) {
		HASH_FIND_STR(d->doc, key, f);
		if(f!= NULL && f->val==docId) { count++; }
	}
	return count;
}

int getRecentVersion(Document *d) {
	
	return sysId;
}
/*
void deleteDoc(Document *docs, Document *d) {
	HASH_DEL(docs, d);
	free(d);
}*/

void printDocs(Document *docs) {
	if(docs==NULL) { printf("Collection empty\n"); }
	Field *item2, *tmp2;
	Document *item1, *tmp1;
	HASH_ITER(hh, docs, item1, tmp1) {
		HASH_ITER(hh, item1->doc, item2, tmp2) {
			printf("$items{%d}{%s} = %d\n", item1->id,
						 item2->key, item2->val);
		}
	}
}

void cleanCollection(Document *docs) {
	Field *item2, *tmp2;
	Document *item1, *tmp1;
	HASH_ITER(hh, docs, item1, tmp1) {
		HASH_ITER(hh, item1->doc, item2, tmp2) {
			HASH_DEL(item1->doc, item2);
			free(item2);
		}
		HASH_DEL(docs, item1);
		free(item1);
	}
}

Document *parseDoc(Document *docs, char *line){
	Field *document=NULL;
	char *saveField, *saveFill;
	char *f = strtok_r(line, " ", &saveField);
	while( f!=NULL ) {
		// processes each token on the line (field)
		char *k = strtok_r(f, ":", &saveFill);
		int v = atoi(strtok_r(NULL, ":", &saveFill));
		if(!strcmp(k,"DocID")) {
			document = addField(document, "vn", getDocVersion(docs, v));
		} 
		document = addField(document, k, v);
		f = strtok_r(NULL, " ", &saveField);
	}
	document = addField(document, "sysId",sysId);
	docs = addDocById(docs, sysId, document);
	sysId++;
	return docs;
}

Document *parse() {
	Document *docs=NULL;
	FILE *fp = fopen("data.txt","r");
	char *line = readLine(fp);
	while(!feof(fp)) {
		// processes each line of the input (document)
		docs = parseDoc(docs, line);
		line = readLine(fp);
	}
	free(line);
	return docs;
}
