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

	// no masterpass
	return 0;
}

int32_t xmlCheckLogin(char *docname)
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

	// se4rching for the login field
	current = current->children;
	while (current != NULL)
	{
		if ((!xmlStrcmp(current->name, (const xmlChar *)"login")))
		{
			xmlChar *key = xmlNodeListGetString(document, current->children, 1);
			if (!xmlStrcmp(key, (xmlChar *)"1"))
			{
				xmlFree(key);
				xmlFreeDoc(document);
				return 1; // logged in
			}
			else
			{
				xmlFree(key);
				xmlFreeDoc(document);
				return 0; // logged off
			}
			xmlFreeDoc(document);
		}
		current = current->next;
	}

	return 0; // control should not be reaching here.
}

void xmlCreateUser(const char* name, const char* password)
{
	xmlDocPtr new_document = NULL;
	xmlNodePtr root_node = NULL;

	// creating the document
	new_document = xmlNewDoc(BAD_CAST "1.0");
	root_node = xmlNewNode(NULL, BAD_CAST "user");
	xmlDocSetRootElement(new_document, root_node);

	// adding the child items, like password and login field
	xmlNewChild(root_node, NULL, BAD_CAST "masterpass", BAD_CAST password);
	// add <login> field with 1
	xmlNewChild(root_node, NULL, BAD_CAST "login", BAD_CAST "1");

	// saving the file to the memory
	xmlSaveFormatFile(name, new_document, 1);
	xmlFreeDoc(new_document);

	xmlCleanupParser();
}

void xmlReplaceLoginField(char *docname, int32_t loginVal)
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

	// se4rching for the login field
	current = current->children;
	while (current != NULL)
	{
		if ((!xmlStrcmp(current->name, (const xmlChar *)"login")))
		{
			xmlNodeSetContent(current, loginVal == 1 ? BAD_CAST "1" : BAD_CAST "0");
			xmlFreeDoc(document);
		}
		current = current->next;
	}
}