#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <time.h>
#include <io.h>
/*
#include <sys/uio.h>
#include <unistd.h>
*/

#define ERROR (-1)

#include "emlimg.h"

int writepixel(EMLImg *img);
int readpixel(EMLImg *img);

int wrteml(EMLImg *img);
char header[98];
unsigned char *int_img;


  
/*
=====================================================
  WRITE EML IMAGE HEADER
=====================================================
*/
int wrteml(EMLImg *img) {
    EMLHeadFmt header;
    char *cp, *dummy;
    FILE *fp;
    unsigned long bytes;

    if((fp = fopen(img->filename, "wb")) == NULL) return FAILURE;

    strncpy(header.comment, img->comment,COMMENTBYTE - 1);
    strcpy(header.date,img->date);
    cp = header.coord;
    sprintf(cp, "%4d",img->left);     cp +=4;
    sprintf(cp, "%4d",img->top);      cp +=4;
    sprintf(cp, "%4d",img->right);    cp +=4;
    sprintf(cp, "%4d",img->bottom);   cp +=4;
    *cp = '\0';

    fwrite(&header, sizeof(EMLHeadFmt), 1, fp);

    bytes = img->width-MINWIDTH;
    if((dummy = (char *)malloc(sizeof(char)*bytes*sizeof(Pixel))) == NULL) 
        return FAILURE;
    fwrite(dummy,sizeof(Pixel), bytes,fp);
    free(dummy);

    fwrite(img->pix, sizeof(Pixel), img->width*img->height, fp);

    fclose(fp);
    return SUCCESS;
}

  
/*
=====================================================
  WRITE EML IMAGE HEADER
=====================================================
*/
int writeimg(EMLImg *img) {
  EMLHeadFmt header;
  char *cp;

  /* set comment, date and image size */
  strncpy(header.comment, img->comment,COMMENTBYTE - 1);
  strcpy(header.date,img->date);
  cp = header.coord;
  sprintf(cp, "%4d",img->left);     cp +=4;
  sprintf(cp, "%4d",img->top);      cp +=4;
  sprintf(cp, "%4d",img->right);    cp +=4;
  sprintf(cp, "%4d",img->bottom);   cp +=4;
  *cp = '\0';

  /* file open & write the image header */
  if((img->fh = creat(img->filename,0644|O_BINARY)) == ERROR)  return FAILURE;
//  if(write(img->fh,&header,HEADERBYTE) != HEADERBYTE)  return FAILURE;

  /* write pixel values */
  if(writepixel(img) == FAILURE) return FAILURE;
  /* file close */
  close(img->fh);
  return SUCCESS;
}/* writeimg */


/*
=====================================================
  WRITE EML IMAGE PIXEL VALUES
=====================================================
*/
int writepixel(EMLImg *img)
{
  unsigned long bytes;
  Pixel         *px1,*px2;
  int           iy;
  char          *dummy;

  /* if image width < MINWIDTH */
  if(img->width < MINWIDTH){
    bytes = MINWIDTH*img->height*sizeof(Pixel);
    if((img->pix = (Pixel *)realloc(img->pix,bytes)) == NULL)  return FAILURE;
    bytes = img->width*sizeof(Pixel);
    for(iy=img->height-1;iy<0;iy--){
      px1 = &(img->pix[img->width*iy]);
      px2 = &(img->pix[MINWIDTH*iy]);
      memcpy(px2,px1,bytes);
    }
    bytes = MINWIDTH*img->height*sizeof(Pixel);
  }
  else if(img->width > MINWIDTH){
    bytes = (img->width - MINWIDTH)*sizeof(Pixel);
    if((dummy = (char *)malloc(sizeof(char)*bytes)) == NULL)   return FAILURE;
    if(write(img->fh,dummy,bytes) != (long)bytes) return FAILURE;
    free(dummy);
    bytes = img->width*img->height*sizeof(Pixel);
  }

  /*write pixel values */
  if(write(img->fh,img->pix,bytes) != (long)bytes)  return FAILURE;

  return SUCCESS;
}/* writepixel */


