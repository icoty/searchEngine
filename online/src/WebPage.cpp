 ///
 /// @file    WebPage.cpp
 /// @author  yangyu/Icot(jobyang@163.com)
 /// @date    2017-03-29 19:38:20
 ///
#include "WebPage.hpp"

#include <sstream>
#include <queue>
#include <iostream>
using std::priority_queue;

namespace wd
{

cppjieba::Jieba jieba(DICT_PATH,HMM_PATH,
					  USER_DICT_PATH,IDF_PATH,STOP_WORD_PATH);

struct FreqCompare{
	bool operator()(const pair<string,int> &lhs,
					const pair<string,int> &rhs)
	{
		if(lhs.second<rhs.second)
			return true;
		else if(lhs.second==rhs.second && lhs.first > rhs.first)
			return true;
		else
			return false;
	}
};

WebPage::WebPage(string &doc,Configuration &config)
{
	processDoc(doc,config);
}

string WebPage::getAbstract(const vector<string> &queryWords)
{
	std::istringstream iss(_docContent);
	string line;
	int count=5;
	
	while(iss>>line && (count--))//以空格or \n ??
	{
		for(auto word: queryWords)
			if(line.find(word)!=string::npos)
			{	_docAbstract.append(line).append("\n");//'' or ""??
				std::cout<<_docAbstract<<std::endl;
				break;
			}
	}
	return _docAbstract;
}

void WebPage::processDoc(const string& doc,Configuration& config)
{
	int begin=doc.find("<docid>");
	int end=doc.find("</docid>");
	_docId=atoi(doc.substr(begin+7,end-begin-7).c_str());

	begin=doc.find("<title>");
	end=doc.find("</title>");
	_docTitle=doc.substr(begin+7,end-begin-7);

	begin=doc.find("<link>");
	end=doc.find("</link>");
	_docUrl=doc.substr(begin+6,end-begin-6);

	begin=doc.find("<content>");
	end=doc.find("</content>");
	_docContent=doc.substr(begin+9,end-begin-9);

	vector<string> jiebaWords;
	jieba.Cut(doc,jiebaWords, true);

	calcTopK(jiebaWords,TOPK_NUMBER,config.getStopWordsList());
}

void WebPage::calcTopK(vector<string>& jiebaWords,int k,set<string>& stopWordList)
{
	for(size_t idx=0;idx!=jiebaWords.size();++idx)	
	{
		auto it=stopWordList.find(jiebaWords[idx]);
		if(it!=stopWordList.end())
			continue;
		else
		{
			auto im=_wordsFreq.find(jiebaWords[idx]);
			if(im!=_wordsFreq.end())
				++_wordsFreq[jiebaWords[idx]];
			else
				_wordsFreq[jiebaWords[idx]]=1;
		}
	}
	
	priority_queue<pair<string,int>,vector<pair<string,int>>,
		FreqCompare> _freqSort(_wordsFreq.begin(),_wordsFreq.end());
	while((!_freqSort.empty()) && (_topWords.size() < static_cast<size_t>(k)))
	{
		string top=_freqSort.top().first;
		_freqSort.pop();
		if(top.size()==1 && (top[0]==10||top[0]==13))
			continue;
		_topWords.push_back(top);
	}
}

int WebPage::getDocId()
{
	return _docId;
}

string WebPage::getUrl()
{
	return _docUrl;
}

string WebPage::getTitle()
{
	return _docTitle;
}

std::map<string,int> & WebPage::getWordsFreq()
{
	return _wordsFreq;
}

bool operator==(const WebPage & lhs,const WebPage & rhs)
{
	set<string> mix;
	mix.insert(lhs._topWords.begin(),lhs._topWords.end());
	mix.insert(rhs._topWords.begin(),rhs._topWords.end());
	return ((static_cast<float>(mix.size())/(lhs._topWords.size()+rhs._topWords.size()))<0.7);
}

bool operator < (const WebPage & lhs,const WebPage & rhs)
{
	return lhs._docId<rhs._docId;
}

}
