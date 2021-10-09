#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define N 288 /* frame dimension for QCIF format */
#define M 352 /* frame dimension for QCIF format */
#define filename "flower_cif_150_yuv444.yuv"
#define file_y "flower_cif_150_yuv444y.yuv"

/* code for armulator*/
#pragma arm section zidata="sram"
int current_y[N][M];
int A[N+2][M+2];
#pragma arm section
int newA[N+2][M+2];
/*Me ton parakatw pinaka tha ginei to filtrarisma stin fotografia
 int Laplacianfilter [3][3] = {
                                {0, 1, 0} ,
                                {1,-4, 1} ,
                                {0, 1, 0}
                              };  */

int i,j;



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
	for(i=0;i<N+2;i++){
		for(j=0;j<M;j+=4){
			A[i][j]=0;
			A[i][j+1]=0;
			A[i][j+2]=0;
			A[i][j+3]=0;
		}
		A[i][352]=0;
    	A[i][353]=0;
}
/*Oi dio extra grammes kai stiles periexoun tin timi 0, oi ypoloipes theseis tou pinaka, dexontai times apo ton pinaka y*/
	for(i=1;i<N+1;i++)
		for(j=1;j<M+1;j++)
			A[i][j]=current_y[i-1][j-1];

/*Akolouthei diadikasia filtrarismatos me vasi ton pinaka pou anaferthike parapanw*/
	for(i=1;i<N+1;i++){
		for(j=1;j<M+1;j+=4){
		
		newA[i][j]=(A[i][j]   *(-4) + // kentro
                          A[i-1][j-1] *(0) +   //  panw aristera
                          A[i][j-1]   *(1) +   //  aristera
                          A[i+1][j-1] *(0) +   //  katw aristera
                          A[i-1][j]   *(1) +   //  kentro panw
                          A[i+1][j]   *(1) +   // kentro katw
                          A[i-1][j+1] *(0) +   //  panw deksia
                          A[i][j+1]   *(1) +   //  deksia
                          A[i+1][j+1] *(0)  //  katw deksia
                          );
        newA[i][j+1]=(A[i][j+1] *(-4) + // kentro
                          A[i-1][j] 	*(0) +   //  panw aristera
                          A[i][j]   	*(1) +   //  aristera
                          A[i+1][j] 	*(0) +   //  katw aristera
                          A[i-1][j+1]   *(1) +   //  kentro panw
                          A[i+1][j+1]   *(1) +   //  kentro katw
                          A[i-1][j+2] 	*(0) +   //  panw deksia
                          A[i][j+2]   	*(1) +   //  deksia
                          A[i+1][j+2] 	*(0)  //  katw deksia
                          );

      newA[i][j+2]=(A[i][j+2] *(-4) + // kentro
                          A[i-1][j+1] *(0) +   //  panw aristera
                          A[i][j+1]   *(1) +   //  aristera
                          A[i+1][j+1] *(0) +   //  katw aristera
                          A[i-1][j+2] *(1) +   //  kentro panw
                          A[i+1][j+2] *(1) +   //  kentro katw
                          A[i-1][j+3] *(0) +   //  panw deksia
                          A[i][j+3]   *(1) +   //  deksia
                          A[i+1][j+3] *(0)  //  katw deksia
                          );

      newA[i][j+3]=(A[i][j+3] *(-4) + // kentro
                          A[i-1][j+2] *(0) +   //  panw aristera
                          A[i][j+2]   *(1) +   //  aristera
                          A[i+1][j+2] *(0) +   //  katw aristera
                          A[i-1][j+3] *(1) +   //  kentro panw
                          A[i+1][j+3] *(1) +   //  kentro katw
                          A[i-1][j+4] *(0) +   //  panw deksia
                          A[i][j+4]   *(1) +   //  deksia
                          A[i+1][j+4] *(0)  //  katw deksia
                          );
     	}
    }
/*Afairoume tis parapanw prosthetes grammes kai stiles kai epanaferoume ton pinaka ston arxiko tou megethos. Tautoxrona ginetai k h klimakwsh*/
      
      for(i=1;i<N+1;i++){
		for(j=1;j<M+1;j+=4){
			current_y[i-1][j-1]=newA[i][j] + 127;
      		current_y[i-1][j]=newA[i][j+1] + 127;
      		current_y[i-1][j+1]=newA[i][j+2] + 127;
      		current_y[i-1][j+2]=newA[i][j+3] + 127;

    	  	if (current_y[i][j] > 255) current_y[i][j] = 255;
     	 	if (current_y[i][j+1] > 255) current_y[i][j+1] = 255;
      		if (current_y[i][j+2] > 255) current_y[i][j+2] = 255;
      		if (current_y[i][j+3] > 255) current_y[i][j+3] = 255;

     	 	if (current_y[i][j] < 0) current_y[i][j] = 0;
      		if (current_y[i][j+1] < 0) current_y[i][j+1] = 0;
      		if (current_y[i][j+2] < 0) current_y[i][j+2] = 0;
      		if (current_y[i][j+3] < 0) current_y[i][j+3] = 0;
      
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
