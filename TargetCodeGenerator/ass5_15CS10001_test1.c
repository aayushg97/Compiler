void main(){
	int a,s;
	double b;
	s = printStr("Enter an integer \n");
	s = readInt(&a);
	s = printStr("You entered :- ");
	s = printInt(a);
	s = printStr("\nEnter a double\n");
	s = readFlt(&b);
	s = printStr("You entered :- ");
	s = printFlt(b);
	s = printStr("\n");
}
