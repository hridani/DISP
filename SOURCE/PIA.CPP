#include <dos.h> 
#include "..\include\pia.h"    
#include "..\include\memnew.h"
#include "..\include\convert.h"

#include <stdio.h>
#include <conio.h>
byte error=0xf0;
List  input;
List output;
extern flag flag_change;
BasePia::BasePia(unsigned ba,byte nm,byte tA,byte tB,byte tC1,byte tC2)
{             
   baseadres=ba;
   number=nm;
   typeA=tA;
   typeB=tB;
   typeC1=tC1;
   typeC2=tC2;
   next=0;
}

  
  Matrix::Matrix(unsigned ba,byte nm,byte tA,byte  tB,byte tC1,byte tC2,byte mi,byte mo)
	   :BasePia(ba,nm,tA,tB,tC1,tC2)
  {                   
    byte si=0;
    byte so=0;
    if(tA==0)
	 si=si+8;
      else
	so=so+8;
       if(tB==0)
	si=si+8;
       else
	so=so+8;
       if(tC1==0)
	si=si+4;
       else
	so=so+4;
       if(tC2==0)
	si=si+4;
       else
	so=so+4;
	   pinput=NULL;
	   poutput=NULL;
	   pitable=NULL;
	   potable=NULL;
	   pvtable=NULL;
      minput=mi;
      moutput=mo;
      vinput=si -mi;
      voutput=so -mo;
      if(mi!=0 && mo!=0)
      {
	if(fNewMemory((void **)&pinput,(mo*mi)+vinput +1,&"TInput") ==FALSE)
	 printf("No memory");
       else
       *(pinput +(mo*mi)+vinput )=0xa5;//���� �� ������� ��� ��������� �� ���������
      }
      if(voutput!=0)
      {
	if(fNewMemory((void **)&poutput,voutput+1,&"TOutput") ==FALSE)
	 printf("No memory");
       else
	*(poutput+voutput)=0xa5;//���� �� ������� ��� ��������� �� ��������
      }
   }
    
 void Matrix::TMatrix(byte iA,byte iB,byte iC1,byte iC2)
 {    
    byte nmi=0;/* ���� ������ ������� ����������� � ��������� */
    byte nmo=0;/* ���� ������� ������� ����������� � ��������� */
    byte nvi=0;/* ���� ������ ������� ��� �������� ������� */
    unsigned *ptable; /* ������� �������� ��� ��������� � ����� */
    unsigned *pvt; /* �������� ��� ����� � ����� �� ���������� ������� */
    byte *pbtable;/* ������� �������� ��� ��������� � ����� */
    int vinp=0;
    int vout=0;
    extern byte numboutput[8];// ������ �� ������ ,����������� � �������
    /* ������������� �� ���� �� ���� � ���� �� ��������� ,����������� � ������� */
     tport[0].number=iA;
     tport[0].type=typeA;
     tport[1].number=iB;
     tport[1].type=typeB;
     tport[2].number=iC1;
     tport[2].type=typeC1;
     tport[3].number=iC2;
     tport[3].type=typeC2;
     /* ������� �� ���� �� ��������� ,����������� � ��������� */
     for(int  i=0; i<minput*moutput;i++)
       input.add();//�������� � ������� �� ���������
     for( i=0;i<4;i++)
     {                               
	if(tport[i].type==0) /* ���� */
	{
	   if(tport[i].number!=0)
	     nmi=nmi+1; /* ���� ������� ,����������� � ��������� */

	   if( tport[i].number<8 && i<2)//A ,B
	   {
	     nvi=nvi+1; /* ���� ������� ��� �������� ������� */
	     vinp=8-tport[i].number; /* ���� �������� �������  ��� ���� A,B*/
	     for(int j=0;j<vinp;j++)
		input.add(baseadres+i,j+tport[i].number);//�������� �� ���������� ������� ��� �������
	   }
	   if(tport[i].number<4 && i>1)  //C1 ,C2
	   {
	     nvi=nvi+1; /* ���� ������� ��� �������� ������� */
	     vinp=4-tport[i].number; /* ���� �������� �������  ��� ���� C1,C2*/
	     if(i!=3)
	     {
	      for(int j=0;j<vinp;j++)
		input.add(baseadres+i,j+tport[i].number);//�������� �� ���������� ������� ��� �������
	     }                       
	     else
	     {
		 for(int j=0;j<vinp;j++)
		     input.add(baseadres+i-1,4+j+tport[i].number);
	      }
	   }//if c1,c2
	}
	else   /* ������ */
	{
	   if(i<2)
	     vout=8-tport[i].number; /* ���� �������� ������  ��� ���� A,B*/
	   else
	     vout=4-tport[i].number; /* ���� �������� ������  ��� ���� C1,C2*/
			  /* ������������� �� �������� */
	int numb=0;
	int p=0;
	  if(i!=3)
	  {
	   for(int j=0;j<vout;j++)
	   {

	     p=0;
	     while(numboutput[p]!=0)
	     {
	      if(numb!=numboutput[p]) 
		p++;
	      else
	      { 
		numb++;
		p=0;
	      }
	     }
	     output.add(baseadres+i,numb);
	     numb=numb+1;
	   }
	  }
	  else
	  {
	    for(int j=0;j<vout;j++)
	      output.add(baseadres+i-1,4+j+tport[i].number);
	  }
    }  
  }
	     /* ������������� �� �������� */
     /* �������� �� ����� �� ������� � ����� */
	if(fNewMemory((void **)&pitable,( nmi*4)+1,&"TMinput") ==FALSE) /* �� �������� ������� */
	 printf("No memory");
     else
       *(pitable +nmi *4)=0xa5;     /* ���� �� ����� */
	if(fNewMemory((void **)&pvtable,( nvi*4)+1,&"TVinput") ==FALSE) /* �� �������� ������ */
	 printf("No memory");
     else
       *(pvtable +nvi *4)=0xa5;     /* ���� �� ����� */
       
	if(fNewMemory((void **)&potable,( moutput*4)+1,&"TMoutput") ==FALSE) /* �� �������� ������� */
	 printf("No memory");
     else
       *(potable +moutput *4)=0xa5;     /* ���� �� ����� */
       /* ������������� �� ��������� � ����� */
      ptable=(unsigned *)pitable; /* �������� ��� ����� � ����� �� ���������� ������� */
      pvt=(unsigned *)pvtable;
      int j=0;
      int k=0;
      for(i=0;i<4;i++)
     {  
	if(tport[i].type==0) /* ���� */
	{
	  if(tport[i].number!=0 ) /* � �������� ������� */
	  {  
	   switch (i)
	   {
	     case 0:
	     case 1:
	     case 2:
	       *(ptable+2*j)=baseadres+i; /* ����� �� ����� -���� */
	       *(pitable+2+4*j)=tport[i].number; /* ���� ������� � ��������� �� ���� ���� */
	       *(pitable+3+4*j)=0; /* ������ �� ������� �� ��������� */
	      break;
	    case 3:
	       *(ptable+2*j)=baseadres+i -1; /* ����� �� ����� -���� */
	       *(pitable+2+4*j)=tport[i].number;/* ���� ������� � ��������� �� ���� ���� */
	       *(pitable+3+4*j)=3;/* ������ �� ������� �� ��������� */
	      break;
	   }     
	  j++;
	 }
	 if(tport[i].number<8 && i<2) /* �������� ������� */
	 {
	       *(pvt+2*k)=baseadres+i; /* ����� �� ����� ��� �������� ������� */
	      *(pvtable+2+4*k)=8-tport[i].number; /* ���� �� ���������� ������� �� ���� A,B */
	     *(pvtable+3+4*k)=1;/* ������ �� ������� �� ��������� �� ���� C2 */
	     k++;
	   }

	 if(tport[i].number<4 && i>1) /* �������� ������� */
	 {
	   if(i!=3)
	   {    
	      *(pvt+2*k)=baseadres+i; /* ����� �� ����� ��� �������� ������� */
	      *(pvtable+3+4*k)=2;/* ������ �� ������� �� ��������� �� ���� A,B,C1*/
	   }
	   else
	   {    
	      *(pvt+2*k)=baseadres+i-1; /* ����� �� ����� ��� �������� ������� */
	      *(pvtable+3+4*k)=1;/* ������ �� ������� �� ��������� �� ���� C2 */
	   }
	   *(pvtable+2+4*k)=4-tport[i].number; /* ���� �� ���������� ������� �� ���� C1,C2 */
	   k++;
	   }

	 }  
	}   
	/* �������� ������ */   
	ptable=(unsigned *)potable;  /* �������� ��� ����� � ����� �� ���������� ������*/
	pbtable=potable;/* �������� ��� ����� � ����� �� ���������� ������*/
	for(i=0;i<4;i++)
	{
	  if(tport[i].number!=0 && tport[i].type==1)
	  {
	    for( j=0;j<tport[i].number;j++)
	    { 
	     if(i!=3)
	      *(ptable +2*j)=baseadres +i;/* ����� �� �������-����� */
	     else
	      *(ptable +2*j)=baseadres +i -1;/* ����� �� �������-����� */
	      *(pbtable+2 +4*j)=numboutput[j];/* ������� ����� �� ��� */
	      *(pbtable+3 +4*j)=0;/* ����� */
	    }
	    ptable=ptable +2* tport[i].number;
	    pbtable=(byte *)ptable;
	   }
	  }

 }
     
     
  
  void Matrix::write(void)
  {
     unsigned bao=baseadres;  /* ����� �� ���������- ������ */
     unsigned ba=baseadres; /* ����� �� PIA */
     byte *vadres=poutput;/* ����� � ������� �� ����� �� ������� �� ���������� ������ */
     byte voutput=0; /* ���� �� ���������� ������ � ���������*/
     byte value=0; /* ������� �� ���������� ���� */
     for(int nop=0;nop<4;nop++)       /* ���� ������� - ���������� ���� 4 */          
     {
	if(tport[nop].type==1)   /*�������� ���� ���������� ���� � ����� */
	{ 
	  if(nop==3)     
	    bao=ba+nop-1; /* ����������� �� ������ �� �����-����� ,��� C1 � C2 ������ � ���� � ��� */
	  else
	    bao=ba+nop; /* ����������� �� ������ �� �����-����� ,��� C1 � C2 ������ � ���� � ��� */
	  if(nop<2)  
	    voutput= 8- tport[nop].number;
	 else
	    voutput= 4- tport[nop].number;
	  if(nop!=2)
	    value=ByteConvert(vadres,voutput,1);
	  else
	    value=ByteConvert(vadres,voutput,0);
	 vadres=vadres +voutput;   
	 _outp(bao,value);
       }
     }
  }
    
 void Matrix::reada(void)
 {
   byte val=0;
   byte maska=0;
   byte numb=0;
   unsigned adresi=0;
   unsigned adreso=0;
   byte j=0;    
   byte *pi=pinput;
   byte strike=0;
   unsigned off=0;
   for(int i=0;i<moutput;i++)
   {
     adreso=*(unsigned *)(potable+4*i);
     val=_inp(adreso);
     maska=(*(potable +3 +4*i)) ^ 1;
     numb=*(potable +2 +4*i) +1;
     maska=maska<<numb;
     val=val | maska;
    _outp(adreso,val);//������� ���� �����
    j=0;
    do
    {
      adresi=*(unsigned *)(pitable +4*j);
      val=_inp(adresi); 
      numb=*(pitable +2 +4*j);
      strike=*(pitable +3 +4*j);
      BitConvert(val,pi, numb,strike);  
      pi=pi +numb;       
      j++;
     }
     while(*(pitable+4*j) !=0xA5);
      val=_inp(adreso);
     maska=(*(potable +3 +4*i)) ^ 0;
     numb=*(potable +2 +4*i) +1;
    val|=maska<<numb;
    _outp(adreso,val);//��������� ���� �����
   }  
   if(*pvtable!=0xa5)
   {
   }
      
 }
  void Matrix::read(void)
  {  
     byte *pto=potable;
     byte *pti=pitable;
     byte *pi=pinput;
     byte *pbi=pvtable;
     unsigned off=0;
     extern unsigned l_value;
   __asm
   { 
//     cli
     push es//1
     push di
     les di,pto
 et1: mov dx,es:[di]
     add di,2
     mov ax,es:[di]
     xor ah,1 /* maska */
     inc al
     mov cl,al
     in al,dx      
     rcr al,cl
     shr ah,1
     rcl al,cl
     out dx,al
     mov cx,l_value
    here:loop here
//    push es
  //  push di
//    call Delay
//    mov flag_change,0;
//    pop di
//    pop es
     sub di,2
     /* ������� */
     push ds //ds1
     push si
     lds si,pti
 et2: mov dx,ds:[si]
     add si,2
     mov bx,ds:[si]
     push ds
     push si
     lds si,pi /* ����� �� ����� � ������� */
     add si,off
     mov ax,0
     mov al,bh /* ������ */
     push ax
     mov al,bl /* ���� */
     push ax
     push ds
     push si      
     add off,ax
     mov ax,0
     in al,dx   
     push ax
     call BitConvert
     add sp ,10
     pop si
     pop ds
     add si,2
     cmp ds:[si],0xA5
     jne et2
     pop si
     pop ds//1==
     /* ���������� �� ������ */
     mov dx,es:[di]
     add di,2
     mov ax,es:[di]
     xor ah,0 /* maska */
     inc al
     mov cl,al
     in al,dx
     rcr al,cl
     shr ah,1
     rcl al,cl
     out dx,al
     mov cx,l_value
     here1:loop here1
     add di,2
     cmp es:[di],0xA5  
     je et11
     jmp et1
     /* �������� ������� */
 et11:    push ds
     push si
     lds si,pbi
     cmp ds:[si],0xa5
     je et4
 et3: mov dx,ds:[si]
     add si,2
     mov bx,ds:[si]
     push ds
     push si
     lds si,pi /* ����� �� ����� � ������� */
     add si,off
     mov ax,0
     mov al,bh /* ������ */
     push ax
     mov al,bl /* ���� */
     push ax
     push ds
     push si      
     add off,ax
     mov ax,0
     in al,dx   
     push ax
     call BitConvert
     add sp ,10
     pop si
     pop ds
     add si,2
     cmp ds:[si],0xA5
     jne et3
et4: pop si
     pop ds
     pop di
     pop es
       sti
     
   }
}
     
