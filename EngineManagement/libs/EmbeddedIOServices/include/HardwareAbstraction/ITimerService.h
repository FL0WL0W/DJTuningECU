#include <map>
#include <list>

#ifndef ITIMERSERVICE_H
#define ITIMERSERVICE_H

#define TIMERSERVICE_MAX_STACK_SIZE 256

namespace HardwareAbstraction
{	
	class ICallBack
	{
	public:
		virtual void Execute() = 0;
	};
	
	struct CallBack : public ICallBack
	{
		CallBack(void(*callBackPointer)(void *), void *parameters)
		{
			CallBackPointer = callBackPointer;
			Parameters = parameters;
		}

		void Execute()
		{
			CallBackPointer(Parameters);
		}

		void(*CallBackPointer)(void *);
		void *Parameters;
	};
	
	class CallBackGroup : public ICallBack
	{
	protected:
		std::list<ICallBack *> _callBackList;
	public:		
		void Execute();
		void Add(ICallBack *callBack);
		void Add(void(*callBackPointer)(void *), void *parameters);
		void Remove(ICallBack *callBack);
		void Clear();
	};

	struct Task
	{
	public:
		Task() {}
		Task(void(*callBack)(void *), void *parameters, bool deleteOnExecution)
		{
			CallBackInstance = new CallBack(callBack, parameters);
			DeleteOnExecution = deleteOnExecution;
		}
		Task(ICallBack *callBack, bool deleteOnExecution)
		{
			CallBackInstance = callBack;
			DeleteOnExecution = deleteOnExecution;
		}

		void Execute()
		{
			CallBackInstance->Execute();
		}

		ICallBack *CallBackInstance;
		bool DeleteOnExecution;
		//only let TimerService edit these values
		unsigned int Tick;
	};

	class ITimerService
	{
	protected:
		void SortCallBackStack();
		virtual void ScheduleCallBack(unsigned int tick) = 0;
	public:
#if TIMERSERVICE_MAX_STACK_SIZE <= 2^8
		unsigned char StackSize = 0;
#elif TIMERSERVICE_MAX_STACK_SIZE <= 2^16
		unsigned short StackSize = 0;
#elif TIMERSERVICE_MAX_STACK_SIZE <= 2^32
		unsigned int StackSize = 0;
#endif
		Task *CallBackStackPointer[TIMERSERVICE_MAX_STACK_SIZE];

		virtual unsigned int GetTick(void) = 0;
		virtual unsigned int GetTicksPerSecond() = 0;

		void ReturnCallBack(void);
		Task *ScheduleTask(void(*)(void *), void *, unsigned int, bool);
		bool ScheduleTask(Task *, unsigned int);
		bool ReScheduleTask(Task *, unsigned int);
		bool UnScheduleTask(Task *);
		
		unsigned int GetElapsedTick(unsigned int);
		float GetElapsedTime(unsigned int);
	};
}
#endif