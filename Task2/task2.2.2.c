#include <stdio.h>

int main(){

    double x;

    scanf("%lf", &x);

    double a, dif = 0.0;

    int i = 1;

    while(scanf("%lf", &a) != EOF){
        dif = dif*x + a;
    }

    printf("%.10g\n", dif);

    return 0;
    
}