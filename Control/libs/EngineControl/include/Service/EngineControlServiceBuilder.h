//hardware abstraction 1-1000
#define HARDWARE_ABSTRACTION_COLLECTION_ID		1
#define ANALOG_SERVICE_ID						2				// IAnalogService			voltage
#define DIGITAL_SERVICE_ID						3				// IDigitalService
#define PWM_SERVICE_ID							4				// IPwmService
#define TIMER_SERVICE_ID						5				// ITimerService

//config 1001-2000
#define IGNITION_CONFIG_ID						1001
#define INJECTION_CONFIG_ID						1002

//inputs 2001-3000
#define DECODER_SERVICE_ID						2001			// ICrankCamDecoderService
#define INTAKE_AIR_TEMPERATURE_SERVICE_ID		2002			// IFloatInputService		degrees C
#define ENGINE_COOLANT_TEMPERATURE_SERVICE_ID	2003			// IFloatInputService		degrees C
#define MANIFOLD_ABSOLUTE_PRESSURE_SERVICE_ID	2004			// IFloatInputService		Bar
#define VOLTAGE_SERVICE_ID						2005			// IFloatInputService		Volts
#define THROTTLE_POSITION_SERVICE_ID			2006			// IFloatInputService		TPS 0.0-1.0
#define ETHANOL_CONTENT_SERVICE_ID				2007			// IFloatInputService		Content 0.0-1.0
#define VEHICLE_SPEED_SERVICE_ID				2008			// IFloatInputService		MPH cause thats what people care about

//outputs 3001-4000
#define IGNITOR_SERVICES_ID						3001			// IBooleanOutputService[]
#define INJECTOR_SERVICES_ID					3002			// IBooleanOutputService[]
#define IDLE_AIR_CONTROL_VALVE_SERVICE_ID		3003			// IFloatOutputService		sq mm

//application services 4001-5000
#define TACHOMETER_SERVICE_ID					4001			// TachometerService
#define PRIME_SERVICE_ID						4002			// IPrimeService
#define IDLE_CONTROL_SERVICE_ID					4003			// IIdleControlService
#define AFR_SERVICE_ID							4004			// IAfrService
#define FUEL_TRIM_SERVICE_ID					4005			// IFuelTrimService
#define FUEL_PUMP_SERVICE_ID					4006			// IFuelPumpService
#define IGNITION_SCHEDULING_SERVICE_ID			4007			// IgnitionSchedulingService
#define INJECTION_SCHEDULING_SERVICE_ID			4008			// InjectionSchedulingService
#define SHIFT_SERVICE_ID						4009			// ShiftService
#define GEAR_CONTROL_SERVICE_ID					4010			// GearControlService

//callback groups 5001-6000
#define PRE_DECODER_SYNC_CALL_BACK_GROUP		5001
#define POST_DECODER_SYNC_CALL_BACK_GROUP		5002
#define TICK_CALL_BACK_GROUP					5003

#include "Service/ServiceLocator.h"

//hardwareabstraction includes
#include "HardwareAbstraction/HardwareAbstractionCollection.h"

//IOService Inlcudes
#include "IOServices/FloatInputService/IFloatInputService.h"
#include "IOServices/BooleanInputService/IBooleanInputService.h"
#include "IOServices/ButtonService/IButtonService.h"
#include "IOServices/FloatOutputService/IFloatOutputService.h"
#include "IOServices/BooleanOutputService/IBooleanOutputService.h"
#include "IOServices/StepperOutputService/IStepperOutputService.h"

//Decoder
#include "CrankCamDecoders/ICrankCamDecoder.h"
#include "CrankCamDecoders/Gm24xDecoder.h"

//EngineControlServices Includes
#include "EngineControlServices/TachometerService/TachometerService.h"

#include "EngineControlServices/PrimeService/IPrimeService.h"
#include "EngineControlServices/PrimeService/PrimeService_StaticPulseWidth.h"

#include "EngineControlServices/IdleControlService/IIdleControlService.h"
#include "EngineControlServices/IdleControlService/IdleControlService_Pid.h"

#include "EngineControlServices/AfrService/IAfrService.h"
#include "EngineControlServices/AfrService/AfrService_Static.h"
#include "EngineControlServices/AfrService/AfrService_Map_Ethanol.h"

#include "EngineControlServices/FuelTrimService/IFuelTrimService.h"
#include "EngineControlServices/FuelTrimService/FuelTrimService_InterpolatedTable.h"
#include "EngineControlServices/FuelTrimService/FuelTrimServiceWrapper_MultiChannel.h"

#include "EngineControlServices/FuelPumpService/IFuelPumpService.h"
#include "EngineControlServices/FuelPumpService/FuelPumpService.h"
#include "EngineControlServices/FuelPumpService/FuelPumpService_Analog.h"

#include "EngineControlServices/InjectionService/InjectionSchedulingService.h"
#include "EngineControlServices/InjectionService/IInjectionConfig.h"
#include "EngineControlServices/InjectionService/InjectionConfig_SD.h"
#include "EngineControlServices/InjectionService/InjectionConfigWrapper_DFCO.h"

#include "EngineControlServices/IgnitionService/IgnitionSchedulingService.h"
#include "EngineControlServices/IgnitionService/IIgnitionConfig.h"
#include "EngineControlServices/IgnitionService/IgnitionConfig_Map_Ethanol.h"
#include "EngineControlServices/IgnitionService/IgnitionConfig_Static.h"
#include "EngineControlServices/IgnitionService/IgnitionConfigWrapper_HardRpmLimit.h"
#include "EngineControlServices/IgnitionService/IgnitionConfigWrapper_SoftPidRpmLimit.h"

