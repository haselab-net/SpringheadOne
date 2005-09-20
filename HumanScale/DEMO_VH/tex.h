#include <GL/glaux.h>
#include "mymath.h"

//used inside glutIdle() to keep constant frame rate
#define	GRAVITY 10.0      //13.5
#define STATIC_FRIC 0.87  //99
//JEONG 10/21
#define TEX_NUM 6

const GLfloat deltaRot = 5.0f;
const GLfloat nRange = 25.0f; 
const GLfloat nHeight = 45.0f; //45.0
const GLfloat delta_Pad = 0.5f;
const GLfloat convert_inch = 39.37;
//const GLfloat convert_inch = 39.37 * 3;	// çHçÏé∫SPIDARópÉXÉPÅ[Éãí≤êÆ
const GLfloat GnHeight = 2.5f; //
//JEONG
DWORD dwLastTime;
DWORD dwTime;

GLfloat green[] = {0.25, 1.0, 0.25, 1.0};
GLfloat red[] = {1.0, 0.25, 0.25, 1.0};
GLfloat blue[] = {0.25, 0.25, 1.0, 1.0};
GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
GLfloat gray[] = {0.5, 0.5, 0.5, 1.0};
GLfloat black[] = {0.0, 0.0, 0.0, 1.0};
GLfloat yellow[] = {1.0, 1.0, 0.25, 1.0};
GLfloat headcolor[] = {1.0, 0.8, 0.6, 1.0};
GLfloat wallcolor[] = {0.64, 0.47, 0.36, 1.0};
GLfloat shadow[] = { 0.0, 0.0, 0.0, 0.5   };

//SLCamera
AUX_RGBImageRec *texRec[TEX_NUM];
GLuint tex[TEX_NUM];
//~SLCamera

// using glaux.h texture load definition(^-^)
AUX_RGBImageRec* LoadBMPFile(char *filename)
{
	FILE *hFile = NULL;

	if(filename == "") return NULL;

	hFile = fopen(filename, "r");

	if(hFile) {
		fclose(hFile);
		return auxDIBImageLoad(filename);
	}

	return NULL;
}

void Load_Texture()
{
// Texture mapM_PIng array(^ ^), this files must be same size(ex;256*256)
	memset(texRec, 0, sizeof(void *)*TEX_NUM);

	if((texRec[0]=LoadBMPFile("texture/face.bmp")) && //
	   (texRec[1]=LoadBMPFile("texture/floor.bmp")) &&
	//SLCamera 10/20
		(texRec[2]=LoadBMPFile("texture/Eye.bmp"))  &&
						(texRec[3]=LoadBMPFile("texture/face2.bmp")) &&
							(texRec[4]=LoadBMPFile("texture/face3.bmp")) &&
								(texRec[5]=LoadBMPFile("texture/face4.bmp")))
	{   
		
		glGenTextures(TEX_NUM, &tex[0]);
		for(int i=0; i<TEX_NUM; i++) {
			glBindTexture(GL_TEXTURE_2D, tex[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  		 gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, texRec[i]->sizeX, texRec[i]->sizeY,
                     GL_RGB, GL_UNSIGNED_BYTE, texRec[i]->data);
		}
	} //else return FALSE;

	for(int i=0; i<TEX_NUM; i++) {
		if(texRec[i])
		{
			if(texRec[i]->data) free(texRec[i]->data);
			free(texRec[i]);
		} 
	}

	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);	// Set Up Sphere MapM_PIng
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);	// Set Up Sphere MapM_PIng
}

