 ///
 /// @file    PageLibPreProcessor.h
 /// @author  yangyu/Icot(jobyang@163.com)
 /// @date    2017-03-30 00:44:06
 ///
#ifndef __WD_PAGELIBPREPROCESSOR_H__
#define __WD_PAGELIBPREPROCESSOR_H__

#include "FileName.h"
#include "WebPage.h"
#include "Configuration.h"
#include <tr1/unordered_map>
#include <string>
#include <vector>
#include <map>
#include <set>
using std::vector;
using std::map;
using std::pair;
using std::string;
using std::set;
using namespace std::tr1;

namespace wd
{
	
class PageLibPreProcessor{
public:
	PageLibPreProcessor(Configuration & conf);
	void doProcess();
private:
	void readInfoFromFile();
	void dealRepeatePages();
	void buildInvertIndexTable();
	void storeOnDisk();
private:
	Configuration & _conf;
	vector<WebPage> _PageLib;
	unordered_map<int,pair<int,int>>			   _offsetLib;
	unordered_map<string,vector<pair<int,double>>> _invertIndexTable;
	//vector可修改，set内的内容不可更改
};

}
#endif
