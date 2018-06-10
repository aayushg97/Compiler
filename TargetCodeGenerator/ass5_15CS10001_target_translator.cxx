#include "ass5_15CS10001_translator.h"
#include "ass5_15CS10001.tab.h"
#include <cstring>
#include <cmath>

regdet regbank[12];

void getRep(double num,char bit1[]) {
	int ab;
	int bit[64];
	int power=1025;
    double actnum = num;

	if(num<0.0)bit[0] = 1;
	else bit[0] = 0;
	
    for(int i=-50;i<50;i++){
    	if(num>=pow(2,i) && num<=pow(2,i+1)){
    		power = i;
    		break;
		}
    }
    
    if(power!=1025){
    	actnum /= pow(2,power);
    	power += 1023;
    	for(int i=52;i<63;i++){
    		bit[63-i] = (power%2);
    		power /= 2;
    	}
    }
    
    actnum--;
    for(int i=51;i>=0;i--){
    	actnum *= 2;
    	if(actnum>=1){
    		bit[63-i] = 1;
    		actnum--;
		}
    	else bit[63-i] = 0;
    }
    
    for(int i=0;i<64;i+=4){
    	ab = (8*bit[i] + 4*bit[i+1] + 2*bit[i+2] + bit[i+3]);
    	if(ab<=9)bit1[(i/4)+2] = '0' + ab;
    	else bit1[(i/4)+2] = 'A' + (ab-10);
    }
    bit1[18] = '\0';
    bit1[0] = '0';
    bit1[1] = 'x';
}

void setregbank(){
	regbank[0].name[0] = "rax";
	regbank[1].name[0] = "rbx";
	regbank[2].name[0] = "rcx";
	regbank[3].name[0] = "rdx";
	regbank[4].name[0] = "rsi";
	regbank[5].name[0] = "rdi";
	regbank[6].name[0] = "r8";
	regbank[7].name[0] = "r9";
	regbank[8].name[0] = "r10";
	regbank[9].name[0] = "r13";
	regbank[10].name[0] = "r14";
	regbank[11].name[0] = "r15";
	
	regbank[0].name[1] = "eax";
	regbank[1].name[1] = "ebx";
	regbank[2].name[1] = "ecx";
	regbank[3].name[1] = "edx";
	regbank[4].name[1] = "esi";
	regbank[5].name[1] = "edi";
	regbank[6].name[1] = "r8d";
	regbank[7].name[1] = "r9d";
	regbank[8].name[1] = "r10d";
	regbank[9].name[1] = "r13d";
	regbank[10].name[1] = "r14d";
	regbank[11].name[1] = "r15d";
	
	regbank[0].name[2] = "al";
	regbank[1].name[2] = "bl";
	regbank[2].name[2] = "cl";
	regbank[3].name[2] = "dl";
	regbank[4].name[2] = "sil";
	regbank[5].name[2] = "dil";
	regbank[6].name[2] = "r8b";
	regbank[7].name[2] = "r9b";
	regbank[8].name[2] = "r10b";
	regbank[9].name[2] = "r13b";
	regbank[10].name[2] = "r14b";
	regbank[11].name[2] = "r15b";
	
	for(int i=0;i<12;i++){
		regbank[i].free = 1;
		regbank[i].var = "";
	}
}

artable::artable(tableSym *en,int pcount){
	this->tab = en;												/// Activation record
	int i=1;
	this->poffset = 0;
	entrySym *p;
	this->head = NULL;
	arec *q,*r;
	p = en->head;
	while(p){
		if(i<pcount && i>6){
			this->poffset += p->size;
		}
		r = new arec();
		r->name = p->name;
		r->offset = (-1)*(p->offset+p->size);
		r->hasreg = 0;
		r->inMem = 1;
		r->regind = -1;
		if(p->size==8){r->regbit = 0; r->regch = 'q';}
		else{
			if(p->size==4){r->regbit = 1; r->regch = 'l';}
			else if(p->size==1){r->regbit = 2; r->regch = 'b';}
		}
		r->regname = "";
		r->lastuse = getlastuse(en->qbegin,en->qend,r->name);
		r->next = NULL;
		if(this->head!=NULL)q->next = r;
		else this->head = r;
		q = r;
		i++;
		p = p->next;
	}
}

int getlastuse(int s,int e,string name){
	int i=0;
	quad *p = global_quad.head;
	while(i<s){
		p = p->next;
		i++;
	}
	
	while(i<=e){
		if(p->arg1==name || p->arg2==name)return i;
		p = p->next;
		i++;
	}
	
	return -1;
}

int artable::getoffset(string s){
	arec *p;
	p = this->head;
	while(p){
		if(p->name==s)return p->offset;
		p = p->next;
	}
	return 0;
}

int artable::getinMem(string s){
	arec *p;
	p = this->head;
	while(p){
		if(p->name==s)return p->inMem;
		p = p->next;
	}
	return -1;
}

int artable::gethasreg(string s){
	arec *p;
	p = this->head;
	while(p){
		if(p->name==s)return p->hasreg;
		p = p->next;
	}
	return -1;
}

int artable::getregind(string s){
	arec *p;
	p = this->head;
	while(p){
		if(p->name==s)return p->regind;
		p = p->next;
	}
	return -1;
}

int artable::getregbit(string s){
	arec *p;
	p = this->head;
	while(p){
		if(p->name==s)return p->regbit;
		p = p->next;
	}
	return -1;
}

char artable::getregch(string s){
	arec *p;
	p = this->head;
	while(p){
		if(p->name==s)return p->regch;
		p = p->next;
	}
	return 'a';
}

string artable::getregname(string s){
	arec *p;
	p = this->head;
	while(p){
		if(p->name==s)return p->regname;
		p = p->next;
	}
	return "";
}

void artable::setoffset(string s,int off){
	arec *p;
	p = this->head;
	while(p){
		if(p->name==s){
			p->offset = off;
			break;
		}
		p = p->next;
	}
}

void artable::setinMem(string s,int im){
	arec *p;
	p = this->head;
	while(p){
		if(p->name==s){
			p->inMem = im;
			break;
		}
		p = p->next;
	}
}

void artable::sethasreg(string s,int hr){
	arec *p;
	p = this->head;
	while(p){
		if(p->name==s){
			p->hasreg = hr;
			break;
		}
		p = p->next;
	}
}

void artable::setreg(string s,int index,string rname){
	arec *p;
	p = this->head;
	while(p){
		if(p->name==s){
			p->regind = index;
			p->regname = rname;
			break;
		}
		p = p->next;
	}
}

void artable::addrecord(string s,int off,int im,int hr,int index,int bit,char ch,string rname,int luse){
	arec *p = this->head;
	if(p==NULL){
		this->head = new arec();
		this->head->name = s;
		this->head->offset = off;
		this->head->inMem = im;
		this->head->hasreg = hr;
		this->head->regind = index;
		this->head->regbit = bit;
		this->head->regch = ch;
		this->head->regname = rname;
		this->head->lastuse = luse;
		this->head->next = NULL;	
	}
	else{
		while(p->next){
			p = p->next;
		}
		arec *q = new arec();
		q->name = s;
		q->offset = off;
		q->inMem = im;
		q->hasreg = hr;
		q->regind = index;
		q->regbit = bit;
		q->regch = ch;
		q->regname = rname;
		q->lastuse = luse;
		q->next = NULL;
		p->next = q;
	}
}
		
void artable::updaterec(string s,int off,int im,int hr,int index,string rname){
	arec *p = this->head;
	while(p){
		if(p->name==s){
			p->offset = off;
			p->inMem = im;
			p->hasreg = hr;
			p->regind = index;
			p->regname = rname;	
		}
		p = p->next;
	}
}

void artable::spillall(){
	char ch;
	int bit,off;
	for(int i=0;i<12;i++){
		ch = this->getregch(regbank[i].var);
		bit = this->getregbit(regbank[i].var);
		off = this->getoffset(regbank[i].var);
		printf("\tmov%c\t%%%s, %d(%%rbp)\n",ch,regbank[i].name[bit].c_str(),off);
		this->sethasreg(regbank[i].var,0);
		this->setreg(regbank[i].var,-1,"");
		regbank[i].var = "";
		regbank[i].free = 1;
	}
}

void tableSym::assign_reg(string s1,string s2){
	int bit,off;
	char ch;
	string rname;
	
	artable *art = this->actrectab;
	
	ch = art->getregch(s1);
	bit = art->getregbit(s1);
	off = art->getoffset(s1);
	
	if(ch!='a'){
		for(int i=0;i<12;i++){
			if(regbank[i].free==1){		
				printf("\tmov%c\t%d(%%rbp), %%%s\n",ch,off,regbank[i].name[bit].c_str());
				regbank[i].free = 0;
				regbank[i].var = s1;
				art->sethasreg(s1,1);
				art->setreg(s1,i,regbank[i].name[bit]);
				break;
			}
		}
	}

	if(art->gethasreg(s1)==0 && ch!='a'){
		for(int i=0;i<12;i++){
			if(regbank[i].var!=s2){
				ch = art->getregch(regbank[i].var);
				bit = art->getregbit(regbank[i].var);
				off = art->getoffset(regbank[i].var);
				if(art->getinMem(regbank[i].var)==0)printf("\tmov%c\t%%%s, %d(%%rbp)\n",ch,regbank[i].name[bit].c_str(),off);
				art->sethasreg(regbank[i].var,0);
				art->setreg(regbank[i].var,-1,"");
				
				ch = art->getregch(s1);
				bit = art->getregbit(s1);
				off = art->getoffset(s1);
				printf("\tmov%c\t%d(%%rbp), %%%s\n",ch,off,regbank[i].name[bit].c_str());
				regbank[i].free = 0;
				regbank[i].var = s1;
				art->sethasreg(s1,1);
				art->setreg(s1,i,regbank[i].name[bit]);
			}
		}
	}
}

void tableSym::replace_var(string arg,string res){
	artable *art = this->actrectab;
	int index,bit;
	
	bit = art->getregbit(arg);
	index = art->getregind(arg);	printf("%d \n",index);
	art->sethasreg(res,1);
	art->setinMem(res,0);
	art->setreg(res,index,regbank[index].name[bit]);
	regbank[index].free = 0;
	regbank[index].var = res;
	art->sethasreg(arg,0);
	art->setreg(arg,-1,"");
}

