#include <stdio.h>

void dtoa(double num, char s[])
{
    //int i = 0;
    int sign = 0; 
    int zap = 0;
    double n = num;

    if ((sign = num) < 0)
        num = -num;

    for ( ; (int)num; zap++) {

        printf("num == %f\n", num);
        num /= 10;
    }
    
    for (int i = zap; i; i--) {
        n *= 10;
    printf("N == %f\n", n);
    }




    // if(sign < 0)
    //     s[i++] = '-';

    // s[i] = '\0';

    // reverse(s);

    // for(i = 20; i > 0; i--){
    //     while(s[zap] != ','){
    //         s[i] = s[i - 1];
    //         s[i - 1] = ',';
    //     }
    // }
}

int main() {
    double n = 3466.34527;

    char *s;
    dtoa(n, s);
}
