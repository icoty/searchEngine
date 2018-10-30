 ///
 /// @file    PageLib.cpp
 /// @author  yangyu/Icot(jobyang@163.com)
 /// @date    2017-03-28 23:52:26
 ///
#include "PageLib.h"
#include "FileName.h"
#include "Log4func.h"

//#include <boost/regex.hpp>
#include <regex>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string.h>
using std::ostringstream;
using std::ofstream;
//using std::regex_constants;
using std::regex;
using std::string;
using std::regex_replace;

namespace wd
{

PageLib::PageLib(Configuration& conf,DirScanner& dirScanner)
:_conf(conf)
,_dirScanner(dirScanner)
	{}

void PageLib::create()
{	
	for(size_t idx=0;idx!=_dirScanner.getVecAbsPath().size();++idx)
		parseRss(_dirScanner.getVecAbsPath()[idx]);
	makePages();//函数调用
}

void PageLib::parseRss(const string& filepath)
{
	XMLDocument doc;
	if( XML_SUCCESS != doc.LoadFile(filepath.c_str()))
	{
		LogError("doc.LoadFile failed!");
		return;
	}
	XMLElement *root=doc.FirstChildElement();
	XMLElement *chanel=root->FirstChildElement("channel");
	XMLElement *item=chanel->FirstChildElement("item");//xml如果没有channel,此句产生段错误
	
	for(int i=0;item;item=item->NextSiblingElement(),i++)//指向兄弟结点
	{
		shared_ptr<rssItem> rssItemPtr(new rssItem);
		const char* itemTitle=item->FirstChildElement("title")->GetText();
		const char * itemUrl = item->FirstChildElement("link")->GetText();
		const char* itemDescription=item->FirstChildElement("description")->GetText();
		const char* content;
		XMLElement * contentEncoded = item->FirstChildElement("content:encoded");
		if(contentEncoded)
			content=contentEncoded->GetText();
		else
			content=itemDescription;
		string pattern="<.*?>";//正则表达式？？？？
		regex r("<.*?>");
		std::string scontent=regex_replace(string(content),r,string(""));
		
		rssItemPtr->_url= itemUrl ? itemUrl : string("");
		rssItemPtr->_title=itemTitle ? itemTitle : string("");
		rssItemPtr->_content=scontent;
		
		//std::cout<<rssItemPtr->_url<<endl;
		//std::cout<<rssItemPtr->_title<<std::endl;
		//std::cout<<rssItemPtr->_content<<std::endl;

		_vecRssItem.push_back(rssItemPtr);
	}
}

void PageLib::makePages()
{
	for(size_t idx = 0; idx != _vecRssItem.size(); ++idx)
	{
		std::ostringstream oss;
		oss	<< "<doc>\n"
			<< "  <docid>" << (idx + 1) << "</docid>\n"
			<< "  <title>" << _vecRssItem[idx]->_title << "</title>\n"
			<< "  <link>"  << _vecRssItem[idx]->_url << "</link>\n" 
			<< "  <content>" << _vecRssItem[idx]->_content << "</content>\n"
			<< "/<doc>\n";
		
		_vecFormatFiles.push_back(oss.str());
	}
}

#if 0
Map<int,std::pair<int,int>> &PageLib::getOffset()
{
	return _offsetLib;
}
#endif

void PageLib::store()
{
	ofstream off(_conf.getConfigMap()[OFFSET].c_str());
	ofstream ripage(_conf.getConfigMap()[WRIPEPAGE].c_str());
	if(!off.good()||!ripage.good())
	{
		LogError("offset or ripage open error!");
		off.close();
		ripage.close();
		return;
	}
	
	for(size_t idx=0;idx!=_vecFormatFiles.size();++idx)
	{
		int id=idx+1;
		int length=_vecFormatFiles[idx].size();
		std::ofstream::pos_type offset=ripage.tellp();

			//_offsetLib[id]=make_pair(length,offset);
		ripage<<_vecFormatFiles[idx];
		off<<id<<'\t'<<offset<<'\t'<<length<<'\n';
	}
	off.close();
	ripage.close();
}

}
