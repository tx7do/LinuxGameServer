
#include "FPEnvironment.h"

FPEnvironment::FPEnvironment()
{
	fegetenv(&_env);
}


FPEnvironment::FPEnvironment(const FPEnvironment& env)
{
	_env = env._env;
}


FPEnvironment::~FPEnvironment()
{
	fesetenv(&_env);
}


FPEnvironment& FPEnvironment::operator = (const FPEnvironment& env)
{
	_env = env._env;
	return *this;
}

void FPEnvironment::keepCurrent()
{
	fegetenv(&_env);
}


void FPEnvironment::clearFlags()
{
	feclearexcept(FE_ALL_EXCEPT);
}
