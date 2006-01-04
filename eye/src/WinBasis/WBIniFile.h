// IniFile Loader
// Contributed from Gimite

#ifndef WBINIFILE_H
#define WBINIFILE_H

#include <windows.h>
#include <string>
#include <boost/lexical_cast.hpp>


//INIファイルから設定を読み込む。
class WBIniFile
{
public:
	//これを最初に呼んでファイル名をセット。
	static void setFileName(const std::string& fn);
	//セクションとキーを指定して、値を読み込む。
	static std::string getString(const std::string& section, const std::string& key);
	//セクションとキーを指定して、値を読み込む。
	//存在しない時はdefValueを返す。
	static std::string getString(const std::string& section, const std::string& key, const std::string& defValue);
	//セクションとキーを指定して、文字列を書き込む。
	static void setString(const std::string& section, const std::string& key, const std::string& value);

	//セクションとキーを指定して、値を読み込み、指定の型に変換。
	//例: int markers= WBIniFile::getValue<int>("hoge", "markers");
	template <class T>
	static T getValue(const std::string& section, const std::string& key){
		//何故かlexical_castだとVec3dが読めない…。
		std::istringstream ss(getString(section, key));
		T value;
		if (ss >> value){
			return value;
		}else{
			onError("INIファイルの値が変です。\n"+fileName+"\nセクション:"+section+" キー:"+key);
			return T();
		}
	}
	//セクションとキーを指定して、値を読み込み、指定の型に変換。
	//存在しない時はdefValueを返す。
	//例: int markers= WBIniFile::getValue<int>("hoge", "markers", 3);
	template <class T>
	static T getValue(const std::string& section, const std::string& key, const T& defValue){
		//何故かlexical_castだとVec3dが読めない…。
		std::string str= getString(section, key);
		if (str=="") return defValue;
		std::istringstream ss(str);
		T value;
		if (ss >> value){
			return value;
		}else{
			onError("INIファイルの値が変です。\n"+fileName+"\nセクション:"+section+" キー:"+key);
			return T();
		}
	}

	//セクションとキーを指定して、値を書き込む。
	template <class T>
		static void setValue(const std::string& section, const std::string& key, const T& value){
			std::ostringstream ss;
			ss << value;
			setString(section, key, ss.str());
		}

private:
	static std::string fileName;

	//エラーの時の処理。
	static void onError(const std::string& message);

};

#endif

