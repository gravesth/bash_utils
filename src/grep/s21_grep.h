int streq(char *a, char *b);
int e_flag(char *pattern, char *filename);


typedef struct flags {
  int e;  
  int i;  
  int v;  
  int c;  
  int l;  
  int n;  
} s_flags;

typedef struct pattern {
  char *p; 
} s_pattern;

typedef struct s_argv {
  s_flags flags;
  s_pattern pattern; 
  int count_files;
  char** file;
} s_argv;

