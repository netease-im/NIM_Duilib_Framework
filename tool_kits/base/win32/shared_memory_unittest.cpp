// Copyright (c) 2011, NetEase Inc. All rights reserved.
// All rights reserved.
//
// Author: Wang Rongtao <rtwang@corp.netease.com>
// Date: 2011/7/4
//
// Unittest of Windows shared memory

#if defined(WITH_UNITTEST)
#include "base/win32/shared_memory.h"
#if defined(OS_WIN)
#include "gtest/gtest.h"

TEST(Win32, SharedMemory)
{
	const int kItemSize = sizeof(int);
	const int kItemCount = 1024;
	nbase::win32::SharedMemory memory;
	nbase::win32::SharedMemory memory2;
	EXPECT_TRUE(memory.Create(L"test", kItemCount * kItemSize));
	EXPECT_TRUE(memory2.Open(L"test"));
	{
		nbase::win32::SharedMemory::MappedView view1;
		nbase::win32::SharedMemory::MappedViewOf<int> view2;
		EXPECT_TRUE(view1.OpenView(&memory2, true));
		EXPECT_TRUE(view2.OpenView(&memory2, true));
		for (int i = 0; i < kItemCount; i++)
			view2.get()[i] = i;
		for (int i = 0; i < kItemCount; i++)
			EXPECT_EQ(i, view2.get()[i]);
	}
}

#endif // OS_WIN
#endif // WITH_UNITTEST