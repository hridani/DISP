#include <dos.h> 
#include "..\include\cage.h"    
#include "..\include\memnew.h"

#include <stdio.h>
#include <conio.h>   
#include <string.h> 


BaseCage::BaseCage(byte nm,byte i_m,byte e_s,byte o_m,byte o_l)
{ 
    numb_money=0;             
    number=nm;  
    input_money=i_m;   
    input_emsstop=e_s;
    output_money=o_m;
    output_light=o_l    ;    
    numb_money=0;    
    next=0;
}

BaseCage::BaseCage()
{    
    numb_money=0;          
    number=0;  
    input_money=0;   
     input_emsstop=0;
    output_money=0;
    output_light=0 ;    
    numb_money=0;   
	next=0;
}
  
Wash::Wash(byte nm,byte i_m,byte e_s,byte  o_m,byte o_l,byte i_cold,byte o_cold,unsigned t_cold,byte i_hot,byte o_hot,unsigned t_hot,byte i_soap,byte o_soap,unsigned t_soap)
	   :BaseCage(nm,i_m,e_s,o_m,o_l)
  {                   
    input_cold=i_cold;
    input_hot=i_hot;
    input_soap=i_soap;   
    output_cold=o_cold;
    output_hot=o_hot;
    output_soap=o_soap;
    time_cold=t_cold;
    time_hot=t_hot;
    time_soap=t_soap;     
    m_cold=0;
	m_soap=0;
	m_hot=0;
   }

Wash::Wash()
	   :BaseCage()
  {                   
    input_cold=0;
    input_hot=0;
    input_soap=0;   
    output_cold=0;
    output_hot=0;
    output_soap=0;
    time_cold=0;
    time_hot=0;
    time_soap=0;    
    m_cold=0;
	m_soap=0;
	m_hot=0;
   }    
 
 
 
Wash::~Wash()
{
   return;  
}      
     
Vac::Vac(byte nm,byte i_m,byte e_s,byte  o_m,byte o_l,byte o_vac, unsigned t_vac)
	   :BaseCage(nm,i_m,e_s,o_m,o_l)
{                  
  output_vac=o_vac;
  time_vac=t_vac;    
  m_vac=0;
}   
             
BaseCage::~BaseCage()
{
   return;
}
 
Vac::~Vac()
{
     return ;
}
   
void ListCage::find(byte nm)
{
      if(*cur==NULL)
	 return;
      if((*cur)->number==nm)
	 return;
      cur=&(*cur)->next;
      find(nm);  
} 



flag ListCage ::add(char key,byte i_m,byte e_s,byte o_m,byte o_l,byte i_c, byte o_c, unsigned t_c, byte i_h, byte o_h, unsigned t_h,
                          byte i_s, byte o_s, unsigned t_s)
{
      cur=&top;
      find(nm);
      if(*cur)
	   return FALSE;
      else
      {
		if(key=='w')
	 		*cur=new Wash(nm+1,i_m,e_s,o_m,o_l,i_c,i_h,i_s,o_c,o_h,o_s,t_c,t_h,t_s);   
      	 if(key=='v')
	 			*cur=new Vac(nm+1,i_m,e_s,o_m,o_l,o_c,t_c);     
	 			
    	nm++;
	    return TRUE;
      }
}


ListCage::ListCage()
{
  top=NULL;
  cur=&top;
  nm=0;
}

void ListCage::removeall(BaseCage **bCage)
{
     if(*bCage==NULL)
       return;
     removeall(&((*bCage)->next));
     delete *bCage;
     nm--;
     *bCage=NULL; 
}
