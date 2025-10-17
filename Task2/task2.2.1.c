#include <stdio.h>

int main(){

    double x;

    scanf("%lf", &x);

    double a, res = 0.0, dif = 0.0;

    while(scanf("%lf", &a) != EOF){
        dif = dif*x + res;
        res = res*x + a;
    }

    printf("%.10g\n%.10g\n", res, dif);

    return 0;
    
}