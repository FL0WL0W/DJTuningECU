#include "Services.h"

namespace EngineManagement
{
	HardwareAbstraction::ITimerService *CurrentTimerService;
	HardwareAbstraction::IDigitalService *CurrentDigitalService;
	HardwareAbstraction::IAnalogService *CurrentAnalogService;
	HardwareAbstraction::IPwmService *CurrentPwmService;
	Decoder::IDecoder *CurrentDecoder;
	IIgnitorService *CurrentIgnitorServices[MAX_CYLINDERS];
	IMapService *CurrentMapService;
	IEngineCoolantTemperatureService *CurrentEngineCoolantTemperatureService;
	IIntakeAirTemperatureService *CurrentIntakeAirTemperatureService;
	IVoltageService *CurrentVoltageService;
	IEthanolService *CurrentEthanolService;
	IAfrService *CurrentAfrService;
	ITpsService *CurrentThrottlePositionService;
#ifndef NOINJECTION
	IFuelTrimService *CurrentFuelTrimService;
	IInjectorService *CurrentInjectorServices[MAX_CYLINDERS];
	IPrimeService *CurrentPrimeService;
	IFuelPumpService *CurrentFuelPumpService;
#endif
}