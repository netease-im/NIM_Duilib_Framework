// cpu description

#ifndef BASE_HARDWARE_CPU_H_
#define BASE_HARDWARE_CPU_H_

#include <string>
#include "base/base_config.h"
#include "base/base_export.h"

namespace nbase
{
// Query information about the processor.
class BASE_EXPORT CPU {
public:
	// Constructor
	CPU();

	// Accessors for CPU information.
	const std::string& vendor_name() const { return cpu_vendor_; }
	int stepping() const { return stepping_; }
	int model() const { return model_; }
	int family() const { return family_; }
	int type() const { return type_; }
	int extended_model() const { return ext_model_; }
	int extended_family() const { return ext_family_; }
	int has_mmx() const { return has_mmx_; }
	int has_sse() const { return has_sse_; }
	int has_sse2() const { return has_sse2_; }
	int has_sse3() const { return has_sse3_; }
	int has_ssse3() const { return has_ssse3_; }
	int has_sse41() const { return has_sse41_; }
	int has_sse42() const { return has_sse42_; }

private:
	// Query the processor for CPUID information.
	void Initialize();

	int type_;  // process type
	int family_;  // family of the processor
	int model_;  // model of processor
	int stepping_;  // processor revision number
	int ext_model_;
	int ext_family_;
	bool has_mmx_;
	bool has_sse_;
	bool has_sse2_;
	bool has_sse3_;
	bool has_ssse3_;
	bool has_sse41_;
	bool has_sse42_;
	std::string cpu_vendor_;
};

#if defined(ARCH_CPU_X86_FAMILY)
#ifndef COMPILER_MSVC
#if defined(__pic__) && defined(__i386__)
BASE_EXPORT void __cpuid(int cpu_info[4], int info_type);
BASE_EXPORT void __cpuidex(int cpu_info[4], int info_type, int info_index);
#else
BASE_EXPORT void __cpuid(int cpu_info[4], int info_type);
BASE_EXPORT void __cpuidex(int cpu_info[4], int info_type, int info_index);
#endif // __i386___
#endif // COMPILER_MSVC
#endif // ARCH_CPU_X86_FAMILY

}  // namespace

#endif // BASE_HARDWARE_CPU_H_
