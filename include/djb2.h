/*
This is the hashing function I am going to use for the users.
I do not want to store the IP's in a table without some sort of protection
of the information. And it is an opportunity to learn about hashing in general
*/

#ifndef DJB2_H_
#define DJB2_H_

unsigned long
hash(unsigned char *str);

#endif