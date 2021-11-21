#pragma once

#include <fenv.h>
#include <cmath>


class FPEnvironment
{
public:
	enum RoundingMode
	{
#if defined(FE_DOWNWARD)
		FP_ROUND_DOWNWARD = FE_DOWNWARD,
#else
		FP_ROUND_DOWNWARD = 0,
#endif

#if defined(FE_UPWARD)
		FP_ROUND_UPWARD = FE_UPWARD,
#else
		FP_ROUND_UPWARD = 0,
#endif

#if defined(FE_TONEAREST)
		FP_ROUND_TONEAREST = FE_TONEAREST,
#else
		FP_ROUND_TONEAREST = 0,
#endif

#if defined(FE_TOWARDZERO)
		FP_ROUND_TOWARDZERO = FE_TOWARDZERO
#else
		FP_ROUND_TOWARDZERO = 0
#endif
	};

	enum Flag
	{
#if defined(FE_DIVBYZERO)
		FP_DIVIDE_BY_ZERO = FE_DIVBYZERO,
#else
		FP_DIVIDE_BY_ZERO = 0,
#endif

#if defined(FE_INEXACT)
		FP_INEXACT = FE_INEXACT,
#else
		FP_INEXACT = 0,
#endif

#if defined(FE_OVERFLOW)
		FP_OVERFLOW = FE_OVERFLOW,
#else
		FP_OVERFLOW = 0,
#endif

#if defined(FE_UNDERFLOW)
		FP_UNDERFLOW = FE_UNDERFLOW,
#else
		FP_UNDERFLOW = 0,
#endif

#if defined(FE_INVALID)
		FP_INVALID = FE_INVALID
#else
		FP_INVALID = 0
#endif
	};

	FPEnvironment();

	FPEnvironment(RoundingMode mode);

	FPEnvironment(const FPEnvironment& env);

	~FPEnvironment();

	FPEnvironment& operator = (const FPEnvironment& env);

	void keepCurrent();

	static void clearFlags();

	static bool isFlag(Flag flag);

	static void setRoundingMode(RoundingMode mode);

	static RoundingMode getRoundingMode();

	static bool isInfinite(float value);
	static bool isInfinite(double value);
	static bool isInfinite(long double value);

	static bool isNaN(float value);
	static bool isNaN(double value);
	static bool isNaN(long double value);

	static float copySign(float target, float source);
	static double copySign(double target, double source);
	static long double copySign(long double target, long double source);

private:
	fenv_t _env;
};


typedef FPEnvironment FPE;


inline bool FPEnvironment::isFlag(Flag flag)
{
	return fetestexcept(flag) != 0;
}


inline void FPEnvironment::setRoundingMode(RoundingMode mode)
{
	fesetround(mode);
}


inline FPEnvironment::RoundingMode FPEnvironment::getRoundingMode()
{
	return (RoundingMode)fegetround();
}


inline bool FPEnvironment::isInfinite(float value)
{
	return std::isinf(value) != 0;
}


inline bool FPEnvironment::isInfinite(double value)
{
	return std::isinf(value) != 0;
}


inline bool FPEnvironment::isInfinite(long double value)
{
	return std::isinf((double)value) != 0;
}


inline bool FPEnvironment::isNaN(float value)
{
	return std::isnan(value) != 0;
}


inline bool FPEnvironment::isNaN(double value)
{
	return std::isnan(value) != 0;
}


inline bool FPEnvironment::isNaN(long double value)
{
	return std::isnan((double)value) != 0;
}


inline float FPEnvironment::copySign(float target, float source)
{
	return copysignf(target, source);
}


inline double FPEnvironment::copySign(double target, double source)
{
	return copysign(target, source);
}


inline long double FPEnvironment::copySign(long double target, long double source)
{
	return (source >= 0 && target >= 0) || (source < 0 && target < 0) ? target : -target;
}
