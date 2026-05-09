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

int handle_flag(const char* arg, int* i, int argc, char** argv, s_argv* args) {
  int err = 0;
  for (int j = 1; arg[j] != '\0'; j++) {
    if (arg[j] == 'e') {
      args->flags.e = 1;
      if ((*i) + 1 >= argc) {
        printf("Ошибка: после -e нет паттерна\n");
        err = 1;
      } else {
        args->pattern.p = argv[(*i) + 1];
        (*i)++;
      }
      break;
    } else if (arg[j] == 'i') {
      args->flags.i = 1;
    } else if (arg[j] == 'v') {
      args->flags.v = 1;
    } else if (arg[j] == 'c') {
      args->flags.c = 1;
    } else if (arg[j] == 'l') {
      args->flags.l = 1;
    } else if (arg[j] == 'n') {
      args->flags.n = 1;
    }
  }
  return err;
}

int parse_args(int argc, char** argv, s_argv* args) {
  int err = 0;
  args->file = malloc(sizeof(char*) * argc);

  for (int i = 1; i < argc && !err; i++) {
    // обработка флагов
    if (argv[i][0] == '-') {
      err = handle_flag(argv[i], &i, argc, argv, args);
      continue;
    }
    // паттерн
    if (!args->pattern.p) {
      args->pattern.p = argv[i];
      continue;
    }
    // файлы
    args->file[args->count_files++] = argv[i];
  }

  if (!err && !args->pattern.p) {
    printf("Отсутствует паттерн\n");
    err = 1;
  }

  if (!err && args->count_files == 0) {
    printf("Отсутствует файл\n");
    err = 1;
  }
  return err;
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
        // срабатывает флаг (чтобы не дублировать вывод файла)
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

void runner(const s_argv* args) {
  if (!args->pattern.p) return;

  for (int i = 0; i < (args->count_files); i++) {
    FILE* file = fopen(args->file[i], "r");
    if (file == NULL) {
      printf("%s", "Произошла ошибка при открытии файла");
      continue;
    }

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
    cflags = cflags | REG_ICASE;  // для флага i - не учитываем регистр.
  if (pattern == NULL || line == NULL) return 0;

  regex_t regex;
  int is_match = 0;

  int err = regcomp(&regex, pattern, cflags);

  if (err == 0) {  // компилируем выражение
    is_match = (regexec(&regex, line, 0, NULL, 0) ==
                0);  // сопоставляем строку с шаблоном
    regfree(&regex);
  } else {
    char buf[300];
    regerror(err, NULL, buf, 300);
    printf("Ошибка компииляции regcomp %s\n", buf);
    return 0;
  }
  return is_match;
}
