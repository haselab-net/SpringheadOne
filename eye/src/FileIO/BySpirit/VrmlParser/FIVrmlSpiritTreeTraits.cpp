#include "stdafx.h"

#include "../../FITreeConverter.h"// for assert()
#include "FIVrmlParser.h"
#include "FIVrmlSpiritTreeTraits.h"
using namespace std;
using namespace boost::spirit;
//---------------------------------------------------------------------------
bool FIVrmlSpiritTreeTraits::IsString(const TInValue& value)
{
	return value.n<0 || value.GetPointee()->children.empty();
		//�q���Ȃ��m�[�h�͒l�m�[�h�Ƃ��Ĉ���
}

//---------------------------------------------------------------------------
FIVrmlSpiritTreeTraits::TString 
	FIVrmlSpiritTreeTraits::GetString(const TInValue& value)
{
	if (value.n>=0){
		TNodeIterator nit= value.GetPointee();
		return Trim(std::string(nit->value.begin(), nit->value.end()));
	}else{
		return FIVrmlParser::GetNodeName(value.parent->value.id().to_long());
			//����value.n�́A�m�[�h��type��\���B
	}
}

//---------------------------------------------------------------------------
std::string FIVrmlSpiritTreeTraits::Trim(const std::string& s)
{
	std::string s2= s;
	int headPos= 0;
	while (headPos<int(s2.length()) && isspace(s2[headPos])) ++headPos;
	int tailPos= int(s2.length())-1;
	while (tailPos>=0 && isspace(s2[tailPos])) --tailPos;
	return s2.substr(headPos, tailPos+1-headPos);
}

//---------------------------------------------------------------------------
