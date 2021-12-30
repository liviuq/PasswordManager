#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <sodium.h>

int main(int argc, char **argv)
{
    xmlDocPtr document; // pointer to the document
    xmlNodePtr current; // node pointer ot interact with individual nodes

    // opening the document
    // checkikng to see if the document was successfully parsed
    if ((document = xmlParseFile("user.xml")) == NULL)
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

    // retrieveing the masterpass and printing it

    // current was pointing at the root element
    // now we get it s children nodes by doing this
    current = current->children;

    // while we still have entries
    while (current != NULL)
    {
        if ((!xmlStrcmp(current->name, (const xmlChar *)"masterpass")))
        {
            xmlChar *key = xmlNodeListGetString(document, current->children, 1);
            printf("masterpass: %s\n", key);
            xmlFree(key);
            break;
        }
        current = current->next;
    }

    //trying to hash something with libsodium
    //freeing the memory
    xmlFreeDoc(document);
    return EXIT_SUCCESS;
}