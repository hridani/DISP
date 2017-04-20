#include <dos.h> 
#include "c:\msvc\dani\include\cagesm.h"    
#include "c:\msvc\dani\include\memnew.h"

#include <stdio.h>
#include <conio.h>   
#include <string.h> 




BaseCage::BaseCage()
{    
    numb_money=0;          
    number=0;  
    input_money=0;   
     input_stop=0;
 //   output_money=0;
       cur_kr1=cur_kr2=0;
    numb_money=0;  
     numb_money=0;
    kredit=0;
    cur_kredit=0; 
    nwork=0;
    tcur=0;
    tbegin=0;
           
    input_cold=0;
    input_hot=0;
    input_soap=0;   
    output[0]=0;
    output[1]=0;
    output[2]=0;
    time[0]=0;
    time[1]=0;
    time[2]=0;     
     kredit_s[0]= 0;
    kredit_s[1]= 0;
    kredit_s[2]= 0;     
    kredit_op[0]=0;
     kredit_op[1]=0;
      kredit_op[2]=0;    
        sum_kredit[0]=0;
   sum_kredit[1]=0;
   sum_kredit[2]=0;

}
  
BaseCage::BaseCage(byte nm,byte i_m,double kr,byte e_s,byte  o_m,byte i_cold,byte o_cold,unsigned t_cold,byte i_hot,byte o_hot,unsigned t_hot,byte i_soap,byte o_soap,unsigned t_soap, byte *o, double kr1, double kr2, double kr3)
  {             
    numb_money=0;             
    number=nm;  
    input_money=i_m;  
    kredit=kr;  
     cur_kr1=cur_kr2=0;
    cur_kredit=0;
    nwork=0;
    tcur=0;
    tbegin=0;
    input_stop=e_s;
   // output_money=o_m;
    
    numb_money=0;       
    input_cold=i_cold;
    input_hot=i_hot;
    input_soap=i_soap;   
    output[0]=o_cold;
    output[1]=o_hot;
    output[2]=o_soap;
    time[0]=t_cold;
    time[1]=t_hot;
    time[2]=t_soap;   
    kredit_s[0]= 1/(double)time[0];
    kredit_s[1]= 1/(double)time[1];
    kredit_s[2]= 1/(double)time[2];   
    kredit_op[0]=kr1;
     kredit_op[1]=kr2;
      kredit_op[2]=kr3;  
        sum_kredit[0]=0;
   sum_kredit[1]=0;
   sum_kredit[2]=0;
    for(int i=0; i<6;i++)
    output_money[i]=o[i];
}  
BaseCage::BaseCage(byte nm,byte i_m,double kr,byte e_s,byte  o_m,byte i_cold,byte o_cold,unsigned t_cold,byte i_hot,byte o_hot,unsigned t_hot,byte i_soap,byte o_soap,unsigned t_soap)
  {             
    numb_money=0;             
    number=nm;  
    input_money=i_m;  
    kredit=kr;   
    cur_kr1=cur_kr2=0;
    cur_kredit=0;
    nwork=0;
    tcur=0;
    tbegin=0;
    input_stop=e_s;
   // output_money=o_m;
    
    numb_money=0;       
    input_cold=i_cold;
    input_hot=i_hot;
    input_soap=i_soap;   
    output[0]=o_cold;
    output[1]=o_hot;
    output[2]=o_soap;
    time[0]=t_cold;
    time[1]=t_hot;
    time[2]=t_soap;   
    kredit_s[0]= 1/(double)time[0];
    kredit_s[1]= 1/(double)time[1];
    kredit_s[2]= 1/(double)time[2];   
   sum_kredit[0]=0;
   sum_kredit[1]=0;
   sum_kredit[2]=0;
}


 