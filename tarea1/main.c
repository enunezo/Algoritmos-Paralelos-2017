#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void matrixProduct ( int tam, int ** matrix1, int ** matrix2, int *** matrixResult ){
	for (int i = 0; i < tam ; i++){
		for (int k = 0; k < tam; k++){
			for (int j = 0; j < tam; j++){
				*(matrixResult)[i][k]  += matrix1[i][j] * matrix2[j][k];
			}
		}	
	}	
}



int main(){
	int tam = 16;
	int matrix1[tam][tam];
	int matrix2[tam][tam];
	srand(time(NULL));   
	for (int i = 0; i<tam; i++){
		for (int j = 0; j < tam; ++j){
			matrix1[i][j] = rand()%100;   
			matrix2[i][j] = rand()%100;   
			/*printf("%u\n", matrix1[i][j] );
			printf("%u\n", matrix2[i][j] );*/
		}
	}

	int matrixResult1[tam][tam];
	int tmp;
	//matrixProduct(tam, matrix1, matrix2, &matrixResult);
	for (int i = 0; i < tam ; i++){
		for (int k = 0; k < tam; k++){
			tmp = 0;
			for (int j = 0; j < tam; j++){
				tmp  += matrix1[i][j] * matrix2[j][k];
			}
			matrixResult1[i][k] = tmp;
		}	
	}

	
	//Block matrix product
	int blockSize = 4;
	int matrixResult2[tam][tam];
	for( int i1 = 0; i1 < tam; i1 += blockSize){
		for( int k1 = 0; k1 < tam; k1 += blockSize){
			for(int j1 = 0 ;j1 < tam; j1 += blockSize){
				for(int i = i1; i < MIN(i1 + blockSize, tam); ++i){
					for(int k = k1; k < MIN(k1 + blockSize, tam); ++k){
						tmp = 0;
						for(int j = j1; j < MIN(j1 + blockSize, tam); ++j){
							tmp += matrix1[i][j] * matrix2[j][k];
						}
						matrixResult2[i][k] = tmp;
					}
				}
			}
		}
	}

	/*for (int i = 0; i<tam; i++){
		for (int j = 0; j < tam; ++j){
			printf("%u\n", matrixResult2[i][j] );
		}
	}*/

	return 0;
}