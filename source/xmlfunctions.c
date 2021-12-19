/*
This is password manager, the server. I might have
an idea as to how I am going to implement this.
I am really tired.
*/
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#define FAIL_IF(EXP) (                                            \
	{                                                             \
		if ((EXP) == -1)                                          \
		{                                                         \
			printf("%s on line %d\n", strerror(errno), __LINE__); \
			exit(EXIT_FAILURE);                                   \
		}                                                         \
	})
#define TITLE 49
#define PASSWORD 50
#define URL 51
#define INFORMATION 52

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
				// printf("key:%s", key);
				fflush(stdout);
				xmlFree(key);
				xmlFreeDoc(document);
				return 0; // password is verified, but it does not match
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

void xmlCreateUser(const char *name, const char *password)
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
	xmlKeepBlanksDefault(0);
	xmlSaveFormatFile(name, new_document, 1);
	xmlFreeDoc(new_document);

	xmlCleanupParser();
}

void xmlOpenUserFile(char *docname, xmlDocPtr document, xmlNodePtr current)
{
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
			xmlSaveFormatFile(docname, document, 1);
			xmlFreeDoc(document);
			return;
		}
		current = current->next;
	}
}

int32_t xmlExistsTitle(char *user_file, char *title)
{
	xmlDocPtr document = NULL; // pointer to the document
	xmlNodePtr root = NULL;	   // node pointer ot interact with individual nodes

	// opening the document
	// checkikng to see if the document was successfully parsed
	if ((document = xmlParseFile(user_file)) == NULL)
	{
		printf("Error on parsing the doc\n");
		exit(EXIT_FAILURE);
	}

	// retrieve document's root element
	// check to see if document actually has something in it
	if ((root = xmlDocGetRootElement(document)) == NULL)
	{
		printf("Error on parsing the doc\n");
		xmlFreeDoc(document);
		exit(EXIT_FAILURE);
	}

	// check to see that we opened the right type of document
	if (xmlStrcmp(root->name, (const xmlChar *)"user"))
	{
		printf("Error on the doc\n");
		xmlFreeDoc(document);
		exit(EXIT_FAILURE);
	}

	// searching for a <category> field in order to find title
	xmlNodePtr current = root->children;
	while (current != NULL)
	{
		if (!xmlStrcmp(current->name, (const xmlChar *)"category")) // we found a category label, iterating to check for <title> category
		{
			xmlNodePtr category = current->children;
			while (category != NULL)
			{
				if (!xmlStrcmp(category->name, BAD_CAST "title")) // we found the title label, check for actual content
				{
					xmlChar *key = xmlNodeListGetString(document, category->children, 1);
					if (!xmlStrcmp(key, BAD_CAST title)) // there is already a category with the same title
					{
						xmlFree(key);
						xmlFreeDoc(document);
						return 1;
					}
				}
				category = category->next;
			}
		}
		current = current->next;
	}

	// we did not find a category with title, return 0
	return 0;
}

