int fact(int n){
	int i,prod = 1;
	for(i=1;i<=n;i++){
		prod = prod * i;
	}
	return prod;
}

int main(){
	int a,s,p;
	s = readInt(&a);
	p = fact(a);
	s = printInt(p);
}
