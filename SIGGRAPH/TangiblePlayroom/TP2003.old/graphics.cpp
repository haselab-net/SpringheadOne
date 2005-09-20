#include "graphics.h"

#include "TextureImage.h"


TextureImage *texture[16];
//����
float t=0;

//�e�N�X�`���ԍ�
unsigned int tex;

/*

//PNG�X�v���C�g�\��
void SpritePNG(unsigned int id,Vec3f pos,float r, float alpha) {
	float x,y,z;
	x = pos.X();
	y = pos.Y();
//	z = 1.0f;
	z = pos.Z();
	cout << pos << endl;
	glEnable( GL_BLEND );
	glDepthMask( GL_FALSE );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glBindTexture( GL_TEXTURE_2D, texture[id]->texID );

  //�|���S���ƃe�N�X�`��
	glPushMatrix();
	glTranslatef( x,y,0.0 ); 
	if ((id==0)|(id==1)) {
		glRotatef(t,0,0,1); //Z���𒆐S�ɉ�]
	}
	glBegin(GL_QUADS);
	glColor4f(0.5,0.5,0.5,alpha);
	glTexCoord2f(0.0, 0.0);
	glVertex3f( r, r, z);
	glTexCoord2f(0.0, 1.0);
	glVertex3f( r,-r, z);
	glTexCoord2f(-1.0, 1.0);
	glVertex3f(-r,-r, z);
	glTexCoord2f(-1.0, 0.0);
	glVertex3f(-r, r, z);
	glEnd();
	glPopMatrix();

	glDepthMask( GL_TRUE ) ;
	glDisable( GL_BLEND ) ;


}
*/
void Texture_Setup (void){
	glClearColor (0.0, 0.0, 0.0,1.0);
	glEnable(GL_DEPTH_TEST);
	//�N���C�A���g�̈�̓h��Ԃ��F�̐ݒ�
	glClearColor( 0.5, 0.5, 0.5, 1.0 );
	//�f�v�X�o�b�t�@�N���A�l�̐ݒ�
	glClearDepth( 1.0 );
/*
	//�e�N�X�`���̐���
	tex = GenTextureBitmap("texture.bmp");
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	//BMP�e�N�X�`���̃��[�h�A�ʂɕK�v�Ȃ�����폜���Ă��悢
	blueball = GenTextureBitmap("blueball.bmp");
	glBindTexture(GL_TEXTURE_2D, blueball);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
*/
	//PNG�e�N�X�`���̃��[�h
    texture[0] = new TextureImage();
    texture[0]->IMGLoad( "blueball.png" );
    texture[1] = new TextureImage();
    texture[1]->IMGLoad( "blackball.png" );
    texture[2] = new TextureImage();
    texture[2]->IMGLoad( "free.png" );
    texture[3] = new TextureImage();
    texture[3]->IMGLoad( "grasp.png" );

	glEnable(GL_TEXTURE_2D);

}

