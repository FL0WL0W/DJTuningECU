#include "BooleanOutputService.h"

#ifdef BOOLEANOUTPUTSERVICE_H
namespace IOService
{
	BooleanOutputService::BooleanOutputService(const HardwareAbstractionCollection *hardwareAbstractionCollection, const BooleanOutputServiceConfig *config, bool highZ)
	{
		_hardwareAbstractionCollection = hardwareAbstractionCollection;
		_config = config;
		_highZ = highZ;
		

		if (_highZ && _config->NormalOn)
		{
			_hardwareAbstractionCollection->DigitalService->InitPin(_config->Pin, In);
		}
		else
		{
			_hardwareAbstractionCollection->DigitalService->InitPin(_config->Pin, Out);

			_hardwareAbstractionCollection->DigitalService->WritePin(_config->Pin, _config->NormalOn);
		}
	}
	
	void BooleanOutputService::OutputSet()
	{
		if (_highZ && !_config->NormalOn)
		{
			_hardwareAbstractionCollection->DigitalService->InitPin(_config->Pin, In);
		}
		else
		{
			if (_highZ)
			{
				_hardwareAbstractionCollection->DigitalService->InitPin(_config->Pin, Out);
			}
			
			_hardwareAbstractionCollection->DigitalService->WritePin(_config->Pin, !_config->NormalOn);
		}
	}
	
	void BooleanOutputService::OutputReset()
	{
		if (_highZ && _config->NormalOn)
		{
			_hardwareAbstractionCollection->DigitalService->InitPin(_config->Pin, In);
		}
		else
		{
			if (_highZ)
			{
				_hardwareAbstractionCollection->DigitalService->InitPin(_config->Pin, Out);
			}
			
			_hardwareAbstractionCollection->DigitalService->WritePin(_config->Pin, _config->NormalOn);
		}
	}
		
	void BooleanOutputService::OutputWrite(bool val)
	{
		if (val)
			OutputSet();
		else
			OutputReset();
	}
}
#endif