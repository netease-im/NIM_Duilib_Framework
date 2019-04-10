// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: Ruan Liang <ruanliang@corp.netease.com>
// Date: 2011/6/23
//
// CPU Unittest

#if defined(WITH_UNITTEST)

#include "base/hardware/cpu.h"
#include "gtest/gtest.h"

TEST(CPU, RunExtendedInstructions)
{
#if defined(ARCH_CPU_X86_FAMILY)
	// Retrieve the CPU information.
	nbase::CPU cpu;

#if defined(OS_WIN)
	ASSERT_TRUE(!!cpu.has_mmx());

	// Execute an MMX instruction.
	__asm emms;

	if (cpu.has_sse())
	{
		// Execute an SSE instruction.
		__asm xorps xmm0, xmm0;
	}

	if (cpu.has_sse2())
	{
		// Execute an SSE 2 instruction.
		__asm psrldq xmm0, 0;
	}

	if (cpu.has_sse3())
	{
		// Execute an SSE 3 instruction.
		__asm addsubpd xmm0, xmm0;
	}

	if (cpu.has_ssse3())
	{
		// Execute a Supplimental SSE 3 instruction.
		__asm psignb xmm0, xmm0;
	}

	if (cpu.has_sse41())
	{
		// Execute an SSE 4.1 instruction.
		__asm pmuldq xmm0, xmm0;
	}

	if (cpu.has_sse42())
	{
		// Execute an SSE 4.2 instruction.
		__asm crc32 eax, eax;
	}
#elif defined(OS_POSIX) && defined(__x86_64__)
	ASSERT_TRUE(cpu.has_mmx());

	// Execute an MMX instruction.
	__asm__ __volatile__("emms\n" : : : "mm0");

	if (cpu.has_sse())
	{
		// Execute an SSE instruction.
		__asm__ __volatile__("xorps %%xmm0, %%xmm0\n" : : : "xmm0");
	}

	if (cpu.has_sse2())
	{
		// Execute an SSE 2 instruction.
		__asm__ __volatile__("psrldq $0, %%xmm0\n" : : : "xmm0");
	}

	if (cpu.has_sse3())
	{
		// Execute an SSE 3 instruction.
		__asm__ __volatile__("addsubpd %%xmm0, %%xmm0\n" : : : "xmm0");
	}

	if (cpu.has_ssse3())
	{
		// Execute a Supplimental SSE 3 instruction.
		__asm__ __volatile__("psignb %%xmm0, %%xmm0\n" : : : "xmm0");
	}

	if (cpu.has_sse41())
	{
		// Execute an SSE 4.1 instruction.
		__asm__ __volatile__("pmuldq %%xmm0, %%xmm0\n" : : : "xmm0");
	}

	if (cpu.has_sse42())
	{
		// Execute an SSE 4.2 instruction.
		__asm__ __volatile__("crc32 %%eax, %%eax\n" : : : "eax");
	}
#endif
#endif
}

#endif  // WITH_UNITTEST

