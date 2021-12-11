#ifndef XMLFUNCTIONS_H_
#define XMLFUNCTIONS_H_

int32_t xmlCheckPassword(char *docname, char *password); //returns 1->password correct, 0->pass incorrect
int32_t xmlCheckLogin(char *docname); //1->logged in, 0 ->logged off
void xmlCreateUser(const char* name, const char* password); //creates the basic xml schema for a user
void xmlReplaceLoginField(char *docname, int32_t loginVal);

#endif