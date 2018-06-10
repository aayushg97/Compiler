void main(){
	int a,b,s,e;
	double c,d,f;
	s = printStr("Enter the first Integer\n");
	s = readInt(&a);
	s = printStr("Enter the second Integer\n");
	s = readInt(&b);
	
	/*Arithmetic operations on integers*/
	s = printStr("a + b = ");
	e = a+b;
	s = printInt(e);
	s = printStr("\n");
	
	s = printStr("a - b = ");
	e = a-b;
	s = printInt(e);
	s = printStr("\n");
	
	s = printStr("a * b = ");
	//e = a*b;
	s = printInt(a*b);
	s = printStr("\n");
	
	s = printStr("a / b = ");
	e = a/b;
	s = printInt(e);
	s = printStr("\n");
	
	s = printStr("a % b = ");
	//e = a%b;
	s = printInt(a%b);
	s = printStr("\n");
	s = printStr("\n");
	
	/*Arithmetic operations on double precision numbers*/
	s = printStr("Enter the first double\n");
	s = readFlt(&c);
	s = printStr("Enter the second double\n");
	s = readFlt(&d);
	
	s = printStr("c + d = ");
	f = c+d;
	s = printFlt(f);
	s = printStr("\n");
	
	s = printStr("c - d = ");
	e = c-d;
	s = printFlt(e);
	s = printStr("\n");
	
	s = printStr("c * d = ");
	//e = a+b;
	s = printFlt(c*d);
	s = printStr("\n");
	
	s = printStr("c / d = ");
	e = c/d;
	s = printFlt(e);
	s = printStr("\n");
}
