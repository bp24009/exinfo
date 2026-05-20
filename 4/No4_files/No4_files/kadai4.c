/*            kadai4.c                       */
/* Usage:  time ./a.out bairitu file.ppm     */
/* Output: file_gray.pgm, file_gaussian.pgm, */
/*         file_res.pgm                      */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char UCHAR;

				/* �摜�T�C�Y�͕ύX�s�� */
#define DIM1  960		/* �s�� */
#define DIM2 1280		/* �� */
#define DIM3    3

#define RED   0
#define GREEN 1
#define BLUE  2

#define WHITE 255
#define BLACK 0

#define ON 1
#define OFF 0

#define N_REPEAT 2000	        /* �����̌J�Ԃ��񐔁i����ԑ��莞�͕K��100�j */

void grayscale(UCHAR [][DIM2], UCHAR [][DIM2][DIM3], int , int );
void gaussian_filter(UCHAR [][DIM2], UCHAR [][DIM2], int , int );
void unsharp(UCHAR [][DIM2], UCHAR [][DIM2], UCHAR [][DIM2], int, int , int );

void write_ppm_cip(UCHAR [][DIM2][DIM3],char *,int ,int );
void write_pgm_cip(UCHAR [][DIM2],char *,int ,int );
void read_ppm_cip(UCHAR [][DIM2][DIM3], char *);

  UCHAR org[DIM1][DIM2][DIM3];	/* ���͉摜 */

  UCHAR gray_org[DIM1][DIM2];	/* ���͉摜�̃O���[�X�P�[���摜*/
  UCHAR gray2[DIM1][DIM2];	/* ��������̉摜 */
  UCHAR res[DIM1][DIM2];	/* �o�͉摜 */

int main(int argc, char *argv[])
{
  int i,g,r;
  int bairitu;       /* �i���摜�|�������摜�j�̒l�Ɋ|���鐮���i����ԑ��莞�͕K��5�j */
  char in_fname[200];		/* ���͉摜�t�@�C���� */
  char out_fname[300];		/* �o�͉摜�t�@�C���� */
  
  if(argc < 3)			/* ���̓R�}���h�`�F�b�N */
  {
    printf("usage: a.out bairitu filename \n");
    exit(1);
  }
  else
  {
    strcpy(in_fname,argv[argc-1]); /* ���͉摜�̃t�@�C���� */
    bairitu = atoi(argv[argc-2]);
  }

  read_ppm_cip(org, in_fname);	/* �摜���t�@�C�����ǂݍ��� */


  for(g=0;g<DIM1;g++)		/* �������p�摜��WHITE/2�ɏ�����(�����������ŊO�������������Ȃ�����) */
    for(r=0;r<DIM2;r++)		/* ���̏����͍������s�v */
    {
      gray2[g][r]=WHITE/2;
    }


				/* ���ԑ���̂��ߕK��2000��s�� */
  for(i=0;i<N_REPEAT;i++)
  {
    grayscale(gray_org, org, DIM1, DIM2); /* ���͉摜(�J���[)����O���[�X�P�[���摜�쐬 */

 #ifndef REMOVE

    gaussian_filter(gray2, gray_org, DIM1, DIM2); /* 3�~3�̃K�E�V�A���t�B���^�ɂ�镽���� */

    unsharp(res, gray2, gray_org, bairitu, DIM1, DIM2); /* ���ʉ摜�쐬 */

 #endif
  }  
				
  in_fname[strlen(in_fname)-4]='\0'; /* ���͉摜�t�@�C��������.ppm���� */

  sprintf(out_fname,"%s_gray.pgm",in_fname); /* ���摜�̃O���[�X�P�[���摜�p�o�͉摜�t�@�C�����ݒ� */
  write_pgm_cip(gray_org,out_fname,DIM2,DIM1); /* ���摜�̃O���[�X�P�[���摜���t�@�C���֏o�� */

  sprintf(out_fname,"%s_gaussian.pgm",in_fname); /* �������摜�p�o�͉摜�t�@�C�����ݒ� */
  write_pgm_cip(gray2,out_fname,DIM2,DIM1); /* �������摜���t�@�C���֏o�� */

  sprintf(out_fname,"%s_res.pgm",in_fname); /* ���ʉ摜�t�@�C�����ݒ� */
  write_pgm_cip(res,out_fname,DIM2,DIM1); /* ���ʉ摜���t�@�C���֏o�� */

  return 0;
}

 /* �J���[�摜����O���[�X�P�[���摜�쐬 */
