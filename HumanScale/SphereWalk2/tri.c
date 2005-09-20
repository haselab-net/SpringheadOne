#include "tri.h"
#include "kmenu.h"


void CImage(int imagenum);
void myinit(void){ 
	imgread( 0, "data\\4k-Upper-DR.bmp");
	imgread( 1, "data\\4k-Upper-DL.bmp");
	imgread( 2, "data\\4k-Upper-UL.bmp");
	imgread( 3, "data\\4k-Upper-UR.bmp");
	imgread( 4, "data\\4k-Lower-DR.bmp");
	imgread( 5, "data\\4k-Lower-DL.bmp");
	imgread( 6, "data\\4k-Lower-UL.bmp");
	imgread( 7, "data\\4k-Lower-UR.bmp");
	CImage(8); // (“ü—Í‰æ‘œ”)

}

void CImage(int imagenum){
	int i;
	GLfloat borderColor[4] = {1.0, 0.0, 0.0, 1.0};

	glClearColor (0.0, 0.0, 0.0, 0.0); 
    glShadeModel(GL_FLAT); 
    glEnable(GL_DEPTH_TEST); 

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

    glGenTextures(8, texid); 

	for(i = 0; i < imagenum; i++){
      glBindTexture(GL_TEXTURE_2D, texid[i]); 
  	  glTexImage2D(GL_TEXTURE_2D, 0, 3, bi[i].Height, 
                   bi[i].Width, 0, GL_RGB, GL_UNSIGNED_BYTE, images[i]);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_CLAMP_TO_EDGE_SGIS); 
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //GL_CLAMP_TO_EDGE_SGIS); 
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
      //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 
	}

    glEnable(GL_TEXTURE_2D); 
}

void TexMap(int mode, float* x, float* y, float X, float Y, float Z){
	float s,r;
	static float max = 0, min = 1.0f;
	r = sqrt(X*X+Y*Y+Z*Z);
//	fprintf(stderr, "R = %f\n", r);
	s = (1/(TriPI)) * acos(Z/r);
	//*x = (0.5f - s * X) * 2.0f;
	//*y = (0.5f + s * Y) * 2.0f;

	switch(mode){
	case 0:
		*x = ( s * X * 2.0f ) * (double)((TEXSIZE - 2.0f) / TEXSIZE) + (double)(1.0f / TEXSIZE);
		*y = ( 1.0f - (s * Y * 2.0f) ) * (double)((TEXSIZE - 2.0f) / TEXSIZE) + (double)(1.0f / TEXSIZE);
		break;
	case 1:
		*x = ( (s * X * 2.0f) + 1.0f) * (double)((TEXSIZE - 2.0f) / TEXSIZE) + (double)(1.0f / TEXSIZE);
		*y = ( 1.0f - ( s * Y  * 2.0f) ) * (double)((TEXSIZE - 2.0f) / TEXSIZE) + (double)(1.0f / TEXSIZE);
		break;
	case 2:
		*x = ((s * X * 2.0f) + 1.0f)  * (double)((TEXSIZE - 2.0f) / TEXSIZE) + (double)(1.0f / TEXSIZE);
		*y = ( - s * Y * 2.0f) * (double)((TEXSIZE - 2.0f) / TEXSIZE) + (double)(1.0f / TEXSIZE);
		break;
	case 3:
		*x = ( s * X * 2.0f ) * (double)((TEXSIZE - 2.0f) / TEXSIZE) + (double)(1.0f / TEXSIZE);
		*y = (  - s * Y * 2.0f) * (double)((TEXSIZE - 2.0f) / TEXSIZE) + (double)(1.0f / TEXSIZE);
		break;
	}

}

