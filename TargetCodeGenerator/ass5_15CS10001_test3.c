void main(){
	Matrix A[2][2] = {2.1,3.1;5.2,6.4};
	int i,j,s;
	double sum = 1.0;
	for(i=0;i<2;i++){
		for(j=0;j<2;j++){
			sum = sum + A[i][j];
		}
	}
	s = printStr("Sum of all elements is :- ");
	s = printFlt(sum);
	s = printStr("\n");
}
