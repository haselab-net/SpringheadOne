// wrl2view: VRML2.0 file viewer	Copyright (c)1999 Mamoru Shiroki
//
// This program is free software.
// You can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation.

#include "type.h"
#include "type_imp.h"

#include <cstdio>
#include <cstdlib>
#include <map>
#define GLUT_NO_LIB_PRAGMA
#include <GL/glut.h>

namespace Spr {
using namespace std;

struct TTexInfo{
	int			nc;
	GLuint		texid;
};
typedef map<TNode*, TTexInfo>	TTexObjMap;
static TTexObjMap		texObjMap;

static const GLenum	pxfm[] = {GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_RGB, GL_RGBA};

////////////////////////////////////////////////////////////////////////////////
//	Draw texture nodes

void TTypePixelTexture::Init(TNode *pn)
{
	vector<int>	v;
	int			repeatS = 1;
	int			repeatT = 1;

	PushbackElm(pn, "image", v);
	CopyElm(pn, "repeatS", &repeatS);
	CopyElm(pn, "repeatT", &repeatT);

	if(v.size() == 0 || v[0] < 1 || v[1] < 1 || v[2] < 1 || 4 < v[2]){
		cerr << "PixelTexture has invalid image data. skipping..." << endl;
		return;					// error
	}

	TTexInfo	ti = { v[2] };
	glGenTextures(1, &ti.texid);
	glBindTexture(GL_TEXTURE_2D, ti.texid);
	texObjMap[pn] = ti;

	for(int i = 0; i < v.size() - 3; ++i){
#ifndef BIG_ENDIAN_CPU				// ByteSwaping on Little-Endian CPU
		const int	j = v[3 + i];
		v[3 + i] = (j >> 24) | ((0xff & (j >> 16)) << 8) | ((0xff & (j >> 8)) << 16) | ((0xff & j) << 24);
#endif
		memmove((char*)&v[3] + v[2] * i, (char*)&v[3 + i] + (4 - v[2]), v[2]);	// pack the texture-image array
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeatS ? GL_REPEAT:GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeatT ? GL_REPEAT:GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, v[2], v[0], v[1], pxfm[v[2] - 1], GL_UNSIGNED_BYTE, &v[3]);
}

void TTypePixelTexture::Draw(TNode *pn)
{
	TTexObjMap::iterator		it;
	if((it = texObjMap.find(pn)) == texObjMap.end())
		return;

	glBindTexture(GL_TEXTURE_2D, it->second.texid);
	glEnable(GL_TEXTURE_2D);
	if(it->second.nc > 2){		// RGB or RGBA
		float		diffuseColor[] = {1, 1, 1, 1};	// clear color
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffuseColor);
	}
	if(it->second.nc == 2 || it->second.nc == 4){	// ALPHA
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}


extern "C" {
#include "jpeglib.h"
#include <setjmp.h>
}

struct my_error_mgr {
	struct jpeg_error_mgr pub;	/* "public" fields */
	jmp_buf setjmp_buffer;		/* for return to caller */
};
typedef struct my_error_mgr * my_error_ptr;

void my_error_exit (j_common_ptr cinfo)
{
	my_error_ptr myerr = (my_error_ptr)cinfo->err;
	(*cinfo->err->output_message)(cinfo);
	longjmp(myerr->setjmp_buffer, 1);
}
struct StringLess{
	bool operator () (const std::string& s1, const std::string& s2) const {
		return strcmp(s1.c_str(), s2.c_str()) < 0;
	}
};
class TexFileMap:public std::map<std::string, TTexInfo, StringLess>{
};
TexFileMap texFileMap;

void TTypeImageTexture::Init(TNode *pn)
{
	int			repeatS = 1;
	int			repeatT = 1;
	char		*url = 0;

	CopyElm(pn, "repeatS", &repeatS);
	CopyElm(pn, "repeatT", &repeatT);
	CopyElm(pn, "url", &url);

	TTexInfo	ti;
	
	TexFileMap::iterator it = texFileMap.find(url);
	if ( it == texFileMap.end() ){	//	新しいテクスチャの場合
		char	*texbuf;
		int		tx, ty, nc;

		struct jpeg_decompress_struct cinfo;
		struct my_error_mgr jerr;
		JSAMPARRAY buffer;			/* Output row buffer */
		FILE * infile;				/* source file */
	
		if((infile = fopen(url, "rb")) == NULL) {
			fprintf(stderr, "can't open image-file '%s'\n", url);
			return;
		}
		cinfo.err = jpeg_std_error(&jerr.pub);
		jerr.pub.error_exit = my_error_exit;
		if(setjmp(jerr.setjmp_buffer)) {
			jpeg_destroy_decompress(&cinfo);
			fclose(infile);
			fprintf(stderr, "'%s' is not JPEG format. we support JPEG image only.\n", url);
			return;
		}
		
		jpeg_create_decompress(&cinfo);
		jpeg_stdio_src(&cinfo, infile);
		jpeg_read_header(&cinfo, TRUE);
		jpeg_start_decompress(&cinfo);
		buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE,
											  cinfo.output_width * cinfo.output_components, 1);
		tx = cinfo.output_width;
		ty = cinfo.output_height;
		nc = cinfo.output_components;
		TTexInfo	tiI = { nc };
		ti = tiI;
		texbuf = (char*)malloc(tx * ty * nc);
		if(!texbuf){
			fprintf(stderr, "memory allocate error.\n");
			return;
		}
		
		while(cinfo.output_scanline < cinfo.output_height) {
			char *p = texbuf + nc * tx * (cinfo.output_height - cinfo.output_scanline - 1);
			jpeg_read_scanlines(&cinfo, buffer, 1);
			memcpy(p, *buffer, nc * tx);
		}
		jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
	
		glGenTextures(1, &ti.texid);
		glBindTexture(GL_TEXTURE_2D, ti.texid);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeatS ? GL_REPEAT:GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeatT ? GL_REPEAT:GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, nc, tx, ty, pxfm[nc - 1], GL_UNSIGNED_BYTE, texbuf);
		free(texbuf);
		texFileMap[url] = ti;
	}else{	//	ロード済みの場合
		ti = it->second;
	}
	texObjMap[pn] = ti;
}

