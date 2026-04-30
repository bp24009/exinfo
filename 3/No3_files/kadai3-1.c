/*** kadai3-1.c ***/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>		//mathライブラリを利用する場合はコンパイル時に -lm

#define MAX_REPEAT 1000		//コピー繰り返し回数
#define SIZE 500		//画像サイズ（行＝列）

#ifdef CSQ
#define DIM1 3
#define DIM2 SIZE
#define DIM3 SIZE
#endif
#ifdef CIP
#define DIM1 SIZE
#define DIM2 SIZE
#define DIM3 3
#endif

typedef unsigned char UCHAR;

void write_ppm_csq(UCHAR [][DIM2][DIM3],char *,int ,int);
void write_ppm_cip(UCHAR [][DIM2][DIM3],char *,int ,int);
void error1(char *);


/* 画像のコピー */
void copy3dimg(UCHAR res[][DIM2][DIM3],UCHAR org[][DIM2][DIM3],int dim1,int dim2,int dim3)
{
  int i,j,k;

  for(i=0;i<dim1;i++)
    for(j=0;j<dim2;j++)
      for(k=0;k<dim3;k++)
	res[i][j][k]=org[i][j][k];
}


  UCHAR org[DIM1][DIM2][DIM3];	/* 作成画像 */
  UCHAR res[DIM1][DIM2][DIM3];	/* コピー先画像 */


int main(void)
{
  int repeat;

				/* 画像の作成 */
  {
    int i,j,k;

    for(i=0;i<DIM1;i++)
      for(j=0;j<DIM2;j++)
	for(k=0;k<DIM3;k++)
	  org[i][j][k]=(UCHAR)((i*j+10*j+k*k)%256);
  }
     
				/* 画像のコピー */
  for(repeat=0;repeat<MAX_REPEAT;repeat++)
    copy3dimg(res,org,DIM1,DIM2,DIM3);

				/* ファイルへの出力 */
#ifdef CSQ
  write_ppm_csq(res,"./tmp.ppm",DIM3,DIM2);
#endif
#ifdef CIP
  write_ppm_cip(res,"./tmp.ppm",DIM2,DIM1);
#endif
  
  return 0;
}


/* ファイルへの出力(CSQ） */
void write_ppm_csq(UCHAR data_buf[][DIM2][DIM3],char *fname,int width,int height)
{
  FILE *fp;
  int m, n;
    
  if((fp = fopen(fname, "wb")) == NULL) {
    fprintf(stderr, "file(%s) can't open\n", fname) ;
    exit(1) ;
  }

  fprintf(fp, "P6\n") ;			/* カラー画像かつバイナリーデータの記号 */
  fprintf(fp, "%d %d\n", width, height) ; /* 画像の幅(列数)と高さ(行数) */
  fprintf(fp, "255\n") ;		/* 最大値 */

  for(m=0;m<height;m++)
    for(n=0;n<width;n++)
    {
      fwrite(&data_buf[0][m][n], sizeof(UCHAR), 1, fp);
      fwrite(&data_buf[1][m][n], sizeof(UCHAR), 1, fp);
      fwrite(&data_buf[2][m][n], sizeof(UCHAR), 1, fp);
    }

  fclose(fp) ;
}

/* ファイルへの出力(CIP） */
void write_ppm_cip(UCHAR data_buf[][DIM2][DIM3],char *fname,int width,int height)
{
  FILE *fp;
    
  if((fp = fopen(fname, "wb")) == NULL) {
    fprintf(stderr, "file(%s) can't open\n", fname) ;
    exit(1) ;
  }

  fprintf(fp, "P6\n") ;			/* カラー画像かつバイナリーデータの記号 */
  fprintf(fp, "%d %d\n", width, height) ; /* 画像の幅(列数)と高さ(行数) */
  fprintf(fp, "255\n") ;		/* 最大値 */

  fwrite(&data_buf[0][0][0], sizeof(UCHAR), width*height*3, fp);

  fclose(fp) ;
}

/* エラー処理 */
void error1(char *message)
{
  printf("%s\n",message);
  exit(1);
}
