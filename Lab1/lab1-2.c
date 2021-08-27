/** old
 * 
#include <stdio.h>
#include <stdlib.h>

//n=amount of numbers in the series to compute, seq=array to store series
void tribonacci(int n, int* seq){
    // Complete this function
    ...
}

int main(){

	int n;
	//n, amount of series to compute
	scanf("%d",&n);

	//initialize array to 1, using malloc/calloc
	int *seq = /* complete code 

	int i;
	for(i = /* complete code  i++){

        	//recompute the whole series
        	tribonacci(i, seq);

		//print array
        	int j;
        	for(/* complete code 
        	...

		//resize array, with realloc
        	int newSize=i+/* complete code 
		...

		printf("\n");
	}
	//free array
	...
	return 0;
}
*/ 

#include <stdio.h>
#include <stdlib.h>

//n=amount of numbers in the series to compute, seq=array to store series
void tribonacci(int n, int* seq){
    int* availAddr;
    if (n == 1) {
        availAddr = &seq;
        *availAddr = 0;
    } else if (n == 2) {
        availAddr = &seq + n - 1;
        *availAddr = 1;
    } else if (n == 3) {
        availAddr = &seq + n - 1;
        *availAddr = 1;
    } else {
        availAddr = &seq + n - 1;
        int* minusOneAddr = availAddr - 1;
        int* minusTwoAddr = availAddr - 2;
        int* minusThreeAddr = availAddr - 3;
        *availAddr = *minusOneAddr + *minusTwoAddr + *minusThreeAddr;
    }
}

int main(){

	int n;
	//n, amount of series to compute
	scanf("%d",&n);

	//initialize array to 1, using malloc/calloc
	int *seq = (int*)calloc(1, sizeof(int));

	int i;
	for(i = 1; i <= n; i++){

        //recompute the whole series
        tribonacci(i, seq);

		//print array
        int j;
        for(/* complete code */

		//resize array, with realloc
        int newSize = i++;
        seq = (int*)realloc(seq, newSize * sizeof(int));

		printf("\n");
	}

	//free array
	free(seq);
	return 0;
}