void xmlAddCategory(int32_t fd, char *user_file)
{
	//"Input category"
	const char *input_category = "Input a category title:\n";
	int32_t input_category_length = strlen(input_category);
	FAIL_IF(write(fd, &input_category_length, sizeof(input_category_length)));
	FAIL_IF(write(fd, input_category, input_category_length));

	// reading the catefory title
	int32_t incoming_length;
	FAIL_IF(read(fd, &incoming_length, sizeof(incoming_length)));
	char title[incoming_length];
	FAIL_IF(read(fd, title, incoming_length));
	title[incoming_length - 1] = 0;

	//"Input password"
	const char *input_password = "Input a password: \n";
	int32_t input_password_length = strlen(input_password);
	FAIL_IF(write(fd, &input_password_length, sizeof(input_password_length)));
	FAIL_IF(write(fd, input_password, input_password_length));

	// reading the password
	FAIL_IF(read(fd, &incoming_length, sizeof(incoming_length)));
	char category_password[incoming_length];
	FAIL_IF(read(fd, category_password, incoming_length));
	category_password[incoming_length - 1] = 0;

	// Input url
	const char *input_url = "Input an url: \n";
	int32_t input_url_length = strlen(input_url);
	FAIL_IF(write(fd, &input_url_length, sizeof(input_url_length)));
	FAIL_IF(write(fd, input_url, input_url_length));

	// reading the url
	FAIL_IF(read(fd, &incoming_length, sizeof(incoming_length)));
	char url[incoming_length];
	FAIL_IF(read(fd, url, incoming_length));
	url[incoming_length - 1] = 0;

	// Input notes
	const char *input_notes = "Input some information about it: \n";
	int32_t input_notes_length = strlen(input_notes);
	FAIL_IF(write(fd, &input_notes_length, sizeof(input_notes_length)));
	FAIL_IF(write(fd, input_notes, input_notes_length));

	// reading the notes
	FAIL_IF(read(fd, &incoming_length, sizeof(incoming_length)));
	char notes[incoming_length];
	FAIL_IF(read(fd, notes, incoming_length));
	notes[incoming_length - 1] = 0;

	if (!xmlExistsTitle(user_file, title))
	{
		xmlDocPtr document = NULL; // pointer to the document
		xmlNodePtr root = NULL;	   // node pointer ot interact with individual nodes

		// opening the document
		// checkikng to see if the document was successfully parsed
		if ((document = xmlParseFile(user_file)) == NULL)
		{
			printf("Error on parsing the doc\n");
			exit(EXIT_FAILURE);
		}

		// retrieve document's root element
		// check to see if document actually has something in it
		if ((root = xmlDocGetRootElement(document)) == NULL)
		{
			printf("Error on parsing the doc\n");
			xmlFreeDoc(document);
			exit(EXIT_FAILURE);
		}

		// check to see that we opened the right type of document
		if (xmlStrcmp(root->name, (const xmlChar *)"user"))
		{
			printf("Error on the doc\n");
			xmlFreeDoc(document);
			exit(EXIT_FAILURE);
		}

		// adding the category
		xmlNodePtr category_ptr = xmlNewChild(root, NULL, BAD_CAST "category", NULL);
		xmlNewChild(category_ptr, NULL, BAD_CAST "title", BAD_CAST title);
		xmlNewChild(category_ptr, NULL, BAD_CAST "password", BAD_CAST category_password);
		xmlNewChild(category_ptr, NULL, BAD_CAST "url", BAD_CAST url);
		xmlNewChild(category_ptr, NULL, BAD_CAST "notes", BAD_CAST notes);
		// saving the file to the memory
		xmlKeepBlanksDefault(0);
		xmlSaveFormatFile(user_file, document, 1);
		xmlFreeDoc(document);

		// freeing whatever memory the parser used
		xmlCleanupParser();
	}
}

void xmlRemoveCategory(int32_t fd, char *user_file)
{
	//"Input title to delete"
	const char *input_title = "Input a title:\n";
	int32_t input_title_length = strlen(input_title);
	FAIL_IF(write(fd, &input_title_length, sizeof(input_title_length)));
	FAIL_IF(write(fd, input_title, input_title_length));

	// reading the catefory title
	int32_t incoming_length;
	FAIL_IF(read(fd, &incoming_length, sizeof(incoming_length)));
	char title[incoming_length];
	FAIL_IF(read(fd, title, incoming_length));
	title[incoming_length - 1] = 0;

	// finding the category with "title"
	xmlDocPtr document = NULL; // pointer to the document
	xmlNodePtr root = NULL;	   // node pointer ot interact with individual nodes

	// opening the document
	// checkikng to see if the document was successfully parsed
	if ((document = xmlParseFile(user_file)) == NULL)
	{
		printf("Error on parsing the doc\n");
		exit(EXIT_FAILURE);
	}

	// retrieve document's root element
	// check to see if document actually has something in it
	if ((root = xmlDocGetRootElement(document)) == NULL)
	{
		printf("Error on parsing the doc\n");
		xmlFreeDoc(document);
		exit(EXIT_FAILURE);
	}

	// check to see that we opened the right type of document
	if (xmlStrcmp(root->name, (const xmlChar *)"user"))
	{
		printf("Error on the doc\n");
		xmlFreeDoc(document);
		exit(EXIT_FAILURE);
	}

	// searching for a <category> field in order to find title
	xmlNodePtr current = root->children;
	while (current != NULL)
	{
		if (!xmlStrcmp(current->name, (const xmlChar *)"category")) // we found a category label, iterating to check for <title> category
		{
			xmlNodePtr category = current->children;
			while (category != NULL)
			{
				if (!xmlStrcmp(category->name, BAD_CAST "title")) // we found the title label, check for actual content
				{
					xmlChar *key = xmlNodeListGetString(document, category->children, 1);
					if (!xmlStrcmp(key, BAD_CAST title)) // we found the category with this title, unlink it, save format
					{
						xmlFree(key);
						xmlUnlinkNode(current);
						xmlKeepBlanksDefault(0); // for formatting purposes
						xmlSaveFormatFile(user_file, document, 1);

						// freeing whatever memory the parser used
						xmlCleanupParser();

						xmlFreeDoc(document);
						return;
					}
				}
				category = category->next;
			}
		}
		current = current->next;
	}
}

