all:
	@gcc -Wall -Ixml/include/libxml2 -Iinclude -Lxml/lib source/server.c source/djb2.c  source/xmlfunctions.c -o bin/server -lxml2 -lm -ldl
	@gcc -Wall -Ixml/include/libxml2 -Iinclude -Lxml/lib source/client.c source/djb2.c  source/xmlfunctions.c -o bin/client -lxml2 -lm -ldl
	@gcc -Wall -Ixml/include/libxml2 -Iinclude -Lxml/lib source/test.c source/djb2.c  source/xmlfunctions.c -o bin/test -lxml2 -lm -ldl