Matrix::~Matrix()
{
   FreeMemory(pinput);
   FreeMemory(poutput);
   FreeMemory(pitable);
   FreeMemory(potable);
   FreeMemory(pvtable);
   //printf("MATRIX  ");
}      
     
  
  
Linea::Linea(unsigned ba,byte nm,byte tA,byte  tB,byte tC1,byte tC2)
	   :BasePia(ba,nm,tA,tB,tC1,tC2)
{                  
      linput=0; /* ���� ������� */
      loutput=0;/* ���� ������ */
      byte nip=0; /* ���� ������� -������� */
      unsigned *ptable ;/* ������� �������� ��� ��������� � ����� �� �������� ������� */
      int j=0; /*������� ���������� */
      pinput=NULL;
      poutput=NULL;
      pitable=NULL;
      if(tA==0)
      {
			linput=linput+8;
			nip=nip+1;       
			for(int i=0;i<8;i++)
				 input.add(baseadres,i);
      }
      else
      {
	    	 loutput=loutput+8;
    	   	 for(int i=0;i<8;i++)
		 		output.add(baseadres,i);
      }

       if(tB==0)
       {
	 		linput=linput+8;
	 		nip=nip+1;
	 		for(int i=0;i<8;i++)
	   			input.add(baseadres+1,i);
       }
       else
      {
			loutput=loutput+8;
			for(int i=0;i<8;i++)
	 			output.add(baseadres+1,i);
       }
       if(tC1==0)
       {
	 		linput=linput+4;
	 		nip=nip+1;
	 		for(int i=0;i<4;i++)
	   			input.add(baseadres+2,i);
		}
       else
       {
				loutput=loutput+4;
				for(int i=0;i<4;i++)
	 			output.add(baseadres+2,i);
       }
       if(tC2==0)
       {
			 linput=linput+4;
			 nip=nip+1;
	 		 for(int i=0;i<4;i++)
	  			 input.add(baseadres+2,i+4);
       }
       else
       {
			loutput=loutput+4;
			for(int i=0;i<4;i++)
	 			output.add(baseadres+2,i+4);
       }
    
    if(linput!=0)
    {
      	if(fNewMemory((void **)&pinput,linput+1,&"TInput") ==FALSE)
			printf("No memory");
	    else
    	   *(pinput + linput)=0xa5;
    }                                         
    
    if(loutput!=0)
    {
      if(fNewMemory((void **)&poutput,loutput+1,&"TOutput") ==FALSE)
       printf("No memory");
     else
       *(poutput + loutput)=0xa5;
    }
    
	if(fNewMemory((void **)&pitable,( nip*4)+1,&"Tlinea") ==FALSE)
	 printf("No memory");
     else
       *(pitable +nip *4)=0xa5;     
      ptable=(unsigned *)pitable; 
      if(typeA==0 )
      {
	     *(ptable+2*j)=baseadres;
	     *(pitable+2+4*j)=8;
	     *(pitable+3+4*j)=0;
	     j++;
	}
      if(typeB==0 )
      {
	     *(ptable+2*j)=baseadres+1;
	     *(pitable+2+4*j)=8;
	     *(pitable+3+4*j)=0;
	     j++;
       }
       if(typeC1==0 )
       {
	     *(ptable+2*j)=baseadres+2;
	     *(pitable+2+4*j)=4;
	     *(pitable+3+4*j)=0;
	     j++;
       }
       if(typeC2==0 )
       {
	     *(ptable+2*j)=baseadres+2;
	     *(pitable+2+4*j)=4;
	     *(pitable+3+4*j)=3;
	}

}                
BasePia::~BasePia()
{
   _outp(baseadres,0xff);//�� �� ������� ��������
   return;
}
 
   Linea::~Linea()
   {
     FreeMemory(pinput);
     FreeMemory(poutput);
     FreeMemory(pitable);
   }
   
 void Linea::read(void)
{
    byte *pi=pinput;
    byte *pbi=pitable;              
     unsigned off=0;
    __asm
    {
	 push ds
	 push es
	 lds si,pbi
	 les di,pi
	 cmp ds:[si],0xA5
	 je et4
 et3:  mov dx,ds:[si]
	add si,2
	mov bx,ds:[si]
	add di,off
	mov ax,0
	mov al,bh /* ������ */
	push ax
	mov al,bl /* ���� */
	push ax
	push es
	push di      
	mov off,ax
	mov ax,0
	in al,dx   
	push ax
	call BitConvert
	add sp,10
	add si,2
	cmp ds:[si],0xA5
	jne et3
    et4:  pop es
	pop ds
  }   
}
     
