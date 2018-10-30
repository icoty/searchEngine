 ///
 /// @file    PageLibPreProcessor.cpp
 /// @author  yangyu/Icot(jobyang@163.com)
 /// @date    2017-03-30 00:55:53
 ///
#include "PageLibPreProcessor.h"
#include "Log4func.h"
#include "FileName.h"

#include <math.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
using std::ifstream;
using std::ofstream;
using std::istringstream;


namespace wd
{

PageLibPreProcessor::PageLibPreProcessor(Configuration & conf)
:_conf(conf)
{}

void PageLibPreProcessor::doProcess()
{
	readInfoFromFile();
	dealRepeatePages();
	buildInvertIndexTable();
	storeOnDisk();
}

void PageLibPreProcessor::readInfoFromFile()
{
	ifstream offset(_conf.getConfigMap()[OFFSET].c_str());
	ifstream wripepage(_conf.getConfigMap()[WRIPEPAGE].c_str());
	if(!offset.good()||!wripepage.good())
	{
		LogError("offset or wripepage open error!");
		offset.close();
		wripepage.close();
		return;
	}

	string line;
	int id,pos,length;
	while(getline(offset,line))
	{
		istringstream iss(line);
		iss>>id>>pos>>length;
		_offsetLib[id]=std::make_pair(pos,length);
		
		string doc(length,' ');
		//wripepage.seekg(pos,std::ios::beg);
		wripepage.read(&*doc.begin(),length);//string转char读入
		
		WebPage webpage(doc,_conf);
		_PageLib.push_back(webpage);
	}
}

void PageLibPreProcessor::dealRepeatePages()
{
	for(size_t i=0;i!=_PageLib.size()-1;++i)
		for(size_t j=i+1;j!=_PageLib.size();++j)
			if(_PageLib[i]==_PageLib[j])
			{
				_PageLib[j]=_PageLib[_PageLib.size()-1];
				_PageLib.pop_back();
				--j;
			}
}

void PageLibPreProcessor::buildInvertIndexTable()
{
	for(auto & web: _PageLib)
		for(auto & freq: web.getWordsFreq())
			_invertIndexTable[freq.first].push_back(std::make_pair(web.getDocId(),freq.second));
	
	map<int,double> series;
	for(auto &index: _invertIndexTable)
		for(auto &ele: index.second)
		{
			double w=ele.second*log(static_cast<double>(_PageLib.size())/
					 (index.second.size()+1))/log(2);
			series[ele.first]+=pow(w,2);
			ele.second = w;
		}

	for(auto &index: _invertIndexTable)
		for(auto &ele: index.second)
			ele.second/=sqrt(series[ele.first]);
}

void PageLibPreProcessor::storeOnDisk()
{
	sort(_PageLib.begin(),_PageLib.end());
	ofstream newoffset(_conf.getConfigMap()[NEWOFFSET].c_str());
	ofstream newwripepage(_conf.getConfigMap()[NEWWRIPEPAGE].c_str());
	if(!newoffset.good()||!newwripepage.good())
	{
		LogError("newoffset or newwripepage ofs error!");
		newwripepage.close();
		newoffset.close();
		return;
	}

	for(auto & web: _PageLib)
	{
		int id=web.getDocId();
		int length=web.getDoc().size();
		int offset=newwripepage.tellp();
		
		newwripepage<<web.getDoc();
		newoffset<<id<<'\t'<<offset<<'\t'<<length<<endl;
	}
	newoffset.close();
	newwripepage.close();

	ofstream invertindex(_conf.getConfigMap()[INVERTINDEX].c_str());
	if(!invertindex.good())
	{
		LogError("invertindex ofs error!");
		invertindex.close();
		return;
	}
	
	for(auto &index: _invertIndexTable)
	{
		invertindex<<index.first<<'\t';
		for(auto &ele: index.second)
			invertindex<<ele.first<<'\t'<<ele.second<<'\t';
		
		invertindex<<endl;
	}
}

}
