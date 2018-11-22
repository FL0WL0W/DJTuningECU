#include "HardwareAbstraction/HardwareAbstractionCollection.h"

using namespace HardwareAbstraction;

#ifndef IBOOLEANOUTPUTSERVICE_H
#define IBOOLEANOUTPUTSERVICE_H
namespace IOServices
{
	class IBooleanOutputService
	{
	public:
		virtual void OutputSet() = 0;
		virtual void OutputReset() = 0;
		virtual void OutputWrite(bool value) = 0;

		static void OutputSetCallBack(void *booleanOutputService);
		static void OutputResetCallBack(void *booleanOutputService);
		
		static IBooleanOutputService *CreateBooleanOutputService(const HardwareAbstractionCollection *hardwareAbstractionCollection, void *config, unsigned int *sizeOut);
	};
}
#endif