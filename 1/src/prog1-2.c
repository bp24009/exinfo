#include <stdio.h>

int main(void)
{
  int n;
  short ai[4]={-9, 255, 2305, -32768};
  short *bi=(short *)((int *)((char *)ai-1)+1)-sizeof(ai)/sizeof(int)+2;

  for(n=-1;n<2;n++)
    printf("%d, %d\n",n,bi[n]);
    
  for (int j=0;j<sizeof(ai);j++){
    printf("ai[] %p %02x\n",((char *)&ai+j),*((char *)&ai+j));
    printf("\n");
  }
  printf("\n");

  for (int j=0;j<sizeof(bi);j++){
    printf("bi[] %p %02x\n",((char *)&bi+j),*((char *)&bi+j));
    printf("\n");
  }

  return 0;
}
