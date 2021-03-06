#include "Operations/Operation_EngineInjectorPrime.h"
#include "Config.h"
using namespace EmbeddedIOServices;

#ifdef OPERATION_ENGINEINJECTORPRIME_H
namespace OperationArchitecture
{
	Operation_EngineInjectorPrime::Operation_EngineInjectorPrime(ITimerService *timerService, std::function<void()> openCallBack, std::function<void()> closeCallBack)
	{
		_timerService = timerService;
		_openCallBack = openCallBack;
		_closeTask = new Task(closeCallBack);
	}

	void Operation_EngineInjectorPrime::Execute(float time)
	{
		_openCallBack();
		_timerService->ScheduleTask(_closeTask, _timerService->GetTick() + time * _timerService->GetTicksPerSecond());
	}

	IOperationBase *Operation_EngineInjectorPrime::Create(const void *config, size_t &sizeOut, const EmbeddedIOServiceCollection *embeddedIOServiceCollection, OperationPackager *packager)
	{
		std::function<void()> openCallBack = 0;
		std::function<void()> closeCallBack = 0;

		size_t size = 0;
		IOperationBase *operation = packager->Package(config, size);
		Config::OffsetConfig(config, sizeOut, size);
		if(operation->NumberOfParameters == 1)
		{
			openCallBack = [operation]() { operation->Execute(true); };
			closeCallBack = [operation]() { operation->Execute(false); };
		}
		else
		{
			openCallBack = [operation]() { operation->Execute(); };

			size = 0;
			IOperationBase *operation = packager->Package(config, size);
			Config::OffsetConfig(config, sizeOut, size);
			closeCallBack = [operation]() { operation->Execute(); };
		}
		
		return new Operation_EngineInjectorPrime(embeddedIOServiceCollection->TimerService, openCallBack, closeCallBack);
	}
}
#endif