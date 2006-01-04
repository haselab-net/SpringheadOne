#ifndef BASE_COMBINATION_H
#define BASE_COMBINATION_H
#include "Env.h"
#include <list>
#include <set>

/**	@file Combination.h �L�[���[�h�����@�\�����X�g�̒�`*/
namespace Spr {;

/**	�g�ݍ��킹�z��i����ւ��s�C4�p�`�ɂȂ�j*/
template <class T>
class UTCombination:public std::vector<T>{
	int height_;
	int width_;
public:
	typedef std::vector<T> base_type;
	UTCombination(){ height_ = 0; width_ = 0; }
	///	�T�C�Y�̐ݒ�	
	void resize(int h, int w){
		if (h*w > height_*width_){	//	�傫���Ȃ�Ȃ�C��Ƀ��T�C�Y
			base_type::resize(h * w);
		}
		iterator b = begin();
		if (h < height_){	//	�g��Ȃ��Ȃ����s���N���A
			for(int l=h; l<height_; ++l){
				for(int x=0; x<width_; ++x) b[width_*l+x] = T();
			}
		}
		if (w > width_){	//	����������ꍇ�C��납��ړ�
			for(int l=height_-1; l>=0; --l){
				for(int x=w; x > width_-1; --x) b[w*l+x]=T();
				for(int x=width_-1; x>=0; --x) b[w*l+x] = b[width_*l+x];
			}
		}else if (w < width_){	//	��������ꍇ�C�O����ړ�
			for(int l=1; l<height_; ++l){
				for(int x=0; x<width_; ++x) b[w*l+x] = b[width_*l+x];
			}
		}
		if (h*w > height_*width_){	//	�������Ȃ�Ȃ�Ō�Ƀ��T�C�Y
			base_type::resize(h * w);
		}
		height_ = h;
		width_ = w;
	}
	int height(){ return height_; };
	int width(){ return width_; };
	T& item(int i, int j){
		assert(i < height());
		assert(j < width());
		return begin()[i*width() + j];
	}
	void clear(){
		base_type::clear();
		height_ = 0;
		width_ = 0;
	}
	const T& item(int i, int j) const { return ((UTCombination<T>*)this)->item(i,j); }
//	T& operator [] (int i, int j){ item(i,j); }
//	const T& operator [] (int i, int j) const { item(i,j); }
};

}

#endif
