 ///
 /// @file    WepPage.h
 /// @author  yangyu/Icot(jobyang@163.com)
 /// @date    2017-03-29 08:58:18
 ///
#ifndef __WD_WEBPAGE_H__
#define __WD_WEBPAGE_H__

#include "Configuration.h"
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

const char* const DICT_PATH="/home/yangyu/CPP/searchEngine/offline/data/dict/jieba.dict.utf8";
const char* const HMM_PATH ="/home/yangyu/CPP/searchEngine/offline/data/dict/hmm_model.utf8";
const char* const USER_DICT_PATH ="/home/yangyu/CPP/searchEngine/offline/data/dict/user.dict.utf8";
const char* const IDF_PATH ="/home/yangyu/CPP/searchEngine/offline/data/dict/idf.utf8";
const char* const STOP_WORD_PATH ="/home/yangyu/CPP/searchEngine/offline/data/dict/stop_words.utf8";

namespace wd
{

class WebPage{
friend bool operator == (const WebPage & lhs,
		                 const WebPage & rhs);
friend bool operator < (const WebPage & lhs,
  		                const WebPage & rhs);
public:
	WebPage(string& doc,Configuration& config);
	int getDocId();
	string getDoc();
	map<string,int> & getWordsFreq();
private:
	void processDoc(const string& doc,
					Configuration& config);
	void calcTopK(vector<string>& jiebaWords,
				  int k,set<string>& stopWordList);
private:
	const static int TOPK_NUMBER = 20;
	string _doc;
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
