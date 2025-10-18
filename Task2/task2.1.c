#include <stdio.h>

double modul(double a, double b){
    if (a >= b){
        return a - b;
    }

    return b - a;
}

int main(){
    double eps;
    scanf("%lf", &eps);

    double x;
    while(scanf("%lf", &x) != EOF){
        if (x == 0.0){
            printf("%.10g\n", x);
            continue;
        }
        double xi = 1.0;
        double xii = 0.5*(xi + x/xi);
        printf("%.10g %.10g\n",xi, xii);
        while(modul(xi,xii) >= eps){
            printf("%.10g %.10g\n",xi, xii);
            xi = xii;
            xii = 0.5*(xi + x/xi);
        }
        printf("%.10g\n", xii);
    }

    return 0;
    
}