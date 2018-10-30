 ///
 /// @file    DirScanner.cpp
 /// @author  yangyu/Icot(jobyang@163.com)
 /// @date    2017-03-28 20:41:14
 ///
#include "DirScanner.h"
#include "Configuration.h"
#include "Log4func.h"
#include "FileName.h"

#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#include <iostream>

namespace wd
{

DirScanner::DirScanner(Configuration & conf)
:_conf(conf)
{}

void DirScanner::traverse(const string &dirname)
{
	DIR* dir;
	if(NULL==(dir=opendir(dirname.c_str())))
	{
		LogError("open rss dirname error!");
		return;
	}
	struct dirent *p;
	char path[100]={0};
	while((p=readdir(dir))!=NULL)
	{
		bzero(path,sizeof(path));
		if(!strcmp(p->d_name,".")||!strcmp(p->d_name,"..")||(p->d_type==4))
			continue;
		else
		{
			sprintf(path,"%s%c%s",dirname.c_str(),'/',p->d_name);
			_vecAbsPath.push_back(string(path));
		}
	}
	closedir(dir);
}

void DirScanner::debug()
{
	for(auto & ele:_vecAbsPath)
		std::cout<<ele<<std::endl;
}

vector<string> DirScanner::getVecAbsPath()
{
	return _vecAbsPath;
}

void DirScanner::operator()()
{
	traverse(_conf.getConfigMap()[RSS]);
}

}
