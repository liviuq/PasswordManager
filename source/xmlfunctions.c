/*
This is quizzgame, the server. I might have
an idea as to how I am going to implement this.
I am really tired.
*/
#include <stdio.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

void parseQuestion(xmlDocPtr doc, xmlNodePtr cur)
{

	xmlChar *key;
	cur = cur->xmlChildrenNode;
	while (cur != NULL)
	{
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"title")))
		{
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			printf("keyword: %s\n", key);
			xmlFree(key);
		}
		cur = cur->next;
	}
}

void parseDoc(char *docname)
{

	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile(docname);

	if (doc == NULL)
	{
		fprintf(stderr, "Document not parsed successfully. \n");
		return;
	}

	cur = xmlDocGetRootElement(doc);

	if (cur == NULL)
	{
		fprintf(stderr, "empty document\n");
		xmlFreeDoc(doc);
		return;
	}

	if (xmlStrcmp(cur->name, (const xmlChar *)"set"))
	{
		fprintf(stderr, "document of the wrong type, root node != set");
		xmlFreeDoc(doc);
		return;
	}

	cur = cur->xmlChildrenNode;
	while (cur != NULL)
	{
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"question")))
		{
			parseQuestion(doc, cur);
		}

		cur = cur->next;
	}

	xmlFreeDoc(doc);
}