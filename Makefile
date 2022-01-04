all:
	@gcc -Wall -g -Ixml/include/libxml2 -Iinclude -Lxml/lib source/server.c source/xmlfunctions.c -o bin/server -lxml2 -lm -ldl
	@gcc -Wall -g -Ixml/include/libxml2 -Iinclude -Lxml/lib source/client.c source/xmlfunctions.c -o bin/client -lxml2 -lm -ldl
