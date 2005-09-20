#include "FISpirit.h"
#include "FileIO.h"


namespace Spr{;
/**	構造体のアライメント(変数の位置あわせ)に対応するために作ったが
	簡単には行かないことが分かった．（アライメント8でも，DWORDが2個
	並ぶと，8バイトしか使わない．(隙間ができない)．	*/
const int STRUCTALIGN = 4;
const int VECTORALIGN = 1;
inline size_t AlignSize(size_t sz, size_t align){
	size_t asz = (sz/align)*align + (sz%align ? align : 0);
	return asz;
}

//----------------------------------------------------------------------------
//	FITypeDesc::Field
FITypeDesc::Field::~Field(){
	if (type){
		UTRef<FITypeDesc> t = type;
		type = NULL;
	}
}

bool FITypeDesc::Field::Read(char*& to, size_t align, FIDocNodeBase* node, FIIterator& ctx){
	if (bReference) assert(0);		//	reference は未サポート．必要になったら作る．
	if (bVector){			//	FIVVector だったとき．FIVVector に読み出す．
		if (lengthFieldSize){	//	vectorの長さを先行フィールドで指定
			char* lf = to + lengthFieldOffset;
			if (lengthFieldSize == 1){
				length = *(unsigned char *)lf;
			}else if (lengthFieldSize == 2){
				length = *(unsigned short *)lf;
			}else if (lengthFieldSize == 4){
				length = *(unsigned long *)lf;
			}
		}else{
			length = -1;	//	長さ不明
		}
		FIVVectorBase* v = (FIVVectorBase*)to;
		to += AlignSize(Size(), align);
		if (length>0){
			v->VResize(length);
			char* vTo = v->VBegin();
			for(size_t i=0; i<v->VSize(); ++i){
				if (!type->Read(vTo, VECTORALIGN, node, ctx)) return false;
			}
		}else{
			for(int i=0;; ++i){
				v->VPushBack();
				char* to = v->VBegin() + i*v->VItemSize();
				if (!type->Read(to, VECTORALIGN, node, ctx)){
					v->VPopBack();
					break;
				}
			}
		}
	}else{	//	普通の型
		for(int i=0; i<length; ++i){
			//	DSTR << "Type:" << name << "\t";
			if (!type->Read(to, align, node, ctx)) return false;
		}
	}
	return true;
}
void FITypeDesc::Field::Write(FIDocNodeBase* node, FIIterator& ctx, const char*& from){
	if (bReference) assert(0);		//	reference は未サポート．必要になったら作る．
	if (bVector){			//	FIVVector だったとき．FIVVector に読み出す．
		FIVVectorBase* v = (FIVVectorBase*)from;
		from += Size();
		const char* vFrom = v->VBegin();
		for(size_t i=0; i<v->VSize(); ++i){
			type->Write(node, ctx, vFrom, name);
		}
	}else{
		for(int i=0; i<length; ++i){
			type->Write(node, ctx, from, name);
		}
	}
}

size_t FITypeDesc::Field::Size(){
	size_t sz = 0;
	if (bVector){
		sz = sizeof(FIVVector<int>);
	}else if (bReference){
		sz = sizeof(UTRef<FITypeDesc::Field>);
	}else if (type){
		sz = type->Size();
	}
	return sz;
}
void FITypeDesc::Field::Print(std::ostream& os) const{
	int w = os.width();
	os.width(0);
	os << UTPadding(w) << name.c_str() << " = ";
	if (bVector){
		os << "Vector<";
		if (bReference){
			os << "UTRef<" << type->GetTypeName().c_str() << ">";
		}else{
			os << type->GetTypeName().c_str();
		}
		os << ">";
		if (length>1) os << " [" << length << "]";
		os << " " << sizeof(FIVVector<int>) * length;
	}else if (bReference){
		os << "UTRef<" << type->GetTypeName().c_str() << ">";
		if (length>1) os << " [" << length << "]";
		os << " " << sizeof(UTRef<FITypeDesc::Field>) * length;
	}else{
		os.width(w);
		if (type) type->Print(os);
		else os << "(null)";
		if (length>1) os << " [" << length << "]";
		os.width(0);
	}
	os << std::endl;
	os.width(w);
}

//----------------------------------------------------------------------------
//	FITypeDesc::Composit
int FITypeDesc::Composit::Size(FIString id){
	int rv = 0;
	for(iterator it = begin(); it != end(); ++it){
		if (id.length()==0 || id.compare(it->name)){
			rv += it->Size();
		}
	}
	return rv;
}
void FITypeDesc::Composit::Print(std::ostream& os) const{
	for(const_iterator it = begin(); it != end(); ++it){
		it->Print(os);
	}
}
bool FITypeDesc::Composit::Read(char*& to, size_t align, FIDocNodeBase* node, FIIterator& ctx){
	for(iterator it = begin(); it != end(); ++it){
		if (!it->Read(to, align, node, ctx)) return false;
	}
	return true;
}
void FITypeDesc::Composit::Write(FIDocNodeBase* node, FIIterator& ctx, const char*& from){
	for(iterator it = begin(); it != end(); ++it){
		it->Write(node, ctx, from);
	}
}


//----------------------------------------------------------------------------
//	FITypeDesc
int FITypeDesc::Read(void* c, FIDocNodeBase* node, FIString id){
	assert(composit.size());	//	ノードは必ず組み立て型でなければならない．
	assert(typeName.compare(node->GetType()) == 0);	//	型があっているか確認
	FIIterator pos;
	if (id.length()) pos = node->FindAttr(id);
	else pos = node->FirstAttr();
	char* ptr = (char*)c;
	composit.Read(ptr, STRUCTALIGN, node, pos);
	return ptr - (char*) c;
}
bool FITypeDesc::Read(char*& to, size_t align, FIDocNodeBase* node, FIIterator& it){
	if (composit.size()){
		FIDocNodeBase* attr = it.GetNode();
		if (attr) {
			FIIterator it2 = attr->FirstAttr();
			return composit.Read(to, STRUCTALIGN, attr, it2);
		}else{
			return composit.Read(to, STRUCTALIGN, node, it);
		}
	}else{
		if (!it.GetDataImp(to, size)) return false;
		to += AlignSize(size, align);
	}
	return true;
}
int FITypeDesc::Write(FIDocNodeBase* node, const void* from){
	assert(composit.size());	//	ノードは必ず組み立て型でなければならない．
	assert(typeName.compare(node->GetType()) == 0);	//	型があっているか確認
	FIIterator pos = node->FirstAttr();
	const char* ptr = (const char*)from;
	composit.Write(node, pos, ptr);
	return ptr - (const char*) from;
}
void FITypeDesc::Write(FIDocNodeBase* node, FIIterator& it, const char*& from, UTString id){
	if (composit.size()){
		FIDocNodeBase* attr = node->SetNewNode(id, typeName);
		if (attr) {
			FIIterator it2 = attr->FirstAttr();
			composit.Write(attr, it2, from);
		}else{
			composit.Write(node, it, from);
		}
	}else{
		it.SetDataImp(from, size, size, id, typeName);
		from += size;
	}
}

void FITypeDesc::Print(std::ostream& os) const{
	int w = os.width();
	os.width(0);
	os << typeName << " " << size;
	if (composit.size()){
		os << "{" << std::endl;
		os.width(w+2);
		composit.Print(os);
		os.width(0);
		os << UTPadding(w) << "}";
	}
	os.width(w);
}

//	以下，C++のクラス定義をパースし，TypeDescを作る部分
//	パーサにboost::spiritを利用
#define PDEBUG_EVAL(x)
static FITypeDesc* loadingDesc;
static FITypeDescDb* typeDb;
static void AddField(const char*, const char*){
	loadingDesc->GetComposit().push_back(FITypeDesc::Field());
	PDEBUG_EVAL( DSTR << std::endl << "F"; )
};
static void SetType(const char* s, const char* e){
	std::string tn = std::string(s,e);
	loadingDesc->GetComposit().back().type = typeDb->Find(tn);
	if (!loadingDesc->GetComposit().back().type){
		DSTR << "Error: Field type '" << tn <<"' is not defined." << std::endl;
		DSTR << "       in the definition of '" << loadingDesc->GetTypeName() <<"'." << std::endl;
	}
	PDEBUG_EVAL( DSTR << " t:" << tn; )
};
static void SetGuid(const char* s, const char* e){
	std::string guid = std::string(s,e);
	loadingDesc->guid = WBGuid(guid);
	PDEBUG_EVAL( DSTR << " g:" << loadingDesc->guid; )
};
static void SetId(const char* s, const char* e){
	std::string id = std::string(s,e);
	loadingDesc->GetComposit().back().name = id;
	PDEBUG_EVAL( DSTR << " i:" << id; )
};
static void SetVector(const char*, const char*){
	loadingDesc->GetComposit().back().bVector = true;
	PDEBUG_EVAL( DSTR << " v"; )
};
static void SetUTRef(const char*, const char*){
	loadingDesc->GetComposit().back().bReference = true;
	PDEBUG_EVAL( DSTR << " r"; )
};
static void SetSuffix(int l){
	loadingDesc->GetComposit().back().length = l;
	PDEBUG_EVAL( DSTR << " [" << l << "]"; )
};
static void SetVSuffix(const char* s, const char* e){
	std::string lenId(s,e);
	loadingDesc->GetComposit().back().lengthFieldName = lenId;
	int lenPos = 0;
	FITypeDesc::Composit::iterator it;
	for(it = loadingDesc->GetComposit().begin(); it<loadingDesc->GetComposit().end()-1; ++it){
		if (lenId.compare(it->name) == 0) break;
		lenPos += it->Size();
	}
	assert(it != loadingDesc->GetComposit().end()-1);
	loadingDesc->GetComposit().back().lengthFieldSize = it->Size();
	int curPos = lenPos;
	for(; it<loadingDesc->GetComposit().end()-1; ++it){
		curPos += it->Size();
	}
	loadingDesc->GetComposit().back().lengthFieldOffset = lenPos - curPos;
	PDEBUG_EVAL( DSTR << " [" << lenId << "]"; )
};
//	C++のクラス定義から，TypeDescを作る．
bool FITypeDesc::Init(const char* clsDef, FITypeDescDb& db){
	using namespace boost::spirit;

	//	アクションのために，グローバル変数に設定
	loadingDesc = this;
	typeDb = &db;
	//	最初のフィールドを足しておく．
	AddField(NULL,NULL);
	//	パーサの定義
	FIPhraseParser cls, guid, field, ctype, type, id, suffix;
	cls		=	'{' >> +(guid | field[&AddField]) >> '}';
	guid	=	str_p("GUID") >> "Guid" >> '(' >> ')' >> '{'
				>> "return" >> "WBGuid" >> '(' >> '"'
				>> (*~ch_p('"'))[&SetGuid] >> '"' >> ')' >> ';' >> '}';
	field	=	ctype >> id[&SetId] >> !suffix >> ';';
	ctype	=	str_p("FIVVector")[&SetVector] >> '<' >> type >> '>' | type;
	type	=	str_p("UTRef")[&SetUTRef] >> '<' >> id[&SetType] >> '>' | id[&SetType];
	id		=	lexeme_d[(alpha_p | '_') >> *(alnum_p|'_')];
	suffix	=	'[' >> int_p[&SetSuffix] >> ']' | str_p("VSIZE") >> '(' >> id[&SetVSuffix] >> ')';
	//	スキップパーサ(スペースとコメントを読み出すパーサ)の定義
	FISkipParser cmt;
	cmt		=	space_p
				|	"/*" >> *(~ch_p('*') | '*'>>~ch_p('/')) >> !ch_p('*') >> '/'
				|	"//" >> *~ch_p('\n') >> '\n';
	//	パース
	parse_info<> info = parse(clsDef, cls, cmt);
	//	最後に足したフィールドが余分なので，削除
	GetComposit().pop_back();
	//	グローバル変数をNULLに戻しておく
	loadingDesc = NULL;
	typeDb = NULL;
	//	サイズの計算
	size = GetComposit().Size();
	if (!info.full) return false;
	return true;
}

//----------------------------------------------------------------------------
//	FITypeDescDb
FITypeDescDb::ProtoDescs FITypeDescDb::protoDescs;

FITypeDescDb::~FITypeDescDb(){
	db.clear();
}

FITypeDesc* FITypeDescDb::Find(UTString tn){
	UTRef<FITypeDesc> key = new FITypeDesc;
	if (prefix.length() && tn.compare(0, prefix.length(), prefix)==0){
		tn = tn.substr(prefix.length());
	}
	key->typeName = tn;
	Db::iterator it = db.find(key);
	if (it != db.end()) return *it;
	return NULL;
}
void FITypeDescDb::SetPrefix(FIString p){
	prefix = p;
	typedef std::vector< UTRef< FITypeDesc > > Descs;
	Descs descs;
	for(Db::iterator it = db.begin(); it != db.end(); ++it){
		descs.push_back(*it);
	}
	db.clear();
	for(Descs::iterator it = descs.begin(); it != descs.end(); ++it){
		if (prefix.length() && (*it)->typeName.compare(0, prefix.length(), prefix) == 0){
			(*it)->typeName = (*it)->typeName.substr(prefix.length());
		}
	}
	db.insert(descs.begin(), descs.end());
}
void FITypeDescDb::Print(std::ostream& os) const{
	for(Db::const_iterator it = db.begin(); it != db.end(); ++it){
		(*it)->Print(os);
		os << std::endl;
	}
}
void FITypeDescDb::RegisterProto(FITypeDesc* n){
	protoDescs.push_back(ProtoDesc());
	protoDescs.back().fileType = prefix;
	protoDescs.back().desc = n;
}

}
