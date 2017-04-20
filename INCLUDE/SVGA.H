/*********************************************************************/
/*                       S V G A                                              */
/*-----------------------------------------------------------------------------------*/
/*     Управление на изхода (екрана) в текстов режим      */
/*********************************************************************/
 #ifndef _INC_SVGA         
 #define _INC_SVGA
 #pragma comment(lib,"..\\lib\\ekran.lib")
 #include "..\include\comv.h"                     
 #include "..\include\memnew.h"
 
 #define max_width  80  /* максомална ширина */
 #define max_height 25  /* максимална височина */

//#define COLOR(VG,HG)  ((VG<<4) +HG)  /* изчисляване на атрибута на символа */

/*======Дефиниране на нови типове данни==========================*/
extern VP vptr;
 /* указател към структура VP,която описва двойката ASCII
                                                символ и атрибут  */

/*======Функции=========================*/
 flag disp_init(void);/* инициализация */
 void Save_scr(int,int,int,int,int,union vel   *);/* съхранение на прозореца в RAM */
 void SaveFile(int,int,int,int,int,char *);/* съхранение на прозореца в RAM */
 void Load_scr(int,int,int,int,int,union vel  *,int);/* зареждане на прозорец от RAM във
                                видеопаметта или в буфера */
 void LoadFile(int,int,int,int,int,char  *,int);/* зареждане на прозорец от RAM във */
 void Move_buffer_to_screen(void);/* прехвърляне на буфера във видеопаметта */
 void dprint (byte,byte,byte,byte,char *); /* извеждане на низ на екрана */
 union vel  * Allocate_scr(int,int,int,int,int);//резервиране на памет и съхраняване на прозорец 
 void Unallocate_scr(union vel  *);
 byte Test_CGA(void);

 #endif
