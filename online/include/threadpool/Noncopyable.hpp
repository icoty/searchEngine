/// @file    noncopyable.hpp
/// @author  Icot(jobyang@163.com)
/// @date    2017-03-14 11:01:57
#ifndef __WD_NONCOPYABLE_H__
#define __WD_NONCOPYABLE_H__

namespace wd
{

class Noncopyable{
protected:
	Noncopyable(){ }
	~Noncopyable(){ }
private:
	Noncopyable(const Noncopyable &);
	Noncopyable & operator=(const Noncopyable &);
};

}
#endif
