 #ifndef _INC_V16COLC
 #pragma comment(lib,"c:\\msvc\\dani\\lib\\gr.lib")
 #endif
 
 #ifdef __cplusplus
  extern "C"{
 #endif
typedef unsigned char BYTE;
BYTE IsEgaVga( void );
void PrintChar( char thechar, int x, int y, BYTE fg, BYTE bk );
 void GrfxPrintf( int x, int y, BYTE fg, BYTE bk, char * string, ... );
 unsigned INIT( long VMODE );
 void setpage( int page );
 void showpage( int page );
void setpix( int x, int y, unsigned char pcolor);
 BYTE getpix( int x, int y );
void setpage( int page );
 void showpage( int page );
 void far * getfontptr( void );
void Line( int x1, int y1, int x2, int y2, BYTE lpcol );
  #ifdef __cplusplus
  }
 #endif