void Linea::write(void)
{
     unsigned ba=baseadres;
     byte tA=typeA;
     byte tB=typeB;
     byte tC1=typeC1;
     byte tC2=typeC2; 
     byte *po=poutput;
     byte value=0;
     __asm
     {
       mov dx,ba
       les di,po
       
       cmp tA,1
       jnz et1
       mov ax,0
       push ax
       mov ax,8
       push ax
       push es
       push di
       call ByteConvert
       mov value,al
       out dx,al
       add di,8
 et1:inc dx
       cmp tB,1
       jnz et2
       mov ax,0
       push ax
       mov ax,8
       push ax
       push es
       push di
       call ByteConvert
       mov value,al
       out dx,al
       add di,8
 et2:inc dx
      cmp tC1,1
       jnz et3
       cmp tC2,1
       jnz et4
       mov ax,0
       push ax
       mov ax,8
       push ax
       push es
       push di
       call ByteConvert
       out dx,al
       jmp end
et3: cmp tC2,1
       jnz end
       mov ax,1
       push ax
       mov ax,4
       push ax
       push es
       push di
       call ByteConvert
       out dx,al
       jmp end
et4: mov ax,0
       push ax
       mov ax,4
       push ax
       push es
       push di
       call ByteConvert
       out dx,al
end:
   }
}       
		      



   void ListPia::find(unsigned ad)
{
      if(*cur==NULL)
	 return;
      if((*cur)->baseadres==ad)
	 return;
      cur=&(*cur)->next;
      find(ad);  
} 



