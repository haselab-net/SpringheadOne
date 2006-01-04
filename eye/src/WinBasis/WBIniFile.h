// IniFile Loader
// Contributed from Gimite

#ifndef WBINIFILE_H
#define WBINIFILE_H

#include <windows.h>
#include <string>
#include <boost/lexical_cast.hpp>


//INI�t�@�C������ݒ��ǂݍ��ށB
class WBIniFile
{
public:
	//������ŏ��ɌĂ�Ńt�@�C�������Z�b�g�B
	static void setFileName(const std::string& fn);
	//�Z�N�V�����ƃL�[���w�肵�āA�l��ǂݍ��ށB
	static std::string getString(const std::string& section, const std::string& key);
	//�Z�N�V�����ƃL�[���w�肵�āA�l��ǂݍ��ށB
	//���݂��Ȃ�����defValue��Ԃ��B
	static std::string getString(const std::string& section, const std::string& key, const std::string& defValue);
	//�Z�N�V�����ƃL�[���w�肵�āA��������������ށB
	static void setString(const std::string& section, const std::string& key, const std::string& value);

	//�Z�N�V�����ƃL�[���w�肵�āA�l��ǂݍ��݁A�w��̌^�ɕϊ��B
	//��: int markers= WBIniFile::getValue<int>("hoge", "markers");
	template <class T>
	static T getValue(const std::string& section, const std::string& key){
		//���̂�lexical_cast����Vec3d���ǂ߂Ȃ��c�B
		std::istringstream ss(getString(section, key));
		T value;
		if (ss >> value){
			return value;
		}else{
			onError("INI�t�@�C���̒l���ςł��B\n"+fileName+"\n�Z�N�V����:"+section+" �L�[:"+key);
			return T();
		}
	}
	//�Z�N�V�����ƃL�[���w�肵�āA�l��ǂݍ��݁A�w��̌^�ɕϊ��B
	//���݂��Ȃ�����defValue��Ԃ��B
	//��: int markers= WBIniFile::getValue<int>("hoge", "markers", 3);
	template <class T>
	static T getValue(const std::string& section, const std::string& key, const T& defValue){
		//���̂�lexical_cast����Vec3d���ǂ߂Ȃ��c�B
		std::string str= getString(section, key);
		if (str=="") return defValue;
		std::istringstream ss(str);
		T value;
		if (ss >> value){
			return value;
		}else{
			onError("INI�t�@�C���̒l���ςł��B\n"+fileName+"\n�Z�N�V����:"+section+" �L�[:"+key);
			return T();
		}
	}

	//�Z�N�V�����ƃL�[���w�肵�āA�l���������ށB
	template <class T>
		static void setValue(const std::string& section, const std::string& key, const T& value){
			std::ostringstream ss;
			ss << value;
			setString(section, key, ss.str());
		}

private:
	static std::string fileName;

	//�G���[�̎��̏����B
	static void onError(const std::string& message);

};

#endif

