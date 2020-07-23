/*
1. 当(T&& t)作为模板参数时, 是否为右值引用需看末班参数接收的实际类型, 和实际类型匹配( 看传的参数)
	其他情况都是右值引用
2. 排序
	1. 通过大于小于比较 
	2. 仿函数GreaterA(), LessA()
3. 最简单的lanmbda表达式定义
	[]{};
	mutable: 把捕捉类表当中是变量属性改为非const(默认是const属性)
	[a,b]()mutable{a = 10; b=20; return a + b;};    有mutable必须得有(),
	auto func = []()mutable{};
	//下面这个才是lambda调用/使用
	func(a,b);
*/
#include <thread>
#include <algorithm>
#include <iostream>
#include <functional>
using namespace std;

void testR()
{
	int a = 10;
	//std::move:  移动语义--> 把变量的属性变成右值 
	int&& rr = move(a);
}


class String
{
public:
	String(char* str = "")
		:_str(new char[strlen(str) + 1])
		, _size(strlen(str))
		, _capacity(strlen(str))
	{
		strcpy(_str, str);
		cout << "String(char*)" << endl;
	}

	String(const String& str)
		:_str(new char[strlen(str._str) + 1])
	{
		strcpy(_str, str._str);
		_size = _capacity = strlen(str._str);
		cout << "String(const String&)" << endl;
	}
	//移动构造： 提高拷贝效率
	String(String&& str)
		:_str(str._str)
	{
		str._str = nullptr;
		_size = _capacity = str._size;
		cout << "String(String&&)" << endl;
	}

	String& operator=(const String& str)
	{
		if (this != &str)
		{
			delete[] _str;
			_str = new char[strlen(str._str) + 1];
			strcpy(_str, str._str);
			_size = _capacity = strlen(str._str);
			cout << "String& operator=(const String&)" << endl;
		}
		return *this;
	}

	//移动赋值
	String& operator=(String&& str)
	{
		if (this != &str)
		{
			swap(_str, str._str);
			_size = _capacity = str._size;
			cout << "String operator=(String&&)" << endl;
		}

		return *this;
	}

	~String()
	{
		cout << "调用析构" << endl;
		if (_str)
		{

			delete[] _str;
			cout << "~String()" << endl;
		}

	}

	//private:
public:
	char* _str;
	int _size;
	int _capacity;
};

class Person
{
public:
	Person(const String& name)
		:_name(name)
	{}

	Person(const Person& person)
		:_name(person._name)
	{
		cout << "Person(const Person&)" << endl;
	}

	//移动语义正确场景：
	Person(Person&& person)
		:_name(move(person._name))
	{
		cout << "Person(Person&&)" << endl;
	}
private:
	String _name;
};

Person getPerson()
{
	Person per("bite");
	return per;
}

void testMove()
{
	Person copy(getPerson());
}

void testMove2()
{
	String str("123");
	//移动语义错误示例：把一个后面会用到的左值变成了一个右值
	String copy(move(str));

	cout << str._str << endl;
	//移动语义正确的使用场景：需要保证属性被修改的左值后面不会再用到
}

void Fun(int &x){ cout << "lvalue ref" << endl; }
void Fun(int &&x){ cout << "rvalue ref" << endl; }
void Fun(const int &x){ cout << "const lvalue ref" << endl; }
void Fun(const int &&x){ cout << "const rvalue ref" << endl; }

//特殊： 模板参数： T&& 未定引用类型---> 主要看模板参数接收的实际类型，和实际类型匹配
template<typename T>
//void PerfectForward(const T& t){ Fun(t); }
void PerfectForward(T &&t){ Fun(std::forward<T>(t)); }
void testForward()
{
	PerfectForward(10); // rvalue ref
	int a = 0;
	PerfectForward(a); // lvalue ref
	PerfectForward(std::move(a)); // rvalue ref
	const int b = 8;
	PerfectForward(b); // const lvalue ref
	PerfectForward(std::move(b)); // const rvalue ref

}