flag ListPia ::add(char key,unsigned ad ,byte tA,byte tB,byte tC1,
	byte tC2,byte vi,byte vo)
{
      cur=&top;
      find(ad);
      if(*cur)
	   return FALSE;
      else
      {
		if(key=='m')
	 		*cur=new Matrix(ad,nm+1,tA,tB,tC1,tC2,vi,vo);   
      	 if(key=='l')
	 			*cur=new Linea(ad,nm+1,tA,tB,tC1,tC2);     
	 	if(key=='p')
	 			*cur=new LineaLpt(ad,nm+1,tA,tB,tC1,tC2) ;   
	 	if(key=='c')//���������� ������� �� 2 ���   -1���1 � �� ����� 0x378 
	 			 *cur=new MatrixLpt(0x378,ad) ;   //1adres ���1=0x378 ,(ot komp, 2 lpt1 � �������� � � ���������);
	 			
	nm++;
	return TRUE;
      }
}

Base::Base(unsigned nm)
{
   number=nm;  
   adres=0;
   value=0;      
   next=NULL;
}

Base::Base(unsigned nm,byte val)
{
   number=nm;
   if(val==0)
     value=0;      
   else
     value=0xff;
   next=NULL;
}

Base::Base(unsigned nm,unsigned ad,byte bt)
{
   number=nm;
   adres=ad;
   bit=bt;
   value=0;
   mask=0;
   next=NULL;
}

ListPia::ListPia()
{
  top=NULL;
  cur=&top;
  nm=0;
}

void ListPia::removeall(BasePia **bPia)
{
     if(*bPia==NULL)
       return;
     removeall(&((*bPia)->next));
     delete *bPia;
     nm--;
     *bPia=NULL; 
}
void List::find(unsigned nm)
{
      if(*cur==NULL)
	 return;
      if((*cur)->number==nm)
	 return;
      cur=&(*cur)->next;
      find(nm);  
} 

void List::removeall(Base **pb)
{
     if(*pb==NULL)
       return;
     removeall(&((*pb)->next));
     delete *pb;
     nm--;
     *pb=NULL; 
}

