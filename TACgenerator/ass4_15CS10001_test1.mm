void main(){
	int a,b=-4,c;
	c = a + b;
	double d,e = 4.5;
	double deci = -8.1;
	b = 5;
	d = a - e;
	d= b *e;
	c = a / b;
	c = a % b;
	b = a++;
	c = b--;
	b = --a;
	b = ++c;
	
	int *q = &a,*p;
	//q = &a;
	b = *q;
	*p = c;
	
	if(a==b){
		d = e/4.5;
	}
	else{
		a = b;
	}
	
	while(*p == b){
		a++;
	}
	
	Matrix arr[2][2] = {1.2, -2.0; +3.5, 4.3};
	
	for(a=4;a>=1;--a){
		if(b!=c){
			b = c-b;
		}
		else{
			while((b<c || a>=c) && a!=b){
				b = b + a * c / 9;
			}
		}
	}
}
