#include "s21_grep.h"

#include <stdio.h>

// ./s21_grep -e 'M' test.txt
//
int main(int argc, char* argv[]) {
  s_argv str_argv = {0};

  parse_args(argc, argv, &str_argv);
  runner(&str_argv);
  return 0;
}

void parse_args(int argc, char** argv, s_argv* args) {
  args->file =
      malloc(sizeof(char*) * argc);  // выделяем память под массив файлов
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (streq(argv[i], "-e")) {
        args->flags.e = 1;
      }
      if (streq(argv[i], "-i")) {
        args->flags.i = 1;
      }
      if (streq(argv[i], "-v")) {
        args->flags.v = 1;
      }
      if (streq(argv[i], "-c")) {
        args->flags.c = 1;
      }
      if (streq(argv[i], "-l")) {
        args->flags.l = 1;
      }
      if (streq(argv[i], "-n")) {
        args->flags.n = 1;
      }
    } else if (argv[i][0] = "\'") {
      args->pattern.p = argv[i]; 
    } 
    else {
      args->file[args->count_files++] = argv[i];
    }
  }
}

void match_pattern(const char* line, const s_argv* args) {
  if (args->flags.e) {
    if (find_str(line, args->pattern.p)) {
      printf("%s", line);
      printf("\n");
    }
  }
  if (args->flags.v) {
    if (!find_str(line, args->pattern.p)) {
      printf("%s", line);
      printf("\n");
    }
  }
  if (args -> flags)
}


void runner(s_argv* args) {
  int line_count = 0;
  int prev_empty = 0;
  for (int i = 0; i < (args->count_files); i++) {
    FILE* file = fopen(args->file[i], "r");
    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL) {
      match_pattern(line, args);
    }
    fclose(file);
  }
}


// Поиск паттерна в строке
//  ВОзвращает булево значение
int find_str(const char *line, const char *pattern) {
  int strlen = 0;
  int patternlen = 0;
  // i - пробег по строке
  // j - пробег по паттерну
  // ищем длину строки
  while (line[strlen] != '\0') strlen++;
  // ищем длину паттерна
  while (pattern[patternlen] != '\0') patternlen++;
  int j;
  for (int i = 0; i <= strlen - patternlen; i++) {
    for (j = 0; j < patternlen; j++) {
      if (line[i + j] != pattern[j])
        break;  // выходим из верхнего цикла и перемещаем курсор строки на
                // следующую букву
    }
    if (j == patternlen) return 1;  // если цикл не прервался возращаем True
  }
  return 0;  // Не нашли совпадение паттерна для строки
}

int e_flag(char *pattern, char *filename) {
  // работа с файлом
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("n/a");
    return 0;
  }
  // Обработка строк
  char line[1024];  // создаем буфер для хранения строк
  while (fgets(line, sizeof(line), file) != NULL) {
    if (find_str(line, pattern)) {
      printf("%s", line);
      printf("\n");
    }
  }
  printf("\n");
  fclose(file);
  return 1;
}

// int i_flag(char *pattern, char *filename) { return 0; }

// int v_flag() { return 0; }

// int c_flag() { return 0; }

// int I_flag() { return 0; }

// int n_flag() { return 0; }

// int h_flag() {return 0;}

// int s_flag() {return 0;}
