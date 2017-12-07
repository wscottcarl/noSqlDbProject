#include "parser.h"

Field *document = NULL;
Document *docs = NULL;

void add_field(char *key, int v) {
	Field *f;

	HASH_FIND_STR(document, key, f);
	if(f==NULL) {
		f = (Field*) malloc(sizeof(Field));
		f->key = key;
		f->val = v;
		HASH_ADD_STR(document, key, f);
	}
}

Field *get_field(char *key) {
	Field *f;
	HASH_FIND_STR(document, key, f);
	return f;
}

void delete_field(Field *f) {
	HASH_DEL(document, f);
}

void add_doc(int id, Field *doc) {
	Document *d;

	HASH_FIND_INT(docs, &id, d);
	if(d==NULL) {
		d = (Document*) malloc(sizeof(Document));
		d->id = id;
		d->doc = doc;
		HASH_ADD_INT(docs, id, d);
	}
}

Document *get_doc(int *id) {
	Document *d;
	HASH_FIND_INT(docs, id, d);
	return d;
}

void delete_doc(Document *d) {
	HASH_DEL(docs, d);
}

void print_docs() {
	Field *item2, *tmp2;
	Document *item1, *tmp1;
	HASH_ITER(hh, docs, item1, tmp1) {
		HASH_ITER(hh, item1->doc, item2, tmp2) {
			printf("$items{%d}{%s} = %d\n", item1->id,
						 item2->key, item2->val);
		}
	}
}

Document *parse() {
	FILE *fp = fopen("docs","r");
	char *saveField, *saveFill;
	char *line = readLine(fp);
	int i=0;
	while(!feof(fp)) {
		// processes each line of the input (document)
		char *f = strtok_r(line, " ", &saveField);
		while( f!=NULL ) {
			// processes each token on the line (field)
			char *k = strtok_r(f, ":", &saveFill);
			int v = atoi(strtok_r(NULL, ":", &saveFill));
			add_field(k, v);
			f = strtok_r(NULL, " ", &saveField);
		}
		add_doc(i,document);
		document = NULL;
		line = readLine(fp);
		i++;
	}
	free(line);
	printf("Hello, world\n");
	return docs;
}
