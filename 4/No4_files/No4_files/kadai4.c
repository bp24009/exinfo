/*            kadai4.c                       */
/* Usage:  time ./a.out bairitu file.ppm     */
/* Output: file_gray.pgm, file_gaussian.pgm, */
/*         file_res.pgm                      */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char UCHAR;

				/* 画像サイズは変更不可 */
#define DIM1  960		/* 行数 */
#define DIM2 1280		/* 列数 */
#define DIM3    3

#define RED   0
#define GREEN 1
#define BLUE  2

#define WHITE 255
#define BLACK 0

#define ON 1
#define OFF 0

#define N_REPEAT 2000	        /* 処理の繰返し回数（消費時間測定時は必ず100） */

void grayscale(UCHAR [][DIM2], UCHAR [][DIM2][DIM3], int , int );
void gaussian_filter(UCHAR [][DIM2], UCHAR [][DIM2], int , int );
void unsharp(UCHAR [][DIM2], UCHAR [][DIM2], UCHAR [][DIM2], int, int , int );

void write_ppm_cip(UCHAR [][DIM2][DIM3],char *,int ,int );
void write_pgm_cip(UCHAR [][DIM2],char *,int ,int );
void read_ppm_cip(UCHAR [][DIM2][DIM3], char *);

  UCHAR org[DIM1][DIM2][DIM3];	/* 入力画像 */

  UCHAR gray_org[DIM1][DIM2];	/* 入力画像のグレースケール画像*/
  UCHAR gray2[DIM1][DIM2];	/* 平滑化後の画像 */
  UCHAR res[DIM1][DIM2];	/* 出力画像 */

int main(int argc, char *argv[])
{
  int i,g,r;
  int bairitu;       /* （原画像－平滑化画像）の値に掛ける整数（消費時間測定時は必ず5） */
  char in_fname[200];		/* 入力画像ファイル名 */
  char out_fname[300];		/* 出力画像ファイル名 */
  
  if(argc < 3)			/* 入力コマンドチェック */
  {
    printf("usage: a.out bairitu filename \n");
    exit(1);
  }
  else
  {
    strcpy(in_fname,argv[argc-1]); /* 入力画像のファイル名 */
    bairitu = atoi(argv[argc-2]);
  }

  read_ppm_cip(org, in_fname);	/* 画像をファイルより読み込み */


  for(g=0;g<DIM1;g++)		/* 平滑化用画像をWHITE/2に初期化(平滑化処理で外周部を処理しないため) */
    for(r=0;r<DIM2;r++)		/* この処理は高速化不要 */
    {
      gray2[g][r]=WHITE/2;
    }


				/* 時間測定のため必ず2000回行う */
  for(i=0;i<N_REPEAT;i++)
  {
    grayscale(gray_org, org, DIM1, DIM2); /* 入力画像(カラー)からグレースケール画像作成 */

 #ifndef REMOVE

    gaussian_filter(gray2, gray_org, DIM1, DIM2); /* 3×3のガウシアンフィルタによる平滑化 */

    unsharp(res, gray2, gray_org, bairitu, DIM1, DIM2); /* 結果画像作成 */

 #endif
  }  
				
  in_fname[strlen(in_fname)-4]='\0'; /* 入力画像ファイル名から.ppm除く */

  sprintf(out_fname,"%s_gray.pgm",in_fname); /* 原画像のグレースケール画像用出力画像ファイル名設定 */
  write_pgm_cip(gray_org,out_fname,DIM2,DIM1); /* 原画像のグレースケール画像をファイルへ出力 */

  sprintf(out_fname,"%s_gaussian.pgm",in_fname); /* 平滑化画像用出力画像ファイル名設定 */
  write_pgm_cip(gray2,out_fname,DIM2,DIM1); /* 平滑化画像をファイルへ出力 */

  sprintf(out_fname,"%s_res.pgm",in_fname); /* 結果画像ファイル名設定 */
  write_pgm_cip(res,out_fname,DIM2,DIM1); /* 結果画像をファイルへ出力 */

  return 0;
}

 /* カラー画像からグレースケール画像作成 */