void grayscale(UCHAR gray[][DIM2], UCHAR gen[][DIM2][DIM3], int n_gyou, int n_retu)
{
  int i;
  int total = n_gyou * n_retu;

  UCHAR *pgrey = &gray[0][0];
  UCHAR *pgen = &gen[0][0][0];

  for(i = 0; i < total; i++) {
    *pgrey = (UCHAR)((pgen[0] + pgen[1] + pgen[2]) / 3.0 + 0.5);
    pgrey++;
    pgen += 3;
  }
}

 /* 3�~3�̃K�E�V�A���t�B���^�ɂ�镽���� */
 /* �O�����͏������Ȃ��B */
void gaussian_filter(UCHAR gray[][DIM2], UCHAR data[][DIM2], int n_gyou, int n_retu)
{
  int g, r;
  double keisuu[3][3] = { {1 / 16.0, 2 / 16.0, 1 / 16.0},
                          {2 / 16.0, 4 / 16.0, 2 / 16.0},
                          {1 / 16.0, 2 / 16.0, 1 / 16.0} };

  for(g = 1; g < n_gyou - 1; g++) {
    UCHAR *pgrey = &gray[g][1];
    UCHAR *pdata0 = &data[g - 1][0];
    UCHAR *pdata1 = &data[g][0];
    UCHAR *pdata2 = &data[g + 1][0];

    double d00 = pdata0[0], d01 = pdata0[1];
    double d10 = pdata1[0], d11 = pdata1[1];
    double d20 = pdata2[0], d21 = pdata2[1];

    for(r = 1; r < n_retu - 1; r++) {
      double d02 = pdata0[2];
      double d12 = pdata1[2];
      double d22 = pdata2[2];

      double tmp = keisuu[0][0]*d00 + keisuu[0][1]*d01 + keisuu[0][2]*d02 + 
                   keisuu[1][0]*d10 + keisuu[1][1]*d11 + keisuu[1][2]*d12 + 
                   keisuu[2][0]*d20 + keisuu[2][1]*d21 + keisuu[2][2]*d22;
      
      *pgrey = (UCHAR)(tmp + 0.5);
      
      d00 = d01; d01 = d02;
      d10 = d11; d11 = d12;
      d20 = d21; d21 = d22;

      pgrey++; pdata0++; pdata1++; pdata2++;
    }
  }
}

/* �������摜�ƌ��摜��p�����A���V���[�v�}�X�L���O���� */
void unsharp(UCHAR result[][DIM2], UCHAR smooth[][DIM2], UCHAR gen[][DIM2], int bairitu, int n_gyou, int n_retu)
{
  int i;
  int total = n_gyou * n_retu;

  UCHAR *presult = &result[0][0];
  UCHAR *psmooth = &smooth[0][0];
  UCHAR *pgen = &gen[0][0];

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

  for(i = 0; i < total; i++) {
    *presult = lut[*pgen + bairitu * (*pgen - *psmooth) + 32768];
    presult++;
    psmooth++;
    pgen++;
  }
}



/* ������艺�͕ύX�s�v */

#define W_BYTE DIM1*DIM2*DIM3