flag List ::add()
{
      cur=&top;
      find(nm+1);
      if(*cur)
	   return FALSE;
      else
      {
	 *cur=new Base(nm+1);   
	nm++;
	return TRUE;
      }
}

flag List ::add(unsigned ad,byte bt)
{
      cur=&top;
      find(nm+1);
      if(*cur)
	   return FALSE;
      else
      {
	 *cur=new Base(nm+1,ad,bt);   
	nm++;
	return TRUE;
      }
}

List::List()
{     
  top=NULL;
  cur=&top;
  nm=0;
}

byte& List::operator()  (unsigned nm)
{
  cur=&top;
  find(nm);
  return (*cur)->value;
}
 
byte& List::operator()  (unsigned nm,byte value)
{               
  Base *cr=NULL;
  cur=&top;
  find(nm);
  if(!*cur)
     return error;
 cr=*cur;
 __asm
 {
   push es
   push ds
   push si
   les si,cr 
   mov dx,es:[si]
   add si,2
   mov al,value
   mov es:[si],al
   inc si
   mov ah,es:[si]
   xor ah,al
   add si,1
   mov cl,es:[si]
   inc cl
   in al,dx
   rcr al,cl
   shr ah,1
   rcl al,cl
   out dx,al
   pop si
   pop ds
   pop es

 }
 return (*cur)->value;
}
 
 
LineaLpt::LineaLpt(unsigned ba,byte nm,byte tA,byte  tB,byte tC1,byte tC2)
	   :BasePia(ba,nm,tA,tB,tC1,tC2)
{                  
      linput=0; /* ���� ������� */
      loutput=0;/* ���� ������ */
      byte nip=0; /* ���� ������� -������� */
      unsigned *ptable ;/* ������� �������� ��� ��������� � ����� �� �������� ������� */
      int j=0; /*������� ���������� */
      pinput=NULL;
      poutput=NULL;
      pitable=NULL;       
        //��������� �� 7.04.2012  �� ������ �ys standartno lpt �� �������� ���� tB(Status port)3-7, tC(control port)0-3
        //���: 28.01.2013 -8 ������ ���� �, 5 ����� - ���� B, 4 ������ ���� C(C1)
      if(tA==0)
      {
			linput=linput+8;
			nip=nip+1;       
			for(int i=0;i<8;i++)
				 input.add(baseadres,i);
      }
      else
      {
	    	 loutput=loutput+8;
    	   	 for(int i=0;i<8;i++)
		 		output.add(baseadres,i);
      }
      
       if(tB==0)
       {
	 		linput=linput+5;
	 		nip=nip+1;
	 		for(int i=0;i<5;i++)
	   			input.add(baseadres+1,i+3);
       }
       
       if(tC1==1)
       {
	 	 loutput=loutput+4;
		for(int i=0;i<4;i++)
	 			output.add(baseadres+2,i);
       }
       
    
    if(linput!=0)
    {
      	if(fNewMemory((void **)&pinput,linput+1,&"TInput") ==FALSE)
			printf("No memory");
	    else
    	   *(pinput + linput)=0xa5;
    }                                         
    
    if(loutput!=0)
    {
      if(fNewMemory((void **)&poutput,loutput+1,&"TOutput") ==FALSE)
       printf("No memory");
     else
       *(poutput + loutput)=0xa5;
    }
    
	if(fNewMemory((void **)&pitable,( nip*4)+1,&"Tlinea") ==FALSE)
	 printf("No memory");
     else
       *(pitable +nip *4)=0xa5;     
      ptable=(unsigned *)pitable; 
       if(typeA==0 )
      {
	     *(ptable+2*j)=baseadres;
	     *(pitable+2+4*j)=8;
	     *(pitable+3+4*j)=0;
	     j++;
	}
      if(typeB==0 )
      {
	     *(ptable+2*j)=baseadres+1;
	     *(pitable+2+4*j)=5;
	     *(pitable+3+4*j)=1;
	     j++;
       }
       if(typeC1==0 )
       {
	     *(ptable+2*j)=baseadres+2;
	     *(pitable+2+4*j)=4;
	     *(pitable+3+4*j)=0;
	     j++;
       }
      }                


void LineaLpt::read(void)
{
    byte *pi=pinput;
    byte *pbi=pitable;              
     unsigned off=0;
    __asm
    {
	 push ds
	 push es
	 lds si,pbi
	 les di,pi
	 cmp ds:[si],0xA5
	 je et4
 et3:  mov dx,ds:[si]
	add si,2
	mov bx,ds:[si]
	add di,off
	mov ax,0
	mov al,bh /* ������ */
	push ax
	mov al,bl /* ���� */
	push ax
	push es
	push di      
	mov off,ax
	mov ax,0
	in al,dx   
	push ax
	call BitConvert
	add sp,10
	add si,2
	cmp ds:[si],0xA5
	jne et3
    et4:  pop es
	pop ds
  }   
}
     
void LineaLpt::write(void)
{
     unsigned ba=baseadres;
     byte tA=typeA;
     byte tB=typeB;
     byte tC1=typeC1;
     byte tC2=typeC2; 
     byte *po=poutput;
     byte value=0;
     __asm
     {
       mov dx,ba
       les di,po
       
       cmp tA,1
       jnz et1
       mov ax,0
       push ax
       mov ax,8
       push ax
       push es
       push di
       call ByteConvert
       mov value,al
       out dx,al
       add di,8
 et1:inc dx
       cmp tB,1
       jnz et2
       mov ax,0
       push ax
       mov ax,8
       push ax
       push es
       push di
       call ByteConvert
       mov value,al
       out dx,al
       add di,8
 et2:inc dx
      cmp tC1,1
       jnz et3
       cmp tC2,1
       jnz et4
       mov ax,0
       push ax
       mov ax,8
       push ax
       push es
       push di
       call ByteConvert
       out dx,al
       jmp end
et3: cmp tC2,1
       jnz end
       mov ax,1
       push ax
       mov ax,4
       push ax
       push es
       push di
       call ByteConvert
       out dx,al
       jmp end
et4: mov ax,0
       push ax
       mov ax,4
       push ax
       push es
       push di
       call ByteConvert
       out dx,al
end:
   }
}       
		      
   LineaLpt::~LineaLpt()
   {
     FreeMemory(pinput);
     FreeMemory(poutput);
     FreeMemory(pitable);
   }

