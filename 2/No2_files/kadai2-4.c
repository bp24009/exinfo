#include <stdio.h>

int main(void)
{
    int aai[2][3]={{1,2,3},{4,5,6}};
    int *x;
    int (*y)[3];
    int (*z)[2][3];

    printf("%p\n",&aai[0][0]);
    printf("%p\n",aai[0]);
    printf("%p\n",&aai[0]);
    printf("%p\n",aai);
    printf("%p\n",&aai);

    x=&aai[0][0];
    x=aai[0];
    
    y=&aai[0];
    y=aai;

    z=&aai;
    
    return 0;
}