void WriteSphere(void){
	int i, j, k;


//	glDisable(GL_TEXTURE_2D);
//	glEnable(GL_TEXTURE_2D);
//	glPolygonMode(GL_FRONT, GL_FILL);

	for(k=0; k<4; k++){
	glBindTexture(GL_TEXTURE_2D, texid[k]); 
		glBegin(GL_TRIANGLES);
		for(j=0; j<0.5*num; j++){         // j ‚Í 1/2ƒÎ “®‚­
			for (i=k*0.5*num; i <(k+1)*0.5*num; i++) {  // i ‚Í 2ƒÎ “®‚­
				float X,Y,Z, x,y;
	   			
				X = sin(j*TriPI/num)*cos(i*TriPI/num);
				Y = sin(j*TriPI/num)*sin(i*TriPI/num);
				Z = cos(j*TriPI/num);
				TexMap(k, &x,&y,X,Y,Z);
				glTexCoord2f(x,y);
				glVertex3f(X,Y,Z);

				X = sin(j*TriPI/num)*cos((i+1)*TriPI/num);
				Y = sin(j*TriPI/num)*sin((i+1)*TriPI/num);
				Z = cos(j*TriPI/num);
				TexMap(k, &x,&y,X,Y,Z);
				glTexCoord2f(x,y);
				glVertex3f(X,Y,Z);

				X = sin((j+1)*TriPI/num)*cos(i*TriPI/num);
				Y = sin((j+1)*TriPI/num)*sin(i*TriPI/num);
				Z = cos((j+1)*TriPI/num);
				TexMap(k, &x,&y,X,Y,Z);
				glTexCoord2f(x,y);
				glVertex3f(X,Y,Z);
			}
			for (i=k * 0.5 * num ; i <(k+1)*0.5*num; i++) {
				float X,Y,Z, x,y;
 			
				X = sin(j*TriPI/num)*cos((i+1)*TriPI/num);
				Y = sin(j*TriPI/num)*sin((i+1)*TriPI/num);
				Z = cos(j*TriPI/num);
				TexMap(k, &x,&y,X,Y,Z);
				glTexCoord2f(x,y);
				glVertex3f(X,Y,Z);

				X = sin((j+1)*TriPI/num)*cos((i+1)*TriPI/num);
				Y = sin((j+1)*TriPI/num)*sin((i+1)*TriPI/num);
				Z = cos((j+1)*TriPI/num);
				TexMap(k, &x,&y,X,Y,Z);
				glTexCoord2f(x,y);
				glVertex3f(X,Y,Z);

				X = sin((j+1)*TriPI/num)*cos(i*TriPI/num);
				Y = sin((j+1)*TriPI/num)*sin(i*TriPI/num);
				Z = cos((j+1)*TriPI/num);
				TexMap(k, &x,&y,X,Y,Z);
				glTexCoord2f(x,y);
				glVertex3f(X,Y,Z);
			}
		}
		glEnd();
	}

///
/// ˆÈ~‚Í‰º”¼•ª
///

	for(k=0; k<4; k++){

		glBindTexture(GL_TEXTURE_2D, texid[k+4]); 

		glBegin(GL_TRIANGLES);
		for(j=0.5*num; j<num; j++){
			for (i=k*0.5*num; i <(k+1)*0.5*num; i++) {
				float X,Y,Z, x,y;
	   			
				X = sin(j*TriPI/num)*cos(i*TriPI/num);
				Y = sin(j*TriPI/num)*sin(i*TriPI/num);
				Z = cos(j*TriPI/num);

				TexMap(k, &x,&y,X,Y,-Z);
				glTexCoord2f(x,y);
				glVertex3f(X,Y,Z);

				X = sin(j*TriPI/num)*cos((i+1)*TriPI/num);
				Y = sin(j*TriPI/num)*sin((i+1)*TriPI/num);
				Z = cos(j*TriPI/num);
				TexMap(k, &x,&y,X,Y,-Z);
				glTexCoord2f(x,y);
				glVertex3f(X,Y,Z);

				X = sin((j+1)*TriPI/num)*cos(i*TriPI/num);
				Y = sin((j+1)*TriPI/num)*sin(i*TriPI/num);
				Z = cos((j+1)*TriPI/num);
				TexMap(k, &x,&y,X,Y,-Z);
				glTexCoord2f(x,y);
				glVertex3f(X,Y,Z);

			}
			for (i=k*0.5*num; i <(k+1)*0.5*num; i++) {
				float X,Y,Z, x,y;
 			
				X = sin(j*TriPI/num)*cos((i+1)*TriPI/num);
				Y = sin(j*TriPI/num)*sin((i+1)*TriPI/num);
				Z = cos(j*TriPI/num);
				TexMap(k, &x,&y,X,Y,-Z);
				glTexCoord2f(x,y);
				glVertex3f(X,Y,Z);

				X = sin((j+1)*TriPI/num)*cos((i+1)*TriPI/num);
				Y = sin((j+1)*TriPI/num)*sin((i+1)*TriPI/num);
				Z = cos((j+1)*TriPI/num);
				TexMap(k, &x,&y,X,Y,-Z);
				glTexCoord2f(x,y);
				glVertex3f(X,Y,Z);

				X = sin((j+1)*TriPI/num)*cos(i*TriPI/num);
				Y = sin((j+1)*TriPI/num)*sin(i*TriPI/num);
				Z = cos((j+1)*TriPI/num);
				TexMap(k, &x,&y,X,Y,-Z);
				glTexCoord2f(x,y);
				glVertex3f(X,Y,Z);

			}
		}
		glEnd();
	}

}