MatrixLpt::MatrixLpt(unsigned ba1,unsigned ba2)
	   :BasePia(ba2,0,1,0,1,1)
  {     
  
  //�������� � 2 ���� LPT - �������� �� ��������� ,
  //LPT1- �����- ������ 8, ����B - ������� 5(D2-D7) , ����C1-������4
  //LPT2- ����A- ������ ,����B - 5�����, ���� C1- 4 ������
  // 2(3) �� �������� �� ���� � , LPT1 �� �������� ������ ��� 5 ����� �� ���� B, LPT2
  // ���������� ������ �� �������� ����A , LPT1 - 6 (5)������, ���� C1- 4 ������ , ��������-  5 ����� ����B, LPT1
  //   ����A, LPT2 - 8 ������, port C1, lpt2    -4 ������
                
       baseadres1=ba1;
       baseadres2=ba2;
	   pinput=NULL;
	   poutput=NULL;
	   pitable=NULL;
	   potable=NULL;
	   pvtable=NULL;
       minput=5;          
       // ��������� � �������� - �� ���� ���� ������ �� ������� � 2
       // �� ��������� ����� - 3
       
      //  moutput=2;//�� ����  
       // vinput=6;//za  kiro 22.01.2017
     //  voutput=22;         //�� ����    
     
      moutput=3;// �� �����
      vinput=5;//ili 5  
      voutput=21;// �� ��������� �����         
      
      if(minput!=0 && moutput!=0)
      {
   		if(fNewMemory((void **)&pinput,(moutput*minput)+vinput +1,&"TInput") ==FALSE)
	 		printf("No memory");
       else
       *(pinput +(moutput*minput)+vinput )=0xa5;//���� �� ������� ��� ��������� �� ���������
      }
      if(voutput!=0)
      {
		if(fNewMemory((void **)&poutput,voutput+1,&"TOutput") ==FALSE)
	 		printf("No memory");
       else
			*(poutput+voutput)=0xa5;//���� �� ������� ��� ��������� �� ��������
      }
   }
       
 void MatrixLpt::TMatrix()
 {    
    byte nmi=1;/* ���� ������ ������� ����������� � ��������� */
    byte nmo=1;/* ���� ������� ������� ����������� � ��������� */
    byte nvi=1;/* ���� ������ ������� ��� �������� ������� */   
    
    unsigned *ptable; /* ������� �������� ��� ��������� � ����� */
    unsigned *pvt; /* �������� ��� ����� � ����� �� ���������� ������� */
    byte *pbtable;/* ������� �������� ��� ��������� � ����� */
    int vinp=0;
    int vout=0;
//    extern byte numboutput[8];// ������ �� ������ ,����������� � �������
    
     /* ������� �� ���� �� ��������� ,����������� � ��������� */     
     
        for(int  i=0; i<minput*moutput;i++)    //2*5 ��������         ��� 3*5 (��� �� 2.07.2013)
           input.add();//�������� � ������� �� ���������        
     	for(int j=3;j<8;j++) //�������� �� ���������� ������� ��� ������� 
     			input.add(baseadres1+1,j);  
	 
	 //������
		//�����, ���1-�������� �� 2-7       �� ����       
	//	 for( i=2;i<8;i++)
		//  �����,���1-�������� �� 3-7       �� �����  
		        for( i=3;i<8;i++)
    			output.add(baseadres1,i);						
                                                        
         //���� C1, ���1,lpt2
		  for( i=0;i<4;i++)
    	 			output.add(baseadres1+2,i);	
    			//���� A,lpt2
    	  for( i=0;i<8;i++)
    			output.add(baseadres2,i);
      //���� C1lpt2
		  for( i=0;i<4;i++)
    	 			output.add(baseadres2+2,i);									

 
	     /* ������������� �� �������� */
     /* �������� �� ����� �� ������� � ����� */
	if(fNewMemory((void **)&pitable,( nmi*4)+1,&"TMinput") ==FALSE) /* �� �������� ������� */
	 printf("No memory");
     else
       *(pitable +nmi *4)=0xa5;     /* ���� �� ����� */
	
	if(fNewMemory((void **)&pvtable,( nvi*4)+1,&"TVinput") ==FALSE) /* �� �������� ������ */
	 printf("No memory");
     else
       *(pvtable +nvi *4)=0xa5;     /* ���� �� ����� */
       
	if(fNewMemory((void **)&potable,( moutput*4)+1,&"TMoutput") ==FALSE) /* �� �������� ������� */
	 printf("No memory");
     else
       *(potable +moutput *4)=0xa5;     /* ���� �� ����� */
    
       /* ������������� �� ��������� � ����� */
      ptable=(unsigned *)pitable; /* �������� ��� ����� � ����� �� ���������� ������� */
      pvt=(unsigned *)pvtable;
      j=0;
      int k=0;     
           *(ptable+2*j)=baseadres2+1; /* ����� �� ����� -���� */
	       *(pitable+2+4*j)=5; /* ���� ������� � ��������� �� ���� ���� */
	       *(pitable+3+4*j)=1; /* ������ �� ������� �� ��������� */
	        *(pvt+2*k)=baseadres1+1; /* ����� �� ����� ��� �������� ������� */
	        *(pvtable+2+4*k)=5; /* ���� �� ���������� ������� �� ���� A,B */
	        *(pvtable+3+4*k)=1;/* ������ �� ������� �� ��������� �� ���� C2 */
     

	/* �������� ������ */   
	ptable=(unsigned *)potable;  /* �������� ��� ����� � ����� �� ���������� ������*/
	pbtable=potable;/* �������� ��� ����� � ����� �� ���������� ������*/
      //������� -������ �� �������� ������ -�� ����2
	  //  for( j=0;j<2;j++)
	  // �� ��������� ����� -3         
	  //������  � �� �� ������ �������������� ������ �� 0 �� 2(3) �� �����,LPT1
	   for( j=0;j<3;j++)
	    { 
	      *(ptable +2*j)=baseadres1;/* ����� �� �������-����� */
	      *(pbtable+2 +4*j)=j;/* ������� ����� �� ��� */
	      *(pbtable+3 +4*j)=1;/* ����� */        //korekciq na 3.07.2013 - �� �������� � ����� -�������� ������
	   }
}
/****************************************************************/
/* Base � Base+2 ������ �� ������,� �� ����� Base+1 ������ �� �������    */
/*�.�. �� ����� ������ ������ � ������ �378 , Base+2 ������ 0,1 � 2 �� �� ������ ������ �� ���������.   */
/*�� �����  Base+1 ������ 3,4,5 � 7 �� �� ������� �� ��������� (������ ���� 12)  */
/*��  �����  Base+1 ��� 6 � ���� ,����� ������ � ������ ( 1 )   */
/*��  �����  Base+2 ��� 3 � ��������� ����� ,����� � ����� ������� �������� 10kHz � ��������� ���������� ������ ���������.��� �� �� ��������.
�� ����� ������ ����� 13 ������ � 5 ����� ���� ���������� 18 �����((12+1)+5) � 20 ������ (8+(8+5))    */
        
