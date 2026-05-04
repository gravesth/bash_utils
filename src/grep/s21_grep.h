#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct flags {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
} s_flags;

typedef struct pattern {
  char* p;
} s_pattern;

typedef struct s_argv {
  s_flags flags;
  s_pattern pattern;
  int count_files;
  char** file;
} s_argv;

void parse_args(int argc, char** argv, s_argv* args);
void match_pattern(const char* line, const s_argv* args, int* line_count,
                   const char* filename, int* file_printed);
void runner(s_argv* args);
int find_str(const char* line, const char* pattern, int is_case_insensitive);
void str_to_lower(const char* src, char* dst);