using namespace HardwareAbstraction;
using namespace IOServices;
using namespace EngineControlServices;
using namespace CrankCamDecoders;

namespace Service
{
	class EngineControlServiceBuilder
	{
		template<typename T>
		static T* CastConfig(void **config, unsigned int *size)
		{
			T *castedConfig = T::Cast(*config);
			unsigned int confSize = castedConfig->Size();
			*size += confSize;
			*config = (void *)((unsigned char *)*config + confSize);
			
			return castedConfig;
		}
		template<typename T>
		static const T* CastConfig(const void **config, unsigned int *size)
		{
			T *castedConfig = (T *)(*config);
			const unsigned int confSize = castedConfig->Size();
			*size += confSize;
			*config = (const void *)((const unsigned char *)*config + confSize);
			
			return castedConfig;
		}
				
		static IBooleanOutputService * CreateBooleanOutputService(ServiceLocator *serviceLocator, const void **config, unsigned int *totalSize)
		{
			unsigned int size;
			IBooleanOutputService *booleanOutputService = IBooleanOutputService::CreateBooleanOutputService((HardwareAbstractionCollection*)serviceLocator->Locate(HARDWARE_ABSTRACTION_COLLECTION_ID), *config, &size);
			*config = (const void *)((const unsigned char *)*config + size);
			*totalSize += size;
			return booleanOutputService;
		}
				
		static IBooleanInputService * CreateBooleanInputService(ServiceLocator *serviceLocator, const void **config, unsigned int *totalSize)
		{
			unsigned int size;
			IBooleanInputService *booleanInputService = IBooleanInputService::CreateBooleanInputService((HardwareAbstractionCollection*)serviceLocator->Locate(HARDWARE_ABSTRACTION_COLLECTION_ID), *config, &size);
			*config = (void *)((unsigned char *)*config + size);
			*totalSize += size;
			return booleanInputService;
		}
				
		static IFloatOutputService * CreateFloatOutputService(ServiceLocator *serviceLocator, const void **config, unsigned int *totalSize)
		{
			unsigned int size;
			IFloatOutputService *floatOutputService = IFloatOutputService::CreateFloatOutputService((HardwareAbstractionCollection*)serviceLocator->Locate(HARDWARE_ABSTRACTION_COLLECTION_ID), *config, &size);
			*config = (void *)((unsigned char *)*config + size);
			*totalSize += size;
			return floatOutputService;
		}

		static IFloatInputService * CreateFloatInputService(ServiceLocator *serviceLocator, const void **config, unsigned int *totalSize)
		{
			unsigned int size;
			IFloatInputService *floatInputService = IFloatInputService::CreateFloatInputService((HardwareAbstractionCollection*)serviceLocator->Locate(HARDWARE_ABSTRACTION_COLLECTION_ID), *config, &size);
			*config = (void *)((unsigned char *)*config + size);
			*totalSize += size;
			return floatInputService;
		}
		
		static const unsigned char GetServiceId(const void **config, unsigned int *size)
		{
			const unsigned char serviceId = *((const unsigned char*)*config);
			*config = (void *)((const unsigned char*)*config + 1);
			*size = 1;
			return serviceId;
		}

		static void OffsetConfig(const void **config, unsigned int *totalSize, unsigned int offset) 
		{
			*config = reinterpret_cast<const void *>(reinterpret_cast<const unsigned char *>(*config) + offset);
			if(totalSize != 0)
			{
				*totalSize += offset;
			}
		}

		static void RegisterIfNotNull(ServiceLocator *serviceLocator, uint16_t serviceId, void *pointer) 
		{
			if(pointer != 0)
			{
				serviceLocator->Register(serviceId, pointer);
			}
		}

		static void AddToCallBackGroupIfParametersNotNull(CallBackGroup *callback, void(*callBackPointer)(void *), void *parameters)
		{
			if(parameters != 0)
			{
				callback->Add(callBackPointer, parameters);
			}
		}
		
	public:
		static ServiceLocator *CreateServices(ServiceLocator *serviceLocator, const HardwareAbstractionCollection *hardwareAbstractionCollection, const void *config, unsigned int *totalSize);
		
		static EngineControlServices::TachometerService *CreateTachometerService(ServiceLocator *serviceLocator, const void *config, unsigned int *size);
		static IPrimeService* CreatePrimeService(ServiceLocator *serviceLocator, const void *config, unsigned int *size);
		static IIdleControlService* CreateIdleControlService(ServiceLocator *serviceLocator, const void *config, unsigned int *size);
		static IAfrService *CreateAfrService(ServiceLocator *serviceLocator, const void *config, unsigned int *size);
		static IFuelTrimService *CreateFuelTrimService(ServiceLocator *serviceLocator, const void *config, unsigned int *size);
		static IFuelPumpService *CreateFuelPumpService(ServiceLocator *serviceLocator, const void *config, unsigned int *size);
		static IInjectionConfig *CreateInjectionConfig(ServiceLocator *serviceLocator, const void *config, unsigned int *size);
		static IIgnitionConfig *CreateIgnitionConfig(ServiceLocator *serviceLocator, const void *config, unsigned int *size);
		static IgnitionSchedulingService *CreateIgnitionSchedulingService(ServiceLocator *serviceLocator, const void *config, unsigned int *size);
		static InjectionSchedulingService *CreateInjectionSchedulingService(ServiceLocator *serviceLocator, const void *config, unsigned int *size);
		static ICrankCamDecoder *CreateDecoderService(ServiceLocator *serviceLocator, const void *config, unsigned int *size);
	};
}