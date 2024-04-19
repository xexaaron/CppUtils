#pragma once

#ifdef __GNUC__
									#define VERBOSE_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
									#define VERBOSE_FUNC_SIG __FUNCSIG__
#else  // Unknown
	#if defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600) || defined(__IBMCPP__) && (__IBMCPP__ >= 500)
									#define VERBOSE_FUNC_SIG __FUNCTION__	
	#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
									#define VERBOSE_FUNC_SIG __FUNC__
	#else
									#define VERBOSE_FUNC_SIG __func__
	#endif // __FUNCTION__
#endif // __GNUC__