void TTypeImageTexture::Draw(TNode *pn)
{
	TTexObjMap::iterator		it;
	if((it = texObjMap.find(pn)) == texObjMap.end())
		return;

	glBindTexture(GL_TEXTURE_2D, it->second.texid);
	glEnable(GL_TEXTURE_2D);
	if(it->second.nc > 2){		// RGB or RGBA
		float		diffuseColor[] = {1, 1, 1, 1};	// clear color
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffuseColor);
	}
}


////////////////////////////////////////////////////////////////////////////////
//	JPEG Screen Snapshot

void WriteScreenJPEG(const char* filename, int width, int height, int quality)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE * outfile;
	JSAMPROW ptBuf[1];

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	if((outfile = fopen(filename, "wb")) == NULL) {
		fprintf(stderr, "can't open output-file '%s'\n", filename);
		return;
	}
	jpeg_stdio_dest(&cinfo, outfile);

	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, TRUE);
	cinfo.optimize_coding = TRUE;
	cinfo.dct_method = JDCT_FLOAT;

	ptBuf[0] = (JSAMPLE*)malloc(cinfo.image_width * 3);
	jpeg_start_compress(&cinfo, TRUE);
	while(cinfo.next_scanline < cinfo.image_height) {
		glReadPixels(0, cinfo.image_height - cinfo.next_scanline - 1,
					 cinfo.image_width, 1, GL_RGB, GL_UNSIGNED_BYTE, (void*)ptBuf[0]);
		jpeg_write_scanlines(&cinfo, ptBuf, 1);
	}
	free(ptBuf[0]);
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	fclose(outfile);
}
}	//	namespace Spr