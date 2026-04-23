#include <stdio.h>

struct kouzou1{
  char a[2];
  unsigned char b;
  int c;
};

struct kouzou2{
  char a[2];
  int c;
  unsigned char b;
};

int main(void)
{
  char name1[]="sitsit";
  struct kouzou1 d={{-9, 9}, 255, -1};
  struct kouzou2 e={{'p', '0'}, -2147483648, 128};
  char *f=&d.a[1];
  char *name2="hoge";

  
  {
    int j;

    for(j=0;j<sizeof(name1);j++)
      printf("name1[] %p %02x\n",((char *)&name1+j),*((char *)&name1+j));
    printf("\n");
  }

  {
    int j;

    for(j=0;j<sizeof(e.a);j++)
      printf("e.a %p %02x\n",((char *)&e.a+j),*((char *)&e.a+j));
    printf("\n");
  }

  /* 他の変数の出力も追加する */
  {
    int j;

    for(j=0;j<sizeof(e.c);j++)
      printf("e.c %p %02x\n",((char *)&e.c+j),*((char *)&e.c+j));
    printf("\n");
  }

  {
    int j;

    for(j=0;j<sizeof(e.b);j++)
      printf("e.b %p %02x\n",((char *)&e.b+j),*((char *)&e.b+j));
    printf("\n");
  }

  {
    int j;

    for(j=0;j<sizeof(d.a);j++){
      printf("d.a %p %02x\n",((char *)&d.a+j),*((char *)&d.a+j));
    }
    printf("\n");
  }

  {
    int j;

    for(j=0;j<sizeof(d.b);j++){
      printf("d.b %p %02x\n",((char *)&d.b+j),*((char *)&d.b+j));
    }
    printf("\n");
  }

  {
    int j;

    for(j=0;j<sizeof(d.c);j++){
      printf("d.c %p %02x\n",((char *)&d.c+j),*((char *)&d.c+j));
    }
    printf("\n");
  }

  {
    int j;

    for(j=0;j<sizeof(f);j++){
      printf("f %p %02x\n",((char *)&f+j),*((char *)&f+j));
    }
    printf("\n");
  }


  {
    int j;
  
    for(j=0;j<sizeof(name2);j++){
      printf("name2 %p %02x\n",((char *)&name2+j),*((char *)&name2+j));
    }
    printf("\n");
  }

    return 0;
}