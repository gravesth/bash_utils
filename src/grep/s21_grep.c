#include "s21_grep.h"

int main(int argc, char* argv[]) {
  s_argv str_argv = {0};

  parse_args(argc, argv, &str_argv);
  runner(&str_argv);
  free(str_argv.file);
  return 0;
}

void print_line_like_grep(const char* line) {
  printf("%s", line);
  int i = 0;
  while (line[i] != '\0') i++;
  if (i == 0 || line[i - 1] != '\n') printf("\n");
}

void parse_args(int argc, char** argv, s_argv* args) {
  args->file = malloc(sizeof(char*) *
                      argc);  // выделяем чуть больше под хранение имен файлов

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
    } else if (!args->pattern.p) {  // шаблон еще не задан
      args->pattern.p = argv[i];
    } else {  // если шаблон и флаг уже есть
      args->file[args->count_files++] = argv[i];
    }
  }
}

void match_pattern(const char* line, const s_argv* args, int* line_count,
                   const char* filename, int* file_printed) {
  // флаг -i передаем чтобы в регулярных выражениях поставить флаг REG_ICASE
  int matched = find_str(line, args->pattern.p, args->flags.i);
  int line_number = *line_count;  // для флага -n

  if (args->flags.v)
    matched = !matched;  // для флага -n выводим строки в которых find_str = 0

  if (matched) {
    if (args->flags.c) {  // только счетчик - ничего не выводим
      (*file_printed)++;
    } else if (args->flags.l) {
      if (!(*file_printed)) {  // выводим название файла если в нем есть хотя бы
                               // 1 совпадение
        printf("%s\n", filename);
        *file_printed =
            1;  // срабатывает флаг(чтобы не дублировать вывод файла)
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

void runner(const s_argv* args) {
  if (!args->pattern.p) return;

  for (int i = 0; i < (args->count_files); i++) {
    FILE* file = fopen(args->file[i], "r");
    if (file == NULL) continue;

    char line[1024];
    int line_count = 1;
    int file_printed = 0;  // обнуляем флаг вывода файла для каждой итерации
                           // runner, когда пробегаемся по файлам
    while (fgets(line, sizeof(line), file) != NULL) {
      match_pattern(line, args, &line_count, args->file[i], &file_printed);
    }
    if (args->flags.c) {
      printf("%d\n", file_printed);  // для КАЖДОГО файла печается количестов
                                     // совпадающих строк(флаг -c)
    }
    fclose(file);
  }
}

// Поиск паттерна в строке
//  ВОзвращает булево значение
int find_str(const char* line, const char* pattern, int is_case_insensitive) {
  int cflags =
      REG_EXTENDED;  // Включение ERE расширенного поиска регулярных выражений
  if (is_case_insensitive)
    cflags |= REG_ICASE;  // для флага i - не учитываем регистр
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