void grayscale(UCHAR gray[][DIM2], UCHAR gen[][DIM2][DIM3], int n_gyou, int n_retu)
{
  int g,r,col;
  int tmp;

  for(g=0;g<n_gyou;g++)
    for(r=0;r<n_retu;r++)
    {
      tmp=0;
      for(col=0;col<DIM3;col++)
      {
	tmp += gen[g][r][col];
      }
      gray[g][r]=(UCHAR)(tmp/3.0+0.5); /* 平均値を四捨五入 */
    }
}

 /* 3×3のガウシアンフィルタによる平滑化 */
 /* 外周部は処理しない。 */
void gaussian_filter(UCHAR gray[][DIM2], UCHAR data[][DIM2], int n_gyou, int n_retu)
{
  int g,r,i,j;
  double keisuu[3][3] = { {1 / 16.0, 2 / 16.0, 1 / 16.0},
                   {2 / 16.0, 4 / 16.0, 2 / 16.0},
                   {1 / 16.0, 2 / 16.0, 1 / 16.0} };
  double tmp;

  for(g=1;g<n_gyou-1;g++)
    for(r=1;r<n_retu-1;r++)
    {
      tmp=0.0;
      for (i = -1; i <= 1; i++)
          for (j = -1; j <= 1; j++)
              tmp += keisuu[i + 1][j + 1] * (double)data[g + i][r + j];
      gray[g][r]=(UCHAR)(tmp+0.5); /* 四捨五入 */
    }
}

/* 平滑化画像と原画像を用いたアンシャープマスキング処理 */
void unsharp(UCHAR result[][DIM2], UCHAR smooth[][DIM2], UCHAR gen[][DIM2], int bairitu, int n_gyou, int n_retu)
{
  int g,r;
  int tmp;

  for(g=0;g<n_gyou;g++)
    for(r=0;r<n_retu;r++)
    {
      tmp= gen[g][r] + bairitu * (gen[g][r] - smooth[g][r]);

      if(tmp > WHITE)
	result[g][r]=WHITE;
      else if(tmp < BLACK)
	result[g][r]=BLACK;
      else
	result[g][r]=tmp;
    }
}



/* ここより下は変更不要 */

#define W_BYTE DIM1*DIM2*DIM3

/* CIP形式の画像データよりPPMファイル作成 */
void write_ppm_cip(UCHAR data_buf[][DIM2][DIM3],char *fname,int width,int height)
{
  FILE *fp;
  
				/* ファイルを開く */
  if((fp = fopen(fname, "wb")) == NULL) {
    fprintf(stderr, "file(%s) can't open\n", fname) ;
    exit(1) ;
  }

  fprintf(fp, "P6\n") ;		/* カラー画像かつバイナリーデータの記号 */
  fprintf(fp, "%d %d\n", width, height) ; /* 画像の幅(列数)と高さ(行数) */
  fprintf(fp, "255\n") ;	/* 最大値 */

  {				/* 画像データをrepeat(+1)個に分割して書き込み */
    int i;
    int repeat=(DIM1*DIM2*DIM3)/(W_BYTE); /* 分割数 */
    int rest=DIM1*DIM2*DIM3-repeat*(W_BYTE); /* 余りデータ量 */
    UCHAR *pt=(UCHAR *)&data_buf[0][0][0]; /* 書き込むデータの位置を持つポインタ */

    for(i=0;i<repeat;i++)
    {
      fwrite(pt, sizeof(UCHAR), W_BYTE, fp);
      pt += W_BYTE;
    }
    if(rest > 0)
      fwrite(pt, sizeof(UCHAR), rest, fp);
  }
  
  fclose(fp);			/* ファイルを閉じる */    
}


