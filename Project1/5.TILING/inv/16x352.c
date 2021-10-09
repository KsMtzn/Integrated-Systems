#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define N 288 /* frame dimension for QCIF format */
#define M 352 /* frame dimension for QCIF format */
#define filename "flower_cif_150_yuv444.yuv"
#define file_y "flower_cif_150_yuv4441.yuv"

#define iTile 16

/* code for armulator*/
#pragma arm section zidata="ram"
int current_y[N][M];
int A[N+2][M+2];
int newA[N+2][M+2];
#pragma arm section
int i,j,ii;

/*Me ton parakatw pinaka tha ginei to filtrarisma stin fotografia
 int Laplacianfilter [3][3] = {
                                {0, 1, 0} ,
                                {1,-4, 1} ,
                                {0, 1, 0}
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
	for(i=0; i<N+2; i++)
		for(j=0; j<M+2; j++)
			A[i][j]=0;
			
/*Oi dio extra grammes kai stiles periexoun tin timi 0, oi ypoloipes theseis tou pinaka, dexontai times apo ton pinaka y*/
  	for(ii=1; ii<N+1; ii+=iTile)											
   		for(j=1;j<M+1;j++)
			for(i=ii; i<ii+iTile; i++)
				A[i][j]=current_y[i-1][j-1];

/*Akolouthei diadikasia filtrarismatos me vasi ton pinaka pou anaferthike parapanw*/
  for(ii=1; ii<N+1; ii+=iTile)
    for(j=1;j<M+1;j++)
      for(i=ii; i<ii+iTile; i++)
  			newA[i][j]=(A[i][j] * (-4) + // kentro
                            A[i-1][j-1] *(0) +   // panw aristera
                            A[i][j-1]   *(1) +   //  aristera
                            A[i+1][j-1] *(0) +   //  katw aristera
                            A[i-1][j]   *(1) +   // kentro panw
                            A[i+1][j]   *(1) +   // kentro katw
                            A[i-1][j+1] *(0) +   //  panw deksia
                            A[i][j+1]   *(1) +   //  deksia
                            A[i+1][j+1] *(0)  //  katw deksia
                            );


/*Afairoume tis parapanw prosthetes grammes kai stiles kai epanaferoume ton pinaka ston arxiko tou megethos. Tautoxrona ginetai k h klimakwsh*/
  for(ii=1; ii<N+1; ii+=iTile)
    for(j=1;j<M+1;j++)
      for(i=ii; i<ii+iTile; i++)
  			current_y[i-1][j-1]=newA[i][j];

 
 
  for(ii=0; ii<N; ii+=iTile)
    for(j=0;j<M;j++)
      for(i=ii; i<ii+iTile; i++){
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
