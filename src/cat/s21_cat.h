#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct flags {
  int b;  // line
  int E;  // char
  int e;  // char
  int n;  // line
  int s;  // line
  int t;  // char
  int T;  // char
} s_flags;

typedef struct s_argv {
  s_flags flags;
  int count_files;
  char** file;
} s_argv;

void parse_args(int argc, char** argv, s_argv* args);
void print_string(char* string);
void string_processing(char* string, s_flags* flags, int* line_count,
                       int* prev_empty);
void runner(s_argv* args);

int is_control_sym(unsigned char ch);
void print_control_sym(unsigned char ch);