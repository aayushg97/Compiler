void elapse(){
	int x,y;
	double z;
	x = x*y;
	y = x<<2;
	z = x-y;
}

char diff(double a1,double a2,int a3);

int main(){
	Matrix A[2][3] = {1.1, 2.2, 3.3; 2.4, 4.8, 6.8};
	Matrix B[3][2];
	double a = 5.2,b;
	int c = 4,d;
	char s = 'f',g;
	B = A.';
	b = A[1][2];
	A[1][1] = a;
	
	if(a!=b){
		c = c*5;
	}
	
	elapse();
	c = (a<=b) ? s : g;
	double **point;
	g = diff(a,b,c);
}

char diff(double a1,double a2,int a3){
	if(a1-a2==0){
		return 'c';
	}
	else{
		return 'd';
	}
	
	Matrix A[2][4] = {1.3,2.3,4.4,3.1;1.5,3.6,4.2,6.9},B[2][4] = A,C[2][4],D[2][2],E[4][2] = A.';
	C = A + B;
	C = A - B;
	D = A * E;
}
