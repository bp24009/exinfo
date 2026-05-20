/*             kadai4.c                       */
/* Usage:  time ./a.out bairitu file.ppm      */
/* Output: file_gray.pgm, file_gaussian.pgm, */
/*         file_res.pgm                      */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char UCHAR;

        /* 画像サイズは変更しない */
#define DIM1  960   /* 行数 */
#define DIM2 1280   /* 列数 */
#define DIM3    3

#define RED   0
#define GREEN 1
#define BLUE  2

#define WHITE 255
#define BLACK 0

#define ON 1
#define OFF 0

#define N_REPEAT 2000         /* 全体の反復回数（演習時間計測には必須2000回） */

void grayscale(UCHAR [][DIM2], UCHAR [][DIM2][DIM3], int , int );
void gaussian_filter(UCHAR [][DIM2], UCHAR [][DIM2], int , int );
void unsharp(UCHAR [][DIM2], UCHAR [][DIM2], UCHAR [][DIM2], int, int , int );

void write_ppm_cip(UCHAR [][DIM2][DIM3],char *,int ,int );
void write_pgm_cip(UCHAR [][DIM2],char *,int ,int );
void read_ppm_cip(UCHAR [][DIM2][DIM3], char *);

  UCHAR org[DIM1][DIM2][DIM3];  /* 入力画像 */

  UCHAR gray_org[DIM1][DIM2]; /* 入力画像のグレースケール画像*/
  UCHAR gray2[DIM1][DIM2];  /* 平滑化後画像 */
  UCHAR res[DIM1][DIM2];  /* 出力画像 */

int main(int argc, char *argv[])
{
  int i,g,r;
  int bairitu;       /* （原画像ー平滑化画像）の値に掛ける倍率（演習時間計測には必須10） */
  char in_fname[200];   /* 入力画像ファイル名 */
  char out_fname[300];    /* 出力画像ファイル名 */
  
  if(argc < 3)      /* 引数のコマンドチェック */
  {
    printf("usage: a.out bairitu filename \n");
    exit(1);
  }
  else
  {
    strcpy(in_fname,argv[argc-1]); /* 入力画像のファイル名 */
    bairitu = atoi(argv[argc-2]);
  }

  read_ppm_cip(org, in_fname);  /* 画像をファイルから読み込み */


  for(g=0;g<DIM1;g++)   /* 平滑化用画像をWHITE/2に初期化(境界部分は計算外なのでゴミが入らないように) */
    for(r=0;r<DIM2;r++)   /* 実際の処理は境界を除いた部分 */
    {
      gray2[g][r]=WHITE/2;
    }


        /* 時間計測のために必須2000回実行 */
  for(i=0;i<N_REPEAT;i++)
  {
    grayscale(gray_org, org, DIM1, DIM2); /* 入力画像(カラー)からグレースケール画像作成 */

 #ifndef REMOVE

    gaussian_filter(gray2, gray_org, DIM1, DIM2); /* 3×3のガウシアンフィルタによる平滑化 */

    unsharp(res, gray2, gray_org, bairitu, DIM1, DIM2); /* 鮮鋭化画像作成 */

 #endif
  }  
        
  in_fname[strlen(in_fname)-4]='\0'; /* 入力画像ファイル名から.ppmをとる */

  sprintf(out_fname,"%s_gray.pgm",in_fname); /* 原画像のグレースケール画像用出力画像ファイル名設定 */
  write_pgm_cip(gray_org,out_fname,DIM2,DIM1); /* 原画像のグレースケール画像をファイルへ出力 */

  sprintf(out_fname,"%s_gaussian.pgm",in_fname); /* 平滑化画像用出力画像ファイル名設定 */
  write_pgm_cip(gray2,out_fname,DIM2,DIM1); /* 平滑化画像をファイルへ出力 */

  sprintf(out_fname,"%s_res.pgm",in_fname); /* 鮮鋭化画像ファイル名設定 */
  write_pgm_cip(res,out_fname,DIM2,DIM1); /* 鮮鋭化画像をファイルへ出力 */

  return 0;
}

 /* カラー画像からグレースケール画像作成 */
