#include "stdafx.h"
#include "FIModel3Tree.h"
using namespace std;
//---------------------------------------------------------------------------
void Print(const std::vector<FIModel3Attribute>& attrs, int indent/*= 0*/)
{
	for (size_t i= 0; i<attrs.size(); ++i){
		cout << string(indent*2, ' ') << attrs[i].key;
		if (attrs[i].value.node){
			cout << " {" << endl;
			Print(attrs[i].value.node->attrs, indent+1);
			cout << string(indent*2, ' ') << "}" << endl;
		}else{
			cout << " = \"" << attrs[i].value.str << "\"" << endl;
		}
	}
}

//---------------------------------------------------------------------------
