#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "scanner.h"

int main() {

	Document *collection = parse();
	printf("Collection{%d}{%s}: %d\n", collection->id,
									collection->doc->key,
									collection->doc->val);
	printf("Goodbye, world\n");
	return 0;
}
