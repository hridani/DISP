
 #ifndef _INC_SERIAL
 #define _INC_SERIAL   
 
 #pragma comment(lib,"..\\lib\\serial.lib")      
  
#define timeout     100    /* Readln_com times out at 10000 milliseconds */   
#define max_buffer  1000        /* Circular buffer size */   
#define near_full   900         /* When buffer_length exceeds near_full */   
                          /* RTS line is disabled */  
#define near_empty  100         /* When buffer drops below near_empty */   

  
enum Baud_t
{
  BAUD_50     = 0x900,
  BAUD_75     = 0x600,
  BAUD_110    = 0x417,
  BAUD_300    = 0x180,
  BAUD_1200   = 0x60,
  BAUD_2400   = 0x30,
  BAUD_4800   = 0x18,
  BAUD_9600   = 0x0c,
  BAUD_14400  = 0x8,
  BAUD_19200  = 0x6,
  BAUD_38400  = 0x3,
  BAUD_57600  = 0x2,
  BAUD_115200 = 0x1
};

enum Parity_t
{
  PARITY_NO   = 0x00,
  PARITY_ODD  = 0x08,
  PARITY_EVEN = 0x18,
  PARITY_MARK = 0x28,
  PARITY_Space= 0x38
};

enum Length_t
{
  LENGTH_5_BIT= 0,
  LENGTH_6_BIT= 1,
  LENGTH_7_BIT= 2,
  LENGTH_8_BIT= 3
};

enum StopBit_t
{
  STOPBIT_1_BIT = 0x00,
  STOPBIT_2_BIT = 0x04
};


      
      
#define COM1            1 
#define COM2            2  
#define COM3            3 
#define COM4            4 

 
/* 
  	o TXR ����������� ����� �� �����
	o RXR������� ����� �� �����
	o LCR ���������� �� �������
	o IER ������� �� �������������. 
	o IIR ������������� �� �������������. 
	o MCR ���������� �� ������. 
	o LSR ��������� �� �������
	o MSR ��������� �� ������
	o DLL ������ ���� �� �������� ������� �� ������
	o DHH ������ ���� �� �������� ������� �� ������
*/ 
#define TXR             0       /*  �������������  (WRITE) */ 
#define RXR             0       /*  ���������  (READ)  */ 
#define IER             1       /* ��������� �������������          */ 
#define IIR             2       /*  ������������ ����.             */ 
#define LCR             3       /*  ������� �� �������             */ 
#define MCR             4       /*  ������� �� ������             */ 
#define LSR             5       /*  ������ �� �������               */ 
#define MSR             6       /*  ������ �� ������              */ 
#define DLL             0       /* ������ ����         */ 
#define DLH             1       /*  ������        */ 
 
 
/*-------------------------------------------------------------------* 
  �������� (LCR). 
	bit		meaning 
	---		------- 
	0-1		00=5 bits, 01=6 bits, 10=7 bits, 11=8 bits. 
	2		���� ���. 
	3		0=���� ��� �� �������, 1=��� ���. 
	4		0=�������, 1=�����. 
	5		������� ���. 
	6	 ������������ ��  break. 
	7		������������ ��� ���������� �� �-���� , ��� 1- DLL,DHL, 0-TXR,RXR. 
 *-------------------------------------------------------------------*/ 
#define NO_PARITY       0x00 
#define EVEN_PARITY     0x18 
#define ODD_PARITY      0x08 
 
 
 
/*-------------------------------------------------------------------* 
 Opisanie (LSR). 
	 
	---		------- 
	0		������ �����. 
	1		������������� �� ����������. 
	2		������������� �� ������ ��� ������� �� ������� 
	3		Framing error - No stop bit was found. 
	4		Break �������������. 
	5		������������ ����� � ������. 
	6		������������� �-� � ������. 
	7        ������� ����� 
 *-------------------------------------------------------------------*/ 
#define RCVRDY          0x01 
#define OVRERR          0x02 
#define PRTYERR         0x04 
#define FRMERR          0x08 
#define BRKERR          0x10 
#define XMTRDY          0x20 
#define XMTRSR          0x40 
#define TIMEOUT		    0x80 
 
