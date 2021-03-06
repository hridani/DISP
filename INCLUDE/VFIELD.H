/*******************************************************************************/
/*                               V I D E O   F I E L D                                   */
/*------------------------------------------------------------------------------------------------*/
/*          ����������� ��  ���� VIDEO FIELD     */
/*********************************************************************/
 #ifndef _INC_VFIELD      
 #define _INC_VFIELD
 #pragma comment(lib,"..\\lib\\ekran.lib")    
 #include "..\include\comv.h"    
 #include <stdio.h>
 #define COLOR(VG,HG)  ((VG<<4) +HG)  /* ����������� �� �������� �� ������� */
 class  VField 
 {
    friend class ListVF;
    public:
     byte numb;  /* ����� �� ������ */
     byte row,col,len,atr;/* ��� � ������,������� ,���� �� ��������� � ���� */
     char *ptext;  //�������� ��� ������ �� ������                             
     VField *next; /*��������� */
     VField *back;/* ������������� */
     VField(byte,byte,byte,byte,byte,char*,VField*); /* ����������� */
     ~VField();
     };

/* ���������� �� ���� ListVF */    
 class ListVF
 {     
   public:
   VField *top,**cur,*bcur;  /* ��������� ��� ���������,�������� � ���������� �������� */
   byte nb;  /* ���� ����� */
   int row;//9.05.2001 -���� ���� ������ -�� �� �� ����� �����
   int col;//9.05.2001 -���� ���� ������ -�� �� �� ����� �����
   char *name;
   VP sptr;//�������� ��� ������������� � �������
   ListVF(); /* ����������� */
   ~ListVF();/* ���������� */
   void find(byte );  /* ������� �� ����-����� */
   void removeall(VField **);/* ��������� �� ������� */
 // void L_get_last(void); /* �������� �� ���������  */
   void saveListVF(FILE *,VField *);/* ����� ��� ���� �� ���� ����� */
   void add(byte ,byte ,byte ,byte ,byte ,char *); /* �������� �� ����� */
   void saveall(char *);/* ����� ��� ���� �� ������� */
   unsigned char load (char *); /* ������ �� ����� */
 void  eraseVField( byte ); /* ��������� �� ���� ������� �� ������� */
  };

#endif
