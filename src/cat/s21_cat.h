#include <stdio.h> 
#include <string.h>

int none_flags(char *str, char *line); 
int number_nonblank(char *str, char *line);
int e_posix_flag(char *str, char *line);
int e_gnu_flag (char *str, char *line);
int number(char *str, char *line);
int squeeze_blank(char *str, char *line); 

