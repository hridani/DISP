/********************************************************************/
/*      Деклариране на функциите включени в клас LSID               */
/********************************************************************/
  #ifndef _INC_LSID
 #define _INC_LSID
 #pragma comment(lib,"..\\lib\\lsid.lib")

/*    Kлас LSID -дефинира данни за работа с LS1776 */
 class LSID  {
   public:
     friend class NLSID;
     int index;   /* номер на датчика */
     int v_en; /* вид на датчика - инкрементален датчик или линийка */
     double km; /* масщабен коефициент */
     int kv; /* квадратурен коефициент */
     long int kd; /* коефициент на датчика */
     double kf; /* обощен коефициент на датчика */
     int ad,ac;   /* адрес на регистъра за данни и командния рагистър */
     long int rw,rr;  /*  дума за запис и четене */
     long value;//стойност от измерването
     int status; /* състочние на чип LS1776 */
     LSID *next;  /* указател към следващия елемент */
     LSID(int ,double ,int ,long int,int);  /* конструктор */
     long  ReadLS(void);  /* четене от OL регистъра */
     long  WriteLS(long int); /* запис в PR регистъра */
     void  Inicializacia(void); /* инициализация на чипа LS1776 */
     short ROSR(void); /* четене на статусния регистър */
};        
  
/*   Клас NLSID  - управление на списъка с данни за отделните датчици */  
 class NLSID{     
 public:
   LSID *top,**cur; 
   int nb;
   NLSID();
   ~NLSID();
   
   void find(int );/* Намиране на възел */
   int add(int ,double ,int ,long int,int); /* Добавяне на възел */
   void removeall(LSID **);/* Изтриване на списъка */
  };
 #endif                      
