/*************************************************************************************************/
/*                           C A G E . H                                                                                 */
/* �������� �� ����� ���� BaseCage, ���������� ������� Matrix � Linea .            */
/* �������� �� ���� ������ �� Pia.���� Base -�������� �� ���� ��� �����       */
/* ListBase-������ �� ��������� ��� ��������                                                   */
/*************************************************************************************************/
 #ifndef _INC_CAGE   
 #define _INC_CAGE
 #pragma comment(lib,"c:\\msvc\\dani\\lib\\cagesm.lib")
 #include "c:\msvc\dani\include\comv.h"
 int handle_program_memory_depletion( size_t ); 



//����� ���� BaseCage -���� �������� �� ������
 class BaseCage
 {
     friend class ListCage;
     protected:
          byte number; /* ������� ����� �� ��������� � ������� */

     public:     
     byte numb_money;//���� ������             
     byte nwork; //������ ��������          
     unsigned tcur;   //������ ����� �� ��������
     unsigned tbegin;    //������� ��������� � ����. ������� �� �����
             //�������
     byte input_money; //���� �� ������                      
     byte input_stop;    //          ���� �����     
     
     byte input_cold; /* ����- ������� ���� */
     byte input_hot; /* ���� ����� ���� */ 
     byte input_soap; /* ���� �����*/
     
     //�����
     byte output_money[6];//����� -6 ����
     
     byte output[3]; /* ����� �����*/  /*  �����- ������� ���� */      /* ����� ����� ���� */   
               
     //���������
     unsigned time[3]; /* ����� -������� ������ 1������  */    /* ����� - ����� ���� �� 1  ������ */      /* ����� - ����� �� 1 ������  */
   
    double kredit; //kredit -1 ������ = ���.�������, ������������ �� ������� � �������� �� ���� � ���� ������
    double cur_kr1,cur_kr2;
    double cur_kredit;//����� ������
     
   double kredit_s[3];//������ �� 1 � �� ��������(��.����, ����� ���� , �����)  � �������
   double kredit_op[3];//������ �� �������         
   double sum_kredit[3];//���� �� �������� ������� �� ��������
     BaseCage();/* ����������� */       
     BaseCage(byte, byte,double,byte,byte ,byte ,byte,unsigned , byte,byte,unsigned , byte,byte,unsigned, byte * , double ,double, double); /* ����������� */      
        BaseCage(byte, byte,double,byte,byte ,byte ,byte,unsigned , byte,byte,unsigned , byte,byte,unsigned); 

     ~BaseCage();
};

#endif
