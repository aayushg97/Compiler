double d = 2.3;
int i;
int x = 4, *p, bc;
void func(int i, double d);
char g;

void func(int i,double d){
	Matrix mat[2][2] = {1.2, 2.0; 3.5, 4.3};
	do i = i - 1; while(mat[i][i] < d);
}

double getFib(int n) {
    if(n <= 1) return n;
    Matrix a[2][2];
    a[0][0] = a[0][1] + 1;
    a[1][0] = a[1][1] * 5;
    return (a[0][0] + a[0][1]);
}

double** ptrarr(double a,double v){
	Matrix ap[3][5];
	a = ap[2][1];
	v = (a * ap[1][1])/(a + (ap[2][4] + 3));
	double *p,*q;
	p = &a;
	q = &ap[1][4];
	ap[2][3] = (*q)*(*p);
	return &p;
}

int main(){
	int a;
	int **p;
	double b,c;
	
	func(a,b);
	b = getFib(a);
	p = ptrarr(b,c);
	
}