void grayscale(UCHAR gray[][DIM2], UCHAR gen[][DIM2][DIM3], int n_gyou, int n_retu)
{
  int i;
  int total = n_gyou * n_retu;
  UCHAR *p_gray = &gray[0][0];
  UCHAR *p_gen = &gen[0][0][0];

  /* 浮動小数点計算を排除するためのルックアップテーブル(LUT) */
  static UCHAR lut[766];
  static int lut_init = 0;
  if (!lut_init) {
    for (i = 0; i <= 765; i++) {
      lut[i] = (UCHAR)(i / 3.0 + 0.5);
    }
    lut_init = 1;
  }

  int n = total / 4;
  int rem = total % 4;

  /* ループアンローリングによる高速化 */
  for (i = 0; i < n; i++) {
    p_gray[0] = lut[p_gen[0] + p_gen[1] + p_gen[2]];
    p_gray[1] = lut[p_gen[3] + p_gen[4] + p_gen[5]];
    p_gray[2] = lut[p_gen[6] + p_gen[7] + p_gen[8]];
    p_gray[3] = lut[p_gen[9] + p_gen[10] + p_gen[11]];
    p_gray += 4;
    p_gen += 12;
  }
  for (i = 0; i < rem; i++) {
    *p_gray++ = lut[p_gen[0] + p_gen[1] + p_gen[2]];
    p_gen += 3;
  }
}

 /* 3×3のガウシアンフィルタによる平滑化 */
 /* 境界部分は処理しない。 */
void gaussian_filter(UCHAR gray[][DIM2], UCHAR data[][DIM2], int n_gyou, int n_retu)
{
  int g, r;

  for (g = 1; g < n_gyou - 1; g++) {
    UCHAR *p_gray = &gray[g][1];
    UCHAR *p_data0 = &data[g - 1][0];
    UCHAR *p_data1 = &data[g][0];
    UCHAR *p_data2 = &data[g + 1][0];

    /* スライディングウィンドウを用いてメモリアクセスを削減 */
    int d00 = p_data0[0], d01 = p_data0[1];
    int d10 = p_data1[0], d11 = p_data1[1];
    int d20 = p_data2[0], d21 = p_data2[1];

    for (r = 1; r < n_retu - 1; r++) {
      int d02 = p_data0[2];
      int d12 = p_data1[2];
      int d22 = p_data2[2];

      /* doubleの掛け算と足し算を、整数のシフト演算と加算に置き換え */
      int tmp = d00 + (d01 << 1) + d02 +
                (d10 << 1) + (d11 << 2) + (d12 << 1) +
                d20 + (d21 << 1) + d22;

      /* 16で割る処理を、シフト演算(>> 4)で代用 (+0.5の四捨五入は +8で対応) */
      *p_gray++ = (UCHAR)((tmp + 8) >> 4);

      d00 = d01; d01 = d02;
      d10 = d11; d11 = d12;
      d20 = d21; d21 = d22;

      p_data0++; p_data1++; p_data2++;
    }
  }
}

/* 平滑化画像と原画像を使い、アンシャープマスキングを行う */
void unsharp(UCHAR result[][DIM2], UCHAR smooth[][DIM2], UCHAR gen[][DIM2], int bairitu, int n_gyou, int n_retu)
{
  int i;
  int total = n_gyou * n_retu;
  UCHAR *p_result = &result[0][0];
  UCHAR *p_smooth = &smooth[0][0];
  UCHAR *p_gen = &gen[0][0];

  /* 毎回のif文(分岐)をなくすためのクリッピング用ルックアップテーブル */
  static UCHAR lut[65536];
  static int lut_init = 0;
  if (!lut_init) {
    for (i = -32768; i <= 32767; i++) {
      int val = i;
      if (val > WHITE) val = WHITE;
      else if (val < BLACK) val = BLACK;
      lut[i + 32768] = (UCHAR)val;
    }
    lut_init = 1;
  }

  int n = total / 4;
  int rem = total % 4;

  for (i = 0; i < n; i++) {
    int g0 = p_gen[0], s0 = p_smooth[0];
    int g1 = p_gen[1], s1 = p_smooth[1];
    int g2 = p_gen[2], s2 = p_smooth[2];
    int g3 = p_gen[3], s3 = p_smooth[3];

    p_result[0] = lut[g0 + bairitu * (g0 - s0) + 32768];
    p_result[1] = lut[g1 + bairitu * (g1 - s1) + 32768];
    p_result[2] = lut[g2 + bairitu * (g2 - s2) + 32768];
    p_result[3] = lut[g3 + bairitu * (g3 - s3) + 32768];

    p_result += 4;
    p_gen += 4;
    p_smooth += 4;
  }

  for (i = 0; i < rem; i++) {
    int g0 = p_gen[0], s0 = p_smooth[0];
    p_result[0] = lut[g0 + bairitu * (g0 - s0) + 32768];
    p_result++;
    p_gen++;
    p_smooth++;
  }
}



