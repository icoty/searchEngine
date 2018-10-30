 ///
 /// @file    PageLib.h
 /// @author  yangyu/Icot(jobyang@163.com)
 /// @date    2017-03-28 05:50:52
 ///
#ifndef __WD_PAGELIB_H__
#define __WD_PAGELIB_H__

#include "tinyxml2.h"
#include "DirScanner.h"
#include "Configuration.h"

#include <string>
#include <vector>
#include <map>
#include <memory>
using std::string;
using std::vector;
using std::map;
using std::shared_ptr;
using namespace tinyxml2;

namespace wd
{

struct rssItem{
	string _url;
	string _title;//title和link不含html标签
	string _content;
};

class PageLib{
public:
	PageLib(Configuration& conf,DirScanner& dirScanner);
	void create();
	void store();
private:
	void makePages();
	void parseRss(const string& filepath);

private:
	Configuration &				_conf;
	DirScanner &				_dirScanner;
	vector<shared_ptr<rssItem>> _vecRssItem;//处理xml文件后存入此中间变量
	vector<string>				_vecFormatFiles;//以item即文章为单位存储，而非以xml文件为单位
	//Map<int,std::pair<int,int>>	_offsetLib;
};

}
#endif