/*-------------------------------------------------------------------* 
 �������� (MCR) �� ��������
���     	��������
	---		------- 
	0		��������� �������� DTR 
	1		��������� �������� RTS 
	2		OUT #1. 
	3	    OUT  #2
	4		UART ������� ������ �� ��������. 
	5-7		�� �� ��������� 
 *------------------------------------------------------------------*/ 
#define DTR             0x01 
#define RTS             0x02 
#define MC_INT		0x08 
 
 
/*------------------------------------------------------------------* 
  �������� (MSR). 

	---		------- 
	0		������� �� ���� CTS. 
	1		������� �� ���� DSR. 
	2		������� �� ���� RI. 
	3		������� �� �� ���� CDt. 
	4		�������� ����������� �� CTS . 
	5		�������� ����������� �� DSR. 
	6		�������� ����������� �� RI. 
	7		�������� ����������� �� CD 
 *------------------------------------------------------------------*/ 
#define CTS             0x10 
#define DSR             0x20 
 
 
/*------------------------------------------------------------------* 
  �������� �� (IER). 

	---		------- 
	0		1-��������� ���������� ��� ������� �� ������ �����. 
	1		1- ��������� ���������� ��� ������ ������� �-� �� ���������
	2	    1- ��������� ���������� ��� ������ �� ������� 
	3		1- ��������� ���������� ��� ������� �� ������ 
	4-7	�� �� ��������� 
 *------------------------------------------------------------------*/ 
#define RX_INT          0x01 
 
 
/*------------------------------------------------------------------* 
  �������� ��  (IIR). 

	---		------- 
	0		��������� ������������� 
	1-2       ��� �� ������������ �� ��������� 
			00= ��������� �� ������
			01=������ ����� �� ��������� �� �����, 
			10=������� �� ����� ���� ����� 
			11=���������� ��� ����������
	3-7		Not used. 
 *------------------------------------------------------------------*/ 
#define RX_ID           0x04 
#define RX_MASK         0x07 
 
 
/* 
   ������ ��  8259 �������� �� ������������ ��� ���������� �� �������������
    C 
*/ 
#define IMR             0x21   /* �-� �� �����*/ 
#define ICR             0x20   /* ����       */ 
 
 
/* 
   ���� �� ������������� - ���������� ��������
   
*/ 
#define EOI             0x20   
 
 
/* 
   �������� �� �����
*/ 
#define IRQ3            0xF7  /* COM2 */ 
#define IRQ4            0xEF  /* COM1 */ 

class serial 
{   
public:
   int flag;  
   int portbase;//����� �� COM ����
   int com_flag;               /*Com open flag */  
  
	int overrun_flag;  
	char com_buffer[max_buffer];    /*Circular com buffer */  
	int com_port;                   /* Current Com port (0 or 1) */  
	int intlev;                     /* Hardware interrut level for com port */  
	int buffer_in;                  /* Pointer for input to com buffer */  
	int buffer_out;                 /* Pointer for output from com buffer */  
	int buffer_length;              /* Current number of chars in com buffer */  
	int bf_hndshk;                  /* Handshake flag for control lines */  
   
   int  SetSpeed(int); 
   int  SetOthers(int Parity, int Bits, int StopBit); 
   int  SetPort(int Port); 
   void init_serial(void); 
   void comm_off(void); 
   void reset_buffer () ; 
	void (__cdecl __interrupt *oldfunc)(); 
     
 
	void open_com (int ,unsigned int,int,int,int,int *);     
 	void   send_com (char ,  int *)  ;
	void close_com ()  ;   
	int  readln_com (char * ,  int *)     ;
	void check_com (char *c, int *error_code) ;  
	int ComPortWrite (char *str, int NumberOfBytes);
	void WRITE_RS232(char *out_str,int *error_code);  
	void READ_RS232(char *str, int *error_code);     
	int ComPortRead(char *str, int *error_code);
	 
    serial(void);
   	serial(int Port, int Speed, int Parity, int Bits, int StopBit);         
   	~serial()   ;
  
}; 
              
 #endif      
 