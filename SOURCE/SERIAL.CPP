/*------------------------------------------------------------------*  
      SERIAL.CPP
*------------------------------------------------------------------*/   
   
#include <DOS.H>    
#include <CONIO.H>    
#include <STDIO.H>    
#include <STRING.H>    
#include "..\include\serial1.h"    
   
 
   
  
//int            portbase        = 0;   

void (__cdecl __interrupt *oldvects1)();  /* вектор на прекъсване 0xb */ 

#define Interrupt_Enable()          _enable()   
#define Interrupt_Disable()         _disable()      
serial *serial4;
 void __interrupt __far   com_isr4 (void);                            /* RTS line is enabled */      

//NEW - 30.11.2012


void delay(int d) 
/* d = delay in milliseconds on 10 MHz AT */  
{ int i; 
  int j; 
  int k; 
  j = 0; 
  for (k=0; k<d; k++) 
    for (i=1; i  <200; i++ ) 
    j += 1; 
} 

void serial::reset_buffer ()  
/* This procedure will reset the com buffer */  
{  
  
  bf_hndshk = 0;                /*Buffer full handshake false */  
  buffer_in = 0;                /*Set circular buffer input to 0 */  
  buffer_out = 0;               /*Set circular buffer output to 0 */  
  buffer_length = 0;            /*Set buffer_length to 0 */  
  overrun_flag = 0;             /*Set overrun flag to false */  
}  
/* установяване на порт на серийния интерфейс */   
int serial::SetPort(int Port)   
{   
    int   Offset, far *RS232_Addr;   
   
    switch (Port)   
    { /* Sort out the base address */   
      case COM1 : Offset = 0x0000;   
                  break;   
      case COM2 : Offset = 0x0002;   
                  break;     
      case COM3 : Offset = 0x0004;   //да се провери
                  break;       
      case COM4 : Offset = 0x0006;   //да се провери
                  break;                  
      default   : return (-1);   
    }   
   
  RS232_Addr = (int far *)MK_FP(0x0040, Offset);  /* намиране на адреса от таблицата с адреси. */   
 if (*RS232_Addr == NULL) return (-1);/* ако е 0. */   
      portbase = *RS232_Addr;              /*извличане на адреса     */      
  //portbase=0x3f8;
     
    return (0);   
}        

int serial::SetSpeed(int Speed)   
{   
    char        c;   
    int     divisor;   
   
    if (Speed == 0)            /* проврка за 0 скорост */   
        return (-1);   
    else   
        divisor = (int) (115200L/Speed);   
   
    if (portbase == 0)   //проверка за адреса на COM
        return (-1);   
   
    _disable();   
    c = inp(portbase + LCR);        //   стойността ор р-ра за контрол на линията
    outp(portbase + LCR, (c | 0x80)); /* установяване на DLAB */   
    outp(portbase + DLL, (divisor & 0x00FF));    //на младшия байт
    outp(portbase + DLH, ((divisor >> 8) & 0x00FF));    //на старшия байт
    outp(portbase + LCR, c);          /*  нулиране на DLAB */   
    _enable();   
   
    return (0);   
}     

void serial::open_com (int Cport,            /*Com port # - 1,2,3,4          */  
          unsigned int baud,    /*baud rate - 110,150,300..9600 */  
          int parity,           /*parity 0 = no parity         */  
                                /*       1 = even parity       */  
                                /*       2 = odd parity        */  
          int stopbits,         /*stop bits - 1 or 2           */  
          int numbits,          /*word length - 7 or 8         */  
          int *error_code)  
{  
  		int comdata;  
  		int ptemp;  
         *error_code = 0;  
  	    
  		if(SetPort(Cport)!=0)  //глобална променлива portbase е с адреса
  		 *error_code = 1;  
   		if(SetSpeed(baud)!=0)
  		         *error_code = 2;  		
  		comdata = 0;  
  		if ((numbits == 7) || (numbits == 8))  
    		comdata = comdata | (numbits - 5);  
  		else  
    		*error_code = 5;  
  
 		 if ((stopbits == 2) || (stopbits == 1))  
    			comdata = comdata | ((stopbits - 1) < 2);  
  		else  
    		*error_code = 4;  
  
		  if ((parity == 1) || (parity == 3) || (parity == 0))  
    			comdata = comdata | (parity < 3);  
  		 else  
			    *error_code = 3;     
		outp(portbase + LCR, comdata);     //запис на стойността			    
     		    
  		intlev = 0xC - Cport+1;         
  		if(Cport==1 || Cport==3)
  		{
        	intlev=0xC;    
            oldfunc= _dos_getvect (intlev);  
           serial4=this;
  			_dos_setvect (intlev, com_isr4);  
  	    }
  	    if(Cport==2 || Cport==4)
  		{
        	intlev=0xB;    
            oldfunc= _dos_getvect (intlev);  
  		//	_dos_setvect (intlev, com_isr4);  
  	    }
 		Interrupt_Disable ();                  /* No interrupts */  

 		 ptemp = inp (portbase+LCR) & 0x7f;  
  
  		 outp (portbase + LCR, ptemp);  
  
 		 ptemp = inp (portbase+LSR);            /* Reset any pending errors */  
  
  		 ptemp = inp (portbase+RXR);            /* Read any pending character */  
  
  		if (Cport == COM1)
  	    {             /* Set irq on 8259 controller */  
    			ptemp = inp (0x21) & 0xef;  
		        outp (0x21, ptemp);  
  		}  
  		else 
  		{  
		    ptemp = inp (0x21) & 0xf7;  
    		outp (0x21, ptemp);  
  		}  
  
  		outp (portbase+IER, 1);                /* Enable data ready interrupt */
  		ptemp = inp (portbase+MCR) | 0xb;       //     -15.05.2013
    //   ptemp = inp (portbase+MCR) | 0x0;      
        outp (portbase+MCR, ptemp);  
        Interrupt_Enable ();                   /*Turn on interrupts */  
        *error_code = 0;              /*Set error code to 0 */  
        com_flag = 1;                 /*Com inititalization flag true */  
        reset_buffer ();  
}   

