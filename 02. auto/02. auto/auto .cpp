#include <iostream>
#include <vector>
#include <functional>

template <class It>
void f(It first, It last)
{
	while (first != last)
	{
		auto val = *first;	
	}
}

int main()
{
	using namespace std;
#if 0
	vector<int> vec1{ 1,2,3,4,5 };
	auto it1 = vec1.begin();
	auto it2 = vec1.end();
	f(it1,it2);
#elif 0
	int a = 1, b = 2;
	auto f1 = [](auto& x, auto& y) {return x < y; };
	function<bool(int&, int&)> f2 = [](auto& a, auto& b) {return a < b; };
	cout << f1(a, b) << " " << f2(a, b);
#elif 1
	vector<bool> BOOL{ true,false,true };
	//for (auto& x:BOOL)
	bool x = BOOL[0];
	auto x2 = BOOL[0];	
	cout << x2;
#endif
	system("pause");
	return 0;
}