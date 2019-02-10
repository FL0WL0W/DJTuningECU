#ifndef IPRIMESERVICE_H
#define IPRIMESERVICE_H
namespace EngineControlServices
{
	class IPrimeService
	{
	public:
		virtual void Prime() = 0;
		virtual void Tick() = 0;

		static void PrimeCallBack(void *primeService);
		static void TickCallBack(void *primeService);
	};
}
#endif