/* CIP�`���̉摜�f�[�^���PPM�t�@�C���쐬 */
void write_ppm_cip(UCHAR data_buf[][DIM2][DIM3],char *fname,int width,int height)
{
  FILE *fp;
  
				/* �t�@�C�����J�� */
  if((fp = fopen(fname, "wb")) == NULL) {
    fprintf(stderr, "file(%s) can't open\n", fname) ;
    exit(1) ;
  }

  fprintf(fp, "P6\n") ;		/* �J���[�摜���o�C�i���[�f�[�^�̋L�� */
  fprintf(fp, "%d %d\n", width, height) ; /* �摜�̕�(��)�ƍ���(�s��) */
  fprintf(fp, "255\n") ;	/* �ő�l */

  {				/* �摜�f�[�^��repeat(+1)�ɕ������ď������� */
    int i;
    int repeat=(DIM1*DIM2*DIM3)/(W_BYTE); /* ������ */
    int rest=DIM1*DIM2*DIM3-repeat*(W_BYTE); /* �]��f�[�^�� */
    UCHAR *pt=(UCHAR *)&data_buf[0][0][0]; /* �������ރf�[�^�̈ʒu�����|�C���^ */

    for(i=0;i<repeat;i++)
    {
      fwrite(pt, sizeof(UCHAR), W_BYTE, fp);
      pt += W_BYTE;
    }
    if(rest > 0)
      fwrite(pt, sizeof(UCHAR), rest, fp);
  }
  
  fclose(fp);			/* �t�@�C������� */    
}


/* CIP�`���̉摜�f�[�^���PGM�t�@�C���쐬 */
void write_pgm_cip(UCHAR data_buf[][DIM2],char *fname,int width,int height)
{
  FILE *fp;
  
				/* �t�@�C�����J�� */
  if((fp = fopen(fname, "wb")) == NULL) {
    fprintf(stderr, "file(%s) can't open\n", fname) ;
    exit(1) ;
  }

  fprintf(fp, "P5\n") ;		/* �O���[�X�P�[���摜���o�C�i���[�f�[�^�̋L�� */
  fprintf(fp, "%d %d\n", width, height) ; /* �摜�̕�(��)�ƍ���(�s��) */
  fprintf(fp, "255\n") ;	/* �ő�l */

  fwrite(&data_buf[0][0], sizeof(UCHAR), DIM1*DIM2, fp);
  
  fclose(fp);			/* �t�@�C������� */    
}


#define R_BYTE DIM1*DIM2*DIM3

/* ppm�t�H�[�}�b�g�̉摜�t�@�C����CIP�`���̉摜�������ɓǂݍ��� */
void read_ppm_cip(UCHAR data_buf[][DIM2][DIM3], char *fname)
{
  FILE	*fp ;
  char	str_buf[1024] ;
  char	magic_num[8] ;		/* �}�W�b�N�i���o�[ */
  int	max_val ;		/* ��f�l�̍ő�l */    
  int width,height;
  int count_limit;
  void *k;
  long j;
				/* �t�@�C�����J�� */    
  if((fp = fopen(fname, "rb")) == NULL) {
    fprintf(stderr, "file(%s) can't open.\n", fname) ;
    exit(1) ;
  }
				/* �}�W�b�N�i���o�[�ǂݍ��� */
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
  magic_num[strlen(magic_num)-1]='\0'; /* \n������ */
  if(strcmp(magic_num, "P6") != 0) 
  {
    fprintf(stderr, "ERROR: magic number(%s) not match.\n", magic_num) ;
    exit(1) ;
  }

				/* �摜�̕�(��)�ƍ���(�s��) */
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
  if((height != DIM1) || (width != DIM2)) /* �摜�̃T�C�Y�����肵���l�ƈقȂ�ꍇ�̓G���[�ŏI�� */
  {
    fprintf(stderr, "ERROR: Dimension dosenot match.\n");
    exit(1) ;
  }

				/* �ő�l */
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

				/* �摜�f�[�^��repeat(+1)�ɕ������ēǂݍ��� */
  {
    int i;
    int repeat=(DIM1*DIM2*DIM3)/(R_BYTE); /* ������ */
    int rest=DIM1*DIM2*DIM3-repeat*(R_BYTE); /* �]��f�[�^�� */
    UCHAR *pt=(UCHAR *)&data_buf[0][0][0]; /* �ǂݍ��݃f�[�^�̈ʒu�����|�C���^ */

    for(i=0;i<repeat;i++)
    {
      j=fread(pt, sizeof(UCHAR), R_BYTE, fp);
      pt += R_BYTE;
    }
    if(rest > 0)
      j=fread(pt, sizeof(UCHAR), rest, fp);
  }

  /* �t�@�C������� */    
  fclose(fp);
}