void MatrixLpt::TMatrixVersion2017()
 {    
    byte nmi=1;/* ���� ������ ������� ����������� � ��������� */
    byte nmo=1;/* ���� ������� ������� ����������� � ��������� */
    byte nvi=1;/* ���� ������ ������� ��� �������� ������� , 1- ���� B, lpt2,  */         
    /* 2-�� - ���� B,lpt1 -6 ����� ...�� �� ���� ���� ���������� �������*/
    
    unsigned *ptable; /* ������� �������� ��� ��������� � ����� */
    unsigned *pvt; /* �������� ��� ����� � ����� �� ���������� ������� */
    byte *pbtable;/* ������� �������� ��� ��������� � ����� */
    int vinp=0;
    int vout=0;
//    extern byte numboutput[8];// ������ �� ������ ,����������� � �������
    
     /* ������� �� ���� �� ��������� ,����������� � ��������� */     
     
        for(int  i=0; i<minput*moutput;i++)    // 3*4 (���. 04.2017) =12   , �� �� �� 3*5=15, ������ ���������� ����� � 6 , � 7 � � �������
           input.add();//�������� � ������� �� ���������                
           
     	for(int j=3;j<8;j++) //�������� �� ���������� ������� ��� �������   12+5=17 
     			input.add(baseadres2+1, j);  
	    // input.add(baseadres1+1, 6);   //base1+1, bit 6, "������"       17+1=18 
	 //������

		//  �����,���1-�������� �� 0-7 
		   for( i=0;i<8;i++)
    			output.add(baseadres1,i);		//�������� ������ 8				
    	//���� A,lpt2
    	  for( i=0;i<8;i++)
    			output.add(baseadres2,i); //�������� ������ 8+8=16
  		//���� C1lpt2
		  for( i=0;i<4;i++)
    	 			output.add(baseadres2+2,i);	   	//�������� ������ 16+4=20		
    			                                                      
         //���� C1, ���1,lpt2   = �� �������
	//	  for( i=0;i<4;i++)
    //	 			output.add(baseadres1+2,i);	
 
     /* ������������� �� �������� */
     /* �������� �� ����� �� ������� � ����� */
	if(fNewMemory((void **)&pitable,( nmi*4)+1,&"TMinput") ==FALSE) /* �� �������� ������� */
	 printf("No memory");
     else
       *(pitable +nmi *4)=0xa5;     /* ���� �� ����� */
	
	if(fNewMemory((void **)&pvtable,( nvi*4)+1,&"TVinput") ==FALSE) /* �� �������� ������� */
	 printf("No memory");
     else
       *(pvtable +nvi *4)=0xa5;     /* ���� �� ����� */
       
	if(fNewMemory((void **)&potable,( moutput*4)+1,&"TMoutput") ==FALSE) /* �������� ������ */
	 printf("No memory");
     else
       *(potable +moutput *4)=0xa5;     /* ���� �� ����� */
    
       /* ������������� �� ��������� � ����� */
      ptable=(unsigned *)pitable; /* �������� ��� ����� � ����� �� ���������� ������� */
      pvt=(unsigned *)pvtable;
      j=0;
      int k=0;     
           *(ptable+2*j)=baseadres1+1; /* ����� �� ����� -���� */
	       *(pitable+2+4*j)=5; /* ���� ������� � ��������� �� ���� ���� */
	       *(pitable+3+4*j)=1; /* ������ �� ������� �� ��������� */ 
	       
	        *(pvt+2*k)=baseadres2+1; /* ����� �� ����� ��� �������� ������� */
	        *(pvtable+2+4*k)=5; /* ���� �� ���������� ������� �� ���� B */
	        *(pvtable+3+4*k)=1;/* ������ �� ������� �� ��������� �� ���� � */
     

	/* �������� ������ */   
	ptable=(unsigned *)potable;  /* �������� ��� ����� � ����� �� ���������� ������*/
	pbtable=potable;/* �������� ��� ����� � ����� �� ���������� ������*/
      //������� -������ �� �������� ������ -
	  //������  � �� �� ������ �������������� ������ �� 0 �� 2(3) �� ����C,LPT1
	   for( j=0;j<3;j++)
	    { 
	      *(ptable +2*j)=baseadres1+2;/* ����� �� �������-����� */
	      *(pbtable+2 +4*j)=j;/* ������� ����� �� ��� */    
	      if(j==0) {
	       *(pbtable+3 +4*j)=1;/* ����� */
	      }else{
	      *(pbtable+3 +4*j)=0;/* ����� */        //korekciq na 3.07.2013 - �� �������� � ����� -�������� ������    
	      }
	   }
}        
         
         
void MatrixLpt::read(void)
  {  
     byte *pto=potable; /* �������� ��� ����������� �� �������� �������� � ������� */
     byte *pti=pitable; /* �������� ��� ��������� � ����� �� ���������� ������� */
     byte *pi=pinput;  ;/* �������� ��� ����������� �� ��������� � ������� */
     byte *pbi=pvtable;   /* �������� ��� ��������� � ����� �� ���������� ������� */
     unsigned off=0;
     extern unsigned l_value;
   __asm
   { 
//     cli
     push es//1
     push di
     les di,pto
 et1: mov dx,es:[di]
     add di,2
     mov ax,es:[di]
     xor ah,1 /* maska */
     inc al
     mov cl,al
     in al,dx      
     rcr al,cl
     shr ah,1
     rcl al,cl
     out dx,al
     mov cx,l_value
    here:loop here    
   // mov cx,l_value
   // here2:loop here2    
   //  mov cx,l_value
   // here3:loop here3     
   
//    push es
  //  push di
//    call Delay
//    mov flag_change,0;
//    pop di
//    pop es
     sub di,2
     /* ������� */
     push ds //ds1
     push si
     lds si,pti
 et2: mov dx,ds:[si]
     add si,2
     mov bx,ds:[si]
     push ds
     push si
     lds si,pi /* ����� �� ����� � ������� */
     add si,off
     mov ax,0
     mov al,bh /* ������ */
     push ax
     mov al,bl /* ���� */
     push ax
     push ds
     push si      
     add off,ax
     mov ax,0
     in al,dx   
     push ax
     call BitConvert
     add sp ,10
     pop si
     pop ds
     add si,2
     cmp ds:[si],0xA5
     jne et2
     pop si
     pop ds//1==
     /* ���������� �� ������ */
     mov dx,es:[di]
     add di,2
     mov ax,es:[di]
     xor ah,0 /* maska */
     inc al
     mov cl,al
     in al,dx
     rcr al,cl
     shr ah,1
     rcl al,cl
     out dx,al
     mov cx,l_value
     here1:loop here1
     add di,2
     cmp es:[di],0xA5  
     je et11
     jmp et1
     /* �������� ������� */
 et11:    push ds
     push si
     lds si,pbi
     cmp ds:[si],0xa5
     je et4
 et3: mov dx,ds:[si]
     add si,2
     mov bx,ds:[si]
     push ds
     push si
     lds si,pi /* ����� �� ����� � ������� */
     add si,off
     mov ax,0
     mov al,bh /* ������ */
     push ax
     mov al,bl /* ���� */
     push ax
     push ds
     push si      
     add off,ax
     mov ax,0
     in al,dx   
     push ax
     call BitConvert
     add sp ,10
     pop si
     pop ds
     add si,2
     cmp ds:[si],0xA5
     jne et3
et4: pop si
     pop ds
     pop di
     pop es
       sti
     
   }
}

