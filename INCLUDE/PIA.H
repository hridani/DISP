/*************************************************************************************************/
/*                           P I A . H                                                                                 */
/* �������� �� ����� ���� BasePia, ���������� ������� Matrix � Linea .            */
/* �������� �� ���� ������ �� Pia.���� Base -�������� �� ���� ��� �����       */
/* ListBase-������ �� ��������� ��� ��������                                                   */
/*************************************************************************************************/
 #ifndef _INC_PIA    
 #define _INC_PIA
 #pragma comment(lib,"..\\lib\\pia.lib")
 #include "..\include\comv.h"
 int handle_program_memory_depletion( size_t ); 
#ifndef prom  

typedef enum BOOL flag;
#endif
typedef struct typeport
{
  byte type; /* ��� �� ���� */
  byte number; /* ���� ������� �� ���� ,����������� � ������� */
}TypePort;     

   
//����� ���� BasePia -���� �������� �� PIA
 class BasePia
 {
     friend class ListPia;
     protected:
     byte typeA; /* ��� �� ���� �*/
     byte typeB; /* ��� �� ���� B */
     byte typeC1;/* ��� �� ���� C1 */
     byte typeC2;/* ��� �� ���� C2 */
    
     byte number; /* ������� ����� �� PIA � ������� */

     public:
      unsigned baseadres;/* ����� ����� */
     BasePia *next;  /*�������� ��� ���������� PIA � ������� � PIA */
     BasePia(unsigned ,byte,byte,byte,byte,byte);/* ����������� */            
     virtual int kind()=0; /* ��������� ������� */
     virtual void read()=0; /* ������ �� ������� */
     virtual void write()=0; /* ������ � ������ */
     virtual ~BasePia();
};

//���������� ���� Matrix-�������� �������� PIA
class Matrix:public BasePia
{          
   friend class ListPIA;
   public:
   struct typeport tport[4]; /* ��� � ���� �� ���������� ������� */
   byte minput; /* ���� �������� ������� */
   byte moutput; /* ���� �������� ������ */
   byte vinput; /* ���� �������� �������*/
   byte voutput; /* ���� �������� ������  */
   byte *pinput;/* �������� ��� ����������� �� ��������� � ������� */
   byte *poutput;/* �������� ��� ����������� �� �������� � ������� */
   byte *pitable; /* �������� ��� ��������� � ����� �� ���������� ������� */
   byte *potable; /* �������� ��� ��������� � ����� �� ���������� ������ */
   byte *pvtable; /* �������� ��� ��������� � ����� �� ���������� ������� */
   Matrix(unsigned,byte,byte,byte,byte,byte,byte,byte);/* ����������� */
   void TMatrix(byte,byte,byte,byte);/* �������� �� ��������� */
   virtual int kind() {return 0;} 
   virtual void read(); /* ������ �� ������� � �������� ������� */
     void reada(void);
   virtual void write(void); /* ������ � ������ */
   ~Matrix();
  
};

//���������� ���� Linea- ������� �������� PIA
class Linea:public BasePia
{          
   friend class ListPIA;
   public:
   unsigned linput; /* ���� ������� */
   unsigned loutput; /* ���� ������ */
   byte *pinput; /* �������� ��� ����������� �� ��������� � ������� */
   byte *poutput;/* �������� ��� ����������� �� �������� � ������� */
   byte *pitable; /* �������� ��� ��������� � ����� ��  ������� */
   Linea(unsigned,byte,byte,byte,byte,byte);/* ����������� */
    virtual int kind() {return 1;}
    virtual void read();  /* ������ �� ������� */
    virtual void write(void); /* ������ � ������ */
    ~Linea();//����������
};        

         //��� ���� �� LPT
class LineaLpt:public BasePia
{          
   friend class ListPIA;
   public:
   unsigned linput; /* ���� ������� */
   unsigned loutput; /* ���� ������ */
   byte *pinput; /* �������� ��� ����������� �� ��������� � ������� */
   byte *poutput;/* �������� ��� ����������� �� �������� � ������� */
   byte *pitable; /* �������� ��� ��������� � ����� ��  ������� */
   LineaLpt(unsigned,byte,byte,byte,byte,byte);/* ����������� */
    virtual int kind() {return 2;}
    virtual void read();  /* ������ �� ������� */
    virtual void write(void); /* ������ � ������ */
    ~LineaLpt();//����������
};
     //������� �� 2 ���
class MatrixLpt:public BasePia
{          
   friend class ListPIA;
   public:
   unsigned baseadres1;
   unsigned baseadres2;
   byte minput; /* ���� �������� ������� */
   byte moutput; /* ���� �������� ������ */
   byte vinput; /* ���� �������� �������*/
   byte voutput; /* ���� �������� ������  */
   byte *pinput;/* �������� ��� ����������� �� ��������� � ������� */
   byte *poutput;/* �������� ��� ����������� �� �������� � ������� */
   byte *pitable; /* �������� ��� ��������� � ����� �� ���������� ������� */
   byte *potable; /* �������� ��� ��������� � ����� �� ���������� ������ */
   byte *pvtable; /* �������� ��� ��������� � ����� �� ���������� ������� */
   MatrixLpt(unsigned,unsigned);/* ����������� */     
   void TMatrix();/* �������� �� ��������� */  
   void TMatrixVersion2017();/* �������� �� ��������� */
    virtual int kind() {return 3;}
    virtual void read();  /* ������ �� ������� */
    virtual void write(void); /* ������ � ������ */
    ~MatrixLpt();//����������
};




//������ �� PIA 
class ListPia
{
   public:
   BasePia *top,**cur;//�������� ������ �� ������� � ������� �������
   int nm;//���� PIA
   ListPia();    //�����������
   ~ListPia() {removeall(&top); }//����������
   void removeall(BasePia **);//������������� �� ������� �� ������� PIA
   flag add(char,unsigned,byte,byte,byte,byte,byte,byte);//�������� �� �������PIA ��� ��.
   void find(unsigned);//������� �� PIA 
  // void find(byte);//������� �� PIA 
};  

//������� �� PIA-���������� ������� � ������  
 class Base
 {
     friend class List;
     public:
     unsigned adres;/* ����� �� ����� ,�� ����� � ������� */
     byte value; /* �������� �� ������� */
     byte mask;/* ����� �� ������� */
     byte bit;/* ����� �� ��� � ����� */
     unsigned number; /* ����� �� ����� */
     Base *next;  /* �������� ����� */          
     Base(unsigned );  //����������� - ����� 
     Base(unsigned,byte);//�����������-����� �� ������� � ��������
     Base(unsigned,unsigned,byte);//�����������-�����,����� � ��� �� �����
 };
//������ �� �������� -���������� �� ������ ������� � ������ ������
class List
{
   public:
   Base *top,**cur;//�������� -������ � ����� ��������
   unsigned nm;//���� ��������
   List();    //����������� �� �������
   ~List() {removeall(&top); }//����������-������������� �� �����
   void removeall(Base**);//��������� �� �������
   byte& operator()  (unsigned);//����� �������� �� �������� ��� ������� �����
   byte& operator() (unsigned,byte);//������������� ������� ��� ���������� ��������
   flag add();          //�������� �� ������� ��� ������� /���� �����/
   flag add(unsigned,byte);//�������� �� ������� ��� ������� /����-�����/,������� �� ����� � ������� ��� � �����
   void find(unsigned);//������� �� ������� �� ������� �� �����
};  
       
 #endif
