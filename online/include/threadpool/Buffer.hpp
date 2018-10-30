 ///
 /// @file    buffer.hpp
 /// @author  Icot(jobyang@163.com)
 /// @date    2017-03-14 16:00:39
 ///
#ifndef __WD_BUFFER_H__
#define __WD_BUFFER_H__

#include "MutexLock.hpp"
#include "Condition.hpp"

#include <queue>
#include <functional>

namespace wd
{

typedef std::function<void()> ElemType;

class Buffer{
public:
	Buffer(size_t size);
	void push(ElemType elem);
	ElemType pop();
	bool empty();
	bool full();
	void wakeup();
private:
	MutexLock _mutex;
	Condition _notFull;
	Condition _notEmpty;
	size_t _size;
	bool _flag;
	std::queue<ElemType> _que;
};

}
#endif
