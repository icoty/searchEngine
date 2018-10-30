 ///
 /// @file    Configuration.hpp
 /// @author  yangyu/Icot(jobyang@163.com)
 /// @date    2017-03-28 05:58:19
 ///
#ifndef __WD_CONFIGURATION_H__
#define __WD_CONFIGURATION_H__

#include "FileName.hpp"
#include <map>
#include <set>
#include <string>
#include <iostream>
using std::map;
using std::set;
using std::string;

namespace wd
{

class Configuration{
public:
	Configuration(const string& filepath);
	void init();
	map<string,string> & getConfigMap();
	set<string> & getStopWordsList();
	void debug();
private:
	string				_filepath;
	map<string,string>	_configMap;
	set<string>			_stopWordsList;
};

}
#endif
