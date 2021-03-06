/********************************************************************/
/*      ����������� �� ��������� �������� � ���� LSID               */
/********************************************************************/
  #ifndef _INC_LSID
 #define _INC_LSID
 #pragma comment(lib,"..\\lib\\lsid.lib")

/*    K��� LSID -�������� ����� �� ������ � LS1776 */
 class LSID  {
   public:
     friend class NLSID;
     int index;   /* ����� �� ������� */
     int v_en; /* ��� �� ������� - ������������� ������ ��� ������� */
     double km; /* �������� ���������� */
     int kv; /* ����������� ���������� */
     long int kd; /* ���������� �� ������� */
     double kf; /* ������ ���������� �� ������� */
     int ad,ac;   /* ����� �� ��������� �� ����� � ��������� �������� */
     long int rw,rr;  /*  ���� �� ����� � ������ */
     long value;//�������� �� �����������
     int status; /* ��������� �� ��� LS1776 */
     LSID *next;  /* �������� ��� ��������� ������� */
     LSID(int ,double ,int ,long int,int);  /* ����������� */
     long  ReadLS(void);  /* ������ �� OL ��������� */
     long  WriteLS(long int); /* ����� � PR ��������� */
     void  Inicializacia(void); /* ������������� �� ���� LS1776 */
     short ROSR(void); /* ������ �� ��������� �������� */
};        
  
/*   ���� NLSID  - ���������� �� ������� � ����� �� ��������� ������� */  
 class NLSID{     
 public:
   LSID *top,**cur; 
   int nb;
   NLSID();
   ~NLSID();
   
   void find(int );/* �������� �� ����� */
   int add(int ,double ,int ,long int,int); /* �������� �� ����� */
   void removeall(LSID **);/* ��������� �� ������� */
  };
 #endif                      