void xmlModifyCategory(int32_t fd, char *user_file)
{
	//"Input category title to modify
	const char *input_title = "Input the category title to apply modifications:\n";
	int32_t input_title_length = strlen(input_title);
	FAIL_IF(write(fd, &input_title_length, sizeof(input_title_length)));
	FAIL_IF(write(fd, input_title, input_title_length));

	// reading the catefory title
	int32_t incoming_length;
	FAIL_IF(read(fd, &incoming_length, sizeof(incoming_length)));
	char title[incoming_length];
	FAIL_IF(read(fd, title, incoming_length));
	title[incoming_length - 1] = 0;

	// finding the category with "title"
	xmlDocPtr document = NULL; // pointer to the document
	xmlNodePtr root = NULL;	   // node pointer ot interact with individual nodes

	// opening the document
	// checkikng to see if the document was successfully parsed
	if ((document = xmlParseFile(user_file)) == NULL)
	{
		printf("Error on parsing the doc\n");
		exit(EXIT_FAILURE);
	}

	// retrieve document's root element
	// check to see if document actually has something in it
	if ((root = xmlDocGetRootElement(document)) == NULL)
	{
		printf("Error on parsing the doc\n");
		xmlFreeDoc(document);
		exit(EXIT_FAILURE);
	}

	// check to see that we opened the right type of document
	if (xmlStrcmp(root->name, (const xmlChar *)"user"))
	{
		printf("Error on the doc\n");
		xmlFreeDoc(document);
		exit(EXIT_FAILURE);
	}

	// searching for a <category> field in order to find title
	xmlNodePtr current = root->children;
	while (current != NULL)
	{
		if (!xmlStrcmp(current->name, (const xmlChar *)"category")) // we found a category label, iterating to check for <title> category
		{
			xmlNodePtr category = current->children;
			while (category != NULL)
			{
				xmlNodePtr category_modifyer = current;
				if (!xmlStrcmp(category->name, BAD_CAST "title")) // we found the title label, check for actual content
				{
					xmlChar *key = xmlNodeListGetString(document, category->children, 1);
					if (!xmlStrcmp(key, BAD_CAST title)) // we found the category with this title, ask what to modify
					{
						xmlFree(key);

						// choice for the answer from client
						int32_t bytes_read;
						char choice;
						int32_t choice_length;

						const char *modifiers = "Here are your options to modify:\n"
												"1) Title\n"
												"2) Password\n"
												"3) URL\n"
												"4) Informations\n. Choice:";
						int32_t modifiers_length = strlen(modifiers);

						int32_t input_is_ok = 0;
						while (!input_is_ok)
						{
							FAIL_IF(write(fd, &modifiers_length, sizeof(modifiers_length)));
							FAIL_IF(write(fd, modifiers, modifiers_length));

							// read the choice
							// Verify that you read at most 2 bytes so the input is valid
							// otherwise, loop untill you get the correct input
							FAIL_IF(bytes_read = read(fd, &choice_length, sizeof(choice_length)));
							if (choice_length > 2) // answer too big to fit in one byte, so we consume the rest
							{
								char temp[1024];
								FAIL_IF(read(fd, temp, 1024));
							}
							else
							{
								FAIL_IF(bytes_read = read(fd, &choice, choice_length));
								if (bytes_read == 2 && choice >= 49 && choice <= 53)
									input_is_ok = 1;
							}
						}

						// category_modifier is now your pointer to modify specific attributes
						switch (choice)
						{
						case TITLE:
						{
							const char *new_title = "Input a new title:\n";
							int32_t new_title_length = strlen(new_title);
							FAIL_IF(write(fd, &new_title_length, sizeof(new_title_length)));
							FAIL_IF(write(fd, new_title, new_title_length));

							// reading the new title
							FAIL_IF(read(fd, &incoming_length, sizeof(incoming_length)));
							char title1[incoming_length];
							FAIL_IF(read(fd, title1, incoming_length));
							title1[incoming_length - 1] = 0;

							// searching for the title label
							category_modifyer = category_modifyer->children;
							while (category_modifyer != NULL)
							{
								if (!xmlStrcmp(category_modifyer->name, (const xmlChar *)"title")) // we found title label
								{
									printf("FOUND TITLE TO REPLACE\n");
									xmlNodeSetContent(category_modifyer, BAD_CAST title1);
									xmlKeepBlanksDefault(0); // for formatting purposes
									xmlSaveFormatFile(user_file, document, 1);

									// freeing whatever memory the parser used
									xmlCleanupParser();
									xmlFreeDoc(document);
									return;
								}
								category_modifyer = category_modifyer->next;
							}
							break;
						}
						case PASSWORD:
							break;
						case URL:
							break;
						case INFORMATION:
							break;
						}
						xmlKeepBlanksDefault(0); // for formatting purposes
						xmlSaveFormatFile(user_file, document, 1);

						// freeing whatever memory the parser used
						xmlCleanupParser();
						xmlFreeDoc(document);
						return;
					}
				}
				category = category->next;
			}
		}
		current = current->next;
	}
}

void xmlGetCategory(int32_t fd, char *user_file)
{
}