
#include <stdio.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "xmlfunctions.h" //functions to parse questions.xml
#include "djb2.h"		  //function for hashing username

int main(int argc, char **argv)
{
	printf("%ld\n", hash("andrew"));
	char *docname;

	if (argc <= 1)
	{
		printf("Usage: %s docname\n", argv[0]);
		return (0);
	}

	docname = argv[1];
	parseDoc(docname);

	return 0;
}