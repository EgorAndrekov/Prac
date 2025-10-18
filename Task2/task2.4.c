#include <stdio.h>
#include <ctype.h>

double str2double(char str[]) {
    double num = 0.0, frac = 0.0;
    int sign = 1, i = 0, has_dot = 0, exp = 0, exp_sign = 1;
    int frac_digits = 0;
    
    while (isspace(str[i])) i++;
    
    if (str[i] == '-') { 
        sign = -1; 
        i++; 
    }
    else if (str[i] == '+') i++;
    
    while (str[i]) {
        if (str[i] == '.') { 
            has_dot = 1; 
            i++; 
            continue; 
        }
        
        if (str[i] == 'e' || str[i] == 'E') {
            i++;
            if (str[i] == '-') { 
                exp_sign = -1; 
                i++; 
            }
            else if (str[i] == '+') i++;
            
            while (isdigit(str[i])) { 
                exp = exp * 10 + (str[i] - '0'); 
                i++; 
            }
            break;
        }
        
        if (has_dot) {
            frac = frac * 10 + (str[i] - '0');
            frac_digits++;
        } else {
            num = num * 10 + (str[i] - '0');
        }
        i++;
    }
    
    double frac_value = frac;
    for (int j = 0; j < frac_digits; j++) {
        frac_value /= 10.0;
    }
    
    double result = num + frac_value;
    
    while (exp--) {
        result = (exp_sign > 0) ? result * 10.0 : result / 10.0;
    }
    
    return sign * result;
}

int main() {
    char str[100];
    while (scanf("%s", str) != EOF) {
        printf("%.10g\n", str2double(str));
    }
    return 0;
}