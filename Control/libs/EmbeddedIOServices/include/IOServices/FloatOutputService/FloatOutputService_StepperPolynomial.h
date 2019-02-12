#include "HardwareAbstraction/HardwareAbstractionCollection.h"
#include "IOServices/StepperOutputService/IStepperOutputService.h"
#include "IFloatOutputService.h"
#include "math.h"
#include "Packed.h"

using namespace HardwareAbstraction;

#if !defined(FLOATOUTPUTSERVICE_STEPPERPOLYNOMIAL_H) && defined(IFLOATOUTPUTSERVICE_H) && defined(ISTEPPEROUTPUTSERVICE_H)
#define FLOATOUTPUTSERVICE_STEPPERPOLYNOMIAL_H
namespace IOServices
{
	PACK(
	template<unsigned char Degree>
	struct FloatOutputService_StepperPolynomialConfig
	{
	private:
		FloatOutputService_StepperPolynomialConfig()
		{
			
		}
			
	public:
		static FloatOutputService_StepperPolynomialConfig* Cast(void *p)
		{
			return (FloatOutputService_StepperPolynomialConfig *)p;
		}
			
		unsigned int Size()
		{
			return sizeof(FloatOutputService_StepperPolynomialConfig<Degree>);
		}
			
		float A[Degree+1];
		int MinStepPosition;
		int MaxStepPosition;
	});

	template<unsigned char Degree>
	class FloatOutputService_StepperPolynomial : public IFloatOutputService
	{
	protected:
		const FloatOutputService_StepperPolynomialConfig<Degree> *_config;

		IStepperOutputService *_stepperService;
		int _currentStepPosition;
			
	public:
		FloatOutputService_StepperPolynomial(const FloatOutputService_StepperPolynomialConfig<Degree> *config, IStepperOutputService *stepperService)
		{
			_config = config;
			_stepperService = stepperService;
			_currentStepPosition = 0;
		}
			
		void SetOutput(float value)
		{
			float newStepPosition = _config->A[0];
			for (int i = 1; i <= Degree; i++)
				newStepPosition += _config->A[i] * pow(value, i);
		
			if (newStepPosition > _config->MaxStepPosition)
				newStepPosition = _config->MaxStepPosition;
			else if (newStepPosition < _config->MinStepPosition)
				newStepPosition = _config->MinStepPosition;

			newStepPosition = round(newStepPosition);
		
			_stepperService->Step(newStepPosition - _currentStepPosition);
		
			_currentStepPosition = newStepPosition;
		}

		void Calibrate() 
		{ 
			_stepperService->Calibrate();
		}
	};
}
#endif