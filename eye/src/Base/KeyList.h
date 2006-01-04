#ifndef BASE_KEYLIST_H
#define BASE_KEYLIST_H
#include "Env.h"
#include <list>
#include <set>

/**	@file KeyList.h �L�[���[�h�����@�\�����X�g�̒�`*/
namespace Spr {;

/**	set ���̃��X�g�D�L�[�ō����ɑ��삪�ł��� list�D
	set�ƈ���āC���Ԃ��ێ�����C���ԂŃA�N�Z�X���邱�Ƃ��ł���D */

template <class T, class Pred=std DOUBLECOLON less<T> >
class UTKeyList: std DOUBLECOLON list<T>{
public:
	typedef std::list<T> base;
	typedef base::iterator iterator;
	typedef base::const_iterator const_iterator;
	struct SetPred{
		typedef TYPENAME std::list<T>::iterator listIt;
		bool operator() (const listIt& i1, const listIt& i2) const {
			Pred pred;
			return pred(*i1, *i2);
		}
	};
	typedef std::set< TYPENAME std::list<T>::iterator, SetPred > Finder;
	Finder finder;
	UTKeyList(){}
	UTKeyList(const UTKeyList<T>& l):std::list<T>(l){
		for(iterator it = begin(); it != end(); ++it) finder.insert(it);
	}
	void push_back(const T& t){
		base::push_back(t);
		InsToFinder(--base::end());
	}
	void pop_back(){
		EraseFromFinder(--base::end());
		base::pop_back();
	}
	void push_front(const T& t){
		base::push_front(t);
		InsToFinder(base::begin());
	}
	void pop_front(){
		EraseFromFinder(base::begin());
		base::pop_front();
	}
	void insert(base::iterator it, T& t = T()){
		base::iterator ins = base::insert(it, t);
		InsToFinder(ins);
	}
	void erase(base::iterator it){
		EraseFromFinder(it);
		base::erase(it);
	}
	void erase(const T& t){
		base::push_back(t);
		Finder::iterator rv = finder.find(--end());
		base::pop_back();
		if (rv != finder.end()) erase(*rv);
	}
	base::iterator find(const T& t){
		base::push_back(t);
		Finder::iterator rv = finder.find(--base::end());
		base::pop_back();
		if (rv == finder.end()){
			return end();
		}else{
			return *rv;
		}
	}
	base::const_iterator find(const T& t) const {
		return ((UTKeyList<T, Pred>*)this)->find(t);
	}
	base::iterator begin(){ return base::begin(); }
	base::const_iterator begin() const { return base::begin(); }
	base::iterator end(){ return base::end(); }
	base::const_iterator end() const { return base::end(); }
	size_t size() const { return base::size(); }

private:
	void InsToFinder(base::iterator& it){
		std::pair<Finder::iterator, bool> rv = finder.insert(it);
		if (!rv.second){	//	���ɓ����L�[�����݂��邽�߁C�ǉ��ł��Ȃ������Ƃ�
			base::erase(*rv.first);	//	���X�g����Â��l���폜���C
			(base::iterator&)*rv.first = it;			//	�V�����l�ɍX�V����D���ʂƂ���list�̗v�f���������Ȃ��D
		}
	}
	void EraseFromFinder(base::iterator& it){
		int ne = finder.erase(it);
		assert(ne == 1);
	}
};

}

#endif