// model foot 
void drawQuad(double width, double height)
{
	 glBegin(GL_QUADS);
			// top
			glNormal3f(0.0f, height, 0.0f);
			glTexCoord2f(0,1);glVertex3f(height, height, height);
			glTexCoord2f(0,0);glVertex3f(width, height, height);
			glTexCoord2f(1,0);glVertex3f(width, height, -height);
			glTexCoord2f(1,1);glVertex3f(height, height, -height);

			// bottom
			glNormal3f(0.0f, -height, 0.0f);
			glTexCoord2f(1,1);glVertex3f(height, -height, -height);
			glTexCoord2f(0,1);glVertex3f(width, -height, -height);
			glTexCoord2f(0,0);glVertex3f(width, -height, height);
			glTexCoord2f(1,0);glVertex3f(height, -height, height);

			// back
			glNormal3f(0.0f, 0.0f, -height);
			glTexCoord2f(1,0);glVertex3f(height, height, -height);
			glTexCoord2f(1,1);glVertex3f(width, height, -height);
			glTexCoord2f(0,1);glVertex3f(width, -height, -height);
			glTexCoord2f(0,0);glVertex3f(height, -height, -height);

			// front
			glNormal3f(0.0f, 0.0f, height);
			glTexCoord2f(0,0);glVertex3f(height, -height, height);
			glTexCoord2f(1,0);glVertex3f(width, -height, height);
			glTexCoord2f(1,1);glVertex3f(width, height, height);
			glTexCoord2f(0,1);glVertex3f(height, height, height);

			// left
			glNormal3f(-height, 0.0f, 0.0f);
			glTexCoord2f(0,0);glVertex3f(height, -height, -height);
			glTexCoord2f(1,0);glVertex3f(height, -height, height);
			glTexCoord2f(1,1);glVertex3f(height, height, height);
			glTexCoord2f(0,1);glVertex3f(height, height, -height);

			// right
			glNormal3f(height, 0.0f, 0.0f);
			glTexCoord2f(1,0);glVertex3f(width, height, -height);
			glTexCoord2f(1,1);glVertex3f(width, height, height);
			glTexCoord2f(0,1);glVertex3f(width, -height, height);
			glTexCoord2f(0,0);glVertex3f(width, -height, -height);
 glEnd();
}

void drawSphere(float rad)
{
  int numMajor = 24;
  int numMinor = 32;
  double majorStep = (M_PI / numMajor);
  double minorStep = (2.0 * M_PI / numMinor);
  int i, j;


  for (i = 0; i < numMajor; ++i) {
    double a = i * majorStep;
    double b = a + majorStep;
    double r0 = rad * sin(a);
    double r1 = rad * sin(b);
    GLfloat z0 = rad * cos(a);
    GLfloat z1 = rad * cos(b);

    glBegin(GL_TRIANGLE_STRIP);
    for (j = 0; j <= numMinor; ++j) {
      double c = j * minorStep;
      GLfloat x = cos(c);
      GLfloat y = sin(c);

      glNormal3f((x * r0) / rad, (y * r0) / rad, z0 / rad);
      glTexCoord2f(j / (GLfloat) numMinor, i / (GLfloat) numMajor);
      glVertex3f(x * r0, y * r0, z0);

      glNormal3f((x * r1) / rad, (y * r1) / rad, z1 / rad);
      glTexCoord2f(j / (GLfloat) numMinor, (i + 1) / (GLfloat) numMajor);
      glVertex3f(x * r1, y * r1, z1);
    }
    glEnd();
	
  }
  //glDisable(GL_TEXTURE_2D);
}

// joint connection drawing
void Draw_Ellipse(float x, float y, float z, MyVector scale)
{
//10/27 JEONG
	float h = sqrt(x*x + z*z);		// parent: x0 = y0 = z0 = 0
    float d;
	glPushMatrix();

		d = sqrt(x*x + y*y + z*z);
		
		if (h > 0)
		{
			// ORIENT PARENT'S Y AXIS TO POINT AT CHILD
			float phi, theta;
			phi = acos(x/h)*180.0/M_PI;
			theta = acos(y/h)*180.0/M_PI;
			glRotatef(phi, 0.0f, 1.0f, 0.0f);		// y
	       	glRotatef(theta, 0.0f, 0.0f, 1.0f);		// z

			if (z == 0) d = -d;		// hack adjustment
		}
		else
		{
			if (y < 0) d = -d;
		}	
        if (scale.x < 0.75 )
		{
			scale.x =1;
			scale.z =1;
		}
		glTranslatef (0.f, d/2, 0.f);
		//glScalef (0.75, fabs(d)/2, 0.75);
		glScalef (scale.x, fabs(d)/2, scale.z);
		glutSolidSphere (1.0, 16, 12);

	glPopMatrix();
}

