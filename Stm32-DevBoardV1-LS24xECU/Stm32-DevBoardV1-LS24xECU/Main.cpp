#include <stm32f10x_rcc.h>
#include <stm32f10x_flash.h>
#include <map>
#include <functional>
#include "ITimerService.h"
#include "Stm32F10xTimerService.h"
#include "IDigitalService.h"
#include "Stm32F10xDigitalService.h"
#include "IAnalogService.h"
#include "Stm32F10xAnalogService.h"
#include "IIgnitorService.h"
#include "IgnitorService.h"
#include "IInjectorService.h"
#include "InjectorService.h"
#include "IMapService.h"
#include "MapService.h"
#include "IEngineCoolantTemperatureService.h"
#include "EngineCoolantTemperatureService.h"
#include "IIntakeAirTemperatureService.h"
#include "IntakeAirTemperatureService.h"
#include "IVoltageService.h"
#include "VoltageService.h"
#include "IAfrService.h"
#include "StaticAfrService.h"
#include "IDecoder.h"
#include "Gm24xDecoder.h"
#include "IFuelTrimService.h"
#include "PistonEngineConfig.h"
#include "IPistonEngineIgnitionConfig.h"
#include "PistonEngineIgnitionMapConfig.h"
#include "IPistonEngineInjectionConfig.h"
#include "PistonEngineInjectionSDConfig.h"
#include "PistonEngineController.h"
#include "EmbeddedResources.h"
#include "stm32f10x_tim.h"

#define INJECTOR_PIN_1 0
#define INJECTOR_PIN_2 0
#define INJECTOR_PIN_3 0
#define INJECTOR_PIN_4 0
#define INJECTOR_PIN_5 0
#define INJECTOR_PIN_6 0
#define INJECTOR_PIN_7 0
#define INJECTOR_PIN_8 0

#define IGNITION_PIN_1 0
#define IGNITION_PIN_2 0
#define IGNITION_PIN_3 0
#define IGNITION_PIN_4 0
#define IGNITION_PIN_5 0
#define IGNITION_PIN_6 0
#define IGNITION_PIN_7 0
#define IGNITION_PIN_8 0

#define MAP_PIN 0
#define ECT_PIN 0
#define IAT_PIN 0
#define VOLTAGE_PIN 0

HardwareAbstraction::ITimerService *_timerService;
HardwareAbstraction::IDigitalService *_digitalService;
EngineManagement::IIgnitorService *_ignitorServices[MAX_CYLINDERS];
EngineManagement::IInjectorService *_injectorServices[MAX_CYLINDERS];
HardwareAbstraction::IAnalogService *_analogService;
EngineManagement::IMapService *_mapService;
EngineManagement::IFuelTrimService *_fuelTrimService;
EngineManagement::IEngineCoolantTemperatureService *_engineCoolantTemperatureService;
EngineManagement::IIntakeAirTemperatureService *_intakeAirTemperatureService;
EngineManagement::IVoltageService *_voltageService;
EngineManagement::IAfrService *_afrService;
Decoder::IDecoder *_decoder;
EngineManagement::PistonEngineConfig *_pistonEngineConfig;
EngineManagement::IPistonEngineInjectionConfig *_pistonEngineInjectionConfig;
EngineManagement::IPistonEngineIgnitionConfig *_pistonEngineIgnitionConfig;
EngineManagement::PistonEngineController *_pistonEngineController;

