#include <iostream>
#include <vector>
#include <type_traits>
#include <tuple>
using namespace std;

#if 0
struct Foo
{
	int a{ 1 };
	int b = 2;
	//int c(3);	 //错误，无法用小括号指定初始值
};

struct Aoo
{
	Aoo() { cout << 1 << endl; }
	Aoo(int a) { cout << 2 << endl; }
	Aoo(initializer_list<int>) { cout << 3 << endl; }

};

template <typename T, typename... Ts>
decltype(auto) f(Ts&&... args) {
	T x(std::forward<Ts>(args)...);  // 用小括号初始化创建临时对象
	return x;
}

template <typename T, typename... Ts>
decltype(auto) g(Ts&&... args)
{
	T x{ std::forward<Ts>(args)... };
	return x;
}

int main()
{
	printf("beg\n");
	double a = 1, b = 10.5;
	//int num1{ a + b };	//error: 大括号不允许从double到int的收缩转换
	int num2(a + b);	//小括号允许截断
	int num3 = a + b;	//截断

	//大括号的优势
	Aoo aoo1();	//注意： 使用小括号不会被解析为构造函数的执行，而是被解析为一个函数
	Aoo aoo2{};	//使用大括号调用构造函数
	
	//大括号的劣势
	Aoo aoo3{ 5 };	//优先匹配 initiailzer_list 
	Aoo aoo4(5);	//小括号则优先匹配int 
	Aoo aoo5{};		//大括号参数为空，调用参数为空的构造函数
	Aoo aoo6{ {} };	//大/小括号里面再添加一层大括号，则调用的是空的 initiailzer_list

	auto vec1 =  f<vector<int>>(5, 5);	//小括号： 5 5 5 5 5
	auto vec2 = g<vector<int>>(5, 5);	//大括号： 5 5

	return 0;
}

#elif 0
void f1(shared_ptr<int>) {}
void f2(unique_ptr<int>) {}
void f3(int*) {}

template<typename Fun,typename T>
void g(Fun F, T data)
{
	F(data);
}
int main()
{
	using namespace Use_nullptr;

	//g(f1, 0);	//error
	//g(f2, NULL);//error
	g(f1, nullptr);
	g(f2, nullptr);
	g(f3, nullptr);
	return 0;
}
#elif 0

template <typename T>
using V1 = std::vector<T>;	//using直接声明

template <typename T>
struct V2
{
	typedef std::vector<T> type;	//typedef的复杂声明
};

//template <typename T>
//typedef std::vector<T> V3;	//error: 此处无法使用typedef

int main()
{
	V1<int> vec1;
	V2<int>::type vec2;
	//判断const：
	cout << "int: " << is_const<int>::value << endl;
	cout << "const int: " << is_const<const int>::value << endl;
	//判断same
	cout << "int and unsigned int: " << is_same<int, unsigned>::value << endl;

	typedef conditional<true, int, char>::type A;

	return 0;
}
#elif 0

enum { a, b, c };
enum class A { a, b, c };
//指定枚举底层类型
enum class B :__int32 { a, b, c };

//封装转换过程
template <typename T>
constexpr auto f(T t) { return static_cast<std::underlying_type_t<T>>(t); }
int main()
{
	//int a = 10;	//error： a已经在enum中出现过
	//cout << a << b << c << endl;

	A val = A::a;
	if (a < 10) {}		//enum： 允许比较
	//if (A::a < 10) {}	//enum class：不允许比较
	if (static_cast<int>(A::b) < 10) {}	//允许将enum class强制转换为int类型，然后比较

	//希望用到 enum 的隐式转换时，使用enum
	enum stu { name, age, number };
	auto t = make_tuple("ylh", 18, 202112048);
	
	auto x = get<stu::name>(t);// name可以隐式转换get的模板参数类型：size_t
	cout << x << endl;	//ylh

	//enum class需要强制转换
	enum class stu2 { name, age, number };
	auto t2 = make_tuple("abc", 20, 232325695, vector<int>{1, 2, 3, 4, 5});
	auto x2 = get<static_cast<size_t>(stu2::number)>(t2);
	auto x3 = get<f(stu2::name)>(t2);
	cout << x2 << endl;	//abc

	//get的推断：get可以按照下标对tuple进行访问，c++14后可以根据类型访问
	//const char* s = nullptr;
	//vector<int> vec{};
	//auto x3 = get<const char*>(t2);	//类型访问
	//auto x4 = get<decltype(s)>(t2);	//decltype推断
	//auto& x5 = get<decltype(vec)>(t2);

	return 0;
}
#elif 0
class Foo
{
public:
	Foo() = delete;
	Foo(int) {}
	Foo(string) {}
	template <typename T>
	void fun(T x) { cout << x << endl; }
private:
};

