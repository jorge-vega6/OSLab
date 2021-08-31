#include <stdio.h>
#include <stdlib.h>

//n=amount of numbers in the series to compute, seq=array to store series
void tribonacci(int n, int *seq){
    // int *availAddr;
    if (n == 0) {
        // availAddr = &seq;
        // *availAddr = 0;
        seq[n] = 0;
    } else if (n == 1) {
        // availAddr = &seq + n - 1;
        // *availAddr = 1;
        seq[n] = 1;
    } else if (n == 2) {
        // availAddr = &seq + n - 1;
        // *availAddr = 1;
        seq[n] = 1;
    } else {
        // availAddr = &seq + n - 1;
        // int *minusOneAddr = availAddr - 1;
        // int *minusTwoAddr = availAddr - 2;
        // int *minusThreeAddr = availAddr - 3;
        // *availAddr = *minusOneAddr + *minusTwoAddr + *minusThreeAddr;
        seq[n] = seq[n-1] + seq[n-2] + seq[n-3];
    }
}

int main(){

	int n;
	//n, amount of series to compute
	scanf("%d",&n);

	//initialize array to 1, using malloc/calloc
	int *seq;
	seq = malloc(sizeof(int));

	int i;
	for (i = 0; i < n; i++){

        //recompute the whole series
        tribonacci(i, seq);

		//print array
        int j;
        for (j = 0; j <= i; j++){
			// int* currentAddr = &seq + j;
			printf("%d ", seq[j]);
		}

		//resize array, with realloc
        int newSize = i+1;
        seq = (int*)realloc(seq, newSize * sizeof(int));

		printf("\n");
	}

	//free array
	free(seq);
	return 0;
}
