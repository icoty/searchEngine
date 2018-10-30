 ///
 /// @file    condition.hpp
 /// @author  Icot(jobyang@163.com)
 /// @date    2017-03-14 19:05:58
 ///
#ifndef _WD_CONDITION_H__
#define _WD_CONDITION_H__

#include "Noncopyable.hpp"
#include <pthread.h>

namespace wd
{

class MutexLock;//前向声明

class Condition:private Noncopyable{
public:
	Condition(MutexLock &mutex);
	~Condition();
	void wait();
	void notify();
	void notifyall();

private:
	pthread_cond_t _cond;
	MutexLock & _mutex;
};

}
#endif