void clock_init() {
  /*Configure all clocks to max for best performance.
   * If there are EMI, power, or noise problems, try slowing the clocks*/

  /* First set the flash latency to work with our clock*/
  /*000 Zero wait state, if 0  MHz < SYSCLK <= 24 MHz
    001 One wait state, if  24 MHz < SYSCLK <= 48 MHz
    010 Two wait states, if 48 MHz < SYSCLK <= 72 MHz */
	FLASH_SetLatency(FLASH_Latency_2);

	  /* Start with HSI clock (internal 8mhz), divide by 2 and multiply by 9 to
	   * get maximum allowed frequency: 36Mhz
	   * Enable PLL, wait till it's stable, then select it as system clock*/
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
	RCC_PLLCmd(ENABLE);
	while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {}
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

	  /* Set HCLK, PCLK1, and PCLK2 to SCLK (these are default */
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div1);
	RCC_PCLK2Config(RCC_HCLK_Div1);

	  /* Set ADC clk to 12MHz (14MHz max, 18MHz default)*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	SystemCoreClockUpdate();
}

int main()
{
	clock_init();
	
	//TODO: create factory
	
	_timerService = new Stm32::Stm32F10xTimerService();
		
	_digitalService = new Stm32::Stm32F10xDigitalService();
	
	//TODO: create unit tests
	_ignitorServices[0] = new EngineManagement::IgnitorService(_digitalService, IGNITION_PIN_1, false, true); 
	_ignitorServices[1] = new EngineManagement::IgnitorService(_digitalService, IGNITION_PIN_2, false, true); 
	_ignitorServices[2] = new EngineManagement::IgnitorService(_digitalService, IGNITION_PIN_3, false, true); 
	_ignitorServices[3] = new EngineManagement::IgnitorService(_digitalService, IGNITION_PIN_4, false, true); 
	_ignitorServices[4] = new EngineManagement::IgnitorService(_digitalService, IGNITION_PIN_5, false, true); 
	_ignitorServices[5] = new EngineManagement::IgnitorService(_digitalService, IGNITION_PIN_6, false, true); 
	_ignitorServices[6] = new EngineManagement::IgnitorService(_digitalService, IGNITION_PIN_7, false, true); 
	_ignitorServices[7] = new EngineManagement::IgnitorService(_digitalService, IGNITION_PIN_8, false, true); 
	
	//TODO: create unit tests
	_injectorServices[0] = new EngineManagement::InjectorService(_digitalService, INJECTOR_PIN_1, false, false); 
	_injectorServices[1] = new EngineManagement::InjectorService(_digitalService, INJECTOR_PIN_2, false, false); 
	_injectorServices[2] = new EngineManagement::InjectorService(_digitalService, INJECTOR_PIN_3, false, false); 
	_injectorServices[3] = new EngineManagement::InjectorService(_digitalService, INJECTOR_PIN_4, false, false); 
	_injectorServices[4] = new EngineManagement::InjectorService(_digitalService, INJECTOR_PIN_5, false, false); 
	_injectorServices[5] = new EngineManagement::InjectorService(_digitalService, INJECTOR_PIN_6, false, false); 
	_injectorServices[6] = new EngineManagement::InjectorService(_digitalService, INJECTOR_PIN_7, false, false); 
	_injectorServices[7] = new EngineManagement::InjectorService(_digitalService, INJECTOR_PIN_8, false, false); 
	
	_analogService = new Stm32::Stm32F10xAnalogService();
	
	//TODO: create unit tests
	_mapService = new EngineManagement::MapService(_timerService, _analogService, MAP_PIN, EmbeddedResources::MapConfigFile_dat.data());
  
	_decoder = new Decoder::Gm24xDecoder(_timerService);
	
	//TODO: Fuel Trim Service
	_fuelTrimService = NULL;
	
	//TODO: Ceate Unit Tests
	_engineCoolantTemperatureService = new EngineManagement::EngineCoolantTemperatureService(_timerService, _analogService, ECT_PIN, EmbeddedResources::EctConfigFile_dat.data());
	
	//TODO: Create Unit Tests
	_intakeAirTemperatureService = new EngineManagement::IntakeAirTemperatureService(_timerService, _analogService, IAT_PIN, EmbeddedResources::IatConfigFile_dat.data()); 
	
	//TODO: Create Unit Tests
	_voltageService = new EngineManagement::VoltageService(_timerService, _analogService, VOLTAGE_PIN, EmbeddedResources::IatConfigFile_dat.data()); 
	
	//TODO: Afr Service
	_afrService = new EngineManagement::StaticAfrService(14.7);
	
	//TODO: create unit tests
	_pistonEngineConfig = new EngineManagement::PistonEngineConfig(EmbeddedResources::PistonEngineConfigFile_dat.data());
	
	//TODO: create unit tests
	//interpolate short pulse adder
	//make temperature bias relative to cylinder volume
	_pistonEngineInjectionConfig = new EngineManagement::PistonEngineInjectionSDConfig(_decoder, _fuelTrimService, _mapService, _intakeAirTemperatureService, _engineCoolantTemperatureService, _voltageService, _afrService, _pistonEngineConfig, EmbeddedResources::PistonEngineInjectionSDConfigFile_dat.data());
	
	//TODO: create unit tests
	_pistonEngineIgnitionConfig = new EngineManagement::PistonEngineIgnitionMapConfig(_decoder, _mapService, _intakeAirTemperatureService, _engineCoolantTemperatureService, _voltageService, _afrService, _pistonEngineConfig, EmbeddedResources::PistonEngineIgnitionMapConfigFile_dat.data());
	
	//TODO: create unit tests
	//finish odd cylinder banks
	_pistonEngineController = new EngineManagement::PistonEngineController(_timerService, _decoder, _ignitorServices, _injectorServices, _pistonEngineInjectionConfig, _pistonEngineIgnitionConfig, _pistonEngineConfig);
	
	//wait until the decoder is synced before any scheduling
	while (!_decoder->IsSynced()) ;
	
	for (;;)
	{
		_mapService->ReadMap();
		_engineCoolantTemperatureService->ReadEct();
		_intakeAirTemperatureService->ReadIat();
		_voltageService->ReadVoltage();
		_pistonEngineController->ScheduleEvents();
	}
}

//TODO: create and enable intterupts for decoder

