#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define N 288 /* frame dimension for QCIF format */
#define M 352 /* frame dimension for QCIF format */
#define filename "flower_cif_150_yuv444.yuv"
#define file_y "unsharp_y.yuv"
/* code for armulator*/
#pragma arm section zidata="ram"
int current_y[N][M];
int A[N+2][M+2];
int newA[N+2][M+2];
int i,j;
#pragma arm section

/*Me ton parakatw pinaka tha ginei to filtrarisma stin fotografia
 int blurfilter [3][3] = {
                                {1, 4, 1} ,
                                {4,-20,4} ,
                                {1, 4, 4}
                              };  */





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
	for(i=1;i<N+1;i++)
		for(j=1;j<M+1;j++){
		    newA[i][j]=(A[i][j] * (-20) + // kentro
                          A[i-1][j-1] *(1) +   //  panw aristera
                          A[i][j-1]   *(4) +   //  aristera
                          A[i+1][j-1] *(1) +   //  katw aristera
                          A[i-1][j]   *(4) +   //  kentro panw
                          A[i+1][j]   *(4) +   //  kentro katw
                          A[i-1][j+1] *(1) +   //  panw deksia
                          A[i][j+1]   *(4) +   //  deksia
                          A[i+1][j+1] *(1)  //  katw deksia
                          );
        
	  newA[i][j] = A[i][j] + (A[i][j] - newA[i][j]) ;
    }


/*Afairoume tis parapanw prosthetes grammes kai stiles kai epanaferoume ton pinaka ston arxiko tou megethos. Tautoxrona ginetai k h klimakwsh*/
	for(i=1;i<N+1;i++)
		for(j=1;j<M+1;j++)
			current_y[i-1][j-1]=newA[i][j];
			
for(i=0;i<N;i++)
	for(j=0;j<M;j++){
			  current_y[i][j] += 127;
        if (current_y[i][j] > 255) current_y[i][j] = 255;
        if (current_y[i][j] < 0) current_y[i][j] = 0;
      }
}


      

void write()
{
  FILE *frame_y;
  frame_y=fopen(file_y,"wb");

  for(i=0;i<N;i++)
    for(j=0;j<M;j++)
      fputc(current_y[i][j],frame_y);
  fclose(frame_y);

}

int main()
{
  read();
  filtering();
  write();
}
