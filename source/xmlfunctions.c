/*
This is quizzgame, the server. I might have
an idea as to how I am going to implement this.
I am really tired.
*/
#include <stdio.h>
#include <stdint.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

int32_t xmlCheckPassword(char *docname, char *password)
{

	xmlDocPtr document; // pointer to the document
	xmlNodePtr current; // node pointer ot interact with individual nodes

	// opening the document
	// checkikng to see if the document was successfully parsed
	if ((document = xmlParseFile(docname)) == NULL)
	{
		printf("Error on parsing the doc\n");
		exit(EXIT_FAILURE);
	}

	// retrieve document's root element
	// check to see if document actually has something in it
	if ((current = xmlDocGetRootElement(document)) == NULL)
	{
		printf("Error on parsing the doc\n");
		xmlFreeDoc(document);
		exit(EXIT_FAILURE);
	}

	// check to see that we opened the right type of document
	if (xmlStrcmp(current->name, (const xmlChar *)"user"))
	{
		printf("Error on the doc\n");
		xmlFreeDoc(document);
		exit(EXIT_FAILURE);
	}

	current = current->children;
	while (current != NULL)
	{
		if ((!xmlStrcmp(current->name, (const xmlChar *)"masterpass")))
		{
			xmlChar *key = xmlNodeListGetString(document, current->children, 1);
			if (!xmlStrcmp(key, (xmlChar *)password))
			{
				xmlFree(key);
				xmlFreeDoc(document);
				return 1; // password is verified
			}
			else
			{
				fflush(stdout);
				printf("key:%s", key);
				fflush(stdout);
				xmlFree(key);
				xmlFreeDoc(document);
				return 0; // password is verified
			}
		}
		current = current->next;
	}

	//no masterpass
	return 0;
}