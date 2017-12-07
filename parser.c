#include "parser.h"

void add_field(Field *f) {
	HASH_ADD_STR(collection, key, f);
}

Field *find_field(char *key) {
	Field *f;
	HASH_FIND_STR(collection, key, f);
	return f;
}

void delete_field(Field *f) {
	HASH_DEL(collection, f);
}
