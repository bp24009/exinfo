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
  int limit = total / 4;  
  int rem = total % 4;    

  UCHAR *pgray = &gray[0][0];
  UCHAR *pgen = &gen[0][0][0];


  for(i = 0; i < limit; i++) {
    pgray[0] = (UCHAR)((pgen[0] + pgen[1] + pgen[2]) / 3.0 + 0.5);
    pgray[1] = (UCHAR)((pgen[3] + pgen[4] + pgen[5]) / 3.0 + 0.5);
    pgray[2] = (UCHAR)((pgen[6] + pgen[7] + pgen[8]) / 3.0 + 0.5);
    pgray[3] = (UCHAR)((pgen[9] + pgen[10] + pgen[11]) / 3.0 + 0.5);

    pgray += 4;
    pgen += 12;
  }
  

  for(i = 0; i < rem; i++) {
    *pgray = (UCHAR)((pgen[0] + pgen[1] + pgen[2]) / 3.0 + 0.5);
    pgray++;
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
  
  int width = n_retu - 2;
  int limit = width / 4;
  int rem = width % 4;

  for(g = 1; g < n_gyou - 1; g++) {
    UCHAR *pgrey = &gray[g][1];
    UCHAR *pdata0 = &data[g - 1][0];
    UCHAR *pdata1 = &data[g][0];
    UCHAR *pdata2 = &data[g + 1][0];

    for(r = 0; r < limit; r++) {
      double t0 = 0.0, t1 = 0.0, t2 = 0.0, t3 = 0.0;

      t0 += keisuu[0][0]*pdata0[0] + keisuu[0][1]*pdata0[1] + keisuu[0][2]*pdata0[2];
      t0 += keisuu[1][0]*pdata1[0] + keisuu[1][1]*pdata1[1] + keisuu[1][2]*pdata1[2];
      t0 += keisuu[2][0]*pdata2[0] + keisuu[2][1]*pdata2[1] + keisuu[2][2]*pdata2[2];

      t1 += keisuu[0][0]*pdata0[1] + keisuu[0][1]*pdata0[2] + keisuu[0][2]*pdata0[3];
      t1 += keisuu[1][0]*pdata1[1] + keisuu[1][1]*pdata1[2] + keisuu[1][2]*pdata1[3];
      t1 += keisuu[2][0]*pdata2[1] + keisuu[2][1]*pdata2[2] + keisuu[2][2]*pdata2[3];

      t2 += keisuu[0][0]*pdata0[2] + keisuu[0][1]*pdata0[3] + keisuu[0][2]*pdata0[4];
      t2 += keisuu[1][0]*pdata1[2] + keisuu[1][1]*pdata1[3] + keisuu[1][2]*pdata1[4];
      t2 += keisuu[2][0]*pdata2[2] + keisuu[2][1]*pdata2[3] + keisuu[2][2]*pdata2[4];

      t3 += keisuu[0][0]*pdata0[3] + keisuu[0][1]*pdata0[4] + keisuu[0][2]*pdata0[5];
      t3 += keisuu[1][0]*pdata1[3] + keisuu[1][1]*pdata1[4] + keisuu[1][2]*pdata1[5];
      t3 += keisuu[2][0]*pdata2[3] + keisuu[2][1]*pdata2[4] + keisuu[2][2]*pdata2[5];

      pgrey[0] = (UCHAR)(t0 + 0.5);
      pgrey[1] = (UCHAR)(t1 + 0.5);
      pgrey[2] = (UCHAR)(t2 + 0.5);
      pgrey[3] = (UCHAR)(t3 + 0.5);

      pgrey += 4;
      pdata0 += 4;
      pdata1 += 4;
      pdata2 += 4;
    }

    for(r = 0; r < rem; r++) {
      double tmp = 0.0;
      tmp += keisuu[0][0]*pdata0[0] + keisuu[0][1]*pdata0[1] + keisuu[0][2]*pdata0[2];
      tmp += keisuu[1][0]*pdata1[0] + keisuu[1][1]*pdata1[1] + keisuu[1][2]*pdata1[2];
      tmp += keisuu[2][0]*pdata2[0] + keisuu[2][1]*pdata2[1] + keisuu[2][2]*pdata2[2];
      
      *pgrey = (UCHAR)(tmp + 0.5);
      pgrey++; pdata0++; pdata1++; pdata2++;
    }
  }
}

/* �������摜�ƌ��摜��p�����A���V���[�v�}�X�L���O���� */
void unsharp(UCHAR result[][DIM2], UCHAR smooth[][DIM2], UCHAR gen[][DIM2], int bairitu, int n_gyou, int n_retu)
{
  int i;
  int total = n_gyou * n_retu;
  int limit = total / 4;
  int rem = total % 4;

  UCHAR *presult = &result[0][0];
  UCHAR *psmooth = &smooth[0][0];
  UCHAR *pgen = &gen[0][0];

  for(i = 0; i < limit; i++) {
    int tmp0 = pgen[0] + bairitu * (pgen[0] - psmooth[0]);
    int tmp1 = pgen[1] + bairitu * (pgen[1] - psmooth[1]);
    int tmp2 = pgen[2] + bairitu * (pgen[2] - psmooth[2]);
    int tmp3 = pgen[3] + bairitu * (pgen[3] - psmooth[3]);

    if(tmp0 > WHITE) presult[0] = WHITE;
    else if(tmp0 < BLACK) presult[0] = BLACK;
    else presult[0] = tmp0;

    if(tmp1 > WHITE) presult[1] = WHITE;
    else if(tmp1 < BLACK) presult[1] = BLACK;
    else presult[1] = tmp1;

    if(tmp2 > WHITE) presult[2] = WHITE;
    else if(tmp2 < BLACK) presult[2] = BLACK;
    else presult[2] = tmp2;

    if(tmp3 > WHITE) presult[3] = WHITE;
    else if(tmp3 < BLACK) presult[3] = BLACK;
    else presult[3] = tmp3;

    presult += 4;
    psmooth += 4;
    pgen += 4;
  }

  for(i = 0; i < rem; i++) {
    int tmp = *pgen + bairitu * (*pgen - *psmooth);
    
    if(tmp > WHITE) *presult = WHITE;
    else if(tmp < BLACK) *presult = BLACK;
    else *presult = tmp;

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
