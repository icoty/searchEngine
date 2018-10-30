 ///
 /// @file    DirScanner.h
 /// @author  yangyu/Icot(jobyang@163.com)
 /// @date    2017-03-28 06:14:04
 ///
#ifndef __WD_DIRSCANNER_H__
#define __WD_DIRSCANNER_H__

#include "Configuration.h"
#include <vector>
using std::vector;

namespace wd
{

class DirScanner{
public:
	DirScanner(Configuration & conf);
	void operator()();
	void traverse(const string &dirname);
	vector<string> getVecAbsPath();
	void debug();
private:
	vector<string>  _vecAbsPath;
	Configuration & _conf;
};

}
#endif
