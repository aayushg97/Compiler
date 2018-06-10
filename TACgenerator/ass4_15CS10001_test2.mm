double a = 2.3;
int b;
double operate(int x,double y);

int main(){
	int i,c;
	double dec;
	dec = operate(b,a);
	for(i=0;i<10;i=i+5){
		a = 5.2*b + dec;
	}
	
	do{
		a = 2 * a;
	}while(a<83.2);
	
	b = i>>5;
	
	if(b>=i){
		b = b/i;
	}
	
	if((b<a && i<c) || i>=a){
		a = a * 2;
	}
	
	c = b & i;
	c = b | i;
	c = b ^ i;
}

double operate(int x,double y){
	int c;
	double d = 2.3;
	y = x*d;
	c = --x;
	d = y + c;
	return d;
}
