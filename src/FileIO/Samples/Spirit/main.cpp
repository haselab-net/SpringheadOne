#include <FileIO/FISpirit.h>
#include <FileIO/FITypeDesc.h>
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace Spr;
using namespace std;
using namespace boost::spirit;

void print_a(const char* s, const char* e){
	UTString str(s,e);
	std::cout << str.c_str() << " ";
}
void p1Func(const char* s, const char* e){
	std::cout << "[p1]";
}
void p2Func(const char* s, const char* e){
	std::cout << "[p2]";
}
#if 1	//	�p�[�X�c���[�����Ȃ��ꍇ�D(���蓮�쒆�D��������g���Ă�������)
void main(){	
	FIPhraseParser start, p1, p2;
	start		=	*(p1[&p1Func] | p2[&p2Func]);
	p1 =		str_p("abc")[&print_a] >> str_p("def")[&print_a];
	p2 =		str_p("abc")[&print_a]  >> str_p("123")[&print_a];

	FISkipParser skip = space_p;
	//	�X�L�b�v�p�[�T(�X�y�[�X�ƃR�����g��ǂݏo���p�[�T)�̒�`
	const char* f = "abc123abcdefabc123abcdef";
	const char* l = f+strlen(f);
	parse_info<> info = parse(f, l,  start, skip);
	std::cout << std::endl;
}
#else	//	�p�[�X�c���[�������ō��ꍇ�D(�܂��������s���D������)
void main(){	
	FIPTPhraseParser start, p1, p2;
	start		=	*(p1[&p1Func] | p2[&p2Func]);
	p1 =		str_p("abc")[&print_a] >> str_p("def")[&print_a];
	p2 =		str_p("abc")[&print_a]  >> str_p("123")[&print_a];

	FIPTSkipParser skip = space_p;
	//	�X�L�b�v�p�[�T(�X�y�[�X�ƃR�����g��ǂݏo���p�[�T)�̒�`
	const char* f = "abc123abcdefabc123abcdef";
	const char* l = f+strlen(f);
	FIPTInfo r = FIPTParse(f, l,  start, skip);	
	std::cout << std::endl;
	std::cout << "--- parse tree -----------------------------------" << std::endl;
	r.Print(cout);
	cin.get();
}
#endif
