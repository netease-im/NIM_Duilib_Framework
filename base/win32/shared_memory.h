// A wrapper class of Windows shared memory

#ifndef BASE_SHARED_MEMORY_H_
#define BASE_SHARED_MEMORY_H_

#include "base/base_config.h"

#if defined(OS_WIN)

#include "base/base_export.h"
#include <assert.h>
#include <windows.h>

namespace nbase
{
namespace win32
{

class BASE_EXPORT SharedMemory
{
public:
	SharedMemory() : mapping_(NULL) {}
	~SharedMemory() { Close(); };
	bool Create(const wchar_t *name, int size);
	bool Open(const wchar_t *name);
	void Close();

	class MappedView
	{
	public:
		MappedView() : view_(NULL) {}
		~MappedView() { CloseView(); }
		bool OpenView(SharedMemory *shared_memory, bool read_write);
		void CloseView();
		unsigned char *view() { return view_; }

	private:
		unsigned char *view_;
	};

	template<class T>
	class MappedViewOf : public MappedView
	{
	public:
		T *get() { return reinterpret_cast<T*>(view()); }
		T* operator->()
		{
			assert(view() != NULL);
			return reinterpret_cast<T*>(view());
		}
	};

private:
	friend class MappedView;
	HANDLE mapping_;
};

} // namespace win32
} // namespace nbase

#endif // OS_WIN
#endif // BASE_SHARED_MEMORY_H_
