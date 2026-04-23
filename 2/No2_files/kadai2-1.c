#include <stdio.h>
#include <stdlib.h>

char c;

void func1(short);
void func2(short *);
void func3(int **);

int main(void)
{
  int *pi1;
  int *pi2;
  short s1=3;    //(1)static short

  printf("&c=%p\n",&c);
  printf("&pi1=%p\n",&pi1);
  printf("&pi2=%p\n",&pi2);
  printf("&s1=%p\n\n",&s1);

  func1(s1);
  func2(&s1);

  pi1=(int *)malloc(sizeof(int));
  printf("*pi1(Before init.)=%d\n",*pi1);
  *pi1=1;
  printf("*pi1=%d\n",*pi1);
  printf("pi1=%p\n",pi1);
  printf("&pi1=%p\n\n",&pi1);

  //  free(pi1);

  func3(&pi2);
  printf("pi2=%p\n\n",pi2);
  printf("*pi2=%d\n\n",*pi2);

  free(pi1);

  free(pi2);

  return 0;
}


void func1(short s2)
{
  printf("s2=%d\n",s2);
  printf("&s2=%p\n\n",&s2);
}

void func2(short *ps)
{
  *ps *= (*ps);
  printf("*ps=%d\n",*ps);
  printf("ps=%p\n",ps);
  printf("&ps=%p\n\n",&ps);
}

void func3(int **ppi)
{
  *ppi=(int *)malloc(sizeof(int));
  printf("**ppi(Before init.)=%d\n",**ppi);
  **ppi=-3;
  printf("**ppi=%d\n",**ppi);
  printf("*ppi=%p\n",*ppi);
  printf("ppi=%p\n",ppi);
  printf("&ppi=%p\n\n",&ppi);
}