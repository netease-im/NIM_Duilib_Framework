// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: Ruan Liang <ruanliang@corp.netease.com>
// Date: 2011/10/6
//
// bind unittest

#if defined(WITH_UNITTEST)

#include "base/util/bind.h"
#include "base/util/callback.h"

#include "gtest/gtest.h"

using namespace nbase;


static const int kParentValue = 1;
static const int kChildValue  = 2;

class Parent
{
public:
	void AddRef(void) const {}
	void Release(void) const {}
	virtual void VirtualSet() { value = kParentValue; }
	void NonVirtualSet() { value = kParentValue; }
	int value;
};

class Child : public Parent
{
public:
	virtual void VirtualSet() { value = kChildValue; }
	void NonVirtualSet() { value = kChildValue; }
};

class NoRefParent
{
public:
	virtual void VirtualSet() { value = kParentValue; }
	void NonVirtualSet() { value = kParentValue; }
	int value;
};

class NoRefChild : public NoRefParent
{
	virtual void VirtualSet() { value = kChildValue; }
	void NonVirtualSet() { value = kChildValue; }
};

// Used for probing the number of copies that occur if a type must be coerced
// during argument forwarding in the Run() methods.
struct DerivedCopyCounter
{
	DerivedCopyCounter(int *copies, int *assigns)
		: copies_(copies), assigns_(assigns)
	{
	}
	int *copies_;
	int *assigns_;
};

// Used for probing the number of copies in an argument.
class CopyCounter
{
public:
	CopyCounter(int *copies, int *assigns)
		: copies_(copies), assigns_(assigns)
	{
	}

	CopyCounter(const CopyCounter& other)
		: copies_(other.copies_),
		assigns_(other.assigns_)
	{
			(*copies_)++;
	}

	// Probing for copies from coerscion.
	CopyCounter(const DerivedCopyCounter &other)
		: copies_(other.copies_),
		assigns_(other.assigns_) {
			(*copies_)++;
	}

	const CopyCounter& operator=(const CopyCounter &rhs)
	{
		copies_ = rhs.copies_;
		assigns_ = rhs.assigns_;

		if (assigns_)
		{
			(*assigns_)++;
		}

		return *this;
	}

	int copies() const
	{
		return *copies_;
	}

	int assigns() const
	{
		return *assigns_;
	}

private:
	int* copies_;
	int* assigns_;
};

// Some test functions that we can Bind to.
template <typename T>
T PolymorphicIdentity(T t)
{
	return t;
}

template <typename T>
void VoidPolymorphic1(T t)
{
}

int Identity(int n)
{
	return n;
}

int ArrayGet(const int array[], int n)
{
	return array[n];
}

int Sum(int a, int b, int c, int d, int e, int f)
{
	return a + b + c + d + e + f;
}

const char* CStringIdentity(const char *s)
{
	return s;
}

int GetCopies(const CopyCounter& counter)
{
	return counter.copies();
}

int UnwrapNoRefParent(NoRefParent p)
{
	return p.value;
}

int UnwrapNoRefParentPtr(NoRefParent* p)
{
	return p->value;
}

int UnwrapNoRefParentConstRef(const NoRefParent& p)
{
	return p.value;
}

void RefArgSet(int &n)
{
	n = 2;
}

// Only useful in no-compile tests.
int UnwrapNoRefParentRef(Parent& p)
{
	return p.value;
}

TEST(BindTest, Basic)
{
	Callback<int(void)> c0 = Bind(&Sum, 32, 16, 8, 4, 2, 1);
	EXPECT_EQ(63, c0.Run());

	Callback<int(int)> c1 = Bind(&Sum, 32, 16, 8, 4, 2);
	EXPECT_EQ(75, c1.Run(13));

	Callback<int(int,int)> c2 = Bind(&Sum, 32, 16, 8, 4);
	EXPECT_EQ(85, c2.Run(13, 12));

	Callback<int(int,int,int)> c3 = Bind(&Sum, 32, 16, 8);
	EXPECT_EQ(92, c3.Run(13, 12, 11));

	Callback<int(int,int,int,int)> c4 = Bind(&Sum, 32, 16);
	EXPECT_EQ(94, c4.Run(13, 12, 11, 10));

	Callback<int(int,int,int,int,int)> c5 = Bind(&Sum, 32);
	EXPECT_EQ(87, c5.Run(13, 12, 11, 10, 9));

	Callback<int(int,int,int,int,int,int)> c6 = Bind(&Sum);
	EXPECT_EQ(69, c6.Run(13, 12, 11, 10, 9, 14));
}

