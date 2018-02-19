namespace Decoder
{
	class MockDecoder : public IDecoder
	{
	public:
		MOCK_METHOD0(GetCrankPosition, float());
		MOCK_METHOD0(GetCamPosition, float());
		MOCK_METHOD0(GetTickPerDegree, unsigned int());
		MOCK_METHOD0(GetRpm, unsigned short());
		MOCK_METHOD1(CrankEdgeTrigger, void(EdgeTrigger));
		MOCK_METHOD1(CamEdgeTrigger, void(EdgeTrigger));
		MOCK_METHOD0(IsSynced, bool());
		MOCK_METHOD0(HasCamPosition, bool());
	};
}