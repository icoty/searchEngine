 ///
 /// @file    WordQuery.hpp
 /// @author  yangyu/Icot(jobyang@163.com)
 /// @date    2017-03-31 02:45:45
 ///
#ifndef __WD_WORDQUERY_H__
#define __WD_WORDQUERY_H__

#include "WebPage.hpp"
#include "Configuration.hpp"

#include <unordered_map>
#include <json/json.h>
#include <map>
#include <string>
#include <vector>
#include <set>
using std::vector;
using std::string;
using std::map;
using std::pair;
using std::set;
using std::unordered_map;
//using namespace std::tr1;

namespace wd
{

class WordQuery{
public:
	WordQuery(Configuration & conf);
	string doQuery(const string & word);
private:
	void loadLibrary();
	vector<double> getQueryWordsWeightVector(
					map<string,int> & wordFeq);

	bool executeQuery(const map<string,int> & wordFreq,
					vector<pair<int,vector<double> > > &resultVec);
	
	string createJson(vector<int> & docIdVec,
					const vector<string> & queryWords);
	string returnNoAnswer();
private:
	Configuration & _conf;
	unordered_map<int, WebPage> _pageLib;
	unordered_map<int, pair<int, int> > _offsetLib;
	unordered_map<string, set<pair<int, double>>> _invertIndexTable;
};

}
#endif
