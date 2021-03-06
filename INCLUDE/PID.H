 #ifndef _INC_PID
 #define _INC_PID
 #pragma comment(lib,"..\\lib\\pid.lib")
 #include "..\include\comv.h"
#include "..\include\memnew.h "

#define RESET  0x00
#define PORT8  0x05
#define PORT12 0x06
#define DFH  0x02
#define  SIP  0x03
#define LPEI  0x1b
#define LPES 0x1a
#define SBPA 0x20
#define SBPR 0x21
#define MSKI 0x1c
#define RSTI  0x1d
#define LFIL  0x1e
#define UDF  0x04
#define LTRJ 0x1f
#define STT   0x01
#define  RDSIGS 0x0c
#define RDRP  0x0a
#define RDIP 0x09
#define RDDP 0x08
#define  RDDV 0x07
#define RDRV 0x0b
#define RDSUM 0x0d
 
 class PIDchip
 {                            
  friend class PIDList;
   public:        
     int number;
     unsigned  adresc;
     unsigned adresd;
     //��������
     int kp; // ����������� ����� �� 32767
     int ki;// ����������� ����� �� 32767
     int kd;// ����������� ����� �� 32767
     int il;    // ����������� ����� �� 32767
     int ds;// ����������� ����� �� 32767
     int filter_control_word;   
     //���������
     int kpdin; // ����������� ����� �� 32767
     int kidin;// ����������� ����� �� 32767
     int kddin;// ����������� ����� �� 32767
     int ildin;    // ����������� ����� �� 32767
     int dsdin;// ����������� ����� �� 32767
     int filter_control_worddin;   

     PIDchip *next;
     PIDchip *back;
     PIDchip( int,unsigned,PIDchip * ) ;
     PIDchip( int,unsigned,int,int,int,int,int,PIDchip * ) ;
     PIDchip( unsigned) ;//����������� -������ �� ������
     void PIDIniFil(int,int,int,int,int);//������������� �� ���������� �����������
     void PIDIniFilDin(int ,int ,int ,int ,int );//������������� �� ����������� ���������� �����������
     void Rsti(int );   
     int INI(void);  
     void Lfil (void); 
     void LfilDin (void);//������� � ����  ����������� �����������
     void Lfil(unsigned ,unsigned);     
     void Ltrj(unsigned ,unsigned long,unsigned long ,unsigned long);
      void Ltrj(unsigned ,unsigned long,unsigned long);
      void Ltrj(unsigned ,unsigned long);
     void Ltrj(unsigned );
     long Rdrp();
     long Rddp();
     int Rdrv();       
     byte Rdstat(void);
     void Mski(int);                
     void Stt(void);       
     void Udf(void);
     void Lpes(int );
     void Dfh(void);
     void Sip(void);
     long Rdip(void);
} ;
   
   class PIDList
   {           
    public:  
      PIDchip *top,**cur,*bcur;
      int nm;      
      PIDList();         
      ~PIDList() {removeall(&top); }
      flag add(int,unsigned);
      void add(int,unsigned,int,int,int,int,int);
      void find(int );     
      void removeall(PIDchip**  );
      void erasePIDchip(int );
      void saveList(FILE *,PIDchip *);    
     void saveall( char * ); 
      void load( char * ); 
      void L_get_last();
   };                  
   
   class Axis: public PIDchip
   {
    protected :
       unsigned long acl; // ����������� �����,������������ �� double * 65536
       unsigned long vel;// ����������� �����,������������ �� double * 65536
     public:
      unsigned kfd; // ���������� �� ������� [inc/ob]
      unsigned kv; //����������� ����������
      double pstep;//������ �� ����� [mm/ob]
      double kmach; //���������� �� ��������� �� [inc/mm]
      unsigned numboutput;
      Axis(unsigned ,unsigned ,unsigned,double ,unsigned);//����������� -����� ,kd,kv,pstep
      void IniTr(double ,double);
      void IniTr(void);
      unsigned long Newvel(double );
    };

    
   
 #endif