//-----------------------------------------------------------------------
void myglSolidFrustum(short Nxy,float ratio)//â~êçë‰
{   //â∫íÍÇÃíÜêSÇï®ëÃÇÃíÜêS
	//íºåaÅAçÇÇ≥ÇPÇÃë‰å`,è„íÍÇÃïùª≤ΩﬁÇÕratioî{,
	double nn[3];//ñ@ê¸ê¨ï™
	float p[32][3]; //í∏ì_ç¿ïW
	double r; //îºåa
	double theta0,theta,alpha,a,sa;
	short i,ii;

	if(Nxy >16) { Nxy = 16; }
	theta0 = 2.0*M_PI/(double)Nxy;
	r=0.5;
	for(i=0;i<Nxy;i++)
	{   theta = theta0*(double)i;
		p[i][0] = (float)(r*cos(theta)*ratio); //è„íÍÇÃxê¨ï™
		p[i][1] = (float)(r*sin(theta)*ratio);       //Çôê¨ï™
		p[i][2] = 1.0f;                              //Çöê¨ï™
		p[i+Nxy][0] = (float)(r*cos(theta)); //â∫íÍÇÃxê¨ï™
		p[i+Nxy][1] = (float)(r*sin(theta)); //yê¨ï™
		p[i+Nxy][2] = 0.0f;                  //Çöê¨ï™
	}

	//è„íÍ
	glBegin(GL_POLYGON);
		glNormal3f(0.0f,0.0f,1.0f); 
		for(i=0;i<Nxy;i++) glVertex3fv(p[i]); 
	glEnd();
	//â∫íÍ
	glBegin(GL_POLYGON);
		glNormal3f(0.0f,0.0f,-1.0f); 
		for(i=2*Nxy-1;i>=Nxy;i--) glVertex3fv(p[i]); 
	glEnd();

	alpha=atan(0.5*(1.0-ratio));
	sa=sin(alpha);
	a=sqrt(1.0+sa*sa);
	for(i=0;i<Nxy;i++)
	{
		if(i == Nxy-1) ii = 0; else ii = i+1;
		glBegin(GL_POLYGON);
			theta = theta0*(double)i;
			nn[0]=cos(theta)/a;nn[1]=sin(theta)/a;nn[2]=sa/a;
			glNormal3d(nn[0],nn[1],nn[2]); 
			glVertex3fv(p[i]);
			theta = theta0*(double)(i+Nxy);
			nn[0]=cos(theta)/a;nn[1]=sin(theta)/a;nn[2]=sa/a;
			glNormal3d(nn[0],nn[1],nn[2]); 
			glVertex3fv(p[i+Nxy]);
			theta = theta0*(double)(ii+Nxy);
			nn[0]=cos(theta)/a;nn[1]=sin(theta)/a;nn[2]=sa/a;
			glNormal3d(nn[0],nn[1],nn[2]); 
			glVertex3fv(p[ii+Nxy]);
			theta = theta0*(double)ii;
			nn[0]=cos(theta)/a;nn[1]=sin(theta)/a;nn[2]=sa/a;
			glNormal3d(nn[0],nn[1],nn[2]); 
			glVertex3fv(p[ii]);
		glEnd();
	}
}


void Draw_Model(int grp_id)
{

}

float Passed(float oldTime)
{
	float dwLastTime = oldTime;
	float dwTime = GetTickCount();

	return(dwTime - dwLastTime);
}