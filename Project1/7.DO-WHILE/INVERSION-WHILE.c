#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define N 288 /* frame dimension for QCIF format */
#define M 352 /* frame dimension for QCIF format */
#define filename "flower_cif_150_yuv444.yuv"
#define file_y "flower_cif_150_yuv444w.yuv"

/* code for armulator*/
#pragma arm section zidata="ram"
int current_y[N][M];
int A[N+2][M+2];
int newA[N+2][M+2];
#pragma arm section

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

	i=0;
	while (i<N){
		j=0;
		while (j<M){
			current_y[i][j]=fgetc(frame_c);
			j++;
		}
		i++;
	}																		 

	fclose(frame_c);
}
  

void filtering(){

/*Arxikopoioume enan pinaka me dio parapanw grammes kai stiles etsi wste na min prokipsei mikroteros pinakas(apo ta M,N) meta to convolution*/
	i=0;
	while (i<N+2){
		j=0;
		while (j<M+2){
			A[i][j]=0;
			j++;
		}
		i++;
	}																		

/*Oi dio extra grammes kai stiles periexoun tin timi 0, oi ypoloipes theseis tou pinaka, dexontai times apo ton pinaka y*/
	i=1;
	while (i<N+1){
		j=1;
		while (j<M+1){
			A[i][j]=current_y[i-1][j-1];
			j++;
		}
		i++;
	}

																											
/*Akolouthei diadikasia filtrarismatos me vasi ton pinaka pou anaferthike parapanw*/
	i=1;
	while (i<N+1){
		j=1;
		while (j<M+1){
			newA[i][j]=(A[i][j] * (-4) + // kentro
	                          A[i-1][j-1] *(0) +   //  panw aristera
	                          A[i][j-1]   *(1) +   //  aristera
	                          A[i+1][j-1] *(0) +   //  katw aristera
	                          A[i-1][j]   *(1) +   // kentro panw
	                          A[i+1][j]   *(1) +   // kentro katw
	                          A[i-1][j+1] *(0) +   //  panw deksia
	                          A[i][j+1]   *(1) +   //  deksia
	                          A[i+1][j+1] *(0)  //  katw deksia
	                          );
	        j++;
			
		}
		i++;
	}																						
		
/*Afairoume tis parapanw prosthetes grammes kai stiles kai epanaferoume ton pinaka ston arxiko tou megethos. Tautoxrona ginetai k h klimakwsh*/
																			
	i=1;
	while(i<N+1){
		j=1;
		while(j<M+1){
				current_y[i-1][j-1]=newA[i][j] +=127;
				if (current_y[i][j] > 255) current_y[i][j] = 255;
				if (current_y[i][j] < 0) current_y[i][j] = 0;
				j++;
		}
		i++;
	}
}



void write()
{
  	FILE *frame_y;
  	frame_y=fopen(file_y,"wb");

	i=0;
	while (i<N){
		j=0;
		while (j<M){
			fputc(current_y[i][j],frame_y);
			j++;
		}
		i++;
	}													 
  	fclose(frame_y);
}

int main()
{
  read();									
  filtering();
  write();
}


