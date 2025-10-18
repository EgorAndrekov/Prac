#include <stdio.h>

long long fib_iter(int i) {
    if (i == 0) return 0;
    if (i == 1) return 1;
    long long a = 0, b = 1, c;
    for (int j = 2; j <= i; j++) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

long long fib_rec(int i) {
    if (i == 0) return 0;
    if (i == 1) return 1;
    return fib_rec(i - 1) + fib_rec(i - 2);
}

int main() {
    int i;
    while (scanf("%d", &i) != EOF) {
        printf("%lld\n", fib_iter(i));
        printf("%lld\n", fib_rec(i));
    }
    return 0;
}