#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define N 288 /* frame dimension for QCIF format */
#define M 352 /* frame dimension for QCIF format */
#define filename "flower_cif_150_yuv444.yuv"
#define file_y "flower_cif_150_yuv444y.yuv"
/* code for armulator*/

int current_y[N][M];
int A[N+2][M+2];
int newA[N+2][M+2];
#pragma arm section zidata="sram1"
int block[3][3];
#pragma arm section

#pragma arm section zidata="sram2"
int buffer1[M+2];
int buffer2[M+2];
int buffer3[M+2];
#pragma arm section

/*Me ton parakatw pinaka tha ginei to filtrarisma stin fotografia
 int Laplacianfilter [3][3] = {
                                {0, 1, 0} ,
                                {1,-4, 1} ,
                                {0, 1, 0}
                              };  */

int i,j,k;



void read()
{
  FILE *frame_c;
  if((frame_c=fopen(filename,"rb"))==NULL)
  {
    printf("current frame doesn't exist\n");
    exit(-1);
  }

  for(i=0;i<N;i++)
  {
    for(j=0;j<M;j++)
    {
      current_y[i][j]=fgetc(frame_c);
    }
  }
  fclose(frame_c);
}


void filtering(){

/*Arxikopoioume enan pinaka me dio parapanw grammes kai stiles etsi wste na min prokipsei mikroteros pinakas(apo ta M,N) meta to convolution*/
	for(i=0;i<N+2;i++)
		for(j=0;j<M+2;j++)
			A[i][j]=0;

/*Oi dio extra grammes kai stiles periexoun tin timi 0, oi ypoloipes theseis tou pinaka, dexontai times apo ton pinaka y*/
	for(i=1;i<N+1;i++)
		for(j=1;j<M+1;j++)
			A[i][j]=current_y[i-1][j-1];

/*Akolouthei diadikasia filtrarismatos me vasi ton pinaka pou anaferthike parapanw*/
	for(i=1;i<N+1;i++){
		for(j=1;j<M+1;j++){
		if(i==1){
			for(k=0;k<M+2;k++){
				buffer1[k]=A[0][k];
				buffer2[k]=A[1][k];
				buffer3[k]=A[2][k];
			}
		}else{
			
				buffer1[j-1]=buffer2[j];
				buffer2[j]=buffer3[j+1];
				buffer3[j+1]=A[i+1][j+2];
			for(k=0;k<3;k++){
			block[k+i-1][j-1]=buffer1[j-1];
			block[k+i-1][j]=buffer2[j];
			block[k+i-1][j+1]=buffer3[j+1];
		}
	}
	
		
		newA[i][j]=(block[i][j]   *(-4) + // kentro
                        	block[i-1][j-1] *(0) +   //  panw aristera
                          	block[i][j-1]   *(1) +   //  aristera
                          	block[i+1][j-1] *(0) +   //  katw aristera
                          	block[i-1][j]   *(1) +   //  kentro panw
                          	block[i+1][j]   *(1) +   // kentro katw
                          	block[i-1][j+1] *(0) +   //  panw deksia
                          	block[i][j+1]   *(1) +   //  deksia
                          	block[i+1][j+1] *(0)  //  katw deksia
                          );
	  current_y[i-1][j-1]=newA[i][j] + 127;
      if (current_y[i-1][j-1] > 255) current_y[i-1][j-1] = 255;
      if (current_y[i-1][j-1] < 0) current_y[i-1][j-1] = 0;
		}
	}
}


void write()
{
  FILE *frame_y;
  frame_y=fopen(file_y,"wb");

  for(i=0;i<N;i++)
  {
    for(j=0;j<M;j++)
    {
      fputc(current_y[i][j],frame_y);
    }
  }
  fclose(frame_y);
  
}

int main()
{
  read();
  filtering();
  write();
}
