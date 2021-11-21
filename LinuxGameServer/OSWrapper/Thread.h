#pragma once


#include "RefCountedObject.h"
#include "SharedPtr.h"
#include "Event.h"
#include "Runnable.h"

#include <limits.h>
#include <sys/select.h>
#include <errno.h>
#include <mutex>
#include <memory>
#include <pthread.h>

#ifndef _THREAD_STACK_SIZE
#define _THREAD_STACK_SIZE 0
#endif


class Runnable;
class ThreadLocalStorage;


typedef SharedPtr<Runnable> RunnablePtr;
//typedef std::shared_ptr<Runnable> RunnablePtr;


class Thread
{
public:
	typedef pthread_t TID;
	typedef void (*Callable)(void*);

	enum Priority
	{
		PRIO_LOWEST,
		PRIO_LOW,
		PRIO_NORMAL,
		PRIO_HIGH,
		PRIO_HIGHEST
	};

	enum Policy
	{
		POLICY_DEFAULT = SCHED_OTHER
	};

	Thread();
	Thread(const std::string& name);
	~Thread();

	int id() const;

	TID tid() const;

	std::string name() const;

	std::string getName() const;

	void setName(const std::string& name);

	void setPriority(Priority prio);


	Priority getPriority() const;

	void setOSPriority(int prio, int policy = POLICY_DEFAULT);

	int getOSPriority() const;

	static int getMinOSPriority(int policy = POLICY_DEFAULT);

	static int getMaxOSPriority(int policy = POLICY_DEFAULT);

	void setStackSize(int size);

	int getStackSize() const;

	void start(Runnable& target);

	void start(RunnablePtr pTarget);

	void start(Callable target, void* pData = 0);

	template <class Functor>
	void startFunc(const Functor& fn)
	{
		startImpl(new FunctorRunnable<Functor>(fn));
	}

	template <class Functor>
	void startFunc(Functor&& fn)
	{
		startImpl(new FunctorRunnable<Functor>(std::move(fn)));
	}

	void join();

	void join(long milliseconds);

	bool tryJoin(long milliseconds);

	bool isRunning() const;

	static bool trySleep(long milliseconds);

	void wakeUp();

	static void sleep(long milliseconds);

	static void yield();

	static Thread* current();

	static TID currentTid();

protected:
	ThreadLocalStorage& tls();

	void clearTLS();

	std::string makeName();

	static int uniqueId();

	template <class Functor>
	class FunctorRunnable : public Runnable
	{
	public:
		FunctorRunnable(const Functor& functor) :
			_functor(functor)
		{
		}

		FunctorRunnable(Functor&& functor) :
			_functor(std::move(functor))
		{
		}

		~FunctorRunnable()
		{
		}

		void run()
		{
			_functor();
		}

	private:
		Functor _functor;
	};

private:
	Thread(const Thread&);
	Thread& operator = (const Thread&);

	int                 _id;
	std::string         _name;
	ThreadLocalStorage* _pTLS;
	Event               _event;
	mutable std::mutex   _mutex;

	friend class ThreadLocalStorage;
	friend class PooledThread;




protected:
	static void* runnableEntry(void* pThread);
	static int mapPrio(int prio, int policy = SCHED_OTHER);
	static int reverseMapPrio(int osPrio, int policy = SCHED_OTHER);

private:
	class CurrentThreadHolder
	{
	public:
		CurrentThreadHolder()
		{
			if (::pthread_key_create(&_key, nullptr))
				throw SystemException("cannot allocate thread context key");
		}
		~CurrentThreadHolder()
		{
			::pthread_key_delete(_key);
		}
		Thread* get() const
		{
			return reinterpret_cast<Thread*>(pthread_getspecific(_key));
		}
		void set(Thread* pThread)
		{
			::pthread_setspecific(_key, pThread);
		}

	private:
		pthread_key_t _key;
	};

	struct ThreadData : public RefCountedObject
	{
		ThreadData() :
			thread(0),
			prio(PRIO_NORMAL),
			osPrio(),
			policy(SCHED_OTHER),
			done(Event::EVENT_MANUALRESET),
			stackSize(_THREAD_STACK_SIZE),
			started(false),
			joined(false)
		{
		}

		RunnablePtr pRunnableTarget;
		pthread_t     thread;
		int           prio;
		int           osPrio;
		int           policy;
		Event         done;
		std::size_t   stackSize;
		bool          started;
		bool          joined;
	};

	std::auto_ptr<ThreadData> _pData;

	static CurrentThreadHolder _currentThreadHolder;
};