/* CIP形式の画像データよりPGMファイル作成 */
void write_pgm_cip(UCHAR data_buf[][DIM2],char *fname,int width,int height)
{
  FILE *fp;
  
				/* ファイルを開く */
  if((fp = fopen(fname, "wb")) == NULL) {
    fprintf(stderr, "file(%s) can't open\n", fname) ;
    exit(1) ;
  }

  fprintf(fp, "P5\n") ;		/* グレースケール画像かつバイナリーデータの記号 */
  fprintf(fp, "%d %d\n", width, height) ; /* 画像の幅(列数)と高さ(行数) */
  fprintf(fp, "255\n") ;	/* 最大値 */

  fwrite(&data_buf[0][0], sizeof(UCHAR), DIM1*DIM2, fp);
  
  fclose(fp);			/* ファイルを閉じる */    
}


#define R_BYTE DIM1*DIM2*DIM3

/* ppmフォーマットの画像ファイルをCIP形式の画像メモリに読み込み */
void read_ppm_cip(UCHAR data_buf[][DIM2][DIM3], char *fname)
{
  FILE	*fp ;
  char	str_buf[1024] ;
  char	magic_num[8] ;		/* マジックナンバー */
  int	max_val ;		/* 画素値の最大値 */    
  int width,height;
  int count_limit;
  void *k;
  long j;
				/* ファイルを開く */    
  if((fp = fopen(fname, "rb")) == NULL) {
    fprintf(stderr, "file(%s) can't open.\n", fname) ;
    exit(1) ;
  }
				/* マジックナンバー読み込み */
  count_limit=0;
  sprintf(str_buf,"#");
  while((str_buf[0]=='#') || (str_buf[0]=='\n') || (str_buf[0]=='\t') || (str_buf[0]==' '))
  {
    k=fgets(str_buf,1024,fp);
    count_limit++;
    if(count_limit > 1000)
    {
      fprintf(stderr,"ERROR: Irregal file format.\n");
      exit(1);
    }
  }
  strcpy(magic_num, str_buf);
  magic_num[strlen(magic_num)-1]='\0'; /* \nを除く */
  if(strcmp(magic_num, "P6") != 0) 
  {
    fprintf(stderr, "ERROR: magic number(%s) not match.\n", magic_num) ;
    exit(1) ;
  }

				/* 画像の幅(列数)と高さ(行数) */
  count_limit=0;
  sprintf(str_buf,"#");
  while((str_buf[0]=='#') || (str_buf[0]=='\n') || (str_buf[0]=='\t') || (str_buf[0]==' '))
  {
    k=fgets(str_buf,1024,fp);
    count_limit++;
    if(count_limit > 1000)
    {
      fprintf(stderr,"ERROR: Irregal file format.\n");
      exit(1);
    }
  }
  sscanf(str_buf,"%d %d",&width,&height);
  if((height != DIM1) || (width != DIM2)) /* 画像のサイズが仮定した値と異なる場合はエラーで終了 */
  {
    fprintf(stderr, "ERROR: Dimension dosenot match.\n");
    exit(1) ;
  }

				/* 最大値 */
  count_limit=0;
  sprintf(str_buf,"#");
  while((str_buf[0]=='#') || (str_buf[0]=='\n') || (str_buf[0]=='\t') || (str_buf[0]==' '))
  {
    k=fgets(str_buf,1024,fp);
    count_limit++;
    if(count_limit > 1000)
    {
      fprintf(stderr,"ERROR: Irregal file format.\n");
      exit(1);
    }
  }
  sscanf(str_buf,"%d",&max_val);
  if(max_val > 255)
  {
    fprintf(stderr, "ERROR: Irregal max value.\n");
    exit(1) ;
  }

				/* 画像データをrepeat(+1)個に分割して読み込み */
  {
    int i;
    int repeat=(DIM1*DIM2*DIM3)/(R_BYTE); /* 分割数 */
    int rest=DIM1*DIM2*DIM3-repeat*(R_BYTE); /* 余りデータ量 */
    UCHAR *pt=(UCHAR *)&data_buf[0][0][0]; /* 読み込みデータの位置を持つポインタ */

    for(i=0;i<repeat;i++)
    {
      j=fread(pt, sizeof(UCHAR), R_BYTE, fp);
      pt += R_BYTE;
    }
    if(rest > 0)
      j=fread(pt, sizeof(UCHAR), rest, fp);
  }

  /* ファイルを閉じる */    
  fclose(fp);
}
