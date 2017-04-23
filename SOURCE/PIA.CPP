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
       *(pinput +(mo*mi)+vinput )=0xa5;//край на паметта със стойности за входовете
      }
      if(voutput!=0)
      {
	if(fNewMemory((void **)&poutput,voutput+1,&"TOutput") ==FALSE)
	 printf("No memory");
       else
	*(poutput+voutput)=0xa5;//край на паметта със стойности за изходите
      }
   }
    
 void Matrix::TMatrix(byte iA,byte iB,byte iC1,byte iC2)
 {    
    byte nmi=0;/* брой входни портове участвуващи в матрицата */
    byte nmo=0;/* брой изходни портове участвуващи в матрицата */
    byte nvi=0;/* брой входни портове със свободни входове */
    unsigned *ptable; /* помощен указател към таблиците с данни */
    unsigned *pvt; /* указател към блока с данни за свободните входове */
    byte *pbtable;/* помощен указател към таблиците с данни */
    int vinp=0;
    int vout=0;
    extern byte numboutput[8];// номера на изходи ,участвуващи в матрица
    /* инициализация на типа на порт и броя на крачетата ,участвуващи в матрица */
     tport[0].number=iA;
     tport[0].type=typeA;
     tport[1].number=iB;
     tport[1].type=typeB;
     tport[2].number=iC1;
     tport[2].type=typeC1;
     tport[3].number=iC2;
     tport[3].type=typeC2;
     /* търсене на броя на портовете ,участвуващи в матрицата */
     for(int  i=0; i<minput*moutput;i++)
       input.add();//добавяне в списъка на входовете
     for( i=0;i<4;i++)
     {                               
	if(tport[i].type==0) /* вход */
	{
	   if(tport[i].number!=0)
	     nmi=nmi+1; /* брой портове ,участвуващи в матрицата */

	   if( tport[i].number<8 && i<2)//A ,B
	   {
	     nvi=nvi+1; /* брой портове със свободни входове */
	     vinp=8-tport[i].number; /* брой свободни входове  при порт A,B*/
	     for(int j=0;j<vinp;j++)
		input.add(baseadres+i,j+tport[i].number);//добавяне на свободните входове към списъка
	   }
	   if(tport[i].number<4 && i>1)  //C1 ,C2
	   {
	     nvi=nvi+1; /* брой портове със свободни входове */
	     vinp=4-tport[i].number; /* брой свободни входове  при порт C1,C2*/
	     if(i!=3)
	     {
	      for(int j=0;j<vinp;j++)
		input.add(baseadres+i,j+tport[i].number);//добавяне на свободните входове към списъка
	     }                       
	     else
	     {
		 for(int j=0;j<vinp;j++)
		     input.add(baseadres+i-1,4+j+tport[i].number);
	      }
	   }//if c1,c2
	}
	else   /* изходи */
	{
	   if(i<2)
	     vout=8-tport[i].number; /* брой свободни изходи  при порт A,B*/
	   else
	     vout=4-tport[i].number; /* брой свободни изходи  при порт C1,C2*/
			  /* инициализация на изходите */
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
	     /* инициализация на изходите */
     /* заделяне на памет за блокове с данни */
	if(fNewMemory((void **)&pitable,( nmi*4)+1,&"TMinput") ==FALSE) /* за матрични входове */
	 printf("No memory");
     else
       *(pitable +nmi *4)=0xa5;     /* край на блока */
	if(fNewMemory((void **)&pvtable,( nvi*4)+1,&"TVinput") ==FALSE) /* за матрични изходи */
	 printf("No memory");
     else
       *(pvtable +nvi *4)=0xa5;     /* край на блока */
       
	if(fNewMemory((void **)&potable,( moutput*4)+1,&"TMoutput") ==FALSE) /* за свободни входове */
	 printf("No memory");
     else
       *(potable +moutput *4)=0xa5;     /* край на блока */
       /* инициализация на блоковете с данни */
      ptable=(unsigned *)pitable; /* указател към блока с данни за матричните входове */
      pvt=(unsigned *)pvtable;
      int j=0;
      int k=0;
      for(i=0;i<4;i++)
     {  
	if(tport[i].type==0) /* вход */
	{
	  if(tport[i].number!=0 ) /* с матрични входове */
	  {  
	   switch (i)
	   {
	     case 0:
	     case 1:
	     case 2:
	       *(ptable+2*j)=baseadres+i; /* адрес на порта -вход */
	       *(pitable+2+4*j)=tport[i].number; /* брой крачета в матрицата от този порт */
	       *(pitable+3+4*j)=0; /* посока на вземане на крачетата */
	      break;
	    case 3:
	       *(ptable+2*j)=baseadres+i -1; /* адрес на порта -вход */
	       *(pitable+2+4*j)=tport[i].number;/* брой крачета в матрицата от този порт */
	       *(pitable+3+4*j)=3;/* посока на вземане на крачетата */
	      break;
	   }     
	  j++;
	 }
	 if(tport[i].number<8 && i<2) /* свободни входове */
	 {
	       *(pvt+2*k)=baseadres+i; /* адрес на порта със свободни входове */
	      *(pvtable+2+4*k)=8-tport[i].number; /* брой на свободните входове за порт A,B */
	     *(pvtable+3+4*k)=1;/* посока на вземане на крачетата за порт C2 */
	     k++;
	   }

	 if(tport[i].number<4 && i>1) /* свободни входове */
	 {
	   if(i!=3)
	   {    
	      *(pvt+2*k)=baseadres+i; /* адрес на порта със свободни входове */
	      *(pvtable+3+4*k)=2;/* посока на вземане на крачетата за порт A,B,C1*/
	   }
	   else
	   {    
	      *(pvt+2*k)=baseadres+i-1; /* адрес на порта със свободни входове */
	      *(pvtable+3+4*k)=1;/* посока на вземане на крачетата за порт C2 */
	   }
	   *(pvtable+2+4*k)=4-tport[i].number; /* брой на свободните входове за порт C1,C2 */
	   k++;
	   }

	 }  
	}   
	/* матрични изходи */   
	ptable=(unsigned *)potable;  /* указател към блока с данни за матричните изходи*/
	pbtable=potable;/* указател към блока с данни за матричните изходи*/
	for(i=0;i<4;i++)
	{
	  if(tport[i].number!=0 && tport[i].type==1)
	  {
	    for( j=0;j<tport[i].number;j++)
	    { 
	     if(i!=3)
	      *(ptable +2*j)=baseadres +i;/* адрес на крачето-изход */
	     else
	      *(ptable +2*j)=baseadres +i -1;/* адрес на крачето-изход */
	      *(pbtable+2 +4*j)=numboutput[j];/* пореден номер на бит */
	      *(pbtable+3 +4*j)=0;/* маска */
	    }
	    ptable=ptable +2* tport[i].number;
	    pbtable=(byte *)ptable;
	   }
	  }

 }
     
     
  
  void Matrix::write(void)
  {
     unsigned bao=baseadres;  /* адрес на портовете- изходи */
     unsigned ba=baseadres; /* адрес на PIA */
     byte *vadres=poutput;/* адрес в паметта за запис на байтове от свободните изходи */
     byte voutput=0; /* брой на свободните изходи в матрицата*/
     byte value=0; /* задание на съответния порт */
     for(int nop=0;nop<4;nop++)       /* брой портове - максимален брой 4 */          
     {
	if(tport[nop].type==1)   /*проверка дали съответния порт е изход */
	{ 
	  if(nop==3)     
	    bao=ba+nop-1; /* изчисляване на адреса на порта-изход ,при C1 и C2 адреса е един и същ */
	  else
	    bao=ba+nop; /* изчисляване на адреса на порта-изход ,при C1 и C2 адреса е един и същ */
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
    _outp(adreso,val);//свтваме матр иьход
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
    _outp(adreso,val);//изгасваме матр иьход
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
     /* входове */
     push ds //ds1
     push si
     lds si,pti
 et2: mov dx,ds:[si]
     add si,2
     mov bx,ds:[si]
     push ds
     push si
     lds si,pi /* адрес за запис в паметта */
     add si,off
     mov ax,0
     mov al,bh /* посока */
     push ax
     mov al,bl /* брой */
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
     /* изключване на изхода */
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
     /* свободни входове */
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
     lds si,pi /* адрес за запис в паметта */
     add si,off
     mov ax,0
     mov al,bh /* посока */
     push ax
     mov al,bl /* брой */
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
      linput=0; /* брой входове */
      loutput=0;/* брой изходи */
      byte nip=0; /* брой портове -входове */
      unsigned *ptable ;/* помошен указател към таблицата с данни за входните портове */
      int j=0; /*помощна променлива */
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
   _outp(baseadres,0xff);//да се изгасят изходите
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
	mov al,bh /* посока */
	push ax
	mov al,bl /* брой */
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
	 	if(key=='c')//кръстосана матрица от 2 лпт   -1лпт1 е на адрес 0x378 
	 			 *cur=new MatrixLpt(0x378,ad) ;   //1adres лпт1=0x378 ,(ot komp, 2 lpt1 е различно и е параметър);
	 			
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
      linput=0; /* брой входове */
      loutput=0;/* брой изходи */
      byte nip=0; /* брой портове -входове */
      unsigned *ptable ;/* помошен указател към таблицата с данни за входните портове */
      int j=0; /*помощна променлива */
      pinput=NULL;
      poutput=NULL;
      pitable=NULL;       
        //Забележка от 7.04.2012  за работа сys standartno lpt се използва само tB(Status port)3-7, tC(control port)0-3
        //заб: 28.01.2013 -8 изхода порт А, 5 входа - порт B, 4 изхода порт C(C1)
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
	mov al,bh /* посока */
	push ax
	mov al,bl /* брой */
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
  
  //заложено е 2 броя LPT - адресите са параметри ,
  //LPT1- портА- изходи 8, портB - входове 5(D2-D7) , портC1-изходи4
  //LPT2- портA- изходи ,портB - 5входа, порт C1- 4 изхода
  // 2(3) от изходите на порт А , LPT1 са матрични изходи със 5 входа на порт B, LPT2
  // останалите изходи са свободни портA , LPT1 - 6 (5)изхода, порт C1- 4 изхода , свободни-  5 входа портB, LPT1
  //   портA, LPT2 - 8 изхода, port C1, lpt2    -4 изхода
                
       baseadres1=ba1;
       baseadres2=ba2;
	   pinput=NULL;
	   poutput=NULL;
	   pitable=NULL;
	   potable=NULL;
	   pvtable=NULL;
       minput=5;          
       // направена е поправка - за Киро броя изходи за матрица е 2
       // за автомивка софия - 3
       
      //  moutput=2;//за киро  
       // vinput=6;//za  kiro 22.01.2017
     //  voutput=22;         //за КИРО    
     
      moutput=3;// за софия
      vinput=5;//ili 5  
      voutput=21;// за автомивка София         
      
      if(minput!=0 && moutput!=0)
      {
   		if(fNewMemory((void **)&pinput,(moutput*minput)+vinput +1,&"TInput") ==FALSE)
	 		printf("No memory");
       else
       *(pinput +(moutput*minput)+vinput )=0xa5;//край на паметта със стойности за входовете
      }
      if(voutput!=0)
      {
		if(fNewMemory((void **)&poutput,voutput+1,&"TOutput") ==FALSE)
	 		printf("No memory");
       else
			*(poutput+voutput)=0xa5;//край на паметта със стойности за изходите
      }
   }
       
 void MatrixLpt::TMatrix()
 {    
    byte nmi=1;/* брой входни портове участвуващи в матрицата */
    byte nmo=1;/* брой изходни портове участвуващи в матрицата */
    byte nvi=1;/* брой входни портове със свободни входове */   
    
    unsigned *ptable; /* помощен указател към таблиците с данни */
    unsigned *pvt; /* указател към блока с данни за свободните входове */
    byte *pbtable;/* помощен указател към таблиците с данни */
    int vinp=0;
    int vout=0;
//    extern byte numboutput[8];// номера на изходи ,участвуващи в матрица
    
     /* търсене на броя на портовете ,участвуващи в матрицата */     
     
        for(int  i=0; i<minput*moutput;i++)    //2*5 матрични         или 3*5 (кор от 2.07.2013)
           input.add();//добавяне в списъка на входовете        
     	for(int j=3;j<8;j++) //добавяне на свободните входове към списъка 
     			input.add(baseadres1+1,j);  
	 
	 //изходи
		//портА, лпт1-свободни от 2-7       за КИРО       
	//	 for( i=2;i<8;i++)
		//  портА,лпт1-свободни от 3-7       за СОФИЯ  
		        for( i=3;i<8;i++)
    			output.add(baseadres1,i);						
                                                        
         //порт C1, лпт1,lpt2
		  for( i=0;i<4;i++)
    	 			output.add(baseadres1+2,i);	
    			//порт A,lpt2
    	  for( i=0;i<8;i++)
    			output.add(baseadres2,i);
      //порт C1lpt2
		  for( i=0;i<4;i++)
    	 			output.add(baseadres2+2,i);									

 
	     /* инициализация на изходите */
     /* заделяне на памет за блокове с данни */
	if(fNewMemory((void **)&pitable,( nmi*4)+1,&"TMinput") ==FALSE) /* за матрични входове */
	 printf("No memory");
     else
       *(pitable +nmi *4)=0xa5;     /* край на блока */
	
	if(fNewMemory((void **)&pvtable,( nvi*4)+1,&"TVinput") ==FALSE) /* за матрични изходи */
	 printf("No memory");
     else
       *(pvtable +nvi *4)=0xa5;     /* край на блока */
       
	if(fNewMemory((void **)&potable,( moutput*4)+1,&"TMoutput") ==FALSE) /* за свободни входове */
	 printf("No memory");
     else
       *(potable +moutput *4)=0xa5;     /* край на блока */
    
       /* инициализация на блоковете с данни */
      ptable=(unsigned *)pitable; /* указател към блока с данни за матричните входове */
      pvt=(unsigned *)pvtable;
      j=0;
      int k=0;     
           *(ptable+2*j)=baseadres2+1; /* адрес на порта -вход */
	       *(pitable+2+4*j)=5; /* брой крачета в матрицата от този порт */
	       *(pitable+3+4*j)=1; /* посока на вземане на крачетата */
	        *(pvt+2*k)=baseadres1+1; /* адрес на порта със свободни входове */
	        *(pvtable+2+4*k)=5; /* брой на свободните входове за порт A,B */
	        *(pvtable+3+4*k)=1;/* посока на вземане на крачетата за порт C2 */
     

	/* матрични изходи */   
	ptable=(unsigned *)potable;  /* указател към блока с данни за матричните изходи*/
	pbtable=potable;/* указател към блока с данни за матричните изходи*/
      //крачета -битове на матрични изходи -за киро2
	  //  for( j=0;j<2;j++)
	  // за автомивка софия -3         
	  //идеята  е че се вземат последователно битове от 0 до 2(3) от портА,LPT1
	   for( j=0;j<3;j++)
	    { 
	      *(ptable +2*j)=baseadres1;/* адрес на крачето-изход */
	      *(pbtable+2 +4*j)=j;/* пореден номер на бит */
	      *(pbtable+3 +4*j)=1;/* маска */        //korekciq na 3.07.2013 - за платката в софия -обърната логика
	   }
}
/****************************************************************/
/* Base и Base+2 винаги са изходи,а на адрес Base+1 винаги са входове    */
/*Т.к. на първа платка адреса е винаги х378 , Base+2 битове 0,1 и 2 да са твърдо изходи за матрицата.   */
/*На адрес  Base+1 битове 3,4,5 и 7 да са входове на матрицата (стават общо 12)  */
/*На  адрес  Base+1 бит 6 е вход ,който винаги е Авария ( 1 )   */
/*На  адрес  Base+2 бит 3 е специален изход ,който в други системи генерира 10kHz и разрешава останалите изходи хардуерно.Ние не го ползваме.
На втора платка имаме 13 изхода и 5 входа така получаваме 18 входа((12+1)+5) и 20 изхода (8+(8+5))    */
        
void MatrixLpt::TMatrixVersion2017()
 {    
    byte nmi=1;/* брой входни портове участвуващи в матрицата */
    byte nmo=1;/* брой изходни портове участвуващи в матрицата */
    byte nvi=1;/* брой входни портове със свободни входове , 1- порт B, lpt2,  */         
    /* 2-ри - порт B,lpt1 -6 краче ...ще се чете чрез матричните входове*/
    
    unsigned *ptable; /* помощен указател към таблиците с данни */
    unsigned *pvt; /* указател към блока с данни за свободните входове */
    byte *pbtable;/* помощен указател към таблиците с данни */
    int vinp=0;
    int vout=0;
//    extern byte numboutput[8];// номера на изходи ,участвуващи в матрица
    
     /* търсене на броя на портовете ,участвуващи в матрицата */     
     
        for(int  i=0; i<minput*moutput;i++)    // 3*4 (кор. 04.2017) =12   , но ще са 3*5=15, защото свободното краче е 6 , а 7 е в матрица
           input.add();//добавяне в списъка на входовете                
           
     	for(int j=3;j<8;j++) //добавяне на свободните входове към списъка   12+5=17 
     			input.add(baseadres2+1, j);  
	    // input.add(baseadres1+1, 6);   //base1+1, bit 6, "авария"       17+1=18 
	 //изходи

		//  портА,лпт1-свободни от 0-7 
		   for( i=0;i<8;i++)
    			output.add(baseadres1,i);		//свободни изходи 8				
    	//порт A,lpt2
    	  for( i=0;i<8;i++)
    			output.add(baseadres2,i); //свободни изходи 8+8=16
  		//порт C1lpt2
		  for( i=0;i<4;i++)
    	 			output.add(baseadres2+2,i);	   	//свободни изходи 16+4=20		
    			                                                      
         //порт C1, лпт1,lpt2   = за матрица
	//	  for( i=0;i<4;i++)
    //	 			output.add(baseadres1+2,i);	
 
     /* инициализация на изходите */
     /* заделяне на памет за блокове с данни */
	if(fNewMemory((void **)&pitable,( nmi*4)+1,&"TMinput") ==FALSE) /* за матрични входове */
	 printf("No memory");
     else
       *(pitable +nmi *4)=0xa5;     /* край на блока */
	
	if(fNewMemory((void **)&pvtable,( nvi*4)+1,&"TVinput") ==FALSE) /* за свободни входове */
	 printf("No memory");
     else
       *(pvtable +nvi *4)=0xa5;     /* край на блока */
       
	if(fNewMemory((void **)&potable,( moutput*4)+1,&"TMoutput") ==FALSE) /* матрични изходи */
	 printf("No memory");
     else
       *(potable +moutput *4)=0xa5;     /* край на блока */
    
       /* инициализация на блоковете с данни */
      ptable=(unsigned *)pitable; /* указател към блока с данни за матричните входове */
      pvt=(unsigned *)pvtable;
      j=0;
      int k=0;     
           *(ptable+2*j)=baseadres1+1; /* адрес на порта -вход */
	       *(pitable+2+4*j)=5; /* брой крачета в матрицата от този порт */
	       *(pitable+3+4*j)=1; /* посока на вземане на крачетата */ 
	       
	        *(pvt+2*k)=baseadres2+1; /* адрес на порта със свободни входове */
	        *(pvtable+2+4*k)=5; /* брой на свободните входове за порт B */
	        *(pvtable+3+4*k)=1;/* посока на вземане на крачетата за порт Б */
     

	/* матрични изходи */   
	ptable=(unsigned *)potable;  /* указател към блока с данни за матричните изходи*/
	pbtable=potable;/* указател към блока с данни за матричните изходи*/
      //крачета -битове на матрични изходи -
	  //идеята  е че се вземат последователно битове от 0 до 2(3) от портC,LPT1
	   for( j=0;j<3;j++)
	    { 
	      *(ptable +2*j)=baseadres1+2;/* адрес на крачето-изход */
	      *(pbtable+2 +4*j)=j;/* пореден номер на бит */
	      *(pbtable+3 +4*j)=1;/* маска */        //korekciq na 3.07.2013 - за платката в софия -обърната логика
	   }
}        
         
         
void MatrixLpt::read(void)
  {  
     byte *pto=potable; /* указател към стойностите на матрчини изходите в паметта */
     byte *pti=pitable; /* указател към таблицата с данни за матричните портове */
     byte *pi=pinput;  ;/* указател към стойностите на входовете в паметта */
     byte *pbi=pvtable;   /* указател към таблицата с данни за свободните портове */
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
     /* входове */
     push ds //ds1
     push si
     lds si,pti
 et2: mov dx,ds:[si]
     add si,2
     mov bx,ds:[si]
     push ds
     push si
     lds si,pi /* адрес за запис в паметта */
     add si,off
     mov ax,0
     mov al,bh /* посока */
     push ax
     mov al,bl /* брой */
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
     /* изключване на изхода */
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
     /* свободни входове */
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
     lds si,pi /* адрес за запис в паметта */
     add si,off
     mov ax,0
     mov al,bh /* посока */
     push ax
     mov al,bl /* брой */
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
     unsigned bao=baseadres;  /* адрес на портовете- изходи */
     unsigned ba=baseadres; /* адрес на PIA */
     byte *vadres=poutput;/* адрес в паметта за запис на байтове от свободните изходи */
//    byte voutput=22; /* брой на свободните изходи в матрицата за киро*/          
  // byte voutput=21; /* брой на свободните изходи в матрицата за автомивка софия*/    
    byte voutput=20; /* брой на свободните изходи04.2017*/    
     byte value=0; /* задание на съответния порт */     
     
      
      //   value=ByteConvert(vadres,6,0);    //      за киро -има свободни 6 изхода от портА, LPT1
      // value=ByteConvert(vadres,5,0);        //  за автомивка  -има свободни 5 изхода от портА, LPT1 
        value=ByteConvert(vadres,8,0);        // има свободни 8 изхода от портА, LPT1     ,04.2017
      //   vadres=vadres + 6;    // за киро
        // vadres=vadres + 5;//за софия  
           vadres=vadres + 8;//04.2017
         _outp(baseadres1,value);      
         //порт C ,lpt1 
         //04.2017 - няма свободни входове
    //      value=ByteConvert(vadres,4,1);  
    //      vadres=vadres +4;   
   //      _outp(baseadres1+2,value);     
         
            //порт А, lpt2
         value=ByteConvert(vadres,8,0);   
         vadres=vadres +8;   
         _outp(baseadres2,value); 
         //port C, lpt2 
          value=ByteConvert(vadres,4,1);      
          vadres=vadres +4;   
         _outp(baseadres2+2,value);   
    	
  }