void serial::close_com ()  
/* This procedure disables the com port interrupt. */  
{  
    int ptemp;     
    //printf( "Close");
  	if (com_flag == 1)
  	 {  
    	Interrupt_Disable ();                /* No interrupts */  
    	ptemp = inp (0x21) | 0x18;  
    	outp (0x21, ptemp);         /* Set mask register to turn off interrupt */  
    	ptemp = inp (portbase+LCR) | 0x7f;  
    	outp (portbase+LCR, ptemp);          /* Turn off 8250 data ready interrupt */  
        outp (portbase+IER, 0);  
        outp (portbase+MCR, 0);              /* Disable out2 on 8250 */  
    /* return to old interrupt vector */  
         _dos_setvect( intlev, oldfunc );
       Interrupt_Enable ();                 /* Turn on interrupts */  
       com_flag = 0;  
   }  
}  
/* прекъсваща програма */       
int br;

void interrupt far com_isr4 ()  
{  
//  printf("\nirq 4= %d\n",br++);
  if(serial4->com_flag == 1)
  {
    /*Get character - store in cicular buffer*/
    serial4->com_buffer[serial4->buffer_in] = inp( serial4->portbase+RXR);

    serial4->buffer_in+=1;                 /*Increment buffer_in pointer*/

    /*Wrap buffer pointer around to start if > max_buffer*/
    if(serial4->buffer_in == max_buffer)
      serial4->buffer_in = 0;

    /*Current number of characters in buffer incremented 1*/
    serial4->buffer_length+=1;
    if(serial4->buffer_length > max_buffer)
    {
      serial4->buffer_length = max_buffer;
      serial4->overrun_flag = 1;
    }
    /*Disable RTS if buffer_length exceeds near_full constant*/
    if(serial4->buffer_length > near_full)
    {
      outp(serial4->portbase+MCR, 9);   /*Disable rts, leave dtr and out2 set*/     // променено на 15.05.2013  
   //  outp(serial4->portbase+MCR, 0);   /*Disable rts, dtr and out2 set*/
      serial4->bf_hndshk = 1;  /*Buffer full handshake = true*/
    }
  }                                          
  outp(0x20,0x20);    /*end of interrupt to 8259*/
}     
   
/**  
 * This procedure returns 1 character from the com_buffer, at the 
 * array element pointed to by the circular buffer pointer buffer_out. 
 **/  
  
/*   0 = no error                */  
/*   6 = no character available  */  
/*   7 = buffer overflow         */  
/*  10 = com port not initialized */  
void serial::check_com (char *c, int *error_code)  
{                               /* error code for check_com       */  
  
  
   if (com_flag == 0)  
    /*Make sure com port has been initialized */  
    *error_code = 10;  
   else   //1
    {  
   		if (buffer_length == 0) 
    		  /*Check to see if any characters in buffer */  
      		*error_code = 6;  
       	else  //2
       	 {  
     		 if (overrun_flag == 1)
        /*buffer overflow */  
        		*error_code = 7;  
              else
        			*error_code = 0;  
               *c = (com_buffer[buffer_out]);    /*Get charater out of buffer */  
     			 buffer_out += 1;          /*Increment buffer_out_pointer */  
     			 /*Wrap buffer_out pointer around if >	max_buffer */  
      		if (buffer_out == max_buffer)
        			buffer_out = 0;  
            buffer_length -= 1;       /*Decrement buffer_length */  
  
		      /*Enable RTS if buffer_length  near_empty */  
      			if (bf_hndshk && (buffer_length <=near_empty))
      			 {  
        			outp (portbase+MCR, 0xb);    // променено 15.05.2013           
        		    //outp (portbase+MCR, 0x0); 
        			bf_hndshk = 0;  
      			}  
    		} //2 
     } //1 
}  
   
