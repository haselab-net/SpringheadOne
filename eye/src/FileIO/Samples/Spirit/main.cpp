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
#if 1	//	パースツリーを作らない場合．(安定動作中．こちらを使ってください)
void main(){	
	FIPhraseParser start, p1, p2;
	start		=	*(p1[&p1Func] | p2[&p2Func]);
	p1 =		str_p("abc")[&print_a] >> str_p("def")[&print_a];
	p2 =		str_p("abc")[&print_a]  >> str_p("123")[&print_a];

	FISkipParser skip = space_p;
	//	スキップパーサ(スペースとコメントを読み出すパーサ)の定義
	const char* f = "abc123abcdefabc123abcdef";
	const char* l = f+strlen(f);
	parse_info<> info = parse(f, l,  start, skip);
	std::cout << std::endl;
}
#else	//	パースツリーを自動で作る場合．(まだ挙動が不明．調整中)
void main(){	
	FIPTPhraseParser start, p1, p2;
	start		=	*(p1[&p1Func] | p2[&p2Func]);
	p1 =		str_p("abc")[&print_a] >> str_p("def")[&print_a];
	p2 =		str_p("abc")[&print_a]  >> str_p("123")[&print_a];

	FIPTSkipParser skip = space_p;
	//	スキップパーサ(スペースとコメントを読み出すパーサ)の定義
	const char* f = "abc123abcdefabc123abcdef";
	const char* l = f+strlen(f);
	FIPTInfo r = FIPTParse(f, l,  start, skip);	
	std::cout << std::endl;
	std::cout << "--- parse tree -----------------------------------" << std::endl;
	r.Print(cout);
	cin.get();
}
#endif
