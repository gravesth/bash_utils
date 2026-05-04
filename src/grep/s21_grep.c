#include "s21_grep.h"

static void print_line_like_grep(const char* line) {
  printf("%s", line);
  int i = 0;
  while (line[i] != '\0') i++;
  if (i == 0 || line[i - 1] != '\n') printf("\n");
}

int main(int argc, char* argv[]) {
  s_argv str_argv = {0};

  parse_args(argc, argv, &str_argv);
  runner(&str_argv);
  free(str_argv.file);
  return 0;
}

void parse_args(int argc, char** argv, s_argv* args) {
  args->file = malloc(sizeof(char*) * argc);

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      for (int j = 1; argv[i][j] != '\0'; j++) {
        if (argv[i][j] == 'e') {
          args->flags.e = 1;
          if (i + 1 < argc) {
            args->pattern.p = argv[++i];
          }
          break;
        } else if (argv[i][j] == 'i') {
          args->flags.i = 1;
        } else if (argv[i][j] == 'v') {
          args->flags.v = 1;
        } else if (argv[i][j] == 'c') {
          args->flags.c = 1;
        } else if (argv[i][j] == 'l') {
          args->flags.l = 1;
        } else if (argv[i][j] == 'n') {
          args->flags.n = 1;
        }
      }
    } else if (!args->pattern.p) {
      args->pattern.p = argv[i];
    } else {
      args->file[args->count_files++] = argv[i];
    }
  }
}

void match_pattern(const char* line, const s_argv* args, int* line_count,
                   const char* filename, int* file_printed) {
  int matched = find_str(line, args->pattern.p, args->flags.i);
  int line_number = *line_count;

  if (args->flags.v) matched = !matched;

  if (matched) {
    if (args->flags.c) {
      (*file_printed)++;
    } else if (args->flags.l) {
      if (!(*file_printed)) {
        printf("%s\n", filename);
        *file_printed = 1;
      }
    } else if (args->flags.n) {
      printf("%d:", line_number);
      print_line_like_grep(line);
    } else {
      print_line_like_grep(line);
    }
  }

  (*line_count)++;
}

void runner(s_argv* args) {
  if (!args->pattern.p) return;

  for (int i = 0; i < (args->count_files); i++) {
    FILE* file = fopen(args->file[i], "r");
    if (file == NULL) continue;

    char line[1024];
    int line_count = 1;
    int file_printed = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
      match_pattern(line, args, &line_count, args->file[i], &file_printed);
    }
    if (args->flags.c) {
      printf("%d\n", file_printed);
    }
    fclose(file);
  }
}

// Поиск паттерна в строке
//  ВОзвращает булево значение
int find_str(const char* line, const char* pattern, int is_case_insensitive) {
  int cflags = REG_EXTENDED;  // специальный флаг
  if (is_case_insensitive) cflags |= REG_ICASE;
  if (pattern == NULL) return 0;

  regex_t regex;
  int is_match = 0;

  if (regcomp(&regex, pattern, cflags) == 0) {  // компилируем выражение
    is_match = (regexec(&regex, line, 0, NULL, 0) ==
                0);  // сопоставляем строку с шаблоном
    regfree(&regex);
  }

  return is_match;
}

void str_to_lower(const char* src, char* dst) {
  int i = 0;
  for (; src[i] != '\0'; i++) {
    if (src[i] >= 'A' && src[i] <= 'Z') {
      dst[i] = src[i] + 32;  // сдвигаем по ascii до нижнего регистра
    } else {
      dst[i] = src[i];
    }
  }
  dst[i] = '\0';
}