class A
{
public:
	A(int a = 0)
		:_a(a)
	{}

	bool operator<(const A& a) const
	{
		return _a < a._a;
	}

	bool operator>(const A& a) const
	{
		return _a > a._a;
	}
	//private:
public:
	int _a;
};

struct LessA
{
	bool operator()(const A& a1, const A& a2)
	{
		return a1 < a2;
	}
};

struct GreaterA
{
	bool operator()(const A& a1, const A& a2)
	{
		return a1 > a2;
	}
};


void testSort()
{
	int array[] = { 4, 1, 8, 5, 3, 7, 0, 9, 2, 6 };
	// 默认按照小于比较，排出来结果是升序
	std::sort(array, array + sizeof(array) / sizeof(array[0]));
	for (auto& e : array)
	{
		cout << e << " ";
	}
	cout << endl;
	// 如果需要降序，需要改变元素的比较规则
	std::sort(array, array + sizeof(array) / sizeof(array[0]), greater<int>());
	for (auto& e : array)
	{
		cout << e << " ";
	}
	cout << endl;
}

void testSort2()
{
	A array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	std::sort(array, array + sizeof(array) / sizeof(array[0]));
	for (auto& e : array)
	{
		cout << e._a << " ";
	}
	cout << endl;
	std::sort(array, array + sizeof(array) / sizeof(array[0]), greater<A>());
	for (auto& e : array)
	{
		cout << e._a << " ";
	}
	cout << endl;
	std::sort(array, array + sizeof(array) / sizeof(array[0]), LessA());
	for (auto& e : array)
	{
		cout << e._a << " ";
	}
	cout << endl;
	std::sort(array, array + sizeof(array) / sizeof(array[0]), GreaterA());
	for (auto& e : array)
	{
		cout << e._a << " ";
	}
	cout << endl;

	cout << "lambda:" << endl;
	std::sort(array, array + sizeof(array) / sizeof(array[0]), [](const A& a1, const A& a2)->bool
	{
		return a1 < a2;
	});

	for (auto& e : array)
	{
		cout << e._a << " ";
	}
	cout << endl;

}

void testLambda2()
{

	int a = 10;
	int b = 5;
	//最简单的lambda表达式
	[]{};
	//mutable: 把捕捉列表中的变量属性改为非const(默认是const属性）
	[a, b]()mutable{a = 100; b = 200; return a + b; };
	auto func = [](int a, int b)->int{a = 1; b = 2; return a + b; };
	//lambda表达式使用
	func(a, b);
}

void testLambda3()
{
	int a = 1;
	int b = 2;
	cout << a << " " << b << endl;
	//[=]:以传值形式捕捉父类作用域的所有变量
	auto func1 = [=](int num)mutable->int{
		a = 5;
		b = 10;
		//c不能捕捉， c还没有定义
		//return a + b +c + num;
		return a + b + num;
	};

	func1(300);
	cout << a << " " << b << endl;
	//如果是传引用的形式，不需要mutable也可以修改捕捉列表中的变量
	auto func2 = [&](int num)->int{
		a = 5;
		b = 10;
		//c不能捕捉， c还没有定义
		//return a + b +c + num;
		return a + b + num;
	};

	func2(300);
	cout << a << " " << b << endl;

	int c = 3;

	//[=,&a]除过a之外， 其它变量以值的形式捕捉，a以引用的形式捕捉
	//[=,a]: 错误写法
	auto func3 = [=, &a](int num)->int{
		return a + b + num;
	};
	//[&,a]: 除过a之外， 其它变量以引用的形式捕捉，a以值的形式捕捉
	auto func4 = [&, a](int num)->int{
		return a + b + num;
	};
}