void tableSym::handle_global(){
	entrySym *p;
	p = this->head;
	int flag = 1,flag1=0;
	
	while(p){
		if(p->type->ntype!="function"){
			if(p->initval==""){
				printf("\t.comm\t%s,%d,%d \n",p->name.c_str(),p->type->width,p->type->width);
			}
			else{
				if(p->type->ntype!="string"){
					if(p->type->ntype!="fconst"){
						printf("\t.globl\t%s \n",p->name.c_str());
						if(flag==1){
							printf("\t.data \n");
							flag=0;
						}
						if(p->type->ntype!="char")printf("\t.align\t%d \n",p->type->width);
						printf("\t.type\t%s, @object\n",p->name.c_str());
						printf("\t.size\t%s, %d\n",p->name.c_str(),p->type->width);
						printf("%s:\n",p->name.c_str());
						if(p->size==1)printf("\t.byte\t%d\n",p->initval[0]);
						else printf("\t.long\t%d\n",atoi(p->initval.c_str()));
					}
					else{
						if(flag1==0){printf("\t.section\t.rodata\n"); flag1 = 1;}
						printf("%s:\n",p->name.c_str());
						char bitrep[19];
						getRep(double(atof(p->initval.c_str())),bitrep);
						
						char fl1[11],fl2[11];
						fl1[0] = '0';
						fl1[1] = 'x';
						fl1[10] = '\0';
						fl2[0] = '0';
						fl2[1] = 'x';
						fl2[10] = '\0';
						for(int i=2;i<=9;i++){
							fl2[i] = bitrep[i];
							fl1[i] = bitrep[i+8];
						}
					
						printf("\t.long\t%s\n",fl1);
						printf("\t.long\t%s\n",fl2);
					}
				}
				else{
					if(flag1==0){printf("\t.section\t.rodata\n"); flag1 = 1;}
					printf("%s:\n",p->name.c_str());
					printf("\t.string %s\n",p->initval.c_str());
				}
			}
		}
		
		p = p->next;
	}
}

void tableSym::prologue(int func_no){
	printf("\t.globl\t%s \n",this->name.c_str());
	printf("\t.type\t%s, @function \n",this->name.c_str());
	printf("%s: \n",this->name.c_str());
	printf(".LFB%d: \n",func_no);
	printf("\tpushq\t%%rbp \n");
	printf("\tmovq\t%%rsp, %%rbp\n");
	printf("\tsubq\t$%d, %%rsp\n",this->offset);
}

