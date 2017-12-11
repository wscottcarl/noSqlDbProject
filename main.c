#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "queries.h"

int main() {

	printf("Hello, world\n");
	Document *final = parse();
//	char w[] = "DocID:555 Salary:100";
//	insert(final, w);

/*	char *abc = "abcdef";
	char *p = abc;
	while(*p) {
		printf("P: %c\n",p[0]);
		*p++;
	}
*/
	final = parseQueries(final);
	printDocs(final);
	printf("Goodbye, world\n");
	cleanCollection(final);
//	printDocs(final);
	return 0;
}