void   serial::send_com (char c,               /*Character to send out com port */  
          int *error_code)  
{  
  /* Error code for send_com       */  
  /*  0 = no error                 */  
  /*  8 = time out error           */  
  /* 10 = com port not initialized */  
  /* This procedure sends a character out the com port.  */  
	 		 int handshake;  
  			int counter;  
  
  		if (com_flag == 0)    /*Make sure com port has been initialized */  
			*error_code = 10;  
	  else
	   {  
    		counter = 0;                /* Initialize time out counter          */  
    		handshake = 0;              /*    0x30;  */     
    		/* Use the following handshake values:  */  
    		/*          0x0 no handshake            */  
    		/*          0x10 CTS handshaking         */  
    		/*          0x20 DSR handshaking         */  
    		/*          0x30 CTS and DSR handshaking */  
    		do
    		 {  
      				counter += 1;  
      				/* delay 1 millisecond - causes timeout at 10 seconds*/  
      			  	delay( 100 );       
    		} 
    		while ((((inp (portbase+MSR) & handshake) != handshake) || /*Check handshake */  
              ((inp (portbase+LSR) & 0x20) != 0x20)) &&  /*Check that xmit reg empty */  
             (counter< timeout));  
  
   /* Give up after 10 seconds */  
 			  if (counter == timeout) 
     			 *error_code = 8;  
		     else
		     {  
      			Interrupt_Disable ();     /* No interrpts */  
      			outp (portbase+TXR, c);            /* Transmit character */  
		        Interrupt_Enable ();      /* Interrupts on */  
               *error_code = 0;  
              }  
       }  
}  
  
  
int serial:: ComPortWrite (char *str,       /* string to send out com port */  
              int NumberOfBytes)  
{                               /* error code for writeln_com */  
  
		  int error_code;  
  		  int i;  
		  for (i = 0; i < NumberOfBytes; i++)
		  {  
    			send_com (str[i], &error_code);      
    		
         }     
         
 			  return i;  
  
}  
  
  
int  serial::readln_com (char *str,          /* string to received from com port */  
            int *error_code)  
{                               /* error code for writeln_com */  
  		int i = 0;  
  		char c;  
  		int counter = 0;  
  		do 
  		{  
		    check_com (&c, error_code);  
		    if (*error_code == 0)
		     {  
      			str[i] = c;  
      			i += 1;  
    		}  
    		else
    		{  
			      delay (1);  
      			 counter += 1;  
		    }  
  		} 
  		while ((i < 255) && (c != 13) && (counter<  timeout));  
  
  		if (counter == timeout)   
    		*error_code = 8;  
  
		  str[i] = 0;  
  
  return i+1;  
}  


void serial::WRITE_RS232(char *out_str,int *error_code)
{
  int i = 0;

  *error_code = 0;
  while(out_str[i] != 0x00 && *error_code==0)
    send_com(out_str[i++], error_code);
 
  printf("Out: %-60s",out_str);
  if(*error_code == 0)
    send_com(13, error_code);                  /* Send <CR> */
}

void serial::READ_RS232(char *str, int *error_code)
{
  int i = 0;
  char c;
  int counter = 0;
  do
  {
    check_com(&c, error_code);
    if(*error_code == 0)
    {
	 counter = 0;
	 str[i++] = c;
    }
    else
    {
	 delay(1);
	 counter++;
    }
    
   // printf("%5d",counter);
  } while((i < 255) && (c != 0x0A) && (counter < timeout));      
  if(counter == timeout)
    *error_code = 8;
  str[i] = 0;            
  
}                
int serial::ComPortRead(char *str, int *error_code)
{
  int i = 0;
  char c;
  int counter = 0;
  do
  {
    check_com(&c, error_code);
    if(*error_code == 0)
    {
	 counter = 0;
	 str[i++] = c;
    }
    else
    {
	 delay(1);
	 counter++;
    }
    
   // printf("%5d",counter);
  } while((i < 255) && (counter < timeout));      // това условие : while((i < 255) && (c != 0x0A) && (counter < timeout)); e promeneno на 9.05.2013 зарди nmc
  if(counter == timeout)
    *error_code = 8;
  str[i] = 0;        
  return i;    
  
}
     
/* създаване на обект */   
serial::serial(int Port, int Speed, int Parity, int StopBit,int Bits)   
{   
   int flag = 0;    
    open_com(Port,Speed,Parity,StopBit,Bits, &flag);
    
}   

/* създаване на обект */   
serial::serial(void)   
{         
    int flag = 0;    
    open_com(1,9600,0,1,8, &flag);
   
}        

serial::~serial()   
{   
    
    Interrupt_Disable ();                /* No interrupts */  
    outp(portbase + MCR, 0);      //забрана на веригата DTR- модема не е готов
    if(oldfunc!=NULL)
     _dos_setvect( intlev, oldfunc );

}   