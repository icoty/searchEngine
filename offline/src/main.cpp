 ///
 /// @file    main.cpp
 /// @author  yangyu/Icot(jobyang@163.com)
 /// @date    2017-03-30 03:56:09
 ///
#include "FileName.h"
#include "Configuration.h"
#include "DirScanner.h"
#include "PageLibPreProcessor.h"
#include "PageLib.h"
#include <iostream>
using std::cout;
using std::endl;

int main(int argc,char **argv)
{
	wd::Configuration conf(argv[1]);
	conf.init();
	//conf.debug();
	
	
	wd::DirScanner dir(conf);
	dir();
	//dir.debug();

	wd::PageLib pagelib(conf,dir);
	pagelib.create();
	pagelib.store();
	
	wd::PageLibPreProcessor preprocess(conf);
	preprocess.doProcess();
	std::cout<<"process success!"<<endl;
	return 0;
}
