// IniFile Loader
// Contributed from Gimite

#include <fstream>
#include "WBIniFile.h"
using namespace std;

std::string WBIniFile::fileName;

//������ŏ��ɌĂ�Ńt�@�C�������Z�b�g�B
void WBIniFile::setFileName(const std::string& fn)
{
	//��΃p�X�ɕϊ�����B�J�����g�f�B���N�g����C:\�Ƃ��̏ꍇ�ɑΉ����ĂȂ��̂ƁA
	//fn�ɐ�΃p�X�������Ă���P�[�X��z�肵�ĂȂ��͎̂蔲���B
	char currentDir[MAX_PATH+1];
	GetCurrentDirectory(MAX_PATH, currentDir);
	fileName= string(currentDir)+"\\"+fn;
	ifstream fs(fileName.c_str());
	if (!fs) onError("INI�t�@�C�����L��܂���B\n"+fileName);
}

//�Z�N�V�����ƃL�[���w�肵�āA�l��ǂݍ��ށB
std::string WBIniFile::getString(const std::string& section, const std::string& key)
{
	char buf[1000];
	GetPrivateProfileString(section.c_str(), key.c_str(), "", buf, 1000, fileName.c_str());
	return buf;
}


//�Z�N�V�����ƃL�[���w�肵�āA�l��ǂݍ��ށB
//���݂��Ȃ�����defValue��Ԃ��B
std::string WBIniFile::getString(const std::string& section, const std::string& key, const std::string& defValue)
{
	string str= getString(section, key);
	if (str=="") return defValue;
	else         return str;
}

void WBIniFile::setString(const std::string& section, const std::string& key, const std::string& value)
{
	BOOL rl = WritePrivateProfileString(
		section.c_str(),  // �Z�N�V������
		key.c_str(),  // �L�[��
		value.c_str(),   // �ǉ�����ׂ�������
		fileName.c_str()  // .ini �t�@�C��
		);

	if(rl == FALSE) onError("INI�t�@�C���ւ̏������݂Ɏ��s���܂����B\n"
				+ fileName
				+ "\n�Z�N�V����:" + section
				+ "\n�L�[:" + key
				+ "\n�l:" + value);
}


//�G���[�̎��̏����B
void WBIniFile::onError(const std::string& message)
{
	MessageBox(NULL, message.c_str(), "IniFile�G���[", 0);
	abort();
}