void tableSym::funchand(){
	int i,bit,off,index,param_count=0,pcoff=0,f1;
	char ch;
	string rname;
	entrySym *ent, *ent1, *ent2;
	quad *q = global_quad.head;
	artable *art = this->actrectab;
	
	if(this->param_no>0){
		entrySym *p = this->head;
		i = 1;
		while(i<=this->param_no && i<=6){
			switch(i){
				case 1 : 
					if(p->size==4)printf("\tmovl\t%%edi, %d(%%rbp)\n",(-1)*(p->offset+p->size));
					else {
						if(p->size==1)printf("\tmovb\t%%dil, %d(%%rbp)\n",(-1)*(p->offset+p->size));
						else {
							if(p->size==8){
								if(p->type->ntype=="double")printf("\tmovq\t%%xmm0, %d(%%rbp)\n",(-1)*(p->offset+p->size));
								else printf("\tmovq\t%%rdi, %d(%%rbp)\n",(-1)*(p->offset+p->size));
							}
						}
					}
					break;
					
				case 2 : 
					if(p->size==4)printf("\tmovl\t%%esi, %d(%%rbp)\n",(-1)*(p->offset+p->size));
					else {
						if(p->size==1)printf("\tmovb\t%%sil, %d(%%rbp)\n",(-1)*(p->offset+p->size));
						else {
							if(p->size==8){
								if(p->type->ntype=="double")printf("\tmovq\t%%xmm1, %d(%%rbp)\n",(-1)*(p->offset+p->size));
								else printf("\tmovq\t%%rsi, %d(%%rbp)\n",(-1)*(p->offset+p->size));
							}
						}
					}
					break;
					
				case 3 : 
					if(p->size==4)printf("\tmovl\t%%edx, %d(%%rbp)\n",(-1)*(p->offset+p->size));
					else {
						if(p->size==1)printf("\tmovb\t%%dl, %d(%%rbp)\n",(-1)*(p->offset+p->size));
						else {
							if(p->size==8){
								if(p->type->ntype=="double")printf("\tmovq\t%%xmm2, %d(%%rbp)\n",(-1)*(p->offset+p->size));
								else printf("\tmovq\t%%rdx, %d(%%rbp)\n",(-1)*(p->offset+p->size));
							}
						}
					}
					break;
					
				case 4 : 
					if(p->size==4)printf("\tmovl\t%%ecx, %d(%%rbp)\n",(-1)*(p->offset+p->size));
					else {
						if(p->size==1)printf("\tmovb\t%%cl, %d(%%rbp)\n",(-1)*(p->offset+p->size));
						else {
							if(p->size==8){
								if(p->type->ntype=="double")printf("\tmovq\t%%xmm3, %d(%%rbp)\n",(-1)*(p->offset+p->size));
								else printf("\tmovq\t%%rcx, %d(%%rbp)\n",(-1)*(p->offset+p->size));
							}
						}
					}
					break;
					
				case 5 : 
					if(p->size==4)printf("\tmovl\t%%r8d, %d(%%rbp)\n",(-1)*(p->offset+p->size));
					else {
						if(p->size==1)printf("\tmovb\t%%r8b, %d(%%rbp)\n",(-1)*(p->offset+p->size));
						else {
							if(p->size==8){
								if(p->type->ntype=="double")printf("\tmovq\t%%xmm4, %d(%%rbp)\n",(-1)*(p->offset+p->size));
								else printf("\tmovq\t%%r8, %d(%%rbp)\n",(-1)*(p->offset+p->size));
							}
						}
					}
					break;
					
				case 6 : 
					if(p->size==4)printf("\tmovl\t%%r9d, %d(%%rbp)\n",(-1)*(p->offset+p->size));
					else {
						if(p->size==1)printf("\tmovb\t%%r9b, %d(%%rbp)\n",(-1)*(p->offset+p->size));
						else {
							if(p->size==8){
								if(p->type->ntype=="double")printf("\tmovq\t%%xmm5, %d(%%rbp)\n",(-1)*(p->offset+p->size));
								else printf("\tmovq\t%%r9, %d(%%rbp)\n",(-1)*(p->offset+p->size));
							}
						}
					}
					break;
			}
			i++;
			p = p->next;
		}
		
		while(i<=this->param_no && p){
			art->setoffset(p->name,this->actrectab->poffset-pcoff-(p->size)); //TODO handle extra offset; 
			pcoff += p->size;
			i++;
			p = p->next;
		}
	}
	
	
	
	i = 0;
	while(i<this->qbegin){
		q = q->next;
		i++;
	}
	
	while(i<=this->qend && q){
		if(q->islabel==1){
			printf("\n%s:\n",q->label.c_str());	
		}
		
		if(q->arg1!="" && q->arg2!="" && (q->opcode=="+" || q->opcode=="-"|| q->opcode=="*"|| q->opcode=="/"|| q->opcode=="%")){			
			if(q->opcode=="+"){ 
				f1 = 0;
				ch = art->getregch(q->arg2);
				ent1 = this->find(q->arg2);
				if(ent1==NULL)ent1 = global_symtab->find(q->arg2);
				if(ch!='a'){
					switch(ch){
						case 'b': printf("\tmovb\t%d(%%rbp), %%bl\n",art->getoffset(q->arg2)); break;
						case 'l': printf("\tmovl\t%d(%%rbp), %%ebx\n",art->getoffset(q->arg2)); break;
						case 'q': 
							if(ent1->type->ntype=="double")printf("\tmovsd\t%d(%%rbp), %%xmm1\n",art->getoffset(q->arg2));
							else printf("\tmovq\t%d(%%rbp), %%rbx\n",art->getoffset(q->arg2)); 
							break;
					}
				}
				else{
					ent = global_symtab->find(q->arg2);
					if(ent){
						if(ent->size==4){
							printf("\tmovl\t%s(%%rip), %%ebx\n",q->arg2.c_str());
							ch = 'l';
						}
						else{
							if(ent->size==1){
								printf("\tmovb\t%s(%%rip), %%bl\n",q->arg2.c_str());
								ch = 'b';	
							}
							else{
								if(ent->size==8){
									if(ent->type->ntype=="double")printf("\tmovsd\t%s(%%rip), %%xmm1\n",q->arg2.c_str());
									else printf("\tmovq\t%s(%%rip), %%rbx\n",q->arg2.c_str());
									ch = 'q';
								}
							}
						}
					}
					else f1 = 1;
				}
				
				ch = art->getregch(q->arg1);
				ent1 = this->find(q->arg1);
				if(ent1==NULL)ent1 = global_symtab->find(q->arg1);
				if(ch!='a'){
					switch(ch){
						case 'b': printf("\tmovb\t%d(%%rbp), %%al\n",art->getoffset(q->arg1)); break;
						case 'l': printf("\tmovl\t%d(%%rbp), %%eax\n",art->getoffset(q->arg1)); break;
						case 'q': 
							if(ent1->type->ntype=="double")printf("\tmovsd\t%d(%%rbp), %%xmm0\n",art->getoffset(q->arg1));
							else printf("\tmovq\t%d(%%rbp), %%rax\n",art->getoffset(q->arg1)); 
							break;
					}
				}
				else{
					ent = global_symtab->find(q->arg1);
					if(ent){
						if(ent->size==4){
							printf("\tmovl\t%s(%%rip), %%eax\n",q->arg1.c_str());
							ch = 'l';	
						}
						else{
							if(ent->size==1){
								printf("\tmovb\t%s(%%rip), %%al\n",q->arg1.c_str());
								ch = 'b';
							}
							else{
								if(ent->size==8){
									if(ent->type->ntype=="double")printf("\tmovsd\t%s(%%rip), %%xmm0\n",q->arg1.c_str());
									else printf("\tmovq\t%s(%%rip), %%rax\n",q->arg1.c_str());
									ch = 'q';
								}
							}
						}
					}
				}
				
				switch(ch){
					case 'b': 
						if(f1==1){printf("\taddb\t$1, %%al\n"); f1=0;} 
						else printf("\taddb\t%%bl, %%al\n");
						break;
					case 'l': 
						if(f1==1){printf("\taddl\t$1, %%eax\n"); f1=0;} 
						else printf("\taddl\t%%ebx, %%eax\n");
						break;
					case 'q': 
						if(f1==1){printf("\taddq\t$1, %%rax\n"); f1=0;} 
						else {
							ent1 = this->find(q->arg1);
							if(ent1==NULL)ent1 = global_symtab->find(q->arg1);
							if(ent1->type->ntype=="double")printf("\taddsd\t%%xmm1, %%xmm0\n");
							else printf("\taddq\t%%rbx, %%rax\n");
						}
						break;
				}
				
				ch = art->getregch(q->result);
				ent1 = this->find(q->result);
				if(ent1==NULL)ent1 = global_symtab->find(q->result);
				if(ch!='a'){
					switch(ch){
						case 'b': printf("\tmovb\t%%al, %d(%%rbp)\n",art->getoffset(q->result)); break;
						case 'l': printf("\tmovl\t%%eax, %d(%%rbp)\n",art->getoffset(q->result)); break;
						case 'q': 
							if(ent1->type->ntype=="double")printf("\tmovsd\t%%xmm0, %d(%%rbp)\n",art->getoffset(q->result));
							else printf("\tmovq\t%%rax, %d(%%rbp)\n",art->getoffset(q->result)); 
							break;
					}
				}
				else{
					ent = global_symtab->find(q->result);
					if(ent){
						if(ent->size==4){
							printf("\tmovl\t%%eax, %s(%%rip)\n",q->result.c_str());
							ch = 'l';
						}
						else{
							if(ent->size==1){
								printf("\tmovb\t%%al, %s(%%rip)\n",q->result.c_str());
								ch = 'b';	
							}
							else{
								if(ent->size==8){
									if(ent->type->ntype=="double")printf("\tmovsd\t%%xmm0, %s(%%rip)\n",q->result.c_str());
									else printf("\tmovq\t%%rax, %s(%%rip)\n",q->result.c_str());
									ch = 'q';
								}
							}
						}
					}
				}
			}
				
			if(q->opcode=="-"){
				f1 = 0;
				ch = art->getregch(q->arg2);
				ent1 = this->find(q->arg2);
				if(ent1==NULL)ent1 = global_symtab->find(q->arg2);
				if(ch!='a'){
					switch(ch){
						case 'b': printf("\tmovb\t%d(%%rbp), %%dl\n",art->getoffset(q->arg2)); break;
						case 'l': printf("\tmovl\t%d(%%rbp), %%edx\n",art->getoffset(q->arg2)); break;
						case 'q': 
							if(ent1->type->ntype=="double")printf("\tmovsd\t%d(%%rbp), %%xmm1\n",art->getoffset(q->arg2));
							else printf("\tmovq\t%d(%%rbp), %%rdx\n",art->getoffset(q->arg2)); 
							break;
					}
				}
				else{
					ent = global_symtab->find(q->arg2);
					if(ent){
						if(ent->size==4){
							printf("\tmovl\t%s(%%rip), %%edx\n",q->arg2.c_str());
							ch = 'l';
						}
						else{
							if(ent->size==1){
								printf("\tmovb\t%s(%%rip), %%dl\n",q->arg2.c_str());
								ch = 'b';	
							}
							else{
								if(ent->size==8){
									if(ent->type->ntype=="double")printf("\tmovsd\t%s(%%rip), %%xmm1\n",q->arg2.c_str());
									else printf("\tmovq\t%s(%%rip), %%rdx\n",q->arg2.c_str());
									ch = 'q';	
								}
							}
						}
					}
					else f1 = 1;
				}
			
				ch = art->getregch(q->arg1);
				ent1 = this->find(q->arg1);
				if(ent1==NULL)ent1 = global_symtab->find(q->arg1);
				if(ch!='a'){
					switch(ch){
						case 'b': printf("\tmovb\t%d(%%rbp), %%cl\n",art->getoffset(q->arg1)); break;
						case 'l': printf("\tmovl\t%d(%%rbp), %%ecx\n",art->getoffset(q->arg1)); break;
						case 'q': 
							if(ent1->type->ntype=="double")printf("\tmovsd\t%d(%%rbp), %%xmm0\n",art->getoffset(q->arg1));
							else printf("\tmovq\t%d(%%rbp), %%rcx\n",art->getoffset(q->arg1)); 
							break;
					}
				}
				else{
					ent = global_symtab->find(q->arg1);
					if(ent){
						if(ent->size==4){
							printf("\tmovl\t%s(%%rip), %%ecx\n",q->arg1.c_str());
							ch = 'l';	
						}
						else{
							if(ent->size==1){
								printf("\tmovb\t%s(%%rip), %%cl\n",q->arg1.c_str());
								ch = 'b';
							}
							else{
								if(ent->size==8){
									if(ent->type->ntype=="double")printf("\tmovsd\t%s(%%rip), %%xmm0\n",q->arg1.c_str());
									else printf("\tmovq\t%s(%%rip), %%rcx\n",q->arg1.c_str());
									ch = 'q';		
								}
							}
						}
					}
				}
				
				switch(ch){
					case 'b': 
						if(f1==1){printf("\tsubb\t$1, %%cl\n"); f1=0;}
						else printf("\tsubb\t%%dl, %%cl\n");
						break;
					case 'l': 
						if(f1==1){printf("\tsubl\t$1, %%ecx\n"); f1=0;}
						else printf("\tsubl\t%%edx, %%ecx\n");
						break;
					case 'q': 
						if(f1==1){printf("\tsubq\t$1, %%rcx\n"); f1=0;}
						else {
							ent1 = this->find(q->arg1);
							if(ent1==NULL)ent1 = global_symtab->find(q->arg1);
							if(ent1->type->ntype=="double")printf("\tsubsd\t%%xmm1, %%xmm0\n");
							else printf("\tsubq\t%%rdx, %%rcx\n");
						} 
						break;
				}
				
				ch = art->getregch(q->result);
				ent1 = this->find(q->result);
				if(ent1==NULL)ent1 = global_symtab->find(q->result);
				if(ch!='a'){
					switch(ch){
						case 'b': printf("\tmovb\t%%cl, %d(%%rbp)\n",art->getoffset(q->result)); break;
						case 'l': printf("\tmovl\t%%ecx, %d(%%rbp)\n",art->getoffset(q->result)); break;
						case 'q': 
							if(ent1->type->ntype=="double")printf("\tmovsd\t%%xmm0, %d(%%rbp)\n",art->getoffset(q->result));
							else printf("\tmovq\t%%rcx, %d(%%rbp)\n",art->getoffset(q->result)); 
							break;
					}
				}
				else{
					ent = global_symtab->find(q->result);
					if(ent){
						if(ent->size==4){
							printf("\tmovl\t%%ecx, %s(%%rip)\n",q->result.c_str());
							ch = 'l';
						}
						else{
							if(ent->size==1){
								printf("\tmovb\t%%cl, %s(%%rip)\n",q->result.c_str());
								ch = 'b';	
							}
							else{
								if(ent->size==8){
									if(ent->type->ntype=="double")printf("\tmovsd\t%%xmm0, %s(%%rip)\n",q->result.c_str());
									else printf("\tmovq\t%%rcx, %s(%%rip)\n",q->result.c_str());
									ch = 'q';
								}
							}
						}
					}
				}
			}
				
			if(q->opcode=="*"){
				f1 = 0;
				ch = art->getregch(q->arg1);
				ent1 = this->find(q->arg1);
				if(ent1==NULL)ent1 = global_symtab->find(q->arg1);
				if(ch!='a'){
					switch(ch){
						case 'b': printf("\tmovb\t%d(%%rbp), %%sil\n",art->getoffset(q->arg1)); break;
						case 'l': printf("\tmovl\t%d(%%rbp), %%esi\n",art->getoffset(q->arg1)); break;
						case 'q': 
							if(ent1->type->ntype=="double")printf("\tmovsd\t%d(%%rbp), %%xmm0\n",art->getoffset(q->arg1));
							else printf("\tmovq\t%d(%%rbp), %%rsi\n",art->getoffset(q->arg1)); 
							break;
					}
				}
				else{
					ent = global_symtab->find(q->arg1);
					if(ent){
						if(ent->size==4){
							printf("\tmovl\t%s(%%rip), %%esi\n",q->arg1.c_str());
							ch = 'l';	
						}
						else{
							if(ent->size==1){
								printf("\tmovb\t%s(%%rip), %%sil\n",q->arg1.c_str());
								ch = 'b';
							}
							else{
								if(ent->size==8){
									if(ent->type->ntype=="double")printf("\tmovsd\t%s(%%rip), %%xmm0\n",q->arg1.c_str());
									else printf("\tmovq\t%s(%%rip), %%rsi\n",q->arg1.c_str());
									ch = 'q';
								}
							}
						}
					}
					else f1 = 1;
				}
				
				ch = art->getregch(q->arg2);
				ent1 = this->find(q->arg2);
				if(ent1==NULL)ent1 = global_symtab->find(q->arg2);
				if(ch!='a'){
					switch(ch){
						case 'b': printf("\tmovb\t%d(%%rbp), %%dil\n",art->getoffset(q->arg2)); break;
						case 'l': printf("\tmovl\t%d(%%rbp), %%edi\n",art->getoffset(q->arg2)); break;
						case 'q': 
							if(ent1->type->ntype=="double")printf("\tmovsd\t%d(%%rbp), %%xmm1\n",art->getoffset(q->arg2));
							else printf("\tmovq\t%d(%%rbp), %%rdi\n",art->getoffset(q->arg2)); 
							break;
					}
				}
				else{
					ent = global_symtab->find(q->arg2);
					if(ent){
						if(ent->size==4){
							printf("\tmovl\t%s(%%rip), %%edi\n",q->arg2.c_str());
							ch = 'l';
						}
						else{
							if(ent->size==1){
								printf("\tmovb\t%s(%%rip), %%dil\n",q->arg2.c_str());
								ch = 'b';	
							}
							else{
								if(ent->size==8){
									if(ent->type->ntype=="double")printf("\tmovsd\t%s(%%rip), %%xmm1\n",q->arg2.c_str());
									else printf("\tmovq\t%s(%%rip), %%rdi\n",q->arg2.c_str());
									ch = 'q';
								}
							}
						}
					}
				}
				
				switch(ch){
					case 'b': 
						if(f1==1)printf("\tmovb\t$%s, %%sil\n",q->arg1.c_str());
						printf("\timulb\t%%dil, %%sil\n"); 
						break;
					case 'l': 
						if(f1==1)printf("\tmovl\t$%s, %%esi\n",q->arg1.c_str());
						printf("\timull\t%%edi, %%esi\n"); 
						break;
					case 'q':
						if(f1==1)printf("\tmovq\t$%s, %%rsi\n",q->arg1.c_str());
						ent1 = this->find(q->arg1);
						if(ent1==NULL)ent1 = global_symtab->find(q->arg1);
						if(ent1->type->ntype=="double")printf("\tmulsd\t%%xmm1, %%xmm0\n");
						else printf("\timulq\t%%rdi, %%rsi\n"); 
						break;
				}
				
				ch = art->getregch(q->result);
				ent1 = this->find(q->result);
				if(ent1==NULL)ent1 = global_symtab->find(q->result);
				if(ch!='a'){
					switch(ch){
						case 'b': printf("\tmovb\t%%sil, %d(%%rbp)\n",art->getoffset(q->result)); break;
						case 'l': printf("\tmovl\t%%esi, %d(%%rbp)\n",art->getoffset(q->result)); break;
						case 'q': 
							if(ent1->type->ntype=="double")printf("\tmovsd\t%%xmm0, %d(%%rbp)\n",art->getoffset(q->result));
							else printf("\tmovq\t%%rsi, %d(%%rbp)\n",art->getoffset(q->result)); 
							break;
					}
				}
				else{
					ent = global_symtab->find(q->result);
					if(ent){
						if(ent->size==4){
							printf("\tmovl\t%%esi, %s(%%rip)\n",q->result.c_str());
							ch = 'l';
						}
						else{
							if(ent->size==1){
								printf("\tmovb\t%%sil, %s(%%rip)\n",q->result.c_str());
								ch = 'b';	
							}
							else{
								if(ent->size==8){	
									if(ent->type->ntype=="double")printf("\tmovsd\t%%xmm0, %s(%%rip)\n",q->result.c_str());	
									else printf("\tmovq\t%%rsi, %s(%%rip)\n",q->result.c_str());
									ch = 'q';
								}
							}
						}
					}
				}
			}
			
			if(q->opcode=="/" || q->opcode=="%"){
				ch = art->getregch(q->arg1);
				ent1 = this->find(q->arg1);
				if(ent1==NULL)ent1 = global_symtab->find(q->arg1); 
				if(ch!='a'){
					switch(ch){
						case 'b': printf("\tmovsbq\t%d(%%rbp), %%rax\n",art->getoffset(q->arg1)); break;
						case 'l': printf("\tmovslq\t%d(%%rbp), %%rax\n",art->getoffset(q->arg1)); break;
						case 'q': 
							if(ent1->type->ntype=="double")printf("\tmovsd\t%d(%%rbp), %%xmm0\n",art->getoffset(q->arg1)); 
							else printf("\tmovq\t%d(%%rbp), %%rax\n",art->getoffset(q->arg1));
							break;
					}
				}
				else{
					ent = global_symtab->find(q->arg1);
					if(ent){
						if(ent->size==4){
							printf("\tmovslq\t%s(%%rip), %%rax\n",q->arg1.c_str()); 
							ch = 'l';	
						}
						else{
							if(ent->size==1){
								printf("\tmovsbq\t%s(%%rip), %%rax\n",q->arg1.c_str());
								ch = 'b';
							}
							else{
								if(ent->size==8){
									if(ent->type->ntype=="double")printf("\tmovsd\t%s(%%rip), %%xmm0\n",q->arg1.c_str());
									else printf("\tmovq\t%d(%%rbp), %%rax\n",art->getoffset(q->arg1));
									ch = 'q';
								}
							}
						}
					}
				}
				
				ch = art->getregch(q->arg2);
				ent1 = this->find(q->arg2);
				if(ent1==NULL)ent1 = global_symtab->find(q->arg2); 
				if(ch!='a'){
					switch(ch){
						case 'b': printf("\tmovsbq\t%d(%%rbp), %%r8\n",art->getoffset(q->arg2)); break;
						case 'l': printf("\tmovslq\t%d(%%rbp), %%r8\n",art->getoffset(q->arg2)); break;
						case 'q': 
							if(ent1->type->ntype=="double")printf("\tmovsd\t%d(%%rbp), %%xmm1\n",art->getoffset(q->arg2)); 
							else printf("\tmovq\t%d(%%rbp), %%r8\n",art->getoffset(q->arg2));
							break;
					}
				}
				else{
					ent = global_symtab->find(q->arg2);
					if(ent){
						if(ent->size==4){
							printf("\tmovslq\t%s(%%rip), %%r8\n",q->arg2.c_str());
							ch = 'l';
						}
						else{
							if(ent->size==1){
								printf("\tmovsbq\t%s(%%rip), %%r8\n",q->arg2.c_str());
								ch = 'b';	
							}
							else{
								if(ent->size==8){
									if(ent->type->ntype=="double")printf("\tmovsd\t%s(%%rip), %%xmm1\n",q->arg2.c_str());
									else printf("\tmovq\t%s(%%rip), %%r8\n",q->arg2.c_str());
									ch = 'q';
								}
							}
						}	
					}
				}
				
				printf("\tmovq\t$0, %%rdx\n");
				ent1 = this->find(q->arg1);
				if(ent1==NULL)ent1 = global_symtab->find(q->arg1);
				if(ent1->type->ntype=="double")printf("\tdivsd\t%%xmm1, %%xmm0\n");
				else printf("\tidivq\t%%r8\n");
				
				if(q->opcode=="/"){
					ch = art->getregch(q->result);
					ent1 = this->find(q->result);
					if(ent1==NULL)ent1 = global_symtab->find(q->result);
					if(ch!='a'){
						switch(ch){		
							case 'b': printf("\tmovb\t%%al, %d(%%rbp)\n",art->getoffset(q->result)); break;
							case 'l': printf("\tmovl\t%%eax, %d(%%rbp)\n",art->getoffset(q->result)); break;
							case 'q': 
								if(ent1->type->ntype=="double")printf("\tmovsd\t%%xmm0, %d(%%rbp)\n",art->getoffset(q->result)); 
								else printf("\tmovq\t%%rax, %d(%%rbp)\n",art->getoffset(q->result));
								break;
						}
					}
					else{
						ent = global_symtab->find(q->result);
						if(ent){
							if(ent->size==4){
								printf("\tmovl\t%%eax, %s(%%rip)\n",q->result.c_str());
								ch = 'l';
							}
							else{
								if(ent->size==1){
									printf("\tmovb\t%%al, %s(%%rip)\n",q->result.c_str());
									ch = 'b';	
								}
								else{
									if(ent->size==8){
										if(ent->type->ntype=="double")printf("\tmovsd\t%%xmm0, %s(%%rip)\n",q->result.c_str());
										else printf("\tmovq\t%%rax, %s(%%rip)\n",q->result.c_str());
										ch = 'q';
									}
								}
							}
						}
					}
				}
				else{
					ch = art->getregch(q->result);
					if(ch!='a'){
						switch(ch){
							case 'b': printf("\tmovb\t%%dl, %d(%%rbp)\n",art->getoffset(q->result)); break;
							case 'l': printf("\tmovl\t%%edx, %d(%%rbp)\n",art->getoffset(q->result)); break;
							case 'q': printf("\tmovq\t%%rdx, %d(%%rbp)\n",art->getoffset(q->result)); break;
						}
					}
					else{
						ent = global_symtab->find(q->result);
						if(ent){
							if(ent->size==4){
								printf("\tmovl\t%%edx, %s(%%rip)\n",q->result.c_str());
								ch = 'l';
							}
							else{
								if(ent->size==1){
									printf("\tmovb\t%%dl, %s(%%rip)\n",q->result.c_str());
									ch = 'b';	
								}
								else{
									if(ent->size==8){
										printf("\tmovq\t%%rdx, %s(%%rip)\n",q->result.c_str());
										ch = 'q';
									}
								}
							}
						}
					}
				}							
			}			
		}
		else{			
			if(q->arg1!="" && q->arg2=="" && (q->opcode=="-" || q->opcode=="+" || q->opcode=="")){
				if(q->opcode=="-"){
					ch = art->getregch(q->arg1);
					ent1 = this->find(q->arg1);
					if(ent1==NULL)ent1 = global_symtab->find(q->arg1);
					if(ch!='a'){
						switch(ch){
							case 'b': printf("\tmovb\t%d(%%rbp), %%r9b\n",art->getoffset(q->arg1)); break;
							case 'l': printf("\tmovl\t%d(%%rbp), %%r9d\n",art->getoffset(q->arg1)); break;
							case 'q': 
								if(ent1->type->ntype=="double")printf("\tmovsd\t%d(%%rbp), %%xmm0\n",art->getoffset(q->arg1)); 
								else printf("\tmovq\t%d(%%rbp), %%r9\n",art->getoffset(q->arg1));
								break;
						}
					}
					else{
						ent = global_symtab->find(q->arg1);
						if(ent){
							if(ent->size==4){
								printf("\tmovl\t%s(%%rip), %%r9d\n",q->arg1.c_str());
								ch = 'l';	
							}
							else{
								if(ent->size==1){
									printf("\tmovb\t%s(%%rip), %%r9b\n",q->arg1.c_str());
									ch = 'b';
								}
								else{
									if(ent->size==8){
										if(ent->type->ntype=="double")printf("\tmovsd\t%s(%%rip), %%xmm0\n",q->arg1.c_str());
										else printf("\tmovq\t%s(%%rip), %%r9\n",q->arg1.c_str());
										ch = 'q';
									}
								}
							}
						}
					}
					
					switch(ch){
						case 'b': printf("\tnegb\t%%r9b\n"); break;
						case 'l': printf("\tnegl\t%%r9d\n"); break;
						case 'q': 
							ent1 = this->find(q->arg1);
							if(ent1==NULL)ent1 = global_symtab->find(q->arg1);
							if(ent1->type->ntype=="double"){
								printf("\txorps\t%%xmm1, %%xmm1\n");
								printf("\tsubsd\t%%xmm0, %%xmm1\n");
							}
							else printf("\tnegq\t%%r9\n"); 
							break;
					}
					
					ch = art->getregch(q->result);
					ent1 = this->find(q->result);
					if(ent1==NULL)ent1 = global_symtab->find(q->result);
					if(ch!='a'){
						switch(ch){		
							case 'b': printf("\tmovb\t%%r9b, %d(%%rbp)\n",art->getoffset(q->result)); break;
							case 'l': printf("\tmovl\t%%r9d, %d(%%rbp)\n",art->getoffset(q->result)); break;
							case 'q': 
								if(ent1->type->ntype=="double")printf("\tmovsd\t%%xmm1, %d(%%rbp)\n",art->getoffset(q->result)); 
								else printf("\tmovq\t%%r9, %d(%%rbp)\n",art->getoffset(q->result));
								break;
						}
					}
					else{
						ent = global_symtab->find(q->result);
						if(ent){
							if(ent->size==4){
								printf("\tmovl\t%%r9d, %s(%%rip)\n",q->result.c_str());
								ch = 'l';
							}
							else{
								if(ent->size==1){
									printf("\tmovb\t%%r9b, %s(%%rip)\n",q->result.c_str());
									ch = 'b';	
								}
								else{
									if(ent->size==8){
										if(ent->type->ntype=="double")printf("\tmovsd\t%%xmm1, %s(%%rip)\n",q->result.c_str());
										else printf("\tmovq\t%%r9, %s(%%rip)\n",q->result.c_str());
										ch = 'q';
									}
								}
							}
						}
					}
				}
				else{
					if(q->opcode=="+" || q->opcode==""){
						ent = this->find(q->result);
						if(ent){
							if(ent->type->ntype=="string"){
								printf("\tmovq\t$.LC%d, %d(%%rbp)\n",ent->type->numelem,art->getoffset(q->result));
								q = q->next;
								i++;
								continue;
							}
						}
						
						ent = this->find(q->arg1);
						if(ent){
							if(ent->type->ntype=="string"){
								printf("\tmovq\t%d(%%rbp), %%rax\n",art->getoffset(q->arg1));
								printf("\tmovq\t%%rax, %d(%%rbp)\n",art->getoffset(q->result));
								q = q->next;
								i++;
								continue;
							}
						}
						
						f1 = 0;
						ch = art->getregch(q->arg1);
						ent1 = this->find(q->arg1);
						if(ent1==NULL)ent1 = global_symtab->find(q->arg1);
						if(ch!='a'){
							switch(ch){
								case 'b': printf("\tmovb\t%d(%%rbp), %%r10b\n",art->getoffset(q->arg1)); break;
								case 'l': printf("\tmovl\t%d(%%rbp), %%r10d\n",art->getoffset(q->arg1)); break;
								case 'q': 
									if(ent1->type->ntype=="double")printf("\tmovsd\t%d(%%rbp), %%xmm0\n",art->getoffset(q->arg1)); 
									else printf("\tmovq\t%d(%%rbp), %%r10\n",art->getoffset(q->arg1));
									break;
							}
						}
						else{
							ent = global_symtab->find(q->arg1);
							if(ent){
								if(ent->size==4){
									printf("\tmovl\t%s(%%rip), %%r10d\n",q->arg1.c_str());
									ch = 'l';	
								}
								else{
									if(ent->size==1){
										printf("\tmovb\t%s(%%rip), %%r10b\n",q->arg1.c_str());
										ch = 'b';
									}
									else{
										if(ent->size==8){
											if(ent->type->ntype=="double")printf("\tmovsd\t%s(%%rip), %%xmm0\n",q->arg1.c_str());
											else printf("\tmovq\t%s(%%rip), %%r10\n",q->arg1.c_str());
											ch = 'q';
										}
									}
								}
							}
							else f1=1;
						}
						
						ch = art->getregch(q->result);
						ent1 = this->find(q->result);
						if(ent1==NULL)ent1 = global_symtab->find(q->result);
						if(ch!='a'){
							switch(ch){
								case 'b': 
									if(f1==1){printf("\tmovb\t$%s, %d(%%rbp)\n",q->arg1.c_str(),art->getoffset(q->result)); f1=0;}
									else printf("\tmovb\t%%r10b, %d(%%rbp)\n",art->getoffset(q->result)); 
									break;
								case 'l': 
									if(f1==1){
										/*if((q->arg1).find('.')!=string::npos)printf("\tmovl\t$%s, %d(%%rbp)\n",getRep(atof(q->arg1)),art->getoffset(q->result));
										else*/ printf("\tmovl\t$%s, %d(%%rbp)\n",q->arg1.c_str(),art->getoffset(q->result));
										f1=0;
									}
									else printf("\tmovl\t%%r10d, %d(%%rbp)\n",art->getoffset(q->result)); 
									break;
								case 'q': 
									if(f1==1){
										//char bitrep[19];
										//getRep((double)(atof(q->arg1.c_str())),bitrep);
										printf("\tmovsd\t.LC%d(%%rip), %%xmm3\n",ent1->type->numelem);
										printf("\tmovsd\t%%xmm3, %d(%%rbp)\n",art->getoffset(q->result)); 
										f1=0;
									}
									else {
										if(ent1->type->ntype=="double")printf("\tmovsd\t%%xmm0, %d(%%rbp)\n",art->getoffset(q->result));
										else printf("\tmovq\t%%r10, %d(%%rbp)\n",art->getoffset(q->result));	
									} 
									break;
							}
						}
						else{
							ent = global_symtab->find(q->result);
							if(ent){
								if(ent->size==4){
									if(f1==1){
										/*if((q->arg1).find('.')!=string::npos)printf("\tmovl\t$%s, %s(%%rip)\n",getRep(atof(q->arg1)),q->result.c_str()); 
										else*/ printf("\tmovl\t$%s, %s(%%rip)\n",q->arg1.c_str(),q->result.c_str()); 
										f1=0;
									}
									else printf("\tmovl\t%%r10d, %s(%%rip)\n",q->result.c_str());
									ch = 'l';
								}
								else{
									if(ent->size==1){
										if(f1==1){printf("\tmovb\t$%s, %s(%%rip)\n",q->arg1.c_str(),q->result.c_str()); f1=0;}
										else printf("\tmovb\t%%r10b, %s(%%rip)\n",q->result.c_str());
										ch = 'b';	
									}
									else{
										if(ent->size==8){
											if(f1==1){
												//char bitrep[19];
												//getRep((double)(atof(q->arg1.c_str())),bitrep);
												printf("\tmovsd\t.LC%d(%%rip), %%xmm3\n",ent->type->numelem);
												printf("\tmovsd\t%%xmm3, %s(%%rip)\n",q->result.c_str()); 
												f1=0;
											}
											else {
												if(ent1->type->ntype=="double")printf("\tmovsd\t%%xmm0, %s(%%rip)\n",q->result.c_str());
												else printf("\tmovq\t%%r10, %s(%%rip)\n",q->result.c_str());	
											}
											ch = 'q';
										}
									}
								}
							}
						}
					}
				}
			}
			else{
				if(q->arg1=="" && q->arg2=="" && q->opcode==""){
					printf("\tjmp\t.L%s\n",q->result.c_str());
				}
				else{
					if(q->opcode=="iflt" || q->opcode=="iflte" || q->opcode=="ifgt" || q->opcode=="ifgte" || q->opcode=="ifeq" || q->opcode=="ifneq"){
						ch = art->getregch(q->arg1);
						ent1 = this->find(q->arg1);
						if(ent1==NULL)ent1 = global_symtab->find(q->arg1);
						if(ch!='a'){
							switch(ch){
								case 'b': printf("\tmovb\t%d(%%rbp), %%r13b\n",art->getoffset(q->arg1)); break;
								case 'l': printf("\tmovl\t%d(%%rbp), %%r13d\n",art->getoffset(q->arg1)); break;
								case 'q': 
									if(ent1->type->ntype=="double")printf("\tmovsd\t%d(%%rbp), %%xmm0\n",art->getoffset(q->arg1)); 
									else printf("\tmovq\t%d(%%rbp), %%r13\n",art->getoffset(q->arg1));
									break;
							}
						}
						else{
							ent = global_symtab->find(q->arg1);
							if(ent){
								if(ent->size==4){
									printf("\tmovl\t%s(%%rip), %%r13d\n",q->arg1.c_str());
									ch = 'l';	
								}
								else{
									if(ent->size==1){
										printf("\tmovb\t%s(%%rip), %%r13b\n",q->arg1.c_str());
										ch = 'b';
									}
									else{
										if(ent->size==8){
											if(ent->type->ntype=="double")printf("\tmovsd\t%s(%%rip), %%xmm0\n",q->arg1.c_str());
											else printf("\tmovq\t%s(%%rip), %%r13\n",q->arg1.c_str());
											ch = 'q';
										}
									}
								}
							}
						}
						
						ch = art->getregch(q->arg2);
						ent1 = this->find(q->arg2);
						if(ent1==NULL)ent1 = global_symtab->find(q->arg2);
						if(ch!='a'){
							switch(ch){
								case 'b': printf("\tmovb\t%d(%%rbp), %%r14b\n",art->getoffset(q->arg2)); break;
								case 'l': printf("\tmovl\t%d(%%rbp), %%r14d\n",art->getoffset(q->arg2)); break;
								case 'q': 
									if(ent1->type->ntype=="double")printf("\tmovsd\t%d(%%rbp), %%xmm1\n",art->getoffset(q->arg2)); 
									else printf("\tmovq\t%d(%%rbp), %%r14\n",art->getoffset(q->arg2));
									break;
							}
						}
						else{
							ent = global_symtab->find(q->arg2);
							if(ent){
								if(ent->size==4){
									printf("\tmovl\t%s(%%rip), %%r14d\n",q->arg2.c_str());
									ch = 'l';
								}
								else{
									if(ent->size==1){
										printf("\tmovb\t%s(%%rip), %%r14b\n",q->arg2.c_str());
										ch = 'b';	
									}
									else{
										if(ent->size==8){
											if(ent->type->ntype=="double")printf("\tmovsd\t%s(%%rip), %%xmm1\n",q->arg2.c_str());
											else printf("\tmovq\t%s(%%rip), %%r14\n",q->arg2.c_str());
											ch = 'q';
										}
									}
								}
							}
						}
						
						switch(ch){
							case 'b': printf("\tcmpb\t%%r14b, %%r13b\n"); break;
							case 'l': printf("\tcmpl\t%%r14d, %%r13d\n"); break;
							case 'q': 
								ent1 = this->find(q->arg1);
								if(ent1==NULL)ent1 = global_symtab->find(q->arg1);
								if(ent1->type->ntype=="double")printf("\tucomisd\t%%xmm1, %%xmm0\n");
								else printf("\tcmpq\t%%r14, %%r13\n"); 
								break;
						}
						
						ent1 = this->find(q->arg1);
						if(ent1==NULL)ent1 = global_symtab->find(q->arg1);
						
						if(ent1->type->ntype=="double"){
							if(q->opcode=="iflt")printf("\tjb\t.L%s\n",q->result.c_str());
							if(q->opcode=="iflte")printf("\tjbe\t.L%s\n",q->result.c_str());
							if(q->opcode=="ifgt")printf("\tja\t.L%s\n",q->result.c_str());
							if(q->opcode=="ifgte")printf("\tjae\t.L%s\n",q->result.c_str());
							if(q->opcode=="ifeq")printf("\tje\t.L%s\n",q->result.c_str());
							if(q->opcode=="ifneq")printf("\tjp\t.L%s\n",q->result.c_str());
						}
						else{		
							if(q->opcode=="iflt")printf("\tjl\t.L%s\n",q->result.c_str());
							if(q->opcode=="iflte")printf("\tjle\t.L%s\n",q->result.c_str());
							if(q->opcode=="ifgt")printf("\tjg\t.L%s\n",q->result.c_str());
							if(q->opcode=="ifgte")printf("\tjge\t.L%s\n",q->result.c_str());
							if(q->opcode=="ifeq")printf("\tje\t.L%s\n",q->result.c_str());
							if(q->opcode=="ifneq")printf("\tjne\t.L%s\n",q->result.c_str());
						}
					}
					else{
						if(q->opcode=="iftrue" || q->opcode=="ifFalse"){
							ch = art->getregch(q->arg1);
							ent1 = this->find(q->arg1);
							if(ent1==NULL)ent1 = global_symtab->find(q->arg1);
							if(ch!='a'){
								switch(ch){
									case 'b': printf("\tmovb\t%d(%%rbp), %%r13b\n",art->getoffset(q->arg1)); break;
									case 'l': printf("\tmovl\t%d(%%rbp), %%r13d\n",art->getoffset(q->arg1)); break;
									case 'q': 
										if(ent1->type->ntype=="double")printf("\tmovsd\t%d(%%rbp), %%xmm0\n",art->getoffset(q->arg1));
										else printf("\tmovq\t%d(%%rbp), %%r13\n",art->getoffset(q->arg1)); 
										break;
								}
							}
							else{
								ent = global_symtab->find(q->arg1);
								if(ent){
									if(ent->size==4){
										printf("\tmovl\t%s(%%rip), %%r13d\n",q->arg1.c_str());
										ch = 'l';	
									}
									else{
										if(ent->size==1){
											printf("\tmovb\t%s(%%rip), %%r13b\n",q->arg1.c_str());
											ch = 'b';
										}
										else{
											if(ent->size==8){
												if(ent->type->ntype=="double")printf("\tmovsd\t%s(%%rip), %%xmm0\n",q->arg1.c_str());
												else printf("\tmovq\t%s(%%rip), %%r13\n",q->arg1.c_str());
												ch = 'q';
											}
										}
									}
								}
							}
							
							ent1 = this->find(q->arg1);
							if(ent1==NULL)ent1 = global_symtab->find(q->arg1);
							
							switch(ch){
								case 'b': printf("\tcmpb\t$0, %%r13b\n"); break;
								case 'l': printf("\tcmpl\t$0, %%r13d\n"); break;
								case 'q': 
									if(ent1->type->ntype=="double"){
										printf("\txorpl\t%%xmm1, %%xmm1\n");
										printf("\tucomisd\t%%xmm1, %%xmm0\n");
									}
									else printf("\tcmpq\t$0, %%r13\n"); 
									break;
							}
						
							if(q->opcode=="iftrue"){
								if(ent1->type->ntype=="double")printf("\tjp\t.L%s\n",q->result.c_str());
								else printf("\tjne\t.L%s\n",q->result.c_str());
							}
							else printf("\tje\t.L%s\n",q->result.c_str());
						}
						else{
							if(q->opcode=="lindexed" || q->opcode=="rindexed"){
								if(q->opcode=="lindexed"){
									ch = art->getregch(q->arg2);
									ent1 = this->find(q->arg2);
									if(ent1==NULL)ent1 = global_symtab->find(q->arg2);
									if(ch!='a'){
										switch(ch){
											case 'b': printf("\tmovb\t%d(%%rbp), %%r15b\n",art->getoffset(q->arg2)); break;
											case 'l': printf("\tmovl\t%d(%%rbp), %%r15d\n",art->getoffset(q->arg2)); break;
											case 'q': 
												if(ent1->type->ntype=="double")printf("\tmovsd\t%d(%%rbp), %%xmm1\n",art->getoffset(q->arg2)); 
												else printf("\tmovq\t%d(%%rbp), %%r15\n",art->getoffset(q->arg2));
												break;
										}
									}
									else{
										ent = global_symtab->find(q->arg2);
										if(ent){
											if(ent->size==4){
												printf("\tmovl\t%s(%%rip), %%r15d\n",q->arg2.c_str());
												ch = 'l';
											}
											else{
												if(ent->size==1){
													printf("\tmovb\t%s(%%rip), %%r15b\n",q->arg2.c_str());
													ch = 'b';	
												}
												else{
													if(ent->size==8){
														if(ent->type->ntype=="double")printf("\tmovsd\t%s(%%rip), %%xmm1\n",q->arg2.c_str());
														else printf("\tmovq\t%s(%%rip), %%r15\n",q->arg2.c_str());
														ch = 'q';
													}
												}
											}
										}
									}
									
									off = art->getoffset(q->result);
									ent1 = this->find(q->arg2);
									if(ent1==NULL)ent1 = global_symtab->find(q->arg2);
									if(off!=0){
										ent2 = this->find(q->arg1);
										if(ent2){
											printf("\tmovl\t%d(%%rbp), %%r14d\n",art->getoffset(q->arg1));
											printf("\taddq\t%%rbp, %%r14\n");
											printf("\tmovsd\t%%xmm1, %d(%%r14)\n",off);
										}
										else{
											ent2 = global_symtab->find(q->arg1);
											if(ent2){
												printf("\tmovl\t%s(%%rip), %%r14d\n",q->arg1.c_str());
												printf("\taddq\t%%rbp, %%r14\n");
												printf("\tmovsd\t%%xmm1, %d(%%r14)\n",off);
											}
											else{
												stringstream ss(q->arg1);
												int x;
												ss>>x;
												switch(ch){
													case 'b': printf("\tmovb\t%%r15b, %d(%%rbp)\n",off+x); break;
													case 'l': printf("\tmovl\t%%r15d, %d(%%rbp)\n",off+x); break;
													case 'q': 
														if(ent1->type->ntype=="double")printf("\tmovsd\t%%xmm1, %d(%%rbp)\n",off+x); 
														else printf("\tmovq\t%%r15, %d(%%rbp)\n",off+x);
														break;
												}
											}
										}
									}
									else{
										ent = global_symtab->find(q->result);
										if(ent){
											ent2 = this->find(q->arg1);
											if(ent2){
												printf("\tmovl\t%d(%%rbp), %%r14d\n",art->getoffset(q->arg1));
												printf("\taddq\t%%rip, %%r14\n");
												printf("\tmovsd\t%%xmm1, %s+0(%%r14)\n",q->result.c_str());
											}
											else{
												ent2 = global_symtab->find(q->arg1);
												if(ent2){
													printf("\tmovl\t%s(%%rip), %%r14d\n",q->arg1.c_str());
													printf("\taddq\t%%rip, %%r14\n");
													printf("\tmovsd\t%%xmm1, %s+0(%%r14)\n",q->result.c_str());
												}
												else{
													stringstream ss(q->arg1);
													int x;
													ss>>x;
													switch(ch){
														case 'b': printf("\tmovb\t%%r15b, %s+%d(%%rip)\n",q->result.c_str(),x); break;
														case 'l': printf("\tmovl\t%%r15d, %s+%d(%%rip)\n",q->result.c_str(),x); break;
														case 'q': 
															if(ent1->type->ntype=="double")printf("\tmovsd\t%%xmm1, %s+%d(%%rip)\n",q->result.c_str(),x); 
															else printf("\tmovq\t%%r15, %s+%d(%%rip)\n",q->result.c_str(),x);
															break;
													}
												}
											}
										}
									}
								}
								else{
									off = art->getoffset(q->arg1);
									ch = art->getregch(q->result);
									if(off!=0){
										ent1 = this->find(q->arg2);
										if(ent1){
											printf("\tmovl\t%d(%%rbp), %%r9d\n",art->getoffset(q->arg2));
											printf("\taddq\t%%rbp, %%r9\n");
											printf("\tmovsd\t%d(%%r9), %%xmm0\n",off);
										}
										else{
											ent1 = global_symtab->find(q->arg2);
											if(ent1){
												printf("\tmovl\t%s(%%rip), %%r9d\n",q->arg2.c_str());
												printf("\taddq\t%%rbp, %%r9\n");
												printf("\tmovsd\t%d(%%r9), %%xmm0\n",off);
											}
											else{
												stringstream ss(q->arg2);
												int x;
												ss>>x;
												switch(ch){
													case 'b': printf("\tmovb\t%d(%%rbp), %%r8b\n",off+x); break;
													case 'l': printf("\tmovl\t%d(%%rbp), %%r8d\n",off+x); break;
													case 'q': printf("\tmovsd\t%d(%%rbp), %%xmm0\n",off+x); break;
												}
											}
										}
									}
									else{
										ent = global_symtab->find(q->arg1);
										if(ent){
											ent1 = this->find(q->arg2);
											if(ent1){
												printf("\tmovl\t%d(%%rbp), %%r9d\n",art->getoffset(q->arg2));
												printf("\taddq\t%%rip, %%r9\n");
												printf("\tmovsd\t%s+0(%%r9), %%xmm0\n",q->arg1.c_str());
											}
											else{
												ent1 = global_symtab->find(q->arg2);
												if(ent1){
													printf("\tmovl\t%s(%%rip), %%r9d\n",q->arg2.c_str());
													printf("\taddq\t%%rip, %%r9\n");
													printf("\tmovsd\t%s+0(%%r9), %%xmm0\n",q->arg1.c_str());
												}
												else{
													stringstream ss(q->arg2);
													int x;
													ss>>x;
													switch(ch){
														case 'b': printf("\tmovb\t%s+%d(%%rip), %%r8b\n",q->arg1.c_str(),x); break;
														case 'l': printf("\tmovl\t%s+%d(%%rip), %%r8d\n",q->arg1.c_str(),x); break;
														case 'q': printf("\tmovsd\t%s+%d(%%rip), %%xmm0\n",q->arg1.c_str(),x); break;
													}
												}
											}
										}
									}
									
									if(ch!='a'){
										switch(ch){
											case 'b': printf("\tmovb\t%%r8b, %d(%%rbp)\n",art->getoffset(q->result)); break;
											case 'l': printf("\tmovl\t%%r8d, %d(%%rbp)\n",art->getoffset(q->result)); break;
											case 'q': printf("\tmovsd\t%%xmm0, %d(%%rbp)\n",art->getoffset(q->result)); break;
										}
									}
									else{
										ent = global_symtab->find(q->result);
										if(ent){
											if(ent->size==4){
												printf("\tmovl\t%%r8d, %s(%%rip)\n",q->result.c_str());
												ch = 'l';
											}
											else{
												if(ent->size==1){
													printf("\tmovb\t%%r8b, %s(%%rip)\n",q->result.c_str());
													ch = 'b';	
												}
												else{
													if(ent->size==8){
														printf("\tmovsd\t%%xmm0, %s(%%rip)\n",q->result.c_str());
														ch = 'q';
													}
												}
											}
										}
									}
								}
							}
							else{
								if(q->opcode=="ref" || q->opcode=="ldref" || q->opcode=="rdref"){
									if(q->opcode=="ref"){
										ch = art->getregch(q->result);
										off = art->getoffset(q->arg1);
										
										if(ch!='a'){
											switch(ch){
												case 'b': printf("\tleab\t%d(%%bpl), %%r8b\n",off); break;
												case 'l': printf("\tleal\t%d(%%ebp), %%r8d\n",off); break;
												case 'q': printf("\tleaq\t%d(%%rbp), %%r8\n",off); break;
											}
											//printf("\tmovl\t%%ebp, %%r8d\n");
											//printf("\taddl\t$%d, %%r8d\n",off);
										}
										/*else{
											//TODO
										}*/
										
										ch = art->getregch(q->result);
										if(ch!='a'){
											switch(ch){		
												case 'b': printf("\tmovb\t%%r8b, %d(%%rbp)\n",art->getoffset(q->result)); break;
												case 'l': printf("\tmovl\t%%r8d, %d(%%rbp)\n",art->getoffset(q->result)); break;
												case 'q': printf("\tmovq\t%%r8, %d(%%rbp)\n",art->getoffset(q->result)); break;
											}
										}
										else{
											ent = global_symtab->find(q->result);
											if(ent){
												if(ent->size==4){
													printf("\tmovl\t%%r8d, %s(%%rip)\n",q->result.c_str());
													ch = 'l';
												}
												else{
													if(ent->size==1){
														printf("\tmovb\t%%r8b, %s(%%rip)\n",q->result.c_str());
														ch = 'b';	
													}
													else{
														if(ent->size==8){
															printf("\tmovq\t%%r8, %s(%%rip)\n",q->result.c_str());
															ch = 'q';
														}
													}
												}
											}
										}
									}
									else{
										if(q->opcode=="ldref"){											
											ch = art->getregch(q->result);
											if(ch!='a'){
												switch(ch){
													case 'b': printf("\tmovb\t%d(%%rbp), %%r15b\n",art->getoffset(q->result)); break;
													case 'l': printf("\tmovl\t%d(%%rbp), %%r15d\n",art->getoffset(q->result)); break;
													case 'q': printf("\tmovq\t%d(%%rbp), %%r15\n",art->getoffset(q->result)); break;
												}
											}
											else{
												ent = global_symtab->find(q->result);
												if(ent){
													if(ent->size==4){
														printf("\tmovl\t%s(%%rip), %%r15d\n",q->result.c_str());
														ch = 'l';
													}
													else{
														if(ent->size==1){
															printf("\tmovb\t%s(%%rip), %%r15b\n",q->result.c_str());
															ch = 'b';	
														}
														else{
															if(ent->size==8){
																printf("\tmovq\t%s(%%rip), %%r15\n",q->result.c_str());
																ch = 'q';
															}
														}
													}
												}
											}
											
											ch = art->getregch(q->arg1);
											ent1 = this->find(q->arg1);
											if(ent1==NULL)ent1 = global_symtab->find(q->arg1);
											if(ch!='a'){
												switch(ch){
													case 'b': printf("\tmovb\t%d(%%rbp), %%r13b\n",art->getoffset(q->arg1)); break;
													case 'l': printf("\tmovl\t%d(%%rbp), %%r13d\n",art->getoffset(q->arg1)); break;
													case 'q': 
														if(ent1->type->ntype=="double")printf("\tmovsd\t%d(%%rbp), %%xmm0\n",art->getoffset(q->arg1));
														else printf("\tmovq\t%d(%%rbp), %%r13\n",art->getoffset(q->arg1)); 
														break;
												}
											}
											else{
												ent = global_symtab->find(q->arg1);
												if(ent){
													if(ent->size==4){
														printf("\tmovl\t%s(%%rip), %%r13d\n",q->arg1.c_str());
														ch = 'l';	
													}
													else{
														if(ent->size==1){
															printf("\tmovb\t%s(%%rip), %%r13b\n",q->arg1.c_str());
															ch = 'b';
														}
														else{
															if(ent->size==8){
																if(ent->type->ntype=="double")printf("\tmovsd\t%s(%%rip), %%xmm0\n",q->arg1.c_str());
																else printf("\tmovq\t%s(%%rip), %%r13\n",q->arg1.c_str());
																ch = 'q';
															}
														}
													}
												}
											}
											
											switch(ch){
												case 'b': printf("\tmovb\t%%r13b, 0(%%r15d)\n"); break;
												case 'l': printf("\tmovl\t%%r13d, 0(%%r15d)\n"); break;
												case 'q': 
													if(ent1->type->ntype=="double")printf("\tmovsd\t%%xmm0, 0(%%r15d)\n");
													else printf("\tmovq\t%%r13, 0(%%r15d)\n"); 
													break;
											}
										}
										else{
											ch = art->getregch(q->arg1);
											if(ch!='a'){
												switch(ch){
													case 'b': printf("\tmovb\t%d(%%rbp), %%r10b\n",art->getoffset(q->arg1)); break;
													case 'l': printf("\tmovl\t%d(%%rbp), %%r10d\n",art->getoffset(q->arg1)); break;
													case 'q': printf("\tmovq\t%d(%%rbp), %%r10\n",art->getoffset(q->arg1)); break;
												}
											}
											else{
												ent = global_symtab->find(q->arg1);
												if(ent){
													if(ent->size==4){
														printf("\tmovl\t%s(%%rip), %%r10d\n",q->arg1.c_str());
														ch = 'l';	
													}
													else{
														if(ent->size==1){
															printf("\tmovb\t%s(%%rip), %%r10b\n",q->arg1.c_str());
															ch = 'b';
														}
														else{
															if(ent->size==8){
																printf("\tmovq\t%s(%%rip), %%r10\n",q->arg1.c_str());
																ch = 'q';
															}
														}
													}
												}
											}
											
											ch = art->getregch(q->result);
											ent1 = this->find(q->result);
											if(ent1==NULL)ent1 = global_symtab->find(q->result);
											if(ch!='a'){
												switch(ch){
													case 'b': 
														printf("\tmovb\t0(%%r10b), %%r9b\n"); 
														printf("\tmovb\t%%r9b, %d(%%rbp)\n",art->getoffset(q->result));
														break;
													case 'l': 
														printf("\tmovl\t0(%%r10d), %%r9d\n"); 
														printf("\tmovl\t%%r9d, %d(%%rbp)\n",art->getoffset(q->result));
														break;
													case 'q':
														if(ent1->type->ntype=="double"){
															printf("\tmovsd\t0(%%r10), %%xmm0\n"); 
															printf("\tmovsd\t%%xmm0, %d(%%rbp)\n",art->getoffset(q->result));		
														} 
														else{
															printf("\tmovq\t0(%%r10), %%r9\n"); 
															printf("\tmovq\t%%r9, %d(%%rbp)\n",art->getoffset(q->result));
														}
														break;
												}
											}
											else{
												ent = global_symtab->find(q->result);
												if(ent){
													if(ent->size==4){
														printf("\tmovb\t0(%%r10b), %%r9b\n");
														printf("\tmovl\t%%r9b, %s(%%rip)\n",q->result.c_str());
														ch = 'l';	
													}
													else{
														if(ent->size==1){
															printf("\tmovl\t0(%%r10d), %%r9d\n"); 
															printf("\tmovb\t%%r9d, %s(%%rip)\n",q->result.c_str());
															ch = 'b';
														}
														else{
															if(ent->size==8){
																if(ent->type->ntype=="double"){
																	printf("\tmovq\t0(%%r10), %%xmm0\n"); 
																	printf("\tmovq\t%%xmm0, %s(%%rip)\n",q->result.c_str());
																}
																else{
																	printf("\tmovq\t0(%%r10), %%r9\n"); 
																	printf("\tmovq\t%%r9, %s(%%rip)\n",q->result.c_str());
																}
																ch = 'q';
															}
														}
													}
												}
											}
										}
									}
								}
								else{
									if(q->opcode=="param"){
										//if(param_count==0)art->spillall();
										ent = this->find(q->result);
										ch = art->getregch(q->result);
										off = art->getoffset(q->result);
										param_count++;
										if(param_count<=6){
											switch(param_count){
												case 1:													
													if(off!=0){
														if(ch=='q' || ent->type->ntype=="string"){
															ent1 = this->find(q->result);
															if(ent1->type->ntype=="double")printf("\tmovsd\t%d(%%rbp), %%xmm0\n",off);
															else printf("\tmovq\t%d(%%rbp), %%rdi\n",off);
														}
														else{
															if(ch=='l')printf("\tmovl\t%d(%%rbp), %%edi\n",off);
															else if(ch=='b')printf("\tmovb\t%d(%%rbp), %%dil\n",off);
														}
													}
													else{
														ent = global_symtab->find(q->result);
														if(ent){
															if(ent->size==8)printf("\tmovq\t%s(%%rip), %%rdi\n",q->result.c_str());
															else{
																if(ent->size==4)printf("\tmovl\t%s(%%rip), %%edi\n",q->result.c_str());
																else if(ent->size==8)printf("\tmovb\t%s(%%rip), %%dil\n",q->result.c_str());
															}
														}	
													}
													break;
													
												case 2:
													if(off!=0){
														if(ch=='q' || ent->type->ntype=="string"){
															ent1 = this->find(q->result);
															if(ent1->type->ntype=="double")printf("\tmovsd\t%d(%%rbp), %%xmm1\n",off);
															printf("\tmovq\t%d(%%rbp), %%rsi\n",off);
														}
														else{
															if(ch=='l')printf("\tmovl\t%d(%%rbp), %%esi\n",off);
															else if(ch=='b')printf("\tmovb\t%d(%%rbp), %%sil\n",off);
														}
													}
													else{
														if(ch=='q')printf("\tmovq\t%s(%%rip), %%rsi\n",q->result.c_str());
														else{
															if(ch=='l')printf("\tmovl\t%s(%%rip), %%esi\n",q->result.c_str());
															else printf("\tmovb\t%s(%%rip), %%sil\n",q->result.c_str());
														}	
													}
													break;
													
												case 3:
													if(off!=0){
														if(ch=='q' || ent->type->ntype=="string"){
															ent1 = this->find(q->result);
															if(ent1->type->ntype=="double")printf("\tmovsd\t%d(%%rbp), %%xmm2\n",off);	
															printf("\tmovq\t%d(%%rbp), %%rdx\n",off);
														}
														else{
															if(ch=='l')printf("\tmovl\t%d(%%rbp), %%edx\n",off);
															else if(ch=='b')printf("\tmovb\t%d(%%rbp), %%dl\n",off);
														}
													}
													else{
														if(ch=='q')printf("\tmovq\t%s(%%rip), %%rdx\n",q->result.c_str());
														else{
															if(ch=='l')printf("\tmovl\t%s(%%rip), %%edx\n",q->result.c_str());
															else printf("\tmovb\t%s(%%rip), %%dl\n",q->result.c_str());
														}	
													}
													break;
													
												case 4:
													if(off!=0){
														if(ch=='q' || ent->type->ntype=="string"){
															ent1 = this->find(q->result);
															if(ent1->type->ntype=="double")printf("\tmovsd\t%d(%%rbp), %%xmm3\n",off);	
															printf("\tmovq\t%d(%%rbp), %%rcx\n",off);
														}
														else{
															if(ch=='l')printf("\tmovl\t%d(%%rbp), %%ecx\n",off);
															else if(ch=='b')printf("\tmovb\t%d(%%rbp), %%cl\n",off);
														}
													}
													else{
														if(ch=='q')printf("\tmovq\t%s(%%rip), %%rcx\n",q->result.c_str());
														else{
															if(ch=='l')printf("\tmovl\t%s(%%rip), %%ecx\n",q->result.c_str());
															else printf("\tmovb\t%s(%%rip), %%cl\n",q->result.c_str());
														}	
													}
													break;
													
												case 5:
													if(off!=0){
														if(ch=='q' || ent->type->ntype=="string"){
															ent1 = this->find(q->result);
															if(ent1->type->ntype=="double")printf("\tmovsd\t%d(%%rbp), %%xmm4\n",off);	
															printf("\tmovq\t%d(%%rbp), %%r8\n",off);
														}
														else{
															if(ch=='l')printf("\tmovl\t%d(%%rbp), %%r8d\n",off);
															else if(ch=='b')printf("\tmovb\t%d(%%rbp), %%r8b\n",off);
														}
													}
													else{
														if(ch=='q')printf("\tmovq\t%s(%%rip), %%r8\n",q->result.c_str());
														else{
															if(ch=='l')printf("\tmovl\t%s(%%rip), %%r8d\n",q->result.c_str());
															else printf("\tmovb\t%s(%%rip), %%r8b\n",q->result.c_str());
														}	
													}
													break;
													
												case 6:
													if(off!=0){
														if(ch=='q' || ent->type->ntype=="string"){
															ent1 = this->find(q->result);
															if(ent1->type->ntype=="double")printf("\tmovsd\t%d(%%rbp), %%xmm5\n",off);
															printf("\tmovq\t%d(%%rbp), %%r9\n",off);
														}
														else{
															if(ch=='l')printf("\tmovl\t%d(%%rbp), %%r9d\n",off);
															else if(ch=='b')printf("\tmovb\t%d(%%rbp), %%r9b\n",off);
														}
													}
													else{
														if(ch=='q')printf("\tmovq\t%s(%%rip), %%r9\n",q->result.c_str());
														else{
															if(ch=='l')printf("\tmovl\t%s(%%rip), %%r9d\n",q->result.c_str());
															else printf("\tmovb\t%s(%%rip), %%r9b\n",q->result.c_str());
														}	
													}
													break;
											}	
										}
										else{
											if(ch=='q')printf("\tsubq\t$8, %%rsp\n");
											else{
												if(ch=='l')printf("\tsubq\t$4, %%rsp\n");
												else if(ch=='b')printf("\tsubq\t$1, %%rsp\n");
											}
											
											ch = art->getregch(q->result);
											
											if(ch!='a'){
												switch(ch){
													case 'b': 
														printf("\tmovb\t%d(%%rbp), %%r9b\n",art->getoffset(q->result)); 
														printf("\tmovb\t%%r9b, 0(%%rsp)\n");
														break;
													case 'l': 
														printf("\tmovl\t%d(%%rbp), %%r9d\n",art->getoffset(q->result)); 
														printf("\tmovl\t%%r9d, 0(%%rsp)\n");
														break;
													case 'q':
														ent1 = this->find(q->result);
														if(ent1->type->ntype=="double"){
															printf("\tmovsd\t%d(%%rbp), %%xmm6\n",art->getoffset(q->result)); 
															printf("\tmovsd\t%%xmm6, 0(%%rsp)\n");
														} 
														else{
															printf("\tmovq\t%d(%%rbp), %%r9\n",art->getoffset(q->result)); 
															printf("\tmovq\t%%r9, 0(%%rsp)\n");
														}
														break;
												}
											}
											else{
												ent = global_symtab->find(q->result);
												if(ent){
													if(ent->size==4){
														printf("\tmovb\t%s(%%rip), %%r9b\n",q->result.c_str());
														printf("\tmovl\t%%r9b, 0(%%rsp)\n");
														ch = 'l';	
													}
													else{
														if(ent->size==1){
															printf("\tmovl\t%s(%%rip), %%r9d\n",q->result.c_str()); 
															printf("\tmovb\t%%r9d, 0(%%rsp)\n");
															ch = 'b';
														}
														else{
															if(ent->size==8){
																printf("\tmovq\t%s(%%rip), %%r9\n",q->result.c_str()); 
																printf("\tmovq\t%%r9, 0(%%rsp)\n");
																ch = 'q';
															}
														}
													}
												}
											}
										}
									}
									else{
										if(q->opcode=="call"){
											param_count = 0;
											printf("\tcall\t%s\n",q->arg1.c_str());
											ent = global_symtab->find(q->arg1);
											if(ent->type->rtype->ntype!="void"){
												ch = art->getregch(q->result);
												off = art->getoffset(q->result);
												ent1 = this->find(q->result);
												if(ent1==NULL)ent1 = global_symtab->find(q->result);
												if(ch!='a'){
													if(ch=='q'){
														if(ent1->type->ntype=="double")printf("\tmovsd\t%%xmm6, %d(%%rbp)\n",off);
														else printf("\tmovq\t%%rax, %d(%%rbp)\n",off);
													}
													else{
														if(ch=='l')printf("\tmovl\t%%eax, %d(%%rbp)\n",off);
														else if(ch=='b')printf("\tmovb\t%%al, %d(%%rbp)\n",off);
													}
												}
												else{
													ent = global_symtab->find(q->result);
													if(ent){
														if(ent->size==4){
															printf("\tmovl\t%%eax, %s(%%rip)\n",q->result.c_str());
														}
														else{
															if(ent->size==8){
																if(ent->type->ntype=="double")printf("\tmovsd\t%%xmm6, %s(%%rip)\n",q->result.c_str());
																else printf("\tmovq\t%%rax, %s(%%rip)\n",q->result.c_str());
															}
															else {
																if(ent->size==1)printf("\tmovb\t%%al, %s(%%rip)\n",q->result.c_str());
															}
														}
													}
												}
											}
										}
										else{
											if(q->opcode=="return"){	
												ch = art->getregch(q->result);
												off = art->getoffset(q->result);
												ent1 = this->find(q->result);
												if(ent1==NULL)ent1 = global_symtab->find(q->result);
												if(ch!='a'){
													if(ch=='q'){
														if(ent1->type->ntype=="double")printf("\tmovsd\t%d(%%rbp), %%xmm6\n",off);
														else printf("\tmovq\t%d(%%rbp), %%rax\n",off);
													}
													else{
														if(ch=='l')printf("\tmovl\t%d(%%rbp), %%eax\n",off);
														else if(ch=='b')printf("\tmovb\t%d(%%rbp), %%al\n",off);
													}
												}
												else{
													ent = global_symtab->find(q->result);
													if(ent){
														if(ent->size==4){
															printf("\tmovl\t%s(%%rip), %%eax\n",q->result.c_str());
														}
														else{
															if(ent->size==8){
																if(ent->type->ntype=="double")printf("\tmovsd\t%s(%%rip), %%xmm6\n",q->result.c_str());
																else printf("\tmovq\t%s(%%rip), %%rax\n",q->result.c_str());
															}
															else if(ent->size==1)printf("\tmovb\t%s(%%rip), %%al\n",q->result.c_str());
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		
		
		
		q = q->next;
		i++;
	}
}

void tableSym::epilogue(int func_no){
	printf("\n.L%d:\n",this->qend+1);
	printf("\taddq\t$%d, %%rsp\n",this->offset);
	printf("\tmovq\t%%rbp, %%rsp\n");
	printf("\tpopq\t%%rbp \n");
	printf("\taddq\t$%d, %%rsp\n",this->actrectab->poffset);
	printf("\tret \n");
	printf(".LFE%d: \n",func_no);
	printf("\t.size\t%s, .-%s\n",this->name.c_str(),this->name.c_str());
}

int main(){
	global_symtab = new tableSym();
	curr_symtab = global_symtab;
	entrySym *fdec;
	
	fdec = global_symtab->lookup("printStr");
	fdec->type = new mtype();
	fdec->type->ntype = "function";
	fdec->type->next = NULL;
	fdec->type->isarr = 0;
	fdec->type->rtype = new mtype();
	fdec->type->rtype->ntype = "int";
	fdec->type->rtype->next = NULL;
	fdec->type->rtype->isarr = 0;
	fdec->type->rtype->rtype = NULL;
	
	fdec = global_symtab->lookup("printInt");
	fdec->type = new mtype();
	fdec->type->ntype = "function";
	fdec->type->next = NULL;
	fdec->type->isarr = 0;
	fdec->type->rtype = new mtype();
	fdec->type->rtype->ntype = "int";
	fdec->type->rtype->next = NULL;
	fdec->type->rtype->isarr = 0;
	fdec->type->rtype->rtype = NULL;
	
	fdec = global_symtab->lookup("readInt");
	fdec->type = new mtype();
	fdec->type->ntype = "function";
	fdec->type->next = NULL;
	fdec->type->isarr = 0;
	fdec->type->rtype = new mtype();
	fdec->type->rtype->ntype = "int";
	fdec->type->rtype->next = NULL;
	fdec->type->rtype->isarr = 0;
	fdec->type->rtype->rtype = NULL;
	
	fdec = global_symtab->lookup("printFlt");
	fdec->type = new mtype();
	fdec->type->ntype = "function";
	fdec->type->next = NULL;
	fdec->type->isarr = 0;
	fdec->type->rtype = new mtype();
	fdec->type->rtype->ntype = "int";
	fdec->type->rtype->next = NULL;
	fdec->type->rtype->isarr = 0;
	fdec->type->rtype->rtype = NULL;
	
	fdec = global_symtab->lookup("readFlt");
	fdec->type = new mtype();
	fdec->type->ntype = "function";
	fdec->type->next = NULL;
	fdec->type->isarr = 0;
	fdec->type->rtype = new mtype();
	fdec->type->rtype->ntype = "int";
	fdec->type->rtype->next = NULL;
	fdec->type->rtype->isarr = 0;
	fdec->type->rtype->rtype = NULL;

	yyparse();
	global_symtab->name = "GLOBAL";
	
	/*printf("Quads for this program \n \n");
	global_quad.print();
	
	/*printf("Symbol Tables for the program \n");
	global_symtab->print();
	entrySym *w;
	w = global_symtab->head;
	while(w){
		if(w->nestTab)w->nestTab->print();
		w = w->next;
	}*/
	
	int ind,i;
	quad *p,*q;
	p = global_quad.head;
	while(p){
		p->islabel = 0;
		p->label = "";
		p = p->next;
	}
	
	p = global_quad.head;
	while(p){
		if((p->arg1=="" && p->arg2=="" && p->opcode=="") || p->opcode=="iftrue" || p->opcode=="ifFalse" || p->opcode=="iflt" || p->opcode=="iflte" || p->opcode=="ifgt" || p->opcode=="ifgte" || p->opcode=="ifeq" || p->opcode=="ifneq"){
			stringstream ss(p->result);
			ss>>ind;
			q = global_quad.head;
			i = 0;
			while(i<ind && q){
				i++;
				q = q->next;
			}
			
			if(q)q->islabel = 1;
			if(q)q->label = ".L"+p->result;
		}
		p = p->next;
	}
	
	global_symtab->handle_global();
	printf("\t.text\n");
	entrySym *p1;
	p1 = global_symtab->head;
	i=0;
	while(p1){
		if(p1->nestTab){
			p1->nestTab->actrectab = new artable(p1->nestTab,p1->nestTab->param_no);
			p1->nestTab->prologue(i);
			p1->nestTab->funchand();
			p1->nestTab->epilogue(i);
			printf("\n");
			i++;
		}
		p1 = p1->next;
	}
	return 0;
}
