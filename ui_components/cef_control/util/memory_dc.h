/** @file memory_dc.h
 * @brief 内存dc类，维护HBITMAP与HDC对象，方便快速的储存位图数据
 * @copyright (c) 2016, NetEase Inc. All rights reserved
 * @author Redrain
 * @date 2016/7/19
 */
#pragma once

class MemoryDC
{
public:
	MemoryDC();
	~MemoryDC();

	// 初始化内存dc数据，并创建出对应的位图对象
	// 如果pBits参数不为空，则用pBits参数的数据填充位图
	// 此函数可以反复调用，会自动删除原来的位图对象
	bool Init(HDC src_dc, int width, int height, const LPVOID pBits = NULL);

	// 删除位图对象并初始化变量
	void DeleteDC();

	// 判断内存dc是否可以使用
	bool IsValid();

	HDC GetDC();
	HBITMAP GetBitmap();

	// 获取内存位图数据指针，可用于填充位图
	BYTE* GetBits();
	// 获取内存位图的宽度
	int	GetWidth();
	// 获取内存位图的高度
	int GetHeight();

private:
	bool CreateMemoryDC(HDC src_dc, int width, int height, void** pBits);

private:
	HDC		mem_dc_;
	HBITMAP	bitmap_;
	HBITMAP	old_bitmap_;
	BITMAP	bitmap_info_;

	SIZE	bitmap_size_;
	bool	valid_;
};