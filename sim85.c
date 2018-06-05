/*  8085 Simulator Program
    Copyright (C) 2001  Mrugesh R. Gajjar

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/    
/*			SOURCE FILE FOR 8085 SIMULATOR PROGRAM
						DEVELOPED IN 'C'

			DEVELOPED BY: MRUGESH R. GAJJAR

			6TH SEMESTER, COMPUTER ENGINEERING

			DHARMSINH DESAI INSTITUTE OF TECHNOLOGY
					(Deemed University)

			DATE : 28-02-2001

			E-MAIL: mrugesh386@gmail.com


			Note: This is single source file containing all code.
				  See readme.txt for more information.
*/
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
struct flags
{
	unsigned c:1;
	unsigned z:1;
	unsigned s:1;
}F;
#define NOINT 1000
#define SSIZE 1000
#define RAMSIZE 4096
#define IPORTSIZE 256
#define OPORTSIZE 256
unsigned char *A,*B,*C,*D,*E,*H,*L,*M,*PORT;
unsigned char a,b,c,d,e,h,l,m,port;
unsigned int *SP,*PC;
unsigned int sp,pc;
int ip;
unsigned int tstates=0,step=0,brpoint=0;
float freq=2.0;
struct inst
{
	char label[17];
	char mnemonic[17];
	char operand[6];
	char data[6];
}ins[NOINT];
int stack[SSIZE];
int top=0;
void refresh();
unsigned char ram[RAMSIZE];
unsigned char iport[IPORTSIZE];
unsigned char oport[OPORTSIZE];
void print_border(char);
void before()
{
	(*M)=ram[256*(*H)+(*L)];
}
void after()
{
	ram[256*(*H)+(*L)]=(*M);
}
void init()
{
	A=&a;
	B=&b;
	C=&c;
	D=&d;
	E=&e;
	H=&h;
	L=&l;
	M=&m;
	SP=&sp;
	PC=&pc;
	PORT=&port;
}
void display_regs()
{
	window(1,2,80,6);
	textcolor(9);
	clrscr();
	printf("\nREGISTERS\n");
	printf("\nA=%02X | B=%02X | C=%02X | D=%02X | E=%02X | H|L=%02X|%02X | SP=%04X | CY=%X | Z=%X | S=%X\n",*A,*B,*C,*D,*E,*H,*L,*SP,F.c,F.z,F.s);
	textcolor(WHITE);
	window(1,25,80,25);
}
void print_border(char c)
{
	int i;
	for(i=0;i<80;i++)
	printf("%c",c);
}
void mov(unsigned char* dest,unsigned char* src)
{
	(*dest)=(*src);
	tstates+=4;
	refresh();
}
void mov_m(unsigned char* dest,unsigned char* src)
{
	before();
	(*dest)=(*src);
	after();
	tstates+=7;
	refresh();
}
void mvi(unsigned char* dest,unsigned char data)
{
	(*dest)=data;
	tstates+=7;
	refresh();
}
void mvi_m(unsigned char* dest,unsigned char data)
{
	(*dest)=data;
	after();
	tstates+=10;
	refresh();
}
void dcr(unsigned char* dest)
{
	(*dest)--;
	if(!(*dest))
	F.z=1;
	else
	F.z=0;
	if((*dest)&128)
	F.s=1;
	else
	F.s=0;
	tstates+=4;
	refresh();
}
void dcr_m(unsigned char* dest)
{
	before();
	(*dest)--;
	if(!(*dest))
	F.z=1;
	else
	F.z=0;
	if((*dest)&128)
	F.s=1;
	else
	F.s=0;
	after();
	tstates+=10;
	refresh();
}
void inr(unsigned char* dest)
{
	(*dest)++;
	if(!(*dest))
	F.z=1;
	else
	F.z=0;
	if((*dest)&128)
	F.s=1;
	else
	F.s=0;
	tstates+=4;
	refresh();
}
void inr_m(unsigned char* dest)
{
	before();
	(*dest)++;
	if(!(*dest))
	F.z=1;
	else
	F.z=0;
	if((*dest)&128)
	F.s=1;
	else
	F.s=0;
	after();
	tstates+=10;
	refresh();
}
void dcx_B()
{
	(*C)--;
	if(*C==255)
	(*B)--;
	tstates+=6;
	refresh();
}
void dcx_D()
{
	(*E)--;
	if(*E==255)
	(*D)--;
	tstates+=6;
	refresh();
}
void dcx_H()
{
	(*L)--;
	if(*L==255)
	(*H)--;
	tstates+=6;
	refresh();
}
void dcx_SP()
{
	(*SP)--;
	tstates+=6;
	refresh();
}
void inx_B()
{
	(*C)++;
	if(*C==0)
	(*B)++;
	tstates+=6;
	refresh();
}
void inx_D()
{
	(*E)++;
	if(*E==0)
	(*D)++;
	tstates+=6;
	refresh();
}
void inx_H()
{
	(*L)++;
	if(*L==0)
	(*H)++;
	tstates+=6;
	refresh();
}
void inx_SP()
{
	(*SP)++;
	tstates+=6;
	refresh();
}
void lxi_B(unsigned int data)
{
	*C=data&255;
	*B=(data>>8)&255;
	tstates+=10;
	refresh();
}
void lxi_D(unsigned int data)
{
	*E=data&255;
	*D=(data>>8)&255;
	tstates+=10;
	refresh();
}
void lxi_H(unsigned int data)
{
	*L=data&255;
	*H=(data>>8)&255;
	tstates+=10;
	refresh();
}
void lxi_SP(unsigned int data)
{
	*SP=data;
	tstates+=10;
	refresh();
}
void nop()
{
	tstates+=4;
	refresh();
}
void aci(unsigned char data)
{
	if(a+(int)data+F.c>255)
	F.c=1;
	else
	F.c=0;
	a=a+data+F.c;
	if(!a)
	F.z=1;
	else
	F.z=0;
	if(a&128)
	F.s=1;
	else
	F.s=0;
	tstates+=7;
	refresh();
}
void adc(unsigned char* dest)
{
	if((a+(*(int *)dest)+F.c)>255)
	F.c=1;
	else
	F.c=0;
	a=a+(*dest)+F.c;
	if(!a)
	F.z=1;
	else
	F.z=0;
	if(((*A)>>7)&1==1)
	F.s=1;
	else
	F.s=0;
	tstates+=4;
	refresh();
}
void adc_m(unsigned char* dest)
{
	before();
	if((a+(*(int *)dest)+F.c)>255)
	F.c=1;
	else
	F.c=0;
	a=a+(*dest)+F.c;
	if(!a)
	F.z=1;
	else
	F.z=0;
	if(((*A)>>7)&1==1)
	F.s=1;
	else
	F.s=0;
	tstates+=7;
	refresh();
}
void add(unsigned char* dest)
{
	if((a+(*(int *)dest))>255)
	F.c=1;
	else
	F.c=0;
	a+=(*dest);
	if(!a)
	F.z=1;
	else
	F.z=0;
	if(((*A)>>7)&1==1)
	F.s=1;
	else
	F.s=0;
	tstates+=4;
	refresh();
}
void add_m(unsigned char* dest)
{
	before();
	if((a+(*(int *)dest))>255)
	F.c=1;
	else
	F.c=0;
	a+=(*dest);
	if(!a)
	F.z=1;
	else
	F.z=0;
	if(((*A)>>7)&1==1)
	F.s=1;
	else
	F.s=0;
	tstates+=7;
	refresh();
}
void adi(unsigned char data)
{
	if(a+(int)data>255)
	F.c=1;
	else
	F.c=0;
	a+=data;
	if(!a)
	F.z=1;
	else
	F.z=0;
	if(a&128)
	F.s=1;
	else
	F.s=0;
	tstates+=7;
	refresh();
}
void ana(unsigned char* dest)
{
	a=a&(*dest);
	F.c=0;
	if(!a)
	F.z=1;
	else
	F.z=0;
	if(a&128)
	F.s=1;
	else
	F.s=0;
	tstates+=4;
	refresh();
}
void ana_m(unsigned char* dest)
{
	before();
	a=a&(*dest);
	F.c=0;
	if(!a)
	F.z=1;
	else
	F.z=0;
	if(a&128)
	F.s=1;
	else
	F.s=0;
	tstates+=7;
	refresh();
}
void ani(unsigned char data)
{
	a=a&data;
	F.c=0;
	if(!a)
	F.z=1;
	else
	F.z=0;
	if(a&128)
	F.s=1;
	else
	F.s=0;
	tstates+=7;
	refresh();
}
void cma()
{
	a=255-a;
	tstates+=4;
	refresh();
}
void cmc()
{
	if(F.c)
	F.c=0;
	else
	F.c=1;
	tstates+=4;
	refresh();
}
void cmp(unsigned char* dest)
{
	if(a<(*dest))
	{
		F.c=1;
		F.z=0;
	}
	if(a==(*dest))
	{
		F.z=1;
		F.c=0;
	}
	if(a>(*dest))
	{
		F.c=0;
		F.z=0;
	}
	tstates+=4;
	refresh();
}
void cmp_m(unsigned char* dest)
{
	before();
	if(a<(*dest))
	{
		F.c=1;
		F.z=0;
	}
	if(a==(*dest))
	{
		F.z=1;
		F.c=0;
	}
	if(a>(*dest))
	{
		F.c=0;
		F.z=0;
	}
	tstates+=7;
	refresh();
}
void cpi(unsigned char data)
{
	if(a<data)
	{
		F.c=1;
		F.z=0;
	}
	if(a==data)
	{
		F.z=1;
		F.c=0;
	}
	if(a>data)
	{
		F.c=0;
		F.z=0;
	}
	tstates+=7;
	refresh();
}
void dad_B()
{
	unsigned int temp;
	if(256.0*(*H)+(*L)+256.0*(*B)+(*C)>65535.0)
	F.c=1;
	else
	F.c=0;
	temp=256*(*H)+(*L)+256*(*B)+(*C);
	*L=temp&255;
	*H=(temp>>8)&255;
	tstates+=10;
	refresh();
}
void dad_D()
{
	unsigned int temp;
	if(256.0*(*H)+(*L)+256.0*(*D)+(*E)>65535.0)
	F.c=1;
	else
	F.c=0;
	temp=256*(*H)+(*L)+256*(*D)+(*E);
	*L=temp&255;
	*H=(temp>>8)&255;
	tstates+=10;
	refresh();
}
void dad_H()
{
	unsigned int temp;
	if(256.0*(*H)+(*L)+256.0*(*H)+(*L)>65535.0)
	F.c=1;
	else
	F.c=0;
	temp=256*(*H)+(*L)+256*(*H)+(*L);
	*L=temp&255;
	*H=(temp>>8)&255;
	tstates+=10;
	refresh();
}
void dad_SP()
{
	unsigned int temp;
	if(256.0*(*H)+(*L)+(*SP)>65535.0)
	F.c=1;
	else
	F.c=0;
	temp=256*(*H)+(*L)+(*SP);
	*L=temp&255;
	*H=(temp>>8)&255;
	tstates+=10;
	refresh();
}
void lda(unsigned int data)
{
	a=ram[data];
	tstates+=13;
	refresh();
}
void ldax_B()
{
	a=ram[256*(*B)+(*C)];
	tstates+=7;
	refresh();
}
void ldax_D()
{
	a=ram[256*(*D)+(*E)];
	tstates+=7;
	refresh();
}
void lhld(unsigned int data)
{
	*L=ram[data++];
	*H=ram[data];
	tstates+=16;
	refresh();
}
void ora(unsigned char* dest)
{
	a=a|(*dest);
	F.c=0;
	if(!a)
	F.z=1;
	else
	F.z=0;
	if(a&128)
	F.s=1;
	else
	F.s=0;
	tstates+=4;
	refresh();
}
void ora_m(unsigned char* dest)
{
	before();
	a=a|(*dest);
	F.c=0;
	if(!a)
	F.z=1;
	else
	F.z=0;
	if(a&128)
	F.s=1;
	else
	F.s=0;
	tstates+=7;
	refresh();
}
void ori(unsigned char data)
{
	a=a|data;
	F.c=0;
	if(!a)
	F.z=1;
	else
	F.z=0;
	if(a&128)
	F.s=1;
	else
	F.s=0;
	tstates+=7;
	refresh();
}
void sbb(unsigned char* dest)
{
	int ab;
	ab=a-(*dest)-F.c;
	a=a-(*dest)-F.c;
	if(ab<0)
	F.c=1;
	else F.c=0;
	if(!a)
	F.z=1;
	else
	F.z=0;
	if(a&128)
	F.s=1;
	else
	F.s=0;
	tstates+=4;
	refresh();
}
void sbb_m(unsigned char* dest)
{
	int ab;
	before();
	ab=a-(*dest)-F.c;
	a=a-(*dest)-F.c;
	if(ab<0)
	F.c=1;
	else F.c=0;
	if(!a)
	F.z=1;
	else
	F.z=0;
	if(a&128)
	F.s=1;
	else
	F.s=0;
	tstates+=7;
	refresh();
}
void sbi(unsigned char data)
{
	int ab;
	ab=a-data-F.c;
	a=a-data-F.c;
	if(ab<0)
	F.c=1;
	else F.c=0;
	if(!a)
	F.z=1;
	else
	F.z=0;
	if(a&128)
	F.s=1;
	else
	F.s=0;
	tstates+=7;
	refresh();
}
void shld(unsigned int data)
{
	ram[data++]=(*L);
	ram[data]=(*H);
	tstates+=16;
	refresh();
}
void sphl()
{
	*SP=256*(*H)+(*L);
	tstates+=6;
	refresh();
}
void sta(unsigned int data)
{
	ram[data]=a;
	tstates+=13;
	refresh();
}
void stax_B()
{
	ram[256*(*B)+(*C)]=a;
	tstates+=7;
	refresh();
}
void stax_D()
{
	ram[256*(*D)+(*E)]=a;
	tstates+=7;
	refresh();
}
void stc()
{
	F.c=1;
	tstates+=4;
	refresh();
}
void sub(unsigned char* dest)
{
	int ab;
	ab=a-(*dest);
	a=a-(*dest);
	if(ab<0)
	F.c=1;
	else F.c=0;
	if(!a)
	F.z=1;
	else
	F.z=0;
	if(a&128)
	F.s=1;
	else
	F.s=0;
	tstates+=4;
	refresh();
}
void sub_m(unsigned char* dest)
{
	int ab;
	before();
	ab=a-(*dest);
	a=a-(*dest);
	if(ab<0)
	F.c=1;
	else F.c=0;
	if(!a)
	F.z=1;
	else
	F.z=0;
	if(a&128)
	F.s=1;
	else
	F.s=0;
	tstates+=7;
	refresh();
}
void sui(unsigned char data)
{
	int ab;
	ab=a-data;
	a=a-data;
	if(ab<0)
	F.c=1;
	else F.c=0;
	if(!a)
	F.z=1;
	else
	F.z=0;
	if(a&128)
	F.s=1;
	else
	F.s=0;
	tstates+=7;
	refresh();
}
void xchg()
{
	unsigned char temp;
	temp=(*H);
	*H=(*D);
	*D=temp;
	temp=(*L);
	*L=(*E);
	*E=temp;
	tstates+=4;
	refresh();
}
void hlt()
{
	tstates+=5;
	refresh();
}
void xra(unsigned char* dest)
{
	a=a^(*dest);
	F.c=0;
	if(!a)
	F.z=1;
	else F.z=0;
	if(a&128)
	F.s=1;
	else
	F.s=0;
	tstates+=4;
	refresh();
}
void xra_m(unsigned char* dest)
{
	before();
	a=a^(*dest);
	F.c=0;
	if(!a)
	F.z=1;
	else F.z=0;
	if(a&128)
	F.s=1;
	else
	F.s=0;
	tstates+=7;
	refresh();
}
void xri(unsigned char data)
{
	a=a^data;
	F.c=0;
	if(!a)
	F.z=1;
	else F.z=0;
	if(a&128)
	F.s=1;
	else
	F.s=0;
	tstates+=7;
	refresh();
}
void xthl()
{
	unsigned char temp;
	temp=(*L);
	*L=ram[*SP];
	ram[*SP]=temp;
	temp=(*H);
	*H=ram[++(*SP)];
	ram[*SP]=temp;
	tstates+=16;
	refresh();
}
void in(unsigned char data)
{
	window(30,18,59,18);
	cprintf("INPUT:%X: ",data);
	cscanf("%x",&iport[data]);
	*A=iport[data];
	tstates+=10;
	refresh();
}
void out(unsigned char data)
{
	window(30,19,59,19);
	oport[data]=(*A);
	cprintf("OUTPUT:%X: %X",data,*A);
	tstates+=10;
	refresh();
}
void push_B()
{
	(*SP)--;
	ram[*SP]=(*B);
	(*SP)--;
	ram[*SP]=(*C);
	tstates+=12;
	refresh();
}
void push_D()
{
	(*SP)--;
	ram[*SP]=(*D);
	(*SP)--;
	ram[*SP]=(*E);
	tstates+=12;
	refresh();
}
void push_H()
{
	(*SP)--;
	ram[*SP]=(*H);
	(*SP)--;
	ram[*SP]=(*L);
	tstates+=12;
	refresh();
}
void push_PSW()
{
	(*SP)--;
	ram[*SP]=(*A);
	(*SP)--;
	ram[*SP]=F.c+64*F.z+128*F.s;
	tstates+=12;
	refresh();
}
void pop_B()
{
	*C=ram[*SP];
	(*SP)++;
	*B=ram[*SP];
	(*SP)++;
	tstates+=10;
	refresh();
}
void pop_D()
{
	*E=ram[*SP];
	(*SP)++;
	*D=ram[*SP];
	(*SP)++;
	tstates+=10;
	refresh();
}
void pop_H()
{
	*L=ram[*SP];
	(*SP)++;
	*H=ram[*SP];
	(*SP)++;
	tstates+=10;
	refresh();
}
void pop_PSW()
{
	F.c=ram[*SP]&1;
	F.z=(ram[*SP]&64)/64;
	F.s=(ram[*SP]&128)/128;
	(*SP)++;
	*A=ram[*SP];
	(*SP)++;
	tstates+=10;
	refresh();
}
void ral()
{
	int tmp;
	tmp=a&128;
	a=(a<<1)+F.c;
	F.c=tmp;
	tstates+=4;
	refresh();
}
void rar()
{
	int tmp;
	tmp=a&1;
	a=(a>>1)+128*F.c;
	F.c=tmp;
	tstates+=4;
	refresh();
}
void rlc()
{
	F.c=a&128;
	a=(a<<1)+F.c;
	tstates+=4;
	refresh();
}
void rrc()
{
	F.c=a&128;
	a=(a>>1)+128*F.c;
	tstates+=4;
	refresh();
}
void show_help()
{
	window(1,1,80,1);
	textcolor(4);
	textbackground(WHITE);
	cprintf("-: THE 8085 SIMULATOR PROGRAM :-");
	textbackground(BLACK);
	window(1,7,29,25);
	textcolor(3);
	cprintf("\n\rpress ");
	textcolor(WHITE+BLINK);
	cprintf("ESC");
	textcolor(3);
	cprintf(" to exit,");
	cprintf("\n\rl to load file,");
	cprintf("\n\re to execute,");
	cprintf("\n\rm to view memory,");
	cprintf("\n\rs to store memory location,");
	cprintf("\n\rt to stepwise execution,");
	cprintf("\n\rf to set clock frequency");
	textcolor(RED);
	textbackground(WHITE);
	cprintf("\n\n\rDEVELOPED BY : \n\rMRUGESH R. GAJJAR\n\rB.E. (CE)-6\n\rD.D.I.T.\n\rNADIAD");
	textcolor(WHITE);
	textbackground(BLACK);
	window(1,25,10,25);
}
void show_ram()
{
	int address=0,i;
	char reply;
	window(60,7,80,25);
	textcolor(10);
	clrscr();
	cprintf("\n\rAddress : ");
	flushall();
	window(71,8,74,8);
	cscanf("%04x",&address);
	window(60,8,80,25);
	for(i=0;i<15;i++)
	cprintf("\n\r%04X : %02X",address+i,ram[address+i]);
	textcolor(WHITE);
	window(1,25,10,25);
}
void store_ram()
{
	int add=0;
	window(30,23,59,25);
	textcolor(YELLOW);
	clrscr();
	cprintf("\r\nIn RAM @Address : ");
	window(48,24,51,24);
	cscanf("%04x",&add);
	window(30,24,59,25);
	cprintf("\r\nStore : ");
	window(38,25,40,25);
	cscanf("%02x",&ram[add]);
	textcolor(WHITE);
	window(1,25,10,25);
}
void showhlram()
{
	window(30,7,59,7);
	clrscr();
	cprintf("MEMORY %04X : %02X",256*(*H)+(*L),ram[256*(*H)+(*L)]);
	window(1,25,10,25);
}
void show_clock()
{
	window(30,9,59,9);
	clrscr();
	cprintf("T-states : %u",tstates);
	window(30,10,59,11);
	clrscr();
	cprintf("Time : %.2f microseconds",tstates/freq);
	window(30,12,59,12);
	cprintf("Clock freq : %.2f MHz",freq);
	window(1,25,10,25);
}
void refresh()
{
	if(step||brpoint)
	{
		brpoint=0;
		display_regs();
		showhlram();
		show_clock();
		window(30,17,59,17);
		clrscr();
		cprintf("%s %s %s %s",ins[ip].label,ins[ip].mnemonic,ins[ip].operand,ins[ip].data);
		window(1,25,10,25);
		getch();
	}
}
void set_step()
{
	step=1;
}
void set_brpoint()
{
	brpoint=1;
	refresh();
}
void set_freq()
{
	window(30,14,59,15);
	clrscr();
	cprintf("Enter clock freq in MHz : ");
	window(55,14,59,15);
	cscanf("%f",&freq);
	window(1,25,10,25);
}
void reset_vars()
{
	a=b=c=d=e=h=l=m=0;
	sp=pc=0;
	F.c=0;
	F.s=0;
	F.z=0;
	tstates=0;
	window(30,16,59,16);
	ip=0;
	clrscr();
}
int index(char s[],char t[])
{
	int i,j,k;
	for(i=0;s[i]!='\0';i++)
		{
		for(j=i,k=0;t[k]!='\0' && s[j]==t[k];j++,k++);
		if(k>0 && t[k]=='\0')
			return i++;
		}
		return(-1);
}
void filetoram()
{
	FILE *fp;
	char s[81],t[51];
	char finame[81];
        int d,i,j=0,k,l,flag,start_flag=0;
	clrscr();
	printf("filename: ");
	scanf("%s",finame);
	fp=fopen(finame,"r");
	if(fp==NULL)
	{
		printf("Error opening %s",finame);
		exit(1);
	}
	if(index(finame,".85\0")==-1)
	{
		printf("Invalid source file %s",finame);
		exit(1);
	}
	clrscr();
	for(i=0,j=0,k=0;fgets(s,80,fp)&&index(s,".END");)
	{
		if(!index(s,".BEGIN"))
		{
			start_flag=1;
			continue;
		}
		if(start_flag)
		{
		flag=0;
		i=0;
		while(isspace(s[i])) i++;
		if(s[i]==';'||s[i]=='\0')
		{
			flag=1;
			continue;
		}
		if((d=index(s,":"))!=(-1))
		{
			i=0;l=0;
			while(isspace(s[i])) i++;
			for(;s[i]!=':';i++)
			t[l++]=s[i];
			t[l]='\0';
			strcpy(ins[j].label,t);
			i++;
		}
		k=0;
		while(isspace(s[i])&&s[i]) i++;
		while(!isspace(s[i])&&s[i]&&s[i]!=';')
		{
			ins[j].mnemonic[k++]=s[i];
			i++;
		}
		ins[j].mnemonic[k]='\0';
		k=0;
		while(isspace(s[i])&&s[i]) i++;
		while(!isspace(s[i])&&s[i]&&s[i]!=';')
		{ 	ins[j].operand[k++]=s[i];
			i++;
		}
		ins[j].operand[k]='\0';
		k=0;
		while(isspace(s[i])&&s[i]) i++;
		while(!isspace(s[i])&&s[i]&&s[i]!=';')
		{
			ins[j].data[k++]=s[i];
			i++;
		}
		ins[j].data[k]='\0';
		if(!flag) j++;
		}
	}
	fclose(fp);
	window(1,1,80,25);
}
unsigned char *fetchop1()
{
	switch(ins[ip].operand[0])
	{
		case 'A': return A;
		case 'B': return B;
		case 'C': return C;
		case 'D': return D;
		case 'E': return E;
		case 'H': return H;
		case 'L': return L;
		case 'M': return M;
		default : printf("\ninvalid oprand in instruction %d\n",ip+1); exit(1);
	}
}
unsigned int fetchdata(char ary[])
{
	int i,j,k=0;
	unsigned int sum=0;
	j=strlen(ary)-1;
	for(i=0;j>=0;i++,j--)
	{
		if(isdigit(ary[i]))
			k=ary[i]-'0';
		else
			switch(ary[i])
			{
				case 'A': case 'a': k=10;
					break;
				case 'B': case 'b': k=11;
					break;
				case 'C': case 'c': k=12;
					break;
				case 'D': case 'd': k=13;
					break;
				case 'E': case 'e': k=14;
					break;
				case 'F': case 'f': k=15;
					break;
			}
		sum+=k*pow(16,j);
	}
	return sum;
}
unsigned char *fetchop2()
{
	switch(ins[ip].data[0])
	{
		case 'A': return A;
		case 'B': return B;
		case 'C': return C;
		case 'D': return D;
		case 'E': return E;
		case 'H': return H;
		case 'L': return L;
		case 'M': return M;
		default : printf("\ninvalid oprand in instruction %d\n",ip+1); exit(1);
	}
}
int getindex(char lab[])
{
	int dumip=0;
	while(strcmp(ins[dumip].label,lab)!=0) dumip++;
	return dumip;
}
void push()
{
	if(top>=SSIZE-1)
	{
		printf("\nstack overflow error\n");
		exit(1);
	}
	stack[++top]=ip+1;
}
void pop()
{
	if(!top)
	{
		printf("\nstack underflow error\n");
		exit(1);
	}
	ip=stack[top--];
}
void decodeexe()
{
	char key[]="MOV,MVI,ADD,LXI,SUB,IN ,DCR,INX,DCX,POP,PUSH,JM ,CM ,ANA,ORA,ADI,SUI,ADC,SBB,ANI,ORI,CALL,XRA,XRI,ACI,SBI,RET,LDA,STA,LDAX,STAX,DAD,HLT,INROUT,JC,JMPJNC,JNZ,JP,NOP,RAL,RAR,RC ,RLC,STC,XCHG,XTHL,CC,CMPCMA,CMC,CNC,CNZ,CP,CPI,CZ,LHLD,PCHL,RRCRM,RNC,RNZ,RP,RZ,SHLD,SPHL,JZ,.BREAK,";
	while(ip!=-1&&ip!=-2)
	switch(index(key,ins[ip].mnemonic))
	{
		case 0: if (ins[ip].operand[0]=='M'||ins[ip].data[0]=='M')
				{ mov_m(fetchop1(),fetchop2()); ip++; break; }
				else { mov(fetchop1(),fetchop2()); ip++; break; }
		case 4: if (ins[ip].operand[0]=='M'||ins[ip].data[0]=='M')
				{ mvi_m(fetchop1(),(unsigned char) fetchdata(ins[ip].data));
				 ip++; break; }
				else { mvi(fetchop1(),(unsigned char) fetchdata(ins[ip].data));
				 ip++; break; }
		case 8: if(ins[ip].operand[0]=='M') { add_m(M); ip++; break; }
				else { add(fetchop1()); ip++; break; }
		case 12: switch(ins[ip].operand[0])
					{
						case 'B':lxi_B(fetchdata(ins[ip].data)); ip++; break;
						case 'D':lxi_D(fetchdata(ins[ip].data)); ip++; break;
						case 'H':lxi_H(fetchdata(ins[ip].data)); ip++; break;
						case 'S':lxi_SP(fetchdata(ins[ip].data)); ip++; break;
						default :
						printf("invalid operand in instruction %d",ip+1); exit(1);
					}   break;
		case 16:if(ins[ip].operand[0]=='M') { sub_m(M); ip++; break; }
				else { sub(fetchop1()); ip++; break; }
		case 20:in(fetchdata(ins[ip].operand)); ip++; break;

		case 24:if(ins[ip].operand[0]=='M') { dcr_m(M); ip++; break; }
				else { dcr(fetchop1()); ip++; break; }
		case 28:switch(ins[ip].operand[0])
					{
						case 'B':inx_B(fetchdata(ins[ip].data)); ip++; break;
						case 'D':inx_D(fetchdata(ins[ip].data)); ip++; break;
						case 'H':inx_H(fetchdata(ins[ip].data)); ip++; break;
						case 'S':inx_SP(fetchdata(ins[ip].data)); ip++; break;
						default :
						printf("invalid operand in instruction %d",ip+1); exit(1);
					}   break;
		case 32:switch(ins[ip].operand[0])
					{
						case 'B':dcx_B(fetchdata(ins[ip].data)); ip++; break;
						case 'D':dcx_D(fetchdata(ins[ip].data)); ip++; break;
						case 'H':dcx_H(fetchdata(ins[ip].data)); ip++; break;
						case 'S':dcx_SP(fetchdata(ins[ip].data)); ip++; break;
						default :
						printf("invalid operand in instruction %d",ip+1); exit(1);
					}   break;
		case 36:switch(ins[ip].operand[0])
					{
						case 'B':pop_B(fetchdata(ins[ip].data)); ip++; break;
						case 'D':pop_D(fetchdata(ins[ip].data)); ip++; break;
						case 'H':pop_H(fetchdata(ins[ip].data)); ip++; break;
						case 'P':pop_PSW(fetchdata(ins[ip].data)); ip++; break;
						default :
						printf("invalid operand in instruction %d",ip+1); exit(1);
					}   break;
		case 40:switch(ins[ip].operand[0])
					{
						case 'B':push_B(fetchdata(ins[ip].data)); ip++; break;
						case 'D':push_D(fetchdata(ins[ip].data)); ip++; break;
						case 'H':push_H(fetchdata(ins[ip].data)); ip++; break;
						case 'P':push_PSW(fetchdata(ins[ip].data)); ip++; break;
						default :
						printf("invalid operand in instruction %d",ip+1); exit(1);
					}   break;
		case 45:  if(F.s==1)     /* JM */
					{
						tstates+=10;
						ip=getindex(ins[ip].operand);
					}
					else
					{
						tstates+=7;
						ip++;
					}
					break;
		case 49: if(F.s==1)
					{
						push();
						ip=getindex(ins[ip].operand);
						tstates+=18;
					}
					else
					{
						tstates+=9;
						ip++;
					}
					break;
		case 53: if(ins[ip].operand[0]=='M') { ana_m(M); ip++; break; }
				else { ana(fetchop1()); ip++; break; }
		case 57: if(ins[ip].operand[0]=='M') { ora_m(M); ip++; break; }
				else { ora(fetchop1()); ip++; break; }
		case 61: adi(fetchdata(ins[ip].operand)); ip++; break;
		case 65: sui(fetchdata(ins[ip].operand)); ip++; break;
		case 69: if(ins[ip].operand[0]=='M') { adc_m(M); ip++; break; }
				else { adc(fetchop1()); ip++; break; }
		case 73: if(ins[ip].operand[0]=='M') { sbb_m(M); ip++; break; }
				else { sbb(fetchop1()); ip++; break; }
		case 77: ani(fetchdata(ins[ip].operand)); ip++; break;
		case 81: ori(fetchdata(ins[ip].operand)); ip++; break;
		case 85: push(); ip=getindex(ins[ip].operand); tstates+=18; break;
		case 90: if(ins[ip].operand[0]=='M') { xra_m(M); ip++; break; }
				else { xra(fetchop1()); ip++; break; }
		case 94: xri(fetchdata(ins[ip].operand)); ip++; break;
		case 98: aci(fetchdata(ins[ip].operand)); ip++; break;
		case 102: sbi(fetchdata(ins[ip].operand)); ip++; break;
		case 106: pop(); tstates+=10; break;
		case 110: lda(fetchdata(ins[ip].operand)); ip++; break;
		case 114: sta(fetchdata(ins[ip].operand)); ip++; break;
		case 118: switch(ins[ip].operand[0])
					{
						case 'B': ldax_B(); ip++; break;
						case 'D': ldax_D(); ip++; break;
						default:
						printf("invalid operand in instruction %d",ip+1); exit(1);
					}   break;
		case 123: switch(ins[ip].operand[0])
					{
						case 'B': stax_B(); ip++; break;
						case 'D': stax_D(); ip++; break;
						default:
						printf("invalid operand in instruction %d",ip+1); exit(1);
					}   break;
		case 128: switch(ins[ip].operand[0])
					{
						case 'B': dad_B(); ip++; break;
						case 'D': dad_D(); ip++; break;
						case 'S': dad_SP(); ip++; break;
						case 'H': dad_H(); ip++; break;
						default:
						printf("invalid operand in instruction %d",ip+1); exit(1);
					}   break;
		case 132:  hlt(); ip=-1; break;
		case 136: if(ins[ip].operand[0]=='M') { inr_m(M); ip++; break; }
				else { inr(fetchop1()); ip++; break; }
		case 139: out(fetchdata(ins[ip].operand)); ip++; break;
		case 143: if(F.c==1)
					{
						tstates+=10;
						ip=getindex(ins[ip].operand);
					}
					else
					{
						tstates+=7;
						ip++;
					}
					break;
		case 146: /* JMP */ip=getindex(ins[ip].operand); tstates+=10; break;

		case 149: if(!F.c)
					{
						tstates+=10;
						ip=getindex(ins[ip].operand);
					}
					else
					{
						tstates+=7;
						ip++;
					}
					break;
		case 153: if(!F.z)
					{
						tstates+=10;
						ip=getindex(ins[ip].operand);
					}
					else
					{
						tstates+=7;
						ip++;
					}
					break;
		case 157: if(!F.s)
					{
						tstates+=10;
						ip=getindex(ins[ip].operand);
					}
					else
					{
						tstates+=7;
						ip++;
					}
					break;
		case 160: nop(); ip++; break;
		case 164: ral(); ip++; break;
		case 168: rar(); ip++; break;
		case 172:if(F.c==1)
					{
						tstates+=12;
						pop();
					}
					else
					{
						tstates+=6;
						ip++;
					}
					break;

		case 176: rlc(); ip++; break;
		case 180: stc(); ip++; break;
		case 184: xchg(); ip++; break;
		case 189: xthl(); ip++; break;
		case 194: if(F.c==1)
					{
						push();
						ip=getindex(ins[ip].operand);
						tstates+=18;
					}
					else
					{
						tstates+=9;
						ip++;
					}
					break;
		case 197:   if(ins[ip].operand[0]=='M') { cmp_m(M); ip++; break; }
				else { cmp(fetchop1()); ip++; break; }

		case 200:  cma(); ip++; break;
		case 204: cmc(); ip++; break;
		case 208: if(!F.c)
					{
						push();
						ip=getindex(ins[ip].operand);
						tstates+=18;
					}
					else
					{
						tstates+=9;
						ip++;
					}
					break;
		case 212: if(!F.z)
					{
						push();
						ip=getindex(ins[ip].operand);
						tstates+=18;
					}
					else
					{
						tstates+=9;
						ip++;
					}
					break;
		case 216: if(!F.s)
					{
						push();
						ip=getindex(ins[ip].operand);
						tstates+=18;
					}
					else
					{
						tstates+=9;
						ip++;
					}
					break;
		case 219:  cpi(fetchdata(ins[ip].operand)); ip++; break;
		case 223:  if(F.z==1)
					{
						push();
						ip=getindex(ins[ip].operand);
						tstates+=18;
					}
					else
					{
						tstates+=9;
						ip++;
					}
					break;
		case 226: lhld(fetchdata(ins[ip].operand)); ip++; break;
		case 236: rrc(); ip++; break;
		case 239: if(F.s==1)
					{
						tstates+=12;
						pop();
					}
					else
					{
						tstates+=6;
						ip++;
					}
					break;
		case 242: if(!F.c)
					{
						tstates+=12;
						pop();
					}
					else
					{
						tstates+=6;
						ip++;
					}
					break;
		case 246: if(!F.z)
					{
						tstates+=12;
						pop();
					}
					else
					{
						tstates+=6;
						ip++;
					}
					break;
		case 250: if(!F.s)
					{
						tstates+=12;
						pop();
					}
					else
					{
						tstates+=6;
						ip++;
					}
					break;
		case 253: if(F.z==1)
					{
						tstates+=12;
						pop();
					}
					else
					{
						tstates+=6;
						ip++;
					}
					break;
		case 256: shld(fetchdata(ins[ip].operand)); ip++; break;
		case 261: sphl(); ip++; break;
		case 266: if(F.z==1)
					{
						tstates+=10;
						ip=getindex(ins[ip].operand);
					}
					else
					{
						tstates+=7;
						ip++;
					}
					break;
		case 269: set_brpoint(); ip++; break;
		default:  printf(" invalid instruction %s OR no file loaded",ins[ip].mnemonic); exit(1);
	}
}
/*------------------------------------------------------*/
void main()
{
	char r;
	init();
	clrscr();
	show_help();
	window(1,25,10,25);
	while(1)
	{
		cprintf("\n\r-");
		r=getche();
		switch(r)
		{
			case 27	: exit(1);
			case 'l':
			case 'L': filetoram(); clrscr(); show_help(); window(1,25,10,25); break;
			case 'E':
			case 'e': goto START;
			case 'M':
			case 'm': show_ram();break;
			case 'T':
			case 't': set_step();break;
			case 'F':
			case 'f': set_freq();break;
			case 'S':
			case 's': store_ram();break;
		}
	}
START:

decodeexe();
display_regs();
showhlram();
show_clock();

END:
	window(30,16,59,16);
	textcolor(RED);
	cprintf("PROGRAM HALTED");
	textcolor(WHITE);
	window(1,25,10,25);
	clrscr();
	while(1)
	{
		cprintf("\n\r-");
		r=getche();
		switch(r)
		{
			case 27	: exit(1);
			case 'l':
			case 'L': filetoram(); clrscr(); show_help(); window(1,25,10,25); break;
			case 'E':
			case 'e': reset_vars();goto START;
			case 'T':
			case 't': set_step();break;
			case 'F':
			case 'f': set_freq();break;
			case 'M':
			case 'm': show_ram();break;
			case 'S':
			case 's': store_ram();break;
		}
	}
}

