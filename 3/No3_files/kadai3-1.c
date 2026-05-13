/*** kadai3-1.c ***/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>		//math���C�u�����𗘗p����ꍇ�̓R���p�C������ -lm

#define MAX_REPEAT 10000		//�R�s�[�J��Ԃ���
#define SIZE 5		//�摜�T�C�Y�i�s����j

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


/* �摜�̃R�s�[ */
void copy3dimg(UCHAR res[][DIM2][DIM3],UCHAR org[][DIM2][DIM3],int dim1,int dim2,int dim3)
{
  int i,j,k;

  for(i=0;i<dim1;i++)
    for(j=0;j<dim2;j++)
      for(k=0;k<dim3;k++)
	res[i][j][k]=org[i][j][k];
}


  UCHAR org[DIM1][DIM2][DIM3];	/* �쐬�摜 */
  UCHAR res[DIM1][DIM2][DIM3];	/* �R�s�[��摜 */


int main(void)
{
  int repeat;

				/* �摜�̍쐬 */
  {
    int i,j,k;

    for(i=0;i<DIM1;i++)
      for(j=0;j<DIM2;j++)
	for(k=0;k<DIM3;k++)
	  org[i][j][k]=(UCHAR)((i*i+j*j+10*k*k)%256);
  }
     
				/* �摜�̃R�s�[ */
  for(repeat=0;repeat<MAX_REPEAT;repeat++)
    copy3dimg(res,org,DIM1,DIM2,DIM3);

				/* �t�@�C���ւ̏o�� */
#ifdef CSQ
  //write_ppm_csq(res,"./tmp.ppm",DIM3,DIM2);
#endif
#ifdef CIP
  //write_ppm_cip(res,"./tmp.ppm",DIM2,DIM1);
#endif
  
  return 0;
}


/* �t�@�C���ւ̏o��(CSQ�j */
void write_ppm_csq(UCHAR data_buf[][DIM2][DIM3],char *fname,int width,int height)
{
  FILE *fp;
  int m, n;
    
  if((fp = fopen(fname, "wb")) == NULL) {
    fprintf(stderr, "file(%s) can't open\n", fname) ;
    exit(1) ;
  }

  fprintf(fp, "P6\n") ;			/* �J���[�摜���o�C�i���[�f�[�^�̋L�� */
  fprintf(fp, "%d %d\n", width, height) ; /* �摜�̕�(��)�ƍ���(�s��) */
  fprintf(fp, "255\n") ;		/* �ő�l */

  for(m=0;m<height;m++)
    for(n=0;n<width;n++)
    {
      fwrite(&data_buf[0][m][n], sizeof(UCHAR), 1, fp);
      fwrite(&data_buf[1][m][n], sizeof(UCHAR), 1, fp);
      fwrite(&data_buf[2][m][n], sizeof(UCHAR), 1, fp);
    }

  fclose(fp) ;
}

/* �t�@�C���ւ̏o��(CIP�j */
void write_ppm_cip(UCHAR data_buf[][DIM2][DIM3],char *fname,int width,int height)
{
  FILE *fp;
    
  if((fp = fopen(fname, "wb")) == NULL) {
    fprintf(stderr, "file(%s) can't open\n", fname) ;
    exit(1) ;
  }

  fprintf(fp, "P6\n") ;			/* �J���[�摜���o�C�i���[�f�[�^�̋L�� */
  fprintf(fp, "%d %d\n", width, height) ; /* �摜�̕�(��)�ƍ���(�s��) */
  fprintf(fp, "255\n") ;		/* �ő�l */

  fwrite(&data_buf[0][0][0], sizeof(UCHAR), width*height*3, fp);

  fclose(fp) ;
}

/* �G���[���� */
void error1(char *message)
{
  printf("%s\n",message);
  exit(1);
}
