#include "Services.h"

namespace EngineManagement
{
	HardwareAbstraction::ITimerService *CurrentTimerService;
	HardwareAbstraction::IDigitalService *CurrentDigitalService;
	HardwareAbstraction::IAnalogService *CurrentAnalogService;
	HardwareAbstraction::IPwmService *CurrentPwmService;
	Decoder::IDecoder *CurrentDecoder;
	
	ISensorService *CurrentIntakeAirTemperatureService;
	ISensorService *CurrentEngineCoolantTemperatureService;
	ISensorService *CurrentManifoldAbsolutePressureService;
	ISensorService *CurrentVoltageService;
	ISensorService *CurrentThrottlePositionService;
	ISensorService *CurrentEthanolContentService;
	ISensorService *CurrentVehicleSpeedSensorService;
}