// Argument binding tests.
//   - Argument binding to primitive.
//   - Argument binding to primitive pointer.
//   - Argument binding to a literal integer.
//   - Argument binding to a literal string.
//   - Argument binding with template function.
//   - Argument binding to an object.
//   - Argument gets type converted.
//   - Pointer argument gets converted.
//   - Const Reference forces conversion.
TEST(BindTest, ArgumentBinding)
{
	int n = 2;

	Callback<int(void)> bind_primitive_cb = Bind(&Identity, n);
	EXPECT_EQ(n, bind_primitive_cb.Run());

	Callback<int*(void)> bind_primitive_pointer_cb =
		Bind(&PolymorphicIdentity<int*>, &n);
	EXPECT_EQ(&n, bind_primitive_pointer_cb.Run());

	Callback<int(void)> bind_int_literal_cb = Bind(&Identity, 3);
	EXPECT_EQ(3, bind_int_literal_cb.Run());

	Callback<const char*(void)> bind_string_literal_cb =
		Bind(&CStringIdentity, "hi");
	EXPECT_STREQ("hi", bind_string_literal_cb.Run());

	Callback<int(void)> bind_template_function_cb =
		Bind(&PolymorphicIdentity<int>, 4);
	EXPECT_EQ(4, bind_template_function_cb.Run());

	NoRefParent p;
	p.value = 5;
	Callback<int(void)> bind_object_cb = Bind(&UnwrapNoRefParent, p);
	EXPECT_EQ(5, bind_object_cb.Run());

	NoRefChild c;
	c.value = 6;
	Callback<int(void)> bind_promotes_cb = Bind(&UnwrapNoRefParent, c);
	EXPECT_EQ(6, bind_promotes_cb.Run());

	c.value = 7;
	Callback<int(void)> bind_pointer_promotes_cb =
		Bind(&UnwrapNoRefParentPtr, &c);
	EXPECT_EQ(7, bind_pointer_promotes_cb.Run());

	c.value = 8;
	Callback<int(void)> bind_const_reference_promotes_cb =
		Bind(&UnwrapNoRefParentConstRef, c);
	EXPECT_EQ(8, bind_const_reference_promotes_cb.Run());
}

// Unbound argument type support tests.
//   - Unbound value.
//   - Unbound pointer.
//   - Unbound reference.
//   - Unbound const reference.
//   - Unbound unsized array.
//   - Unbound sized array.
//   - Unbound array-of-arrays.
TEST(BindTest, UnboundArgumentTypeSupport)
{
	Callback<void(int)> unbound_value_cb = Bind(&VoidPolymorphic1<int>);
	Callback<void(int*)> unbound_pointer_cb = Bind(&VoidPolymorphic1<int*>);
	Callback<void(int&)> unbound_ref_cb = Bind(&VoidPolymorphic1<int&>);
	Callback<void(const int&)> unbound_const_ref_cb =
		Bind(&VoidPolymorphic1<const int&>);
	Callback<void(int[])> unbound_unsized_array_cb =
		Bind(&VoidPolymorphic1<int[]>);
	Callback<void(int[2])> unbound_sized_array_cb =
		Bind(&VoidPolymorphic1<int[2]>);
	Callback<void(int[][2])> unbound_array_of_arrays_cb =
		Bind(&VoidPolymorphic1<int[][2]>);
}

// Function with unbound reference parameter.
//   - Original paraemter is modified by callback.
TEST(BindTest, UnboundReferenceSupport)
{
	int n = 0;
	Callback<void(int&)> unbound_ref_cb = Bind(&RefArgSet);
	unbound_ref_cb.Run(n);
	EXPECT_EQ(2, n);
}

