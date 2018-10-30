
#include "MutexLock.hpp"

//头文件定义有MutexLockGuard类

namespace wd
{

MutexLock::MutexLock()
:_islocking(false)
{
	pthread_mutex_init(&_mutex,NULL);
}

MutexLock::~MutexLock()
{	pthread_mutex_destroy(&_mutex);	}

void MutexLock::lock()
{
	pthread_mutex_lock(&_mutex);
	_islocking=true;
}

void MutexLock::unlock()
{
	pthread_mutex_unlock(&_mutex);
	_islocking=false;
}

bool MutexLock::status()const
{	return _islocking;	}

pthread_mutex_t * MutexLock::getMutexLockPtr()
{	return & _mutex;	}

}
