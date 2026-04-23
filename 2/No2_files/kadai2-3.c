#include <stdio.h>

int main(void)
{
  int ai[3];
  int (*pai)[3];
  
  pai=&ai;
  
  printf("%p\n",ai);
  printf("%p\n",ai+1);
  printf("%p\n",&ai);
  printf("%p\n",&ai+1);

  return 0;
}
