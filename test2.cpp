/*
1. ��(T&& t)��Ϊģ�����ʱ, �Ƿ�Ϊ��ֵ�����迴ĩ��������յ�ʵ������, ��ʵ������ƥ��( �����Ĳ���)
	�������������ֵ����
2. ����
	1. ͨ������С�ڱȽ� 
	2. �º���GreaterA(), LessA()
3. ��򵥵�lanmbda���ʽ����
	[]{};
	mutable: �Ѳ�׽������Ǳ������Ը�Ϊ��const(Ĭ����const����)
	[a,b]()mutable{a = 10; b=20; return a + b;};    ��mutable�������(),
	auto func = []()mutable{};
	//�����������lambda����/ʹ��
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
	//std::move:  �ƶ�����--> �ѱ��������Ա����ֵ 
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
	//�ƶ����죺 ��߿���Ч��
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

	//�ƶ���ֵ
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
		cout << "��������" << endl;
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

	//�ƶ�������ȷ������
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
	//�ƶ��������ʾ������һ��������õ�����ֵ�����һ����ֵ
	String copy(move(str));

	cout << str._str << endl;
	//�ƶ�������ȷ��ʹ�ó�������Ҫ��֤���Ա��޸ĵ���ֵ���治�����õ�
}

void Fun(int &x){ cout << "lvalue ref" << endl; }
void Fun(int &&x){ cout << "rvalue ref" << endl; }
void Fun(const int &x){ cout << "const lvalue ref" << endl; }
void Fun(const int &&x){ cout << "const rvalue ref" << endl; }

//���⣺ ģ������� T&& δ����������---> ��Ҫ��ģ��������յ�ʵ�����ͣ���ʵ������ƥ��
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
	// Ĭ�ϰ���С�ڱȽϣ��ų������������
	std::sort(array, array + sizeof(array) / sizeof(array[0]));
	for (auto& e : array)
	{
		cout << e << " ";
	}
	cout << endl;
	// �����Ҫ������Ҫ�ı�Ԫ�صıȽϹ���
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
	//��򵥵�lambda���ʽ
	[]{};
	//mutable: �Ѳ�׽�б��еı������Ը�Ϊ��const(Ĭ����const���ԣ�
	[a, b]()mutable{a = 100; b = 200; return a + b; };
	auto func = [](int a, int b)->int{a = 1; b = 2; return a + b; };
	//lambda���ʽʹ��
	func(a, b);
}

void testLambda3()
{
	int a = 1;
	int b = 2;
	cout << a << " " << b << endl;
	//[=]:�Դ�ֵ��ʽ��׽��������������б���
	auto func1 = [=](int num)mutable->int{
		a = 5;
		b = 10;
		//c���ܲ�׽�� c��û�ж���
		//return a + b +c + num;
		return a + b + num;
	};

	func1(300);
	cout << a << " " << b << endl;
	//����Ǵ����õ���ʽ������ҪmutableҲ�����޸Ĳ�׽�б��еı���
	auto func2 = [&](int num)->int{
		a = 5;
		b = 10;
		//c���ܲ�׽�� c��û�ж���
		//return a + b +c + num;
		return a + b + num;
	};

	func2(300);
	cout << a << " " << b << endl;

	int c = 3;

	//[=,&a]����a֮�⣬ ����������ֵ����ʽ��׽��a�����õ���ʽ��׽
	//[=,a]: ����д��
	auto func3 = [=, &a](int num)->int{
		return a + b + num;
	};
	//[&,a]: ����a֮�⣬ �������������õ���ʽ��׽��a��ֵ����ʽ��׽
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
		//����������һ������ֱ�Ӹ���������Ҳ������Ƕ�׸���
		auto func1 = [=](int num)->void{
			//a,b����C::printC�����ľֲ���
			cout << a << " " << b << " " << this->_c << endl;
			//_c��������C
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

//������Ǿֲ��򣬲�׽�б��в���ָ������ı��������ǿ���д=��&
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
	//lambda���ʽ֮�䲻�ܸ�ֵ
	//fun2 = fun1;
	//���ǿ��Կ���
	auto fun3(fun2);
	auto fun4 = fun2;
	fptr ptr;
	//���԰�lambda���ʽ����һ������ָ��
	ptr = fun1;

	//�ӿڲ�һ��
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

	//C++ʵ��lambda���ʽ�� ����һ���º�����
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
	cout << "�̵߳ȴ�" << endl;
	t1.join();
	t2.join();
	t3.join();
}

//RAII: ��Դ��ȡ������ʼ��
//  �ڹ��캯���г�ʼ����Դ
//  ������������������Դ
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
	/*cout << "�̵߳ȴ�" << endl;
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
	//�������Ϊ��Ա����������Ҫд���������򣬲�����Ҫ��ʾȡ��ַ��������Ҫ����thisָ��Ķ���
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
	//���������������Ϊ���ã����߳�����Ҫ�޸�ԭʼ�ı���������Ҫͨ��refת��
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

