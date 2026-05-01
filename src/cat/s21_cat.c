#include "s21_cat.h"

// функция для сравнения строк
int streq(const char* a, const char* b) {
  int i = 0;
  while (a[i] && b[i]) {
    if (a[i] != b[i]) return 0;
    i++;
  }
  // если вышли из цикла -> true (строки равны)
  return a[i] == b[i];
}

void parse_args(int argc, char** argv, s_argv* args) {
  args->file =
      malloc(sizeof(char*) * argc);  // выделяем память под массив файлов
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (streq(argv[i], "-b") || streq(argv[i], "--number-nonblank")) {
        args->flags.b = 1;
      }
      if (streq(argv[i], "-E")) {
        args->flags.E = 1;
      }
      if (streq(argv[i], "-e")) {
        args->flags.e = 1;
      }
      if (streq(argv[i], "-n") || streq(argv[i], "--number")) {
        args->flags.n = 1;
      }
      if (streq(argv[i], "-s") || streq(argv[i], "--squeeze-blank")) {
        args->flags.s = 1;
      }
      if (streq(argv[i], "-t")) {
        args->flags.t = 1;
      }
      if (streq(argv[i], "-T")) {
        args->flags.T = 1;
      }
    } else {
      args->file[args->count_files++] = argv[i];
    }
  }
}

// void print_string(const char* string) {
//   for (int i = 0; string[i] != '\0'; i++) {
//     printf("%c", string[i]);
//   }
// }

void string_processing(const char* string, const s_flags* flags,
                       int* line_count, int* prev_empty) {
  int is_empty = (string[0] == '\n');
  int squeeze_blank = flags->s;
  int number_nonblank = flags->b;
  int number_all = flags->n && !flags->b;
  int show_ends = flags->E || flags->e;
  int show_tabs = flags->T || flags->t;
  int show_nonprinting = flags->e || flags->t;

  if (squeeze_blank && is_empty && *prev_empty) {
    return;
  }

  *prev_empty = is_empty;

  if (number_nonblank && !is_empty) {
    (*line_count)++;
    printf("%6d\t", *line_count);
  }
  if (number_all) {
    (*line_count)++;
    printf("%6d\t", *line_count);
  }

  for (int i = 0; string[i] != '\0'; i++) {
    unsigned char ch = (unsigned char)string[i];

    if (ch == '\n') {
      if (show_ends) printf("$");
      printf("\n");
    } else if (ch == '\t' && show_tabs) {
      printf("^I");
    } else if (show_nonprinting && (is_control_sym(ch) || ch >= 128)) {
      print_control_sym(ch);
    } else {
      printf("%c", ch);
    }
  }
}

void runner(s_argv* args) {
  int line_count = 0;
  int prev_empty = 0;
  for (int i = 0; i < (args->count_files); i++) {
    FILE* file = fopen(args->file[i], "r");

    char line[1024];

    while (fgets(line, sizeof(line), file) != NULL) {
      string_processing(line, &args->flags, &line_count, &prev_empty);
    }

    fclose(file);
  }
}

int main(int argc, char* argv[]) {
  s_argv str_argv = {0};

  parse_args(argc, argv, &str_argv);
  runner(&str_argv);

  return 0;
}

int is_control_sym(unsigned char ch) {
  // шестрадцатеричное представление управляющих символов от 0 до 31 и 127
  if (ch == 127) return 1;
  if (ch <= 0x1F && ch != '\t' && ch != '\n') return 1;
  return 0;
}

void print_control_sym(unsigned char ch) {
  if (ch >= 128) {
    printf("M-");
    ch -= 128;
  }
  if (ch == 0x7F)
    printf("^?");  // Отдельно обработаем DEL
  else if (ch == 0x09)
    printf("^I");  // Отдельно отображаем таб, как как он не входит в -v
  else if (ch <= 0x1F)
    printf("^%c", ch + 64);
  // ASCII начинается с 65. Выводим ^ + управляющий символ в ASCII
  else
    printf("%c", (char)ch);
}
