 ///
 /// @file    WepPage.hpp
 /// @author  yangyu/Icot(jobyang@163.com)
 /// @date    2017-03-29 08:58:18
 ///
#ifndef __WD_WEBPAGE_H__
#define __WD_WEBPAGE_H__

#include "Configuration.hpp"
#include "cppjieba/Jieba.hpp"

#include <string>
#include <map>
#include <set>
#include <vector>
using std::map;
using std::vector;
using std::set;
using std::string;
using std::pair;

const char* const DICT_PATH="/home/yangyu/CPP/searchEngine/online/data/dict/jieba.dict.utf8";
const char* const HMM_PATH ="/home/yangyu/CPP/searchEngine/online/data/dict/hmm_model.utf8";
const char* const USER_DICT_PATH ="/home/yangyu/CPP/searchEngine/online/data/dict/user.dict.utf8";
const char* const IDF_PATH ="/home/yangyu/CPP/searchEngine/online/data/dict/idf.utf8";
const char* const STOP_WORD_PATH ="/home/yangyu/CPP/searchEngine/online/data/dict/stop_words.utf8";

namespace wd
{

class WebPage{
public:
	WebPage(){}
	WebPage(string& doc,Configuration & config);
	int getDocId();
	string getUrl();
	string getTitle();
	string getAbstract(const vector<string> &queryWords);
	map<string,int> & getWordsFreq();
friend bool operator == (const WebPage & lhs,
		                 const WebPage & rhs);
friend bool operator < (const WebPage & lhs,
  		                const WebPage & rhs);
private:
	void processDoc(const string& doc,
					Configuration& config);
	void calcTopK(vector<string>& jiebaWords,
				  int k,set<string>& stopWordList);
private:
	const static int TOPK_NUMBER = 20;
	int    _docId;
	string _docTitle;
	string _docUrl;
	string _docContent;
	string _docAbstract;
	vector<string>  _topWords;
	map<string,int> _wordsFreq;
};

}
#endif
