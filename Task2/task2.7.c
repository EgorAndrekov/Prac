#include <stdio.h>
#include <setjmp.h>
#include <ctype.h>
#include <math.h>

jmp_buf begin;
char curlex;
int count_plus = 0;
int count_minus = 0;
int count_mult = 0;
int count_div = 0;
int count_pow = 0;

int maxx(int a, int b, int c, int d, int e){
    int temp = a;

    if(b > temp) temp = b;
    if(c > temp) temp = c;
    if(d > temp) temp = d;
    if(e > temp) temp = e;

    return temp;
}


void getlex(void) {
    while ((curlex = getchar()) == ' ');
}

void error(void) {
    printf("\nОШИБКА!\n");
    while (getchar() != '\n');
    longjmp(begin, 1);
}

int power(void);
int mult(void);
int add(void);
int expr(void);

int power(void) {
    int p = mult();
    if (curlex == '^') {
        count_pow ++;
        getlex();
        int exp = power();
        if (exp < 0) error();
        int result = 1;
        for (int i = 0; i < exp; i++) result *= p;
        return result;
    }
    return p;
}

int mult(void) {
    int m;
    switch (curlex) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            m = curlex - '0'; break;
        case '(':
            getlex();
            m = expr();
            if (curlex != ')') error();
            break;
        default: error();
    }
    getlex();
    return m;
}

int add(void) {
    int t = power();
    while (curlex == '*' || curlex == '/') {
        if (curlex == '*') {
            count_mult ++;
            getlex();
            t *= power();
        } else {
            count_div ++;
            getlex();
            int d = power();
            if (d == 0) error();
            t /= d;
        }
    }
    return t;
}

int expr(void) {
    int e = add();
    while (curlex == '+' || curlex == '-') {
        if (curlex == '+') {
            count_plus ++;
            getlex();
            e += add();
        } else {
            count_minus ++;
            getlex();
            e -= add();
        }
    }
    return e;
}

int main() {
    int result;
    setjmp(begin);
    printf("==> ");
    getlex();
    result = expr();
    if (curlex != '\n') error();
    printf("\n%d\n", result);
    if (maxx(count_plus, count_minus, count_mult, count_div, count_pow) == count_plus){
        printf("+ - самая популярная операция\n");
    }
    if (maxx(count_plus, count_minus, count_mult, count_div, count_pow) == count_minus){
        printf("- - самая популярная операция\n");
    }
    if (maxx(count_plus, count_minus, count_mult, count_div, count_pow) == count_mult){
        printf("* - самая популярная операция\n");
    }
    if (maxx(count_plus, count_minus, count_mult, count_div, count_pow) == count_div){
        printf("/ - самая популярная операция\n");
    }
    if (maxx(count_plus, count_minus, count_mult, count_div, count_pow) == count_pow){
        printf("^ - самая популярная операция\n");
    }
    return 0;
}