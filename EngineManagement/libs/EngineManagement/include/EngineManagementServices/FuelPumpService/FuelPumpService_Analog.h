#include "EngineManagementServices/FuelPumpService/IFuelPumpService.h"
#include "IOServices/FloatOutputService/IFloatOutputService.h"
#include "IOServices/FloatInputService/IFloatInputService.h"
#include "Interpolation.h"
#include "CrankCamDecoders/ICrankCamDecoder.h"
#include "Packed.h"

using namespace HardwareAbstraction;
using namespace IOServices;
using namespace CrankCamDecoders;
using namespace Interpolation;

#if !defined(FUELPUMPSERVICE_ANALOG_H) && defined(IFUELPUMPSERVICE_H)
#define FUELPUMPSERVICE_ANALOG_H
namespace EngineManagementServices
{	
	PACK(
	struct FuelPumpService_AnalogConfig
	{
	private:
		FuelPumpService_AnalogConfig()
		{
		}
	public:
		static FuelPumpService_AnalogConfig* Cast(void *p)
		{
			FuelPumpService_AnalogConfig* ret = (FuelPumpService_AnalogConfig*)p;
			
			ret->AnalogTable = (float *)(ret + 1);
			
			return ret;
		}
		
		unsigned int Size()
		{
			return sizeof(FuelPumpService_AnalogConfig) +
				sizeof(unsigned char) * RpmRes * YRes;
		}

		float PrimeValue;
		float PrimeTime;
			
		bool UseTps;
		
		unsigned short MaxRpm;
		float MaxY;
		unsigned char RpmRes;
		unsigned char YRes;
		float *AnalogTable;
	});
	
	class FuelPumpService_Analog : public IFuelPumpService
	{
		const FuelPumpService_AnalogConfig *_config;
		ITimerService *_timerService;
		IFloatOutputService *_outputService;
		ICrankCamDecoder *_decoder;
		IFloatInputService *_manifoldAbsolutePressureService;
		IFloatInputService *_throttlePositionService;

		unsigned char _currentValue;
		bool _isOn;
	public:
		bool Started = false;
		FuelPumpService_Analog(const FuelPumpService_AnalogConfig *config, ITimerService *timerService, IFloatOutputService *outputService, ICrankCamDecoder *decoder, IFloatInputService *manifoldAbsolutePressureService, IFloatInputService *throttlePositionService);
		void Prime();
		void On();
		void Off();
		void Tick();
		static void PrimeTaskOff(void *parameter);
	};
}
#endif