#include <iostream>
#include <cassert>
#include <vector>
using namespace std;

struct A
{
	int a;
};
auto f = [](A* p)
{
	cout << "删除器1\n";
	delete p;
};
auto g = [](A* p)
{
	cout << "删除器2\n";
	delete p;
};
void func(A* p)
{
	delete p;
}
struct Foo
{
	void operator()(A* p)
	{
		delete p;
	}
};

auto make_a()
{
	auto f = [](A* p) {
		std::cout << "自定义删除!\n";
		delete p;
	};
	unique_ptr<A, decltype(f)> pNew{ new A,f };
	return pNew;
}

void unique_ptr_Func()
{
	unique_ptr<A> p1{ new A };
	unique_ptr<A, decltype(f)> p2{ new A,f };
	unique_ptr<A, decltype(func)*> p3{ new A,func };
	unique_ptr<A, decltype(Foo())> p4{ new A,Foo() };

	static_assert(sizeof(p1) == sizeof(nullptr));	//默认尺寸，一个nullptr
	static_assert(sizeof(p2) == sizeof(nullptr));	//无捕获的lamdba，相当于nullptr
	static_assert(sizeof(p3) == sizeof(nullptr) * 2);	// 函数指针占一个原始指针尺寸
	static_assert(sizeof(p4) == sizeof(nullptr));	//无状态的函数对象，但如果函数对象有状态（如数据成员、虚函数）就会增加尺寸


	auto darr = [](int* arr)
	{
		cout << "destroy! ";
		delete[] arr;
	};
	//unique_ptr的数组版本
	unique_ptr<int[], decltype(darr)> arr{ new int[3] {0,1,2},darr };
	for (int i = 0; i < 3; i++)
	{
		assert(arr[i] == i);
	}
}
void shared_ptr_Func()
{
	int* p = new int{ 42 };
	auto q = make_shared<int>(42);

	static_assert(sizeof(p) == sizeof(nullptr));
	static_assert(sizeof(q) == sizeof(nullptr) * 2);	//shared_ptr的尺寸是原始指针的两倍

	//shared_ptr无需在模板参数中指定删除器类型
	shared_ptr<A> pD1{ new A,f };	
	shared_ptr<A> pD2{ new A,g };

	//模板参数不含删除器类型，灵活性更高，不同的删除器可以放进同一个容器
	vector<shared_ptr<A>> arr{ pD1,pD2 };
}

int main()
{
	shared_ptr_Func();

	return 0;
}