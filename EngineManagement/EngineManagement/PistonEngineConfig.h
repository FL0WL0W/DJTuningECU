namespace EngineManagement
{
	class PistonEngineConfig
	{
	public:
		PistonEngineConfig(void *config)
		{
			Cylinders = *((unsigned char *)config);
			config = (void *)(((unsigned char *)config) + 1);
			
			Ml8thPerCylinder = *((unsigned short *)config); //1/8 ml
			config = (void *)(((unsigned short *)config) + 1);
						
			IsThrottleBodyInjection = *((bool *)config);
			config = (void *)(((bool *)config) + 1);
			
			IsDistributor = *((bool *)config);
			config = (void *)(((bool *)config) + 1);
			
			DecoderId = *((unsigned char *)config);
			config = (void *)(((unsigned char *)config) + 1);
		}
		unsigned char Cylinders;
		unsigned short Ml8thPerCylinder;
		bool IsThrottleBodyInjection;
		bool IsDistributor;
		unsigned char DecoderId;
	};
}