template <typename T>
void f(T x) { cout << __FUNCTION__; }

template <>
void f<string>(string) = delete;	//删除某个特定的模板特化
template <>
void Foo::fun<double>(double) = delete;

int main()
{
	//Foo a{};	//删除默认构造函数

	f(10);
	//f(string("a")); error：模板特化

	Foo a{ 10 };
	//a.fun(50.555);	//error：删除double的类内特化

	return 0;
}
#elif 0
struct A
{
	constexpr int f()& { cout << 1 << endl; }
	constexpr int f()&&
	{
		cout << 2 << endl;
	}
};
constexpr A make_a() { return A{}; }

class Foo //final	//声明这个类不可被继承
{
public:
	Foo();
	virtual void fun1();
	virtual void fun2();
	virtual void fun3()final;	//禁止重写
};

class Boo :public Foo
{
public:
	Boo();
	virtual void fun1()override;
	virtual void fun2()override;
	//virtual void fun3()override;	
};
int main()
{
	
	return 0;
}
#elif 0

//手动实现cbegin和cend
template <typename T>
auto my_cbegin(const T& t) -> decltype(begin(t))
{
	return begin(t);
}
template <typename T>
auto my_cend(const T& t) -> decltype(auto)
{
	return end(t);
}
int main()
{
	vector<int> a{ 1,2,3,4,5 };
	auto it1 =  my_cbegin(a);
	auto it2 = my_cend(a);
	while (it1 != it2)
	{
		cout << *it1 << " ";
		it1++;
	}
	return 0;
}
#elif 0

//noexcept： 不允许抛出异常  throw()
//noexcept(false): 允许抛出异常  throw(...)
int m_div(int a, int b)noexcept(false)//throw(...)
{
	if (b == 0)
	{
		throw(-1);
	}
	a / b;
}
int m_div2(int a, int b)noexcept(true)
{
	return a / b;
}
int main()
{
	try
	{
		auto a = m_div(5, 0);
	}
	catch (int errorcode)
	{
		printf("除数不能是0,%d\n",errorcode);
	}

	return 0;
}
#elif 0

//常量表达式函数的返回值必须是常量表达式的原因很简单:
//如果想在程序编译阶段获得某个函数返回的常量，则该函数的 return 语句中就不能包含程序运行阶段才能确定值的变量。
constexpr int getlen(int len)noexcept;
constexpr int getlen(int len)noexcept
{
	return 1 + 2 + len;	// 常量表达式
}
constexpr int m_pow(int base, int exp)noexcept
{
	return (exp == 0) ? 1 : base * m_pow(base, exp - 1);
}
//constexpr在类中的实现
class Foo
{
public:
	//注意，constexpr 修饰类的构造函数时：
	//要求该构造函数的函数体必须为空，且采用初始化列表的方式为各个成员赋值时，必须使用常量表达式。
	constexpr Foo(int a, const char* s)noexcept
		:a(a), name(s) {}
	constexpr int GetA()const { return a; }
	constexpr auto GetName()const -> decltype(auto) { return name; }
private:
	int a;
	const char* name;
};

constexpr Foo ExChange(const Foo& a, const Foo& b)
{
	return { a.GetA() / 2,b.GetName() };
}

template <typename T>
constexpr T m_copy(const T& t)noexcept
{
	return t;
}
int main()
{
	int num = 5;
	//int arr[num] = { 1,2,3,4,5 }; //num不是常量表达式
	const int num2 = 5;
	constexpr int num3 = 5;
	static_assert(num2 == 5, "false");

	int arr[num3] = { 1,2,3,4,5 };
	int arr2[getlen(5)] = { 1,2,3,4,5 };

	//constexprd的递归
	// mov dword ptr [res],20h  
	constexpr int res = m_pow(2, 5);//编译时计算
	static_assert(res == 32, "false");
	cout << res << endl;

	constexpr Foo a{ 10, "ylh"};	//编译时初始化

	//如果缺少 constexpr：call  m_copy<Foo> (07FF7B96F13DEh)  说明这就是一个普通的函数
	//如果带上 constexpr，则会发现不会调用call，即成功实现了常量表达式函数
	constexpr Foo b =  m_copy(a);

	//   call ExChange (07FF6F75013F7h)  
	constexpr Foo ExRes = ExChange(a, b);

	static_assert(ExRes.GetA() == 5, "测试");
	return 0;
}
//constexpr int getlen(int len)noexcept	//error： 必须放在调用者的前面
//{
//	return len;
//}
#endif