#include <stdio.h>

// Функция для анализа упорядоченности символов (задача 4)
void analyze_characters() {
    printf("=== Анализ упорядоченности кодов символов ===\n");
    printf("=============================================\n");
    
    // Анализ цифр '0'-'9'
    printf("Цифры '0'-'9':\n");
    printf("Коды: \n");
    for (char c = '0'; c <= '9'; c++) {
        printf("%c=%d\n", c, c);
    }
    printf("\nРазность между '9' и '0': %d\n\n", '9' - '0');
    
    // Анализ строчных букв 'a'-'z'
    printf("Строчные буквы 'a'-'z':\n");
    printf("Коды: \n");
    for (char c = 'a'; c <= 'z'; c++) {
        printf("%c=%d\n", c, c);
    }
    printf("\nРазность между 'z' и 'a': %d\n\n", 'z' - 'a');
    
    // Анализ прописных букв 'A'-'Z'
    printf("Прописные буквы 'A'-'Z':\n");
    printf("Коды: \n");
    for (char c = 'A'; c <= 'Z'; c++) {
        printf("%c=%d\n", c, c);
    }
    printf("\nРазность между 'Z' и 'A': %d\n\n", 'Z' - 'A');
    
    // Сравнение диапазонов
    printf("Сравнение диапазонов:\n");
    printf("Пробел ' ' = %d\n", ' ');
    printf("'0' = %d, '9' = %d\n", '0', '9');
    printf("'A' = %d, 'Z' = %d\n", 'A', 'Z');
    printf("'a' = %d, 'z' = %d\n\n", 'a', 'z');
    
    printf("Относительное расположение:\n");
    printf("Пробел < цифры: %s\n", ' ' < '0' ? "да" : "нет");
    printf("Цифры < прописные: %s\n", '9' < 'A' ? "да" : "нет");
    printf("Прописные < строчные: %s\n", 'Z' < 'a' ? "да" : "нет");
    printf("=============================================\n\n");
}

// Функция для проверки операции sizeof (задача 11)
void test_sizeof() {
    printf("=== Проверка операции sizeof ===\n");
    printf("================================\n");
    
    // 1. Проверка всех арифметических типов
    printf("Размеры арифметических типов:\n");
    printf("char: %zu\n", sizeof(char));
    printf("signed char: %zu\n", sizeof(signed char));
    printf("unsigned char: %zu\n", sizeof(unsigned char));
    printf("short: %zu\n", sizeof(short));
    printf("unsigned short: %zu\n", sizeof(unsigned short));
    printf("int: %zu\n", sizeof(int));
    printf("unsigned int: %zu\n", sizeof(unsigned int));
    printf("long: %zu\n", sizeof(long));
    printf("unsigned long: %zu\n", sizeof(unsigned long));
    printf("long long: %zu\n", sizeof(long long));
    printf("unsigned long long: %zu\n", sizeof(unsigned long long));
    printf("float: %zu\n", sizeof(float));
    printf("double: %zu\n", sizeof(double));
    printf("long double: %zu\n\n", sizeof(long double));
    
    // 2. Проверка, что выражение не вычисляется
    printf("Проверка, что выражение в sizeof не вычисляется:\n");
    
    int x = 10;
    printf("До sizeof: x = %d\n", x);
    
    // Выражение x++ не должно вычисляться
    size_t size = sizeof(x++);
    printf("После sizeof(x++): x = %d (не изменилось)\n", x);
    printf("sizeof(x++) = %zu\n\n", size);
    
    printf("================================\n\n");
}

int main(void) {
    analyze_characters();
    test_sizeof();
    
    return 0;
}