class C
{
public:
	void printC()
	{
		int a = 1;
		int b = 2;
		//父类作用域不一定就是直接父类作用域，也可以是嵌套父域
		auto func1 = [=](int num)->void{
			//a,b属于C::printC函数的局部域
			cout << a << " " << b << " " << this->_c << endl;
			//_c属于类域C
			cout << _c << endl;
		};
		func1(30);
	}
public:
	int _c = 5;
};

void testLambda4()
{
	C c;
	c.printC();
}

int global = 10;

//如果不是局部域，捕捉列表中不能指定具体的变量，但是可以写=或&
auto funcG = [=](int a, int b)->void{

	cout << global << endl;
};

void func()
{
	auto funcG = [=](int a, int b)->void{

		cout << global << endl;
	};
}

typedef int(*fptr)(int a, int b);
typedef void(*fptr2)();

void testLambda5()
{
	auto fun1 = [](int a, int b)->int{return a + b; };
	auto fun2 = [](int a, int b)->int{return a + b; };
	//lambda表达式之间不能赋值
	//fun2 = fun1;
	//但是可以拷贝
	auto fun3(fun2);
	auto fun4 = fun2;
	fptr ptr;
	//可以把lambda表达式赋给一个函数指针
	ptr = fun1;

	//接口不一致
	//fptr2 ptr2 = fun1;
}

struct Add
{
	int operator()(int a, int b)
	{
		return a + b;
	}
};

void testLambda6()
{
	Add add;
	add(1, 2);

	//C++实现lambda表达式： 创建一个仿函数类
	auto func = [](int a, int b)->int{return a + b; };
	func(1, 2);
}

void tfunc1()
{
	cout << "tfunc1()" << endl;
}

void tfunc2(int a)
{
	cout << "tfunc2(int)" << endl;
}

void tfunc3(int a, int b, int c)
{
	cout << "tfunc3(int, int,int)" << endl;
}



void testThread()
{
	//thread t1;

	thread t1(tfunc1);
	thread t2(tfunc2, 1);
	thread t3(tfunc3, 1, 2, 3);
	return;
	cout << "线程等待" << endl;
	t1.join();
	t2.join();
	t3.join();
}

//RAII: 资源获取立即初始化
//  在构造函数中初始化资源
//  在析构函数中销毁资源
class ThreadManage
{
public:
	ThreadManage(thread& t)
		:_thread(t)
	{}

	~ThreadManage()
	{
		if (_thread.joinable())
			_thread.join();
	}
private:
	thread& _thread;
};

void testThread2()
{
	//thread t1;

	thread t1(tfunc1);
	thread t2(tfunc2, 1);
	thread t3(tfunc3, 1, 2, 3);
	ThreadManage tm1(t1);
	ThreadManage tm2(t2);
	ThreadManage tm3(t3);

	return;
	/*cout << "线程等待" << endl;
	t1.join();
	t2.join();
	t3.join();*/
}

class ThreadClass
{
public:
	void funcT(int a)
	{
		cout << a << endl;
	}
};

void testClassFunc()
{
	ThreadClass tc;
	//如果函数为成员函数，则需要写完整作用域，并且需要显示取地址，参数需要加上this指向的对象
	thread t1(&ThreadClass::funcT, &tc, 10);
	t1.join();
}

void func2(int& a)
{
	a += 10;
}

void func3(int* a)
{
	*a += 10;
}

void testThreadRef()
{
	int a = 0;
	cout << a << endl;
	//如果函数参数类型为引用，在线程中需要修改原始的变量，则需要通过ref转换
	thread t1(func2, ref(a));
	t1.join();
	cout << a << endl;

	thread t2(func3, &a);
	t2.join();
	cout << a << endl;
}

int main()
{
	//testMove();
	//testMove2();
	//testForward();
	//testSort();
	//testSort2();
	//testLambda3();
	//testLambda4();
	//funcG(1, 2);
	//testLambda6();
	//testThread();
	//testThread2();
	//testClassFunc();
	testThreadRef();
	return 0;
}

