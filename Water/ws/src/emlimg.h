/*------------------------------------------------------------*/
/*        emlg.h                                              */
/*        Image format for Electric Machinery Lab.            */
/*        Hiroshima University                                */
/*        93/08/09 coorded by Kazu Kaneda                     */
/*------------------------------------------------------------*/

#define MINWIDTH     40
#define HEADERBYTE  120
#define COMMENTBYTE  56
#define DATEBYTE     24
// #define ERROR        (-1) 
#define FAILURE      1
#define SUCCESS      0 


typedef struct {
  char comment[56];
  char date[24];
  char coord[16];
  char flag[2];     
  char a_flag[1];   
  char reserved[21];
} EMLHeadFmt;

typedef struct {
  unsigned char r;
  unsigned char g;
  unsigned char b;
} Pixel;

typedef struct {
  int   fh;
  char  filename[80];
  char  comment[80];
  char  date[25];
  int   left,right;
  int   top,bottom;
  int   width,height;
  Pixel *pix;
} EMLImg;

/*typedef struct {
  unsigned int num;
  int    initial;
  double step;
  double Val[1000];
} IMGData;*/
