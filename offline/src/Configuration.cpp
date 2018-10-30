 ///
 /// @file    Configuration.cpp
 /// @author  yangyu/Icot(jobyang@163.com)
 /// @date    2017-03-28 06:03:46
 ///
#include "Configuration.h"
#include "Log4func.h"

#include <fstream>
#include <sstream>
#include <iostream>

namespace wd
{

Configuration::Configuration(const string& filepath)
:_filepath(filepath)
{}

void Configuration::debug()
{
	for(auto & ele:_configMap)
		std::cout << ele.first << '\t' << ele.second << std::endl;
#if 0
	auto it=_stopWordsList;
		cout<<*it<<endl;
#endif
}

void Configuration::init()
{
	std::ifstream _in(_filepath.c_str());
	if(!_in.good()){
		LogError("_in open error!");
		_in.close();
		return;
	}

	string line;
	while(getline(_in,line))
	{
		std::istringstream iss(line);
		std::string key;
		std::string value;
		iss >> key >> value;
		_configMap.insert(make_pair(key,value));
	}
	_in.close();

	std::ifstream ifs(_configMap[STOPWORDS].c_str());
	if(!ifs.good()){
		LogError("stopwords open error!");
		ifs.close();
		return;
	}
	
	while(ifs>>line){
		_stopWordsList.insert(line);
	}
	ifs.close();
}

map<string,string> & Configuration::getConfigMap()
{
	return _configMap;
}

set<string> & Configuration::getStopWordsList()
{
	return _stopWordsList;
}

}
