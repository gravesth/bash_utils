#include <stdio.h>
#include <string.h>

int none_flags(char *str);
int number_nonblank(char *str);
int e_posix_flag(char *str);
int e_gnu_flag(char *str);
int number(char *str);
int squeeze_blank(char *str);
int t_posix_flag(char *str);
int t_gnu_flag(char *str);
int is_control_sym(int ch);
void print_control_sym(int ch);
