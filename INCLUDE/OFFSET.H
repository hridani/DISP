/*******************************************************************************/
/*                                 O F F S E T                                               */
/*------------------------------------------------------------------------------------------------*/
/*          ����������� ��  ���� OFFSET     */
/*********************************************************************/
 #ifndef _INC_OFFSET
 #define _INC_OFFSET
 #pragma comment(lib,"..\\lib\\offset.lib")    
 #include <stdio.h>

 class  Offset
 {
    friend class ListOF;
    public:
     int  numb;  /* ����� �� ������ */
     int numbtool;//����� �� �����������
     int kdw; //�������������� ������
     int npar; //������������ ���� ���������
     double par[8]; //�������� �� ����������� -������ �� ���
     Offset *next; /*��������� */
     Offset *back;/* ������������� */  
     Offset(int ,int ,int,Offset *);  //�����������
     Offset(int ,int ,int ,Offset *,double ,double );
     };

/* ���������� �� ���� ListOF */    
 class ListOF
 {     
   public:
   Offset *top,**cur,*bcur;  /* ��������� ��� ���������,�������� � ���������� �������� */
   int nb;  /* ���� ����� */
   ListOF(); /* ����������� */
   ~ListOF();/* ���������� */
   void find(int );  /* ������� �� ����-����� */
   void findnt (int ); //������� �� ����� �� ���
   void findtkw (int ,int );
   void removeall(Offset **);/* ��������� �� ������� */
   void L_get_last(void); /* �������� �� ���������  */
   void saveListOF(FILE *,Offset *);/* ����� ��� ���� �� ���� ����� */
   void add(int ,int ,int ); /* �������� �� ����� */                                           
   void add(int,int ,int ,double =NULL,double =NULL);
   void saveall(char *);/* ����� ��� ���� �� ������� */
   void load (char *); /* ������ �� ����� */
   void  eraseOffset( int ); /* ��������� �� ���� ������� �� ������� */
  };

#endif
 