MatrixLpt::~MatrixLpt()
{
   FreeMemory(pinput);
   FreeMemory(poutput);
   FreeMemory(pitable);
   FreeMemory(potable);
   FreeMemory(pvtable);
   //printf("MATRIX  ");
}      

 void MatrixLpt::write(void)
  {
     unsigned bao=baseadres;  /* ����� �� ���������- ������ */
     unsigned ba=baseadres; /* ����� �� PIA */
     byte *vadres=poutput;/* ����� � ������� �� ����� �� ������� �� ���������� ������ */
//    byte voutput=22; /* ���� �� ���������� ������ � ��������� �� ����*/          
  // byte voutput=21; /* ���� �� ���������� ������ � ��������� �� ��������� �����*/    
    byte voutput=20; /* ���� �� ���������� ������04.2017*/    
     byte value=0; /* ������� �� ���������� ���� */     
     
      
      //   value=ByteConvert(vadres,6,0);    //      �� ���� -��� �������� 6 ������ �� �����, LPT1
      // value=ByteConvert(vadres,5,0);        //  �� ���������  -��� �������� 5 ������ �� �����, LPT1 
        value=ByteConvert(vadres,8,0);        // ��� �������� 8 ������ �� �����, LPT1     ,04.2017
      //   vadres=vadres + 6;    // �� ����
        // vadres=vadres + 5;//�� �����  
           vadres=vadres + 8;//04.2017
         _outp(baseadres1,value);      
         //���� C ,lpt1 
         //04.2017 - ���� �������� �������
    //      value=ByteConvert(vadres,4,1);  
    //      vadres=vadres +4;   
   //      _outp(baseadres1+2,value);     
         
            //���� �, lpt2
         value=ByteConvert(vadres,8,0);   
         vadres=vadres +8;   
         _outp(baseadres2,value); 
         //port C, lpt2 
          value=ByteConvert(vadres,4,1);      
          vadres=vadres +4;   
         _outp(baseadres2+2,value);   
    	
  }