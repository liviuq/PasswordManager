all:
	@gcc -Wall -Ixml/include/libxml2 -Iinclude -Lxml/lib source/server.c source/djb2.c  source/xmlfunctions.c -o bin/server -lxml2 -lz -lm -ldl
	@gcc -Wall -Ixml/include/libxml2 -Iinclude -Lxml/lib source/client.c source/djb2.c  source/xmlfunctions.c -o bin/client -lxml2 -lz -lm -ldl