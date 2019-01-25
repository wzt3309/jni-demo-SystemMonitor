//
// Created by wzt on 10/25/17.
//

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define istoken(c) \
	isspace(c) || c == ','

#define tobin(a) \


int main()
{
    char line[] = "10H, 12H, 13H, 14H, 15H";
    char *ptr = line;

    while (*ptr)
    {
        int a;
        a = strtoul(ptr, &ptr, 16);
        printf("%d\n", a);
        while (*ptr && !istoken(*ptr) ) ptr++;
    }

}

