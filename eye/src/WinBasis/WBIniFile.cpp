// IniFile Loader
// Contributed from Gimite

#include <fstream>
#include "WBIniFile.h"
using namespace std;

std::string WBIniFile::fileName;

//これを最初に呼んでファイル名をセット。
void WBIniFile::setFileName(const std::string& fn)
{
	//絶対パスに変換する。カレントディレクトリがC:\とかの場合に対応してないのと、
	//fnに絶対パスが入ってくるケースを想定してないのは手抜き。
	char currentDir[MAX_PATH+1];
	GetCurrentDirectory(MAX_PATH, currentDir);
	fileName= string(currentDir)+"\\"+fn;
	ifstream fs(fileName.c_str());
	if (!fs) onError("INIファイルが有りません。\n"+fileName);
}

//セクションとキーを指定して、値を読み込む。
std::string WBIniFile::getString(const std::string& section, const std::string& key)
{
	char buf[1000];
	GetPrivateProfileString(section.c_str(), key.c_str(), "", buf, 1000, fileName.c_str());
	return buf;
}


//セクションとキーを指定して、値を読み込む。
//存在しない時はdefValueを返す。
std::string WBIniFile::getString(const std::string& section, const std::string& key, const std::string& defValue)
{
	string str= getString(section, key);
	if (str=="") return defValue;
	else         return str;
}

void WBIniFile::setString(const std::string& section, const std::string& key, const std::string& value)
{
	BOOL rl = WritePrivateProfileString(
		section.c_str(),  // セクション名
		key.c_str(),  // キー名
		value.c_str(),   // 追加するべき文字列
		fileName.c_str()  // .ini ファイル
		);

	if(rl == FALSE) onError("INIファイルへの書き込みに失敗しました。\n"
				+ fileName
				+ "\nセクション:" + section
				+ "\nキー:" + key
				+ "\n値:" + value);
}


//エラーの時の処理。
void WBIniFile::onError(const std::string& message)
{
	MessageBox(NULL, message.c_str(), "IniFileエラー", 0);
	abort();
}
