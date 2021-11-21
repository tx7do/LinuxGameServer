#include "Thread.h"
#include "BaseException.h"
#include "ThreadLocal.h"
#include "Timespan.h"
#include "Timestamp.h"
#include "Runnable.h"
#include "ErrorHandler.h"
#include <sstream>
#include <signal.h>
#include <time.h>
#include <atomic>

class RunnableHolder : public Runnable
{
public:
	RunnableHolder(Runnable& target) :
		_target(target)
	{
	}

	~RunnableHolder()
	{
	}

	void run()
	{
		_target.run();
	}

private:
	Runnable& _target;
};


class CallableHolder : public Runnable
{
public:
	CallableHolder(Thread::Callable callable, void* pData) :
		_callable(callable),
		_pData(pData)
	{
	}

	~CallableHolder()
	{
	}

	void run()
	{
		_callable(_pData);
	}

private:
	Thread::Callable _callable;
	void* _pData;
};


Thread::CurrentThreadHolder Thread::_currentThreadHolder;

Thread::Thread() :
	_id(uniqueId()),
	_name(makeName()),
	_pTLS(0),
	_event(Event::EVENT_AUTORESET)
{
}


Thread::Thread(const std::string& name) :
	_id(uniqueId()),
	_name(name),
	_pTLS(0),
	_event(Event::EVENT_AUTORESET)
{
}


Thread::~Thread()
{
	delete _pTLS;
}


void Thread::setPriority(Priority prio)
{
	if (prio != _pData->prio)
	{
		_pData->prio = prio;
		_pData->policy = SCHED_OTHER;
		if (isRunning())
		{
			struct sched_param par;
			par.sched_priority = mapPrio(_pData->prio, SCHED_OTHER);
			if (pthread_setschedparam(_pData->thread, SCHED_OTHER, &par))
				throw SystemException("cannot set thread priority");
		}
	}
}


Thread::Priority Thread::getPriority() const
{
	return Priority(_pData->prio);
}


void Thread::start(Runnable& target)
{
	start(new RunnableHolder(target));
}


void Thread::start(RunnablePtr pTarget)
{
	if (_pData->pRunnableTarget)
		throw SystemException("thread already running");

	pthread_attr_t attributes;
	pthread_attr_init(&attributes);

	if (_pData->stackSize != 0)
	{
		if (0 != pthread_attr_setstacksize(&attributes, _pData->stackSize))
		{
			pthread_attr_destroy(&attributes);
			throw SystemException("cannot set thread stack size");
		}
	}

	_pData->pRunnableTarget = pTarget;
	if (pthread_create(&_pData->thread, &attributes, runnableEntry, this))
	{
		_pData->pRunnableTarget = 0;
		pthread_attr_destroy(&attributes);
		throw SystemException("cannot start thread");
	}
	_pData->started = true;
	pthread_attr_destroy(&attributes);

	if (_pData->policy == SCHED_OTHER)
	{
		if (_pData->prio != PRIO_NORMAL)
		{
			struct sched_param par;
			par.sched_priority = mapPrio(_pData->prio, SCHED_OTHER);
			if (pthread_setschedparam(_pData->thread, SCHED_OTHER, &par))
				throw SystemException("cannot set thread priority");
		}
	}
	else
	{
		struct sched_param par;
		par.sched_priority = _pData->osPrio;
		if (pthread_setschedparam(_pData->thread, _pData->policy, &par))
			throw SystemException("cannot set thread priority");
	}
}


void Thread::start(Callable target, void* pData)
{
	start(new CallableHolder(target, pData));
}


void Thread::join()
{
	if (!_pData->started) return;
	_pData->done.wait();
	void* result;
	if (::pthread_join(_pData->thread, &result))
		throw SystemException("cannot join thread");
	_pData->joined = true;
}


void Thread::join(long milliseconds)
{
	if (!tryJoin(milliseconds))
		throw TimeoutException();
}


bool Thread::tryJoin(long milliseconds)
{
	if (_pData->started && _pData->done.tryWait(milliseconds))
	{
		void* result;
		if (::pthread_join(_pData->thread, &result))
			throw SystemException("cannot join thread");
		_pData->joined = true;
		return true;
	}
	else if (_pData->started) {
		return false;
	}
	else return true;
}


bool Thread::trySleep(long milliseconds)
{
	Thread* pT = Thread::current();
	CHECK_PTR(pT);
	return !(pT->_event.tryWait(milliseconds));
}


void Thread::wakeUp()
{
	_event.set();
}


ThreadLocalStorage& Thread::tls()
{
	if (!_pTLS)
		_pTLS = new ThreadLocalStorage;
	return *_pTLS;
}


void Thread::clearTLS()
{
	if (_pTLS)
	{
		delete _pTLS;
		_pTLS = 0;
	}
}


std::string Thread::makeName()
{
	std::ostringstream name;
	name << '#' << _id;
	return name.str();
}


int Thread::uniqueId()
{
	static std::atomic_int counter;
	return ++counter;
}


void Thread::setName(const std::string& name)
{
	std::unique_lock<std::mutex> lock(_mutex);

	_name = name;
}


Thread::TID Thread::tid() const
{
	return _pData->thread;
}


int Thread::id() const
{
	return _id;
}


std::string Thread::name() const
{
	std::unique_lock<std::mutex> lock(_mutex);

	return _name;
}


