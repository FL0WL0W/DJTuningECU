namespace EngineManagement
{
	class TpsService_Analog : public ITpsService
	{
	protected:
		unsigned char _adcPin;
		float A0, A1, A2, A3;
		unsigned int _lastReadTick;
		unsigned short _dotSampleRate;
	public:
		TpsService_Analog(unsigned char adcPin, void *config);
		void ReadTps();
	};
}