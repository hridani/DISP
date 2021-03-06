/*************************************************************************************************/
/*                           C A G E . H                                                                                 */
/* �������� �� ����� ���� BaseCage, ���������� ������� Matrix � Linea .            */
/* �������� �� ���� ������ �� Pia.���� Base -�������� �� ���� ��� �����       */
/* ListBase-������ �� ��������� ��� ��������                                                   */
/*************************************************************************************************/
 #ifndef _INC_CAGE   
 #define _INC_CAGE
 #pragma comment(lib,"..\\lib\\cage.lib")
 #include "..\include\comv.h"
 int handle_program_memory_depletion( size_t ); 



//����� ���� BaseCage -���� �������� �� ������
 class BaseCage
 {
     friend class ListCage;
     protected:
          byte number; /* ������� ����� �� ��������� � ������� */

     public:     
     byte numb_money;//���� ������   
     byte input_money; //���� �� ������                      
     byte input_emsstop;
     byte output_money;//����� -������ ������    
     byte output_light;//����� ���������� �� ��������   
     BaseCage *next;  /*�������� ��� ���������� PIA � ������� � PIA */
     BaseCage(byte,byte,byte,byte,byte);/* ����������� */            
     BaseCage();/* ����������� */ 
     virtual int kind()=0; /* ��������� ������� */
     virtual ~BaseCage();
};

//���������� ���� Wash -������ ����� ��� �������, ����� ���� � �����
class Wash:public BaseCage
{          
   friend class ListCage;
   public:
   byte input_cold; /* ����- ������� ���� */
   byte input_hot; /* ���� ����� ���� */ 
   byte input_soap; /* ���� �����*/
   byte output_soap; /* ����� �����*/      
   byte output_cold; /*  �����- ������� ���� */
   byte output_hot; /* ����� ����� ���� */
   unsigned time_cold; /* ����� -������� ����  */       
   unsigned time_hot; /* ����� - ����� ����  */
   unsigned time_soap; /* ����� - �����  */
    unsigned m_cold;//���� -������ �� ������� ����
     unsigned m_hot;//����- ������ �� �.����
     unsigned m_soap;//����- ������ �� �����

   Wash(byte, byte,byte,byte ,byte,byte ,byte,unsigned , byte,byte,unsigned , byte,byte,unsigned  ); /* ����������� */      
   Wash();
   virtual int kind() {return 0;} 
    ~Wash();
  
};

//���������� ���� Linea- ������� �������� PIA
class Vac:public BaseCage
{          
   friend class ListCage;
   public:
    byte output_vac;//����� -�������.
    unsigned time_vac;//����� �������
    unsigned m_vac;//���� - ������ ����.
    Vac(byte,byte,byte,byte,byte,byte,unsigned);/* ����������� */
    virtual int kind() {return 1;}
    ~Vac();//����������
};        

       
//������ �� ������ -Listcage
class ListCage
{
   public:
   BaseCage *top,**cur;//�������� ������ �� ������� � ������� �������
   int nm;//���� PIA
   ListCage();    //�����������                         
   ~ListCage() {removeall(&top); }//����������
   void removeall(BaseCage **);//������������� �� ������� �� ������� PIA
   flag add(char,byte,byte,byte,byte,byte,byte,unsigned,byte,byte,unsigned,byte,byte,unsigned);//�������� �� �������PIA ��� ��.
   void find(byte);//������� �� cage

};  

 #endif
