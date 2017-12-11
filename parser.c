#include "parser.h"

//Field *document = NULL;
//Document *docs = NULL;
int sysId=1;
char *sortField="";

Field *addField(Field *document, char *key, int v) {
	Field *f;
	
	HASH_FIND_STR(document, key, f);
	if(f==NULL) {
		f = (Field*) malloc(sizeof(Field));
		f->key = key;
		f->val = v;
		HASH_ADD_STR(document, key, f);
	}
	
//	sortFields(document);
	
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

void deleteDoc(Document *docs, Document *d) {
	HASH_DEL(docs, d);
	free(d);
}

void printDocsToFile(Document *docs) {

	Document *s;
	Field *i;
	char *vn = "vn";
	FILE *fp = fopen("wscarl.txt", "a");
	for(s=docs;s!=NULL;s=s->hh.next) {
		fprintf(fp, "\t");
//		i = getField(s->doc, vn);
//		fprintf(fp, "vn:%s ",i);
		for(i=s->doc;i!=NULL;i=i->hh.next) {
//			if(strcmp(i->key,vn)) { continue; }
			fprintf(fp, "%s:%d ",i->key, i->val);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
}

void printDocs(Document *docs) {
	if(docs==NULL) { printf("Collection empty\n"); }

	Document *s;
	Field *i;
	for(s=docs;s!=NULL;s=s->hh.next) {
		for(i=s->doc;i!=NULL;i=i->hh.next) {
			printf("%s: %d ",i->key,i->val);
		}
		printf("\n");
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
	document = addField(document, "sysid",sysId);
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

void sortDb(Document *docs, char *w) {
	sortField = w;
	HASH_SORT(docs, sortDocs);
}

int sortDocs(Document *a, Document *b) {
	Field *fieldA = getField(a->doc,sortField);
	Field *fieldB = getField(b->doc,sortField);
	return fieldA->val - fieldB->val;
}

void sortFields(Field *doc) {
	HASH_SORT(doc,orderFields);
	printf("\n");
}

int orderFields(Field *a, Field *b) {
//	printf("a->key: %s\tb->key: %s\tCompare: %d\n",a->key,b->key,alphabet(a->key,b->key));
	if(strcmp(a->key, "vn")) { /*printf("Strcmp A:vn - %d\n",!strcmp(a->key,"vn"));*/return -1; }
	if(!strcmp(a->key, "sysId") && strcmp(b->key, "vn")) { return -1; }
	return alphabet(a->key,b->key);
}

int alphabet(char *a, char*b) {
	int i; 
	for(i=0;i<strlen(a);i++) {
		if(b[i] == '\0') {
			printf("%s is after %s\n",a,b);
			return -1;
		}
		if(a[i] == b[i]) { continue;  }
		if(a[i] < b[i])  { printf("%s is before %s\n",a,b);return 1;  }
		if(a[i] > b[i])  { printf("%s is after %s\n",a,b);return -1; }
	}
	if(b[i]== '\0') { printf("%s and %s are the same\n",a,b);return 0; }
	else { printf("%s is before %s\n",a,b);return 1; }
}



