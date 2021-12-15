#ifndef XMLFUNCTIONS_H_
#define XMLFUNCTIONS_H_

int32_t xmlCheckPassword(char *docname, char *password); // returns 1->password correct, 0->pass incorrect
int32_t xmlCheckLogin(char *docname);                    // 1->logged in, 0 ->logged off
void xmlOpenUserFile(char *docname, xmlDocPtr document, xmlNodePtr current);
void xmlReplaceLoginField(char *docname, int32_t loginVal);
void xmlCreateUser(const char *name, char *password); // creates the basic xml schema for a user
void xmlAddCategory(int32_t fd, char *user_file);
void xmlRemoveCategory(int32_t fd, char *user_file);
void xmlModifyCategory(int32_t fd, char *user_file);
void xmlGetCategory(int32_t fd, char *user_file);
void xmlGetCategoryByTitle(int32_t fd, char *user_file);

#endif