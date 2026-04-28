#include <stdio.h>
#include <string.h>

typedef struct flags {
    int b; // number_nonblank
    int E;
    int e; 
    int n; 
    int s; 
    int t; 
    int T; 
} s_flags;


int none_flags(const char *str);
int number_nonblank(const char *str);
int e_posix_flag(const char *str);
int e_gnu_flag(const char *str);
int number(const char *str);
int squeeze_blank(const char *str);
int t_posix_flag(const char *str);
int t_gnu_flag(const char *str);
int is_control_sym(int ch);
void print_control_sym(int ch);
