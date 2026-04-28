#include "s21_cat.h"

// ./main -b "text.txt"

// функция для сравнения строк
int streq(const char *a, const char *b) {
  int i = 0;
  while (a[i] && b[i]) {
    if (a[i] != b[i]) return 0;
    i++;
  }
  // если вышли из цикла -> true (строки равны)
  return a[i] == b[i];
}
void parse_args(int argc, char **argv, s_flags *flags) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (streq(argv[i], "-b") || streq(argv[i], "--number-nonblank")) {
        flags->b = 1;
      }
      if (streq(argv[i], "-E")) {
        flags->E = 1;
      }
      if (streq(argv[i], "-e")) {
        flags->e = 1;
      }
      if (streq(argv[i], "-n") || streq(argv[i], "--number")) {
        flags->n = 1;
      }
      if (streq(argv[i], "-s") || streq(argv[i], "--squeeze-blank")) {
        flags->s = 1;
      }
      if (streq(argv[i], "-t")) {
        flags->t = 1;
      }
      if (streq(argv[i], "-T")) {
        flags->T = 1;
      }
    } else {
      // flags->files[flags->file_count++] = argv[i]; 
    }
  }
}

  int main(int argc, char *argv[]) {
    if (argc < 2) {
      return 0;
    }

    const char *file_name = NULL;
    if (argv[1][0] == '-') {
      if (argc < 3) {
        return 0;
      }
      file_name = argv[2];
    } else {
      file_name = argv[1];
    }

    if (streq(argv[1], "-b") || streq(argv[1], "--number-nonblank")) {
      return number_nonblank(file_name);

    } else if (streq(argv[1], "-n") || streq(argv[1], "--number")) {
      return number(file_name);

    } else if (streq(argv[1], "-s") || streq(argv[1], "--squeeze-blank")) {
      return squeeze_blank(file_name);
    }

    // 0 - имя исполняемого файла
    // 1 - флаг
    // 2 - ad

    // 2 - имя файла
    if (argv[1][0] == '-') {
      switch (argv[1][1]) {
        case 'b':
          if (number_nonblank(file_name)) return 0;
          break;
        case 'E':
          if (e_gnu_flag(file_name)) return 0;
          break;
        case 'e':
          if (e_posix_flag(file_name)) return 0;
          break;
        case 'n':
          if (number(file_name)) return 0;
          break;
        case 's':
          if (squeeze_blank(file_name)) return 0;
          break;
        case 't':
          if (t_posix_flag(file_name)) return 0;
          break;
        case 'T':
          if (t_gnu_flag(file_name)) return 0;
          break;
      }
    } else {
      if (none_flags(file_name)) return 0;
    }

    return 0;
  }

  int is_control_sym(int ch) {
    // шестрадцатеричное представление управляющих символов от 0 до 31 и 127
    if (ch == 127) return 1;
    if (ch >= 0x00 && ch <= 0x1F && ch != '\t' && ch != '\n') return 1;
    return 0;
  }

  void print_control_sym(int ch) {
    if (ch >= 128) {
      printf("M-");
      ch -= 128;
    }

    if (ch == 0x7F)
      printf("^?");  // Отдельно обработаем DEL
    else if (ch == 0x09)
      printf("^I");  // Отдельно отображаем таб, как как он не входит в -v
    else if (ch >= 0x00 && ch <= 0x1F)
      printf("^%c", ch + 64);
    // ASCII начинается с 65. Выводим ^ + управляющий символ в ASCII
    else
      printf("%c", (char)ch);
  }

  // Flag -b
  int number_nonblank(const char *str) {
    FILE *file = fopen(str, "r");
    if (file == NULL) {
      printf("n/a");
      return 0;
    }

    char line[1024];
    int count = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
      if (line[0] == '\n') {
        printf("%s", line);
      } else {
        count++;
        printf("%6d\t%s", count, line);
      }
    }

    fclose(file);
    return 1;
  }

  // Реализация флага -E без -v (GNU only).
  // Отображаем только $ если \n
  int e_gnu_flag(const char *str) {
    FILE *file = fopen(str, "r");
    if (file == NULL) {
      printf("%s", "n/a");
      return 0;
    }
    int ch;
    while ((ch = fgetc(file)) != EOF) {
      if (ch == '\n') {
        printf("$\n");
        continue;
      }
      printf("%c", (char)ch);
    }
    fclose(file);
    return 1;
  }

  // Реализация флага -е c -v (POSIX)
  // Отображаем все управляющие(непечатаемые) символы кроме таба
  // В конце строки также отображаем $
  int e_posix_flag(const char *str) {
    FILE *file = fopen(str, "r");
    if (file == NULL) {
      printf("n/a");
      return 0;
    }
    int ch;
    while ((ch = fgetc(file)) != EOF) {
      if (ch == '\n') {
        printf("$\n");
        continue;
      } else if (is_control_sym(ch) || ch >= 128) {
        print_control_sym(ch);
        continue;
      }
      printf("%c", (char)ch);
    }
    fclose(file);
    return 1;
  }

  // Flag -n
  int number(const char *str) {
    FILE *file = fopen(str, "r");
    int count = 0;
    char line[1024];

    if (file == NULL) {
      printf("%s", "n/a");
      return 0;
    }

    while (fgets(line, 1024, file) != NULL) {
      count++;
      printf("%6d\t%s", count, line);
    }

    fclose(file);
    return 1;
  }

  // Flag -s
  int squeeze_blank(const char *str) {
    FILE *file = fopen(str, "r");
    if (file == NULL) return 1;

    char line[1024];
    int prev_empty = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
      int is_empty = (line[0] == '\n');

      if (is_empty && prev_empty) {
        continue;  // пропускаем вторую и более пустую строку
      }

      printf("%s", line);
      prev_empty = is_empty;
    }

    fclose(file);
    return 0;
  }

  // Flag -t posix with -v
  int t_posix_flag(const char *str) {
    FILE *file = fopen(str, "r");
    if (file == NULL) {
      printf("n/a");
      return 0;
    }
    int ch;
    while ((ch = fgetc(file)) != EOF) {
      if (ch == '\t') {
        printf("^I");
        continue;
      } else if (ch == '\n') {
        putchar('\n');
        continue;
      } else if (is_control_sym(ch) || ch >= 128) {
        print_control_sym(ch);
        continue;
      }
      printf("%c", (char)ch);
    }
    fclose(file);
    return 1;
  }

  // Flag -T gnu
  int t_gnu_flag(const char *str) {
    FILE *file = fopen(str, "r");
    if (file == NULL) {
      printf("%s", "n/a");
      return 0;
    }
    int ch;
    while ((ch = fgetc(file)) != EOF) {
      if (ch == '\t') {
        printf("^I");  // отображаем табы
        continue;
      }
      printf("%c", (char)ch);
    }
    fclose(file);
    return 1;
  }

  // None flags
  int none_flags(const char *str) {
    FILE *file = fopen(str, "r");
    if (file == NULL) {
      printf("%s", "n/a");
      return 0;
    }

    char line[1024];

    while (fgets(line, 1024, file) != NULL) {
      printf("%s", line);
    }
    // line - буффер.
    // читаем не более 1024 символов
    // входной поток

    fclose(file);
    return 1;
  }