#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <FileIO/FISpirit.h>
#include <FileIO/FITypeDesc.h>
#include <SceneGraph/SGObject.h>
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace Spr;
using namespace std;

DEF_RECORD(XVectorTest, {
	FIVVector<Vec3f> pos;
})

DEF_RECORD(XVectorTest2, {
	int len;
	FIVVector<Vec3f> pos VSIZE(len); 
})

DEF_RECORD(XArrayTest, {
	float size[3];
})

DEF_RECORD(XCompTest, {
	FIVVector<XVectorTest2> vt2;
})

using namespace boost::spirit;

#if 0
void print_a(const char* s, const char* e){
	UTString str(s,e);
	std::cout << str.c_str() << " ";
}
void main(){

	
	FIPTPhraseParser start, alter;
	start		=	*alter; ///my_guard(alter)[handler];
//	alter =		(str_p("abc")[&print_a] >> str_p("def"))
//				| (str_p("abc")[&print_a] >> str_p("123"));
	alter =		(str_p("abc") >> str_p("def"))
				| (str_p("abc") >> str_p("123"));
	FIPTSkipParser skip = space_p;
	//	スキップパーサ(スペースとコメントを読み出すパーサ)の定義
	const char* f = "abc123abcdefabc123abcdef";
	const char* l = f+strlen(f);
	FIPTInfo r = FIPTParse(f, l,  start, skip);	
	std::cout << "--- parse tree -----------------------------------" << std::endl;
	r.Print(cout);
	cin.get();
}

#else
void main(){
	std::cout << classDefOfXVectorTest2 << std::endl;

	int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpFlag = tmpFlag | _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag( tmpFlag );


	UTRef<FITypeDescDb> db = new FITypeDescDb;
	db->REG_FIELD(int);
	db->REG_FIELD(float);
	db->REG_FIELD(Vec3f);
	db->REG_RECORD(XVectorTest);
	db->REG_RECORD(XVectorTest2);
	db->REG_RECORD(XArrayTest);
	db->REG_RECORD(XCompTest);
	db->SetPrefix("X");
	db->Print(std::cout);

	XCompTest target;
	target.vt2.push_back(XVectorTest2());
	target.vt2.back().len = 2;
	target.vt2.back().pos.push_back(Vec3f(0,1,2));
	target.vt2.back().pos.push_back(Vec3f(3,4,5));

	UTRef<FIDocNode> top = new FIDocNode;
	FITypeDesc* desc = db->Find("CompTest");
	top->SetType("CompTest");
	desc->Write(top, &target);
	top->Print(std::cout);
}
#endif
