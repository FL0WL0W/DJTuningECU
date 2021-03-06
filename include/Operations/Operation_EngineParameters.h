#include "Operations/IOperation.h"
#include "Operations/Operation_EnginePosition.h"
#include <tuple>

#ifndef OPERATION_ENGINEPARAMETERS_H
#define OPERATION_ENGINEPARAMETERS_H
namespace OperationArchitecture
{
	class Operation_EngineParameters : public IOperation<std::tuple<float, bool>, EnginePosition>
	{
	protected:
		static Operation_EngineParameters *_instance;
		volatile bool _sequential;
	public:		
		std::tuple<float, bool> Execute(EnginePosition enginePosition) override;

		static IOperationBase *Create(const void *config, size_t &sizeOut);
		static Operation_EngineParameters *Construct();
	};
}
#endif