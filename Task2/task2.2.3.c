#include <stdio.h>


int main() {
    int n;
    double x, a;
    scanf("%d %lf", &n, &x);   
    
    double integral = 0.0;
    

    for (int i = n; i >= 0; i--) {
        scanf("%lf", &a);
        integral = integral * x + a / (i + 1);
    }
    
    integral *= x; 
    
    printf("%.10g\n", integral);
    return 0;
}