#include "graphics.h"

#include "TextureImage.h"


TextureImage *texture[16];
//時間
float t=0;

//テクスチャ番号
unsigned int tex;

/*

//PNGスプライト表示
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

  //ポリゴンとテクスチャ
	glPushMatrix();
	glTranslatef( x,y,0.0 ); 
	if ((id==0)|(id==1)) {
		glRotatef(t,0,0,1); //Z軸を中心に回転
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
	//クライアント領域の塗りつぶし色の設定
	glClearColor( 0.5, 0.5, 0.5, 1.0 );
	//デプスバッファクリア値の設定
	glClearDepth( 1.0 );
/*
	//テクスチャの生成
	tex = GenTextureBitmap("texture.bmp");
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	//BMPテクスチャのロード、別に必要ないから削除してもよい
	blueball = GenTextureBitmap("blueball.bmp");
	glBindTexture(GL_TEXTURE_2D, blueball);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
*/
	//PNGテクスチャのロード
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