/*
=====================================================
  READ EML IMAGE PIXEL VALUES
=====================================================
*/
int reademl(EMLImg *img)
{
  EMLHeadFmt     header;
  char          *cp;

  /* file open & read an image header */
  if((img->fh = open(img->filename, O_RDONLY|O_BINARY)) == ERROR) return FAILURE;
  if(read(img->fh, &header, HEADERBYTE) != HEADERBYTE) return FAILURE;
  /* get image size */
  cp = header.flag;
  *cp = '\0';  cp -= 4;
  sscanf(cp, "%4d", &(img->bottom));
  *cp = '\0';  cp -= 4;
  sscanf(cp, "%4d", &(img->right));
  *cp = '\0';  cp -= 4;
  sscanf(cp, "%4d", &(img->top));
  *cp = '\0';  cp -= 4;
  sscanf(cp, "%4d", &(img->left));
  img->width  = img->right - img->left + 1;
  img->height = img->top - img->bottom + 1;
  if(img->width < 1 || img->height < 1) return FAILURE;

  /* get date & comment */
  *cp = '\0';
  strcpy(img->date, header.date);
  *(header.date) = '\0';
  strcpy(img->comment, header.comment);

  /* read pixel values */
  if(readpixel(img) == FAILURE) return FAILURE;

  /* file close */
  close(img->fh);
  return SUCCESS;
} /* readimg */

int readpixel(EMLImg *img)
{
  unsigned long  bytes;
  Pixel         *px1, *px2;
  int            /*ix,*/ iy;
  unsigned long  test;
  
  /* set start point of pixel values */
  bytes = (img->width < MINWIDTH) ? MINWIDTH * sizeof(Pixel)
                                : img->width * sizeof(Pixel);
  lseek(img->fh, bytes, 0);
/*
  printf("here:%d\n",bytes);
*/
  /* read pixel values */
  bytes *= img->height;
  if(img->pix == NULL)
      if((img->pix = (Pixel *) malloc (bytes)) == NULL)
         return FAILURE;
 /* if(read(img->fh, img->pix, bytes) != bytes) return FAILURE;*/
  test=read(img->fh, img->pix, bytes);
/*
  printf("here:%d, %d\n", test, bytes);
*/
/*  if(test!=bytes) return FAILURE;*/

  /* if image width < MINWIDTH */
  if(img->width < MINWIDTH) {
    bytes = img->width * sizeof(Pixel);
    for(iy=1; iy<img->height; iy++) {
      px1 = &(img->pix[img->width * iy]);
      px2 = &(img->pix[MINWIDTH * iy]);
      memcpy(px1, px2, bytes);
    }
  }

  return SUCCESS;
} /* readpixel */


/*
int writepix(EMLImg *img)
{
    long lseek();
    register int  i;
    register unsigned char *c;
    register double maxlev = 0.0;
    int  id, ilen;
    char *cp;
    unsigned char *ip;

    id = creat(img->filename, 0644|O_BINARY);
    if ( id == -1 ) return FAILURE;

    cp = header;
    strncpy(cp, img->comment,56); cp += 56;
    strcpy(cp,img->date); cp += 24;
    sprintf(cp, "%4d",img->left);     cp +=4;
    sprintf(cp, "%4d",img->top);      cp +=4;
    sprintf(cp, "%4d",img->right);    cp +=4;
    sprintf(cp, "%4d",img->bottom);   cp +=4;
    header[ 96 ] = '\0';
    write( id, header, 97 );
    ilen = img->width * 3;
    if ( ilen * sizeof(char) < 120 ) ilen = 120 / sizeof(char);

    int_img = (unsigned char *)malloc( ilen * img->height * sizeof(char) );

    ip = int_img;
    for(i=0; i<img->width*img->height; i++) {
        *ip = img->pix[i].r; ip++;
        *ip = img->pix[i].g; ip++;
        *ip = img->pix[i].b; ip++;
    }

    lseek( id, ( off_t )( ilen * sizeof(char) ), 0 );

    write( id, ( char * )int_img, ilen * img->height);
    close( id );
}
*/