// Functions that take reference parameters.
//  - Forced reference parameter type still stores a copy.
//  - Forced const reference parameter type still stores a copy.
TEST(BindTest, ReferenceArgumentBinding)
{
	int n = 1;
	int& ref_n = n;
	const int& const_ref_n = n;

	Callback<int(void)> ref_copies_cb = Bind(&Identity, ref_n);
	EXPECT_EQ(n, ref_copies_cb.Run());
	n++;
	EXPECT_EQ(n - 1, ref_copies_cb.Run());

	Callback<int(void)> const_ref_copies_cb = Bind(&Identity, const_ref_n);
	EXPECT_EQ(n, const_ref_copies_cb.Run());
	n++;
	EXPECT_EQ(n - 1, const_ref_copies_cb.Run());
}

// Check that we can pass in arrays and have them be stored as a pointer.
//  - Array of values stores a pointer.
//  - Array of const values stores a pointer.
TEST(BindTest, ArrayArgumentBinding)
{
	int array[4] = {1, 1, 1, 1};
	const int (*const_array_ptr)[4] = &array;

	Callback<int(void)> array_cb = Bind(&ArrayGet, array, 1);
	EXPECT_EQ(1, array_cb.Run());

	Callback<int(void)> const_array_cb = Bind(&ArrayGet, *const_array_ptr, 1);
	EXPECT_EQ(1, const_array_cb.Run());

	array[1] = 3;
	EXPECT_EQ(3, array_cb.Run());
	EXPECT_EQ(3, const_array_cb.Run());
}

// ConstRef() wrapper support.
//   - Binding w/o ConstRef takes a copy.
//   - Binding a ConstRef takes a reference.
//   - Binding ConstRef to a function ConstRef does not copy on invoke.
TEST(BindTest, ConstRef)
{
	int n = 1;

	Callback<int(void)> copy_cb = Bind(&Identity, n);
	Callback<int(void)> const_ref_cb = Bind(&Identity, ConstRef(n));
	EXPECT_EQ(n, copy_cb.Run());
	EXPECT_EQ(n, const_ref_cb.Run());
	n++;
	EXPECT_EQ(n - 1, copy_cb.Run());
	EXPECT_EQ(n, const_ref_cb.Run());

	int copies = 0;
	int assigns = 0;
	CopyCounter counter(&copies, &assigns);
	Callback<int(void)> all_const_ref_cb =
		Bind(&GetCopies, ConstRef(counter));
	EXPECT_EQ(0, all_const_ref_cb.Run());
	EXPECT_EQ(0, copies);
	EXPECT_EQ(0, assigns);
}

// Argument Copy-constructor usage for non-reference parameters.
//   - Bound arguments are only copied once.
//   - Forwarded arguments are only copied once.
//   - Forwarded arguments with coerscions are only copied twice (once for the
//     coerscion, and one for the final dispatch).
TEST(BindTest, ArgumentCopies)
{
	int copies = 0;
	int assigns = 0;

	CopyCounter counter(&copies, &assigns);

	Callback<void(void)> copy_cb =
		Bind(&VoidPolymorphic1<CopyCounter>, counter);
	EXPECT_GE(1, copies);
	EXPECT_EQ(0, assigns);

	copies = 0;
	assigns = 0;
	Callback<void(CopyCounter)> forward_cb =
		Bind(&VoidPolymorphic1<CopyCounter>);
	forward_cb.Run(counter);
	EXPECT_GE(1, copies);
	EXPECT_EQ(0, assigns);

	copies = 0;
	assigns = 0;
	DerivedCopyCounter dervied(&copies, &assigns);
	Callback<void(CopyCounter)> coerce_cb =
		Bind(&VoidPolymorphic1<CopyCounter>);
	coerce_cb.Run(dervied);
	EXPECT_GE(2, copies);
	EXPECT_EQ(0, assigns);
}


#endif  // WITH_UNITTEST