/* 以下の入出力変数は変更しない */

#define W_BYTE DIM1*DIM2*DIM3

/* CIP形式の画像データをPPMファイルに作成 */
void write_ppm_cip(UCHAR data_buf[][DIM2][DIM3],char *fname,int width,int height)
{
  FILE *fp;
  
        /* ファイルを開く */
  if((fp = fopen(fname, "wb")) == NULL) {
    fprintf(stderr, "file(%s) can't open\n", fname) ;
    exit(1) ;
  }

  fprintf(fp, "P6\n") ;   /* カラー画像であるバイナリーデータの記述 */
  fprintf(fp, "%d %d\n", width, height) ; /* 画像の幅(列)と高さ(行数) */
  fprintf(fp, "255\n") ;  /* 最大値 */

  {       /* 画像データをrepeat(+1)回に分割して書き込む */
    int i;
    int repeat=(DIM1*DIM2*DIM3)/(W_BYTE); /* 分割数 */
    int rest=DIM1*DIM2*DIM3-repeat*(W_BYTE); /* 残りデータ数 */
    UCHAR *pt=(UCHAR *)&data_buf[0][0][0]; /* 書き込むデータの位置を示すポインタ */

    for(i=0;i<repeat;i++)
    {
      fwrite(pt, sizeof(UCHAR), W_BYTE, fp);
      pt += W_BYTE;
    }
    if(rest > 0)
      fwrite(pt, sizeof(UCHAR), rest, fp);
  }
  
  fclose(fp);     /* ファイルを閉じる */    
}


/* CIP形式の画像データをPGMファイルに作成 */
void write_pgm_cip(UCHAR data_buf[][DIM2],char *fname,int width,int height)
{
  FILE *fp;
  
        /* ファイルを開く */
  if((fp = fopen(fname, "wb")) == NULL) {
    fprintf(stderr, "file(%s) can't open\n", fname) ;
    exit(1) ;
  }

  fprintf(fp, "P5\n") ;   /* グレースケール画像であるバイナリーデータの記述 */
  fprintf(fp, "%d %d\n", width, height) ; /* 画像の幅(列)と高さ(行数) */
  fprintf(fp, "255\n") ;  /* 最大値 */

  fwrite(&data_buf[0][0], sizeof(UCHAR), DIM1*DIM2, fp);
  
  fclose(fp);     /* ファイルを閉じる */    
}


#define R_BYTE DIM1*DIM2*DIM3

/* ppmフォーマットの画像ファイルをCIP形式の画像配列に読み込む */
void read_ppm_cip(UCHAR data_buf[][DIM2][DIM3], char *fname)
{
  FILE  *fp ;
  char  str_buf[1024] ;
  char  magic_num[8] ;    /* マジック・ナンバー */
  int max_val ;   /* 各画素の最大値 */    
  int width,height;
  int count_limit;
  void *k;
  long j;
        /* ファイルを開く */    
  if((fp = fopen(fname, "rb")) == NULL) {
    fprintf(stderr, "file(%s) can't open.\n", fname) ;
    exit(1) ;
  }
        /* マジック・ナンバー読み込み */
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
  magic_num[strlen(magic_num)-1]='\0'; /* \nを削除 */
  if(strcmp(magic_num, "P6") != 0) 
  {
    fprintf(stderr, "ERROR: magic number(%s) not match.\n", magic_num) ;
    exit(1) ;
  }

        /* 画像の幅(列)と高さ(行数) */
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
  if((height != DIM1) || (width != DIM2)) /* 画像のサイズが指定した値と異なる場合はエラーで終了 */
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

        /* 画像データをrepeat(+1)回に分割して読み込む */
  {
    int i;
    int repeat=(DIM1*DIM2*DIM3)/(R_BYTE); /* 分割数 */
    int rest=DIM1*DIM2*DIM3-repeat*(R_BYTE); /* 残りデータ数 */
    UCHAR *pt=(UCHAR *)&data_buf[0][0][0]; /* 読み込むデータの位置を示すポインタ */

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