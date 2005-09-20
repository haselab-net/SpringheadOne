#include "tri.h"
#include "kmenu.h"

void CImage(int ma, int mc);
void myinit(void){ 
	int n, m ;
	int ma, mc; /*入力画像バッファ番号*/
	m=0;
	ma=m; 
	imgread( m, "data\\haku201a.bmp");
	n=1;
	mc=n;
	imgread( n, "data\\haku202a.bmp");
	CImage(ma, mc);
}

void CImage(int ma, int mc){
	glClearColor (0.0, 0.0, 0.0, 0.0); 
    glShadeModel(GL_FLAT); 
    glEnable(GL_DEPTH_TEST); 

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

    glGenTextures(2, texid); 

    glBindTexture(GL_TEXTURE_2D, texid[0]); 
  	glTexImage2D(GL_TEXTURE_2D, 0, 3, bi[ma].Height, 
     bi[ma].Width , 0, GL_RGB, GL_UNSIGNED_BYTE, images[ma]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); 
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); 

    glBindTexture(GL_TEXTURE_2D, texid[1]); 
    glTexImage2D(GL_TEXTURE_2D, 0, 3, bi[mc].Height, 
       bi[mc].Width , 0, GL_RGB, GL_UNSIGNED_BYTE, images[mc]); 
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); 
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); 

    glEnable(GL_TEXTURE_2D); 
}

void TexMap(float* x, float* y, float X, float Y, float Z){
	float s,r;
	r = sqrt(X*X+Y*Y+Z*Z);
	s = (1/(TriPI)) * acos(Z/r);
	*x = 0.5f - s * X;
	*y = 0.5f + s * Y;
}

void WriteSphere(void){
	int i, j;
	
	glBindTexture(GL_TEXTURE_2D, texid[0]); 

	glBegin(GL_TRIANGLES);
	for(j=0; j<num/2; j++){
		for (i=0; i <2*num; i++) {
			float X,Y,Z, x,y;
	   		
			X = sin(j*TriPI/num)*cos(i*TriPI/num);
			Y = sin(j*TriPI/num)*sin(i*TriPI/num);
			Z = cos(j*TriPI/num);
			TexMap(&x,&y,X,Y,Z);
			glTexCoord2f(x,y);
			glVertex3f(X,Y,Z);

			X = sin(j*TriPI/num)*cos((i+1)*TriPI/num);
			Y = sin(j*TriPI/num)*sin((i+1)*TriPI/num);
			Z = cos(j*TriPI/num);
			TexMap(&x,&y,X,Y,Z);
			glTexCoord2f(x,y);
			glVertex3f(X,Y,Z);

			X = sin((j+1)*TriPI/num)*cos(i*TriPI/num);
			Y = sin((j+1)*TriPI/num)*sin(i*TriPI/num);
			Z = cos((j+1)*TriPI/num);
			TexMap(&x,&y,X,Y,Z);
			glTexCoord2f(x,y);
			glVertex3f(X,Y,Z);
		}
		for (i=0; i <2*num; i++) {
			float X,Y,Z, x,y;
 		
			X = sin(j*TriPI/num)*cos((i+1)*TriPI/num);
			Y = sin(j*TriPI/num)*sin((i+1)*TriPI/num);
			Z = cos(j*TriPI/num);
			TexMap(&x,&y,X,Y,Z);
			glTexCoord2f(x,y);
			glVertex3f(X,Y,Z);

			X = sin((j+1)*TriPI/num)*cos((i+1)*TriPI/num);
			Y = sin((j+1)*TriPI/num)*sin((i+1)*TriPI/num);
			Z = cos((j+1)*TriPI/num);
			TexMap(&x,&y,X,Y,Z);
			glTexCoord2f(x,y);
			glVertex3f(X,Y,Z);

			X = sin((j+1)*TriPI/num)*cos(i*TriPI/num);
			Y = sin((j+1)*TriPI/num)*sin(i*TriPI/num);
			Z = cos((j+1)*TriPI/num);
			TexMap(&x,&y,X,Y,Z);
			glTexCoord2f(x,y);
			glVertex3f(X,Y,Z);
		}
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texid[1]);

	glBegin(GL_TRIANGLES);
	for(j=num/2; j<num; j++){
		for (i=0; i <2*num; i++) {
			float X,Y,Z, x,y;
	   		
			X = sin(j*TriPI/num)*cos(i*TriPI/num);
			Y = sin(j*TriPI/num)*sin(i*TriPI/num);
			Z = cos(j*TriPI/num);

			TexMap(&x,&y,X,Y,-Z);
			glTexCoord2f(x,y);
			glVertex3f(X,Y,Z);

			X = sin(j*TriPI/num)*cos((i+1)*TriPI/num);
			Y = sin(j*TriPI/num)*sin((i+1)*TriPI/num);
			Z = cos(j*TriPI/num);
			TexMap(&x,&y,X,Y,-Z);
			glTexCoord2f(x,y);
			glVertex3f(X,Y,Z);

			X = sin((j+1)*TriPI/num)*cos(i*TriPI/num);
			Y = sin((j+1)*TriPI/num)*sin(i*TriPI/num);
			Z = cos((j+1)*TriPI/num);
			TexMap(&x,&y,X,Y,-Z);
			glTexCoord2f(x,y);
			glVertex3f(X,Y,Z);

		}
		for (i=0; i <2*num; i++) {
			float X,Y,Z, x,y;
 		
			X = sin(j*TriPI/num)*cos((i+1)*TriPI/num);
			Y = sin(j*TriPI/num)*sin((i+1)*TriPI/num);
			Z = cos(j*TriPI/num);
			TexMap(&x,&y,X,Y,-Z);
			glTexCoord2f(x,y);
			glVertex3f(X,Y,Z);

			X = sin((j+1)*TriPI/num)*cos((i+1)*TriPI/num);
			Y = sin((j+1)*TriPI/num)*sin((i+1)*TriPI/num);
			Z = cos((j+1)*TriPI/num);
			TexMap(&x,&y,X,Y,-Z);
			glTexCoord2f(x,y);
			glVertex3f(X,Y,Z);

			X = sin((j+1)*TriPI/num)*cos(i*TriPI/num);
			Y = sin((j+1)*TriPI/num)*sin(i*TriPI/num);
			Z = cos((j+1)*TriPI/num);
			TexMap(&x,&y,X,Y,-Z);
			glTexCoord2f(x,y);
			glVertex3f(X,Y,Z);

		}
	}
	glEnd();
}