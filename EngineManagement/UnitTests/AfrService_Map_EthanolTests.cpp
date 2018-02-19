#include "stdafx.h"
#include "CppUnitTest.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Services.h"
#include "AfrService_Map_Ethanol.h"
#include "MockTimerService.h"
#include "MockMapService.h"
#include "MockEngineCoolantTemperatureService.h"
#include "MockTpsService.h"
#include "MockEthanolService.h"
#include "MockDecoder.h"
using ::testing::AtLeast;
using ::testing::Return;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{		
	TEST_CLASS(AfrService_Map_EthanolTests)
	{
	public:
		HardwareAbstraction::MockTimerService *_timerService;
		EngineManagement::MockMapService *_mapService;
		EngineManagement::MockEngineCoolantTemperatureService *_ectService;
		EngineManagement::MockTpsService *_tpsService;
		EngineManagement::MockEthanolService *_ethanolService;
		Decoder::MockDecoder *_decoder;

		void CreateServices()
		{
			EngineManagement::CurrentDecoder = _decoder = new Decoder::MockDecoder();
			EngineManagement::CurrentTimerService = _timerService = new HardwareAbstraction::MockTimerService();
			EngineManagement::CurrentMapService = _mapService = new EngineManagement::MockMapService();
			EngineManagement::CurrentEngineCoolantTemperatureService = _ectService = new EngineManagement::MockEngineCoolantTemperatureService();
			EngineManagement::CurrentThrottlePositionService = _tpsService = new EngineManagement::MockTpsService();
			EngineManagement::CurrentEthanolService = _ethanolService = new EngineManagement::MockEthanolService();

			
			//GAS AFR TABLE, values in 1/1024	0	   2000	  4000	 6000
			unsigned short gasTable[4 * 4] = {	16076, 16076, 16076, 16076,	//0
												16076, 15052, 15052, 15052,	//33
												15052, 15052, 15052, 15052,	//67
												13245, 12041, 12041, 12041};//100
			
			//ETHANOL AFR TABLE, values in 1/1024	0	   2000	  4000	 6000	
			unsigned short ethanolTable[4 * 4] = {	11024, 11024, 11024, 11024,	//0
													11024, 10000, 10000, 10000,	//33
													10000, 10000, 10000, 10000,	//67
													9000, 8400, 8400, 8400};	//100
			
			//ECT MULTIPLIER TABLE		   -40	 13	   67	 120
			float ectMultiplierTable[4] = { 0.8, 0.85, 0.95, 1 };

			//TPS MIN AFR TABLE GAS values in 1/1024	 0		33	   67	  100
			unsigned short tpsMinAfrTableGasTable[4] = { 16076, 15052, 14148, 13245 };

			//TPS MIN AFR TABLE ETHANOL values in 1/1024	 0		33	   67	 100
			unsigned short tpsMinAfrTableEthanolTable[4] = { 11024, 10000, 9200, 9000 };


			void *config = malloc(142);
			void *buildConfig = config;
			//MaxRpm
			*((unsigned short *)buildConfig) = 6000;
			buildConfig = (void*)((unsigned short *)buildConfig + 1);

			//MaxMapKpa
			*((float *)buildConfig) = 100;
			buildConfig = (void*)((float *)buildConfig + 1);

			//MinEct
			*((float *)buildConfig) = -40;
			buildConfig = (void*)((float *)buildConfig + 1);

			//MaxEct
			*((float *)buildConfig) = 120;
			buildConfig = (void*)((float *)buildConfig + 1);

			//RPM resolution
			*((unsigned char *)buildConfig) = 4;
			buildConfig = (void*)((unsigned char *)buildConfig + 1);

			//MAP resolution
			*((unsigned char *)buildConfig) = 4;
			buildConfig = (void*)((unsigned char *)buildConfig + 1);

			//ECT resolution
			*((unsigned char *)buildConfig) = sizeof(ectMultiplierTable) / sizeof(float);
			buildConfig = (void*)((unsigned char *)buildConfig + 1);

			//TPS resolution
			*((unsigned char *)buildConfig) = sizeof(tpsMinAfrTableGasTable) / sizeof(unsigned short);
			buildConfig = (void*)((unsigned char *)buildConfig + 1);
			
			//GAS AFR TABLE
			memcpy(buildConfig, gasTable, sizeof(gasTable));
			buildConfig = (void*)((unsigned char *)buildConfig + sizeof(gasTable));
				
			//ETHANOL AFR TABLE
			memcpy(buildConfig, ethanolTable, sizeof(ethanolTable));
			buildConfig = (void*)((unsigned char *)buildConfig + sizeof(ethanolTable));

			//ECT MULTIPLIER TABLE
			memcpy(buildConfig, ectMultiplierTable, sizeof(ectMultiplierTable));
			buildConfig = (void*)((unsigned char *)buildConfig + sizeof(ectMultiplierTable));

			//TPS MIN AFR TABLE GAS
			memcpy(buildConfig, tpsMinAfrTableGasTable, sizeof(tpsMinAfrTableGasTable));
			buildConfig = (void*)((unsigned char *)buildConfig + sizeof(tpsMinAfrTableGasTable));

			//TPS MIN AFR TABLE ETHANOL
			memcpy(buildConfig, tpsMinAfrTableEthanolTable, sizeof(tpsMinAfrTableEthanolTable));
			buildConfig = (void*)((unsigned char *)buildConfig + sizeof(tpsMinAfrTableEthanolTable));

			//Startup AFR multiplier
			*((float *)buildConfig) = 0.9;
			buildConfig = (void*)((float *)buildConfig + 1);

			//Startup AFR delay
			*((float *)buildConfig) = 1;
			buildConfig = (void*)((float *)buildConfig + 1);

			//Startup AFR decay
			*((float *)buildConfig) = 10;
			buildConfig = (void*)((float *)buildConfig + 1);

			EXPECT_CALL(*_timerService, GetTicksPerSecond())
				.Times(1)
				.WillOnce(Return(5000));
			EngineManagement::CurrentAfrService = new EngineManagement::AfrService_Map_Ethanol(config);
		}

		TEST_METHOD(WhenGettingAfrThenCorrectAfrIsReturned)
		{
			CreateServices();

			EXPECT_CALL(*_timerService, GetTick()).Times(1).WillOnce(Return(0));
			EXPECT_CALL(*_decoder, GetRpm()).Times(1).WillOnce(Return(0));
			_mapService->MapKpa = 0;
			_ectService->EngineCoolantTemperature = -40;
			_tpsService->Tps = 0;
			_ethanolService->EthanolContent = 0;
			Assert::AreEqual(11.3034375f, EngineManagement::CurrentAfrService->GetAfr(), 0.1f);

		}

	};
}