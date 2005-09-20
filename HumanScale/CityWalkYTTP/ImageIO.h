//
// ImageIO
//   - simple library for image reading and saving
//   - hooks into OpenGL for loading textures and dumping screen captures
//  
//  Evan Hart - ATI Research Inc. - 9/2/2002
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef IMAGE_IO_H
#define IMAGE_IO_H

#ifdef __cplusplus
extern "C"{
#endif
//
// Types
//
////////////////////////////////////////////////////////////////////////////////

#define Image void*

//
// Entrypoints
//
////////////////////////////////////////////////////////////////////////////////

//
// CreateImage
//
//////////////////////////////////////////////////////////////////////
Image imgCreateImage();
void imgDestroyImage( Image img);
int imgLoadImage( Image img, char *file);
int imgSaveImage( Image img, char *file);
int imgGetWidth( Image img);
int imgGetHeight( Image img);
int imgGetDepth( Image img);
int imgIsFloat( Image img);

void imgGrabImage( Image img, int x, int y, int width, int depth, GLenum format, GLenum type);
void imgLoadImageAsTexture( Image img);
void imgLoadImageAsTextureFormat( Image img, GLenum format);

#ifdef __cplusplus
};
#endif

#endif //IMAGEIO_H