std::string Thread::getName() const
{
	std::unique_lock<std::mutex> lock(_mutex);

	return _name;
}


bool Thread::isRunning() const
{
	return !_pData->pRunnableTarget.isNull();
}


void Thread::sleep(long milliseconds)
{
	Timespan remainingTime(1000 * Timespan::TimeDiff(milliseconds));
	int rc=0;
	do
	{
		struct timespec ts;
		ts.tv_sec = (long)remainingTime.totalSeconds();
		ts.tv_nsec = (long)remainingTime.useconds() * 1000;
		Timestamp start;
		rc = ::nanosleep(&ts, 0);
		if (rc < 0 && errno == EINTR)
		{
			Timestamp end;
			Timespan waited = start.elapsed();
			if (waited < remainingTime)
				remainingTime -= waited;
			else
				remainingTime = 0;
		}
	} while (remainingTime > 0 && rc < 0 && errno == EINTR);
	if (rc < 0 && remainingTime > 0) throw SystemException("Thread::sleep(): nanosleep() failed");
}


void Thread::yield()
{
	::sched_yield();
}


Thread* Thread::current()
{
	return _currentThreadHolder.get();
}


void Thread::setOSPriority(int prio, int policy)
{
	if (prio != _pData->osPrio || policy != _pData->policy)
	{
		if (_pData->pRunnableTarget)
		{
			struct sched_param par;
			par.sched_priority = prio;
			if (pthread_setschedparam(_pData->thread, policy, &par))
				throw SystemException("cannot set thread priority");
		}
		_pData->prio = reverseMapPrio(prio, policy);
		_pData->osPrio = prio;
		_pData->policy = policy;
	}
}


 int Thread::getOSPriority() const
{
	return _pData->osPrio;
}


 int Thread::getMinOSPriority(int policy)
{
#if defined(_THREAD_PRIORITY_MIN)
	return _THREAD_PRIORITY_MIN;
#elif defined(__digital__)
	return PRI_OTHER_MIN;
#else
	return sched_get_priority_min(policy);
#endif
}


int Thread::getMaxOSPriority(int policy)
{
#if defined(_THREAD_PRIORITY_MAX)
	return _THREAD_PRIORITY_MAX;
#elif defined(__digital__)
	return PRI_OTHER_MAX;
#else
	return sched_get_priority_max(policy);
#endif
}


void Thread::setStackSize(int size)
{
#ifndef PTHREAD_STACK_MIN
	_pData->stackSize = 0;
#else
	if (size != 0)
	{
		if (size < PTHREAD_STACK_MIN)
			size = PTHREAD_STACK_MIN;
	}
	_pData->stackSize = size;
#endif
}


 int Thread::getStackSize() const
{
	return static_cast<int>(_pData->stackSize);
}


 Thread::TID Thread::currentTid()
{
	return ::pthread_self();
}

 void* Thread::runnableEntry(void* pvThread)
 {
	 _currentThreadHolder.set(reinterpret_cast<Thread*>(pvThread));

	 //sigset_t sset;
	 //sigemptyset(&sset);
	 //sigaddset(&sset, SIGQUIT);
	 //sigaddset(&sset, SIGTERM);
	 //sigaddset(&sset, SIGPIPE);
	 //pthread_sigmask(SIG_BLOCK, &sset, 0);

	 Thread* pThread = reinterpret_cast<Thread*>(pvThread);
#if defined(_DEBUG)
	 setThreadName(pThread->_pData->thread, reinterpret_cast<Thread*>(pThread)->getName());
#endif
	 std::auto_ptr<ThreadData> pData = pThread->_pData;
	 try
	 {
		 pData->pRunnableTarget->run();
	 }
	 catch (Exception& exc)
	 {
		 ErrorHandler::handle(exc);
	 }
	 catch (std::exception& exc)
	 {
		 ErrorHandler::handle(exc);
	 }
	 catch (...)
	 {
		 ErrorHandler::handle();
	 }

	 pData->pRunnableTarget = 0;
	 pData->done.set();
	 return 0;
 }


 int Thread::mapPrio(int prio, int policy)
 {
	 int pmin = getMinOSPriority(policy);
	 int pmax = getMaxOSPriority(policy);

	 switch (prio)
	 {
	 case PRIO_LOWEST:
		 return pmin;
	 case PRIO_LOW:
		 return pmin + (pmax - pmin) / 4;
	 case PRIO_NORMAL:
		 return pmin + (pmax - pmin) / 2;
	 case PRIO_HIGH:
		 return pmin + 3 * (pmax - pmin) / 4;
	 case PRIO_HIGHEST:
		 return pmax;
	 default:
		 BUGCHECK_MSG("invalid thread priority");
	 }
	 return -1; // just to satisfy compiler - we'll never get here anyway
 }


 int Thread::reverseMapPrio(int prio, int policy)
 {
	 if (policy == SCHED_OTHER)
	 {
		 int pmin = getMinOSPriority(policy);
		 int pmax = getMaxOSPriority(policy);
		 int normal = pmin + (pmax - pmin) / 2;
		 if (prio == pmax)
			 return PRIO_HIGHEST;
		 if (prio > normal)
			 return PRIO_HIGH;
		 else if (prio == normal)
			 return PRIO_NORMAL;
		 else if (prio > pmin)
			 return PRIO_LOW;
		 else
			 return PRIO_LOWEST;
	 }
	 else return PRIO_HIGHEST;
 }

