 ///
 /// @file    MutexLock.hpp
 /// @author  Icot(jobyang@163.com)
 /// @date    2017-03-14 15:24:20
 ///
#ifndef __WD_MUTEXLOCK_H__
#define __WD_MUTEXLOCK_H__

#include "Noncopyable.hpp"
#include <pthread.h>

namespace wd
{

class MutexLock{
public:
	MutexLock();
	~MutexLock();
	void lock();
	void unlock();
	bool status()const;
	pthread_mutex_t * getMutexLockPtr();
private:
	MutexLock(const MutexLock&);
	MutexLock& operator=(const MutexLock&);
private:
	bool _islocking;
	pthread_mutex_t _mutex;
};

class MutexLockGuard{
public:
	MutexLockGuard(MutexLock &mutex)
	:_mutex(mutex)
	{
		_mutex.lock();
	}
	
	~MutexLockGuard()
	{
		_mutex.unlock();
	}
private:
	MutexLock &_mutex;
};

}
#endif
