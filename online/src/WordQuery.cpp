 ///
 /// @file    WordQuery.cpp
 /// @author  yangyu/Icot(jobyang@163.com)
 /// @date    2017-03-31 02:54:05
 ///
#include "WordQuery.hpp"
#include "FileName.hpp"
#include "Log4func.hpp"
#include <math.h>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>
using std::istringstream;
using std::ifstream;
using std::istringstream;
using std::vector;
using std::pair;

cppjieba::Jieba jieba(DICT_PATH,HMM_PATH,
				USER_DICT_PATH,IDF_PATH,STOP_WORD_PATH);

namespace wd
{

struct SimilarityCompare{
	SimilarityCompare(vector<double> & base)
	:_base(base)
	{}

	bool operator()(const pair<int,vector<double>> & lhs,
					const pair<int,vector<double>> & rhs)
	{
		double lbase=0.0;
		double rbase=0.0;	
		double labs=0.0;
		double rabs=0.0;
		double base_abs=0.0;
		for(auto &ele: _base)
		{
			lbase += _base[ele] * lhs.second[ele];
			rbase += _base[ele] * rhs.second[ele];
			
			labs  += pow(lhs.second[ele],2);
			rabs  += pow(rhs.second[ele],2);
			
			base_abs += pow(_base[ele],2);
		}

		return (lbase/sqrt(labs * base_abs) > rbase/sqrt(rabs * base_abs));
	}
	
	vector<double> & _base;
};


WordQuery::WordQuery(Configuration & conf)
:_conf(conf)
{	loadLibrary();	}

string WordQuery::doQuery(const string & word)
{
	if(word.size()==0)
		return returnNoAnswer();
	
	vector<string> jiebawords;//未去重
	jieba.Cut(word,jiebawords, true);
	map<string,int> mapFreq;//统计词频
	for(auto & word: jiebawords)
		++mapFreq[word];

	for(auto &ele:mapFreq)
	{
		auto it=_invertIndexTable.find(ele.first);
		if(it==_invertIndexTable.end())
		{
			LogInfo("no answer!");
			return returnNoAnswer();
		}
	}
	
	vector<double> weightVec=
		getQueryWordsWeightVector(mapFreq);//计算查询文章的权重值并返回存储
	SimilarityCompare similarityCmp(weightVec);//初始化比较类
	
	vector<pair<int,vector<double>>> ret;
	if(executeQuery(mapFreq,ret))
	{
		stable_sort(ret.begin(), ret.end(), similarityCmp);		
		vector<int> docId;//存放排好序的文章id
		for(auto item: ret)
			docId.push_back(item.first);

		return createJson(docId,jiebawords);
	}else{
		return returnNoAnswer();
	}
}

void WordQuery::loadLibrary()
{
	std::cout<<_conf.getConfigMap()[NEWOFFSET]<<std::endl;
	std::cout<<_conf.getConfigMap()[NEWRIPEPAGE]<<std::endl;
	std::cout<<_conf.getConfigMap()[INVERTINDEX]<<std::endl;
	ifstream offset(_conf.getConfigMap()[NEWOFFSET].c_str());
	ifstream ripepage(_conf.getConfigMap()[NEWRIPEPAGE].c_str());
	ifstream invertindex(_conf.getConfigMap()[INVERTINDEX].c_str());
	if(!offset.good()||!ripepage.good()||!invertindex.good())
	{
		LogError("offset or ripepage ofs error!");
		offset.close();
		ripepage.close();
		invertindex.close();
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
		//ripepage.seekg(pos,std::ios::beg);
		ripepage.read(&*doc.begin(),length);
		_pageLib[id]=WebPage(doc,_conf);
	//	WebPage wp(doc,_conf);
	//	_pageLib.insert(std::make_pair(id,wp));
	}
	
	while(getline(invertindex,line))
	{
		std::istringstream iss(line);
		string word;
		int weight;
		iss>>word;
		while(iss>>id>>weight)
			_invertIndexTable[word].insert(std::make_pair(id,weight));
	}
	invertindex.close();
}

vector<double> WordQuery::getQueryWordsWeightVector(
				map<string,int> & wordFreq)
{
	vector<double> weightVec;
	double sumweight=0;
	for(auto &ele:wordFreq)
	{
		double w=ele.second*log(static_cast<double>(_invertIndexTable[ele.first].size())/
					(_offsetLib.size()+1))/log(2);
		sumweight+=pow(w,2);
		weightVec.push_back(w);
	}
	for(auto &ele:weightVec)
		ele/=sumweight;
	return weightVec;
}

bool WordQuery::executeQuery(const map<string,int> & wordFreq,
				vector<pair<int,vector<double> > > & resultVec)
{
	map<int,set<string>> tmp;//id与w对应id存放的word对
	for(auto &word: wordFreq)
		for(auto &ele: _invertIndexTable[word.first])
			tmp[ele.first].insert(word.first);
	
	for(auto &ele : tmp)
		if(ele.second.size()==wordFreq.size())
		{
			vector<double> tv;
			for(auto & wf: wordFreq)
			{
				for(auto & in:_invertIndexTable[wf.first])
					if(in.first==ele.first)
					{	tv.push_back(in.second); break;	}
			}
			resultVec.push_back(std::make_pair(ele.first,tv));
		}
	return resultVec.size();
}

string WordQuery::createJson(vector<int> & docIdVec,
				const vector<string> & queryWords)
{	
	Json::Value root;
	Json::Value arr;
	int count=10;
	for(auto id : docIdVec)
	{
		cout<<id<<endl;
		string abstract = _pageLib[id].getAbstract(queryWords);
		string title = _pageLib[id].getTitle();
		string url = _pageLib[id].getUrl();

		Json::Value elem;
		elem["title"]=title;
		elem["abstract"]=abstract;
		elem["url"]=url;
		arr.append(elem);
		//root["content"]=elem;
		
		if(!count--)
			break;
	}
	root["Result"]=arr;
	Json::StyledWriter writer;
	return writer.write(root);
}

string WordQuery::returnNoAnswer()
{	
	Json::Value root;
	Json::Value arr;

	Json::Value elem;
	elem["title"]="404,not found!";
	elem["abstract"]="not find relative content!";
	elem["url"]="not find!";
	arr.append(elem);
	root["Error"]=elem;
	
	Json::StyledWriter writer;
	return writer.write(root);
}

}
