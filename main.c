#include <stdio.h>
#include <string.h>
#include "parser.h"

int main() {

	Document *final = parse();
	printf("Collection{%d}{%s}: %d\n", final->id, final->doc->key, 
										final->doc->val);
	printf("Goodbye, world\n");
	clean_collection();
	print_docs();
	return 0;
}
