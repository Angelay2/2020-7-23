/*
98不支持自定义类型的数组初始化, 
C++11: 支持内置类型和自定义类型的列表初始化
	vector<int>array{1,2,3}和vector<int>array = {1,2,3}是一样的
	map<int, int> a = {{}};, 内部每个元素是pair<int, int>类型. 外部是map类型
auto: 编译时根据初始化表达式进行类型推导
decltype: 运行时类型识别. typeid可以查看类型
		如果没有参数列表,只有函数名时, 推导为函数的接口类型
		如果有参数列表, 推导为返回值类型
定义一个不能被继承的类,用final在类名后 针对虚函数 
override 显示子类要重写父类的虚函数, 
引用: 左值引用, 右值引用 ---> 语法意义: 都是变量的别名
	左值引用: 既可以引用左值, 也可以引用右值
			如果引用右值, 需要为const左值引用 const int& a = 10;
	右值引用: 只能引用右值 int&& a = 10; 最大的作用时标记右值引用指向的实体(本身也为右值)的资源可以被直接拿走, 从而提高拷贝的效率, 本质为浅拷贝.右值的实体后面就不会再使用了,相当于二手车给别人了, 而左值引用的构造就相当于新车借人后还是要还的,人家还要用的
	右值引用指向的实体声明周期结束时(创建一临时对象后)只是调了析构,但是本身并没有释放资源,把将亡值的资源拿走了,移动构造, 但是右值引用不会释放资源, 
	左值引用结束时要释放资源, 拷贝构造

	移动构造: 参数类型为右值引用, 提高拷贝的效率
		1. 相对于拷贝构造, 可以在实现浅拷贝的情况下, 不产生错误
		2. 右值引用指向的实体一般是生命周期马上要结束的实体 ---> 将亡值, 可以直接获取右值引用所指向的实体资源, 不需要深拷贝.
	 移动赋值: 参数类型为右值引用
		也是浅拷贝, 原理同移动构造

*/
#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;
#include <string>
#include <vector>
#include <iostream>
#include <map>
using namespace std;

class   A
{
public:
	A(int a, int b)
		:_a(a)
		, _b(b)
	{}

	A(int a)
		:_a(a)
		, _b(0)
	{}
private:
	int _a;
	int _b;
};
template <class T>
class Vector
{
public:
	Vector(int n, int a)
		:_array(new T[n])
		, _size(0)
		, _capacity(n)
	{}

	Vector(const initializer_list<T>& lst)
		:_array(new T[lst.size()])
		, _size(0)
		, _capacity(lst.size())
	{
		for (auto& e : lst)
			_array[_size++] = e;
	}

private:
	T* _array;
	int _size;
	int _capacity;
};

void test()
{
	int array[] = { 1, 2, 3, 5 };

	//C++11： 支持内置类型的列表初始化
	int a = 1;
	int b = { 1 };
	int c{ 1 };
	float d = { 1.2f };

	//C++11： 支持自定义类型的列表初始化
	vector<int> array3{ 1, 2, 3, 4, 5 };
	vector<int> array2 = { 1, 2 };

	pair<int, int> p = { 1, 1 };
	map<int, int>   m = { { 1, 1 }, { 2, 2 }, { 3, 3 } };

	//自定义类型：创建单个对象
	A a3 = { 1, 2 };
	A a2(3, 4);
	//单参构造函数的隐式类型转换
	A a4 = 5;

	//自定义类型：初始化多个元素  ----> 不是天然支持列表初始化
	// 需要显示定义参数类型为initializer_list的构造函数

	// 三个元素
	Vector<int> vec = { 1, 2, 3 };

	//匹配第一个有两个参数的构造函数
	//没有实际元素的空的Vector
	Vector<int> vec2(10, 0);
	Vector<int> vec3{ 20, 1 };
}

int func(int a)
{
	cout << a << endl;
	return a;
}

void func2()
{
	cout << "func2（）" << endl;
}

void testAuto()
{
	map<string, string> m;
	m["123"] = "456";
	m["789"] = "012";

	std::map<std::string, std::string>::iterator it = m.begin();
	//auto: 编译时根据初始化表达式进行类型推导
	// auto: 编译时类型识别
	auto it2 = m.begin();

	//decltype: 运行时类型识别
	decltype(1 + 2) a;

	decltype(func(10)) b;
	//decltype: 如果有参数列表，推导返回值类型
	cout << "b: type: " << typeid(b).name() << endl;
	//decltype: 如果没有参数列表，只有函数名，推导为函数的接口类型
	cout << typeid(decltype(func)).name() << endl;
}

//定义一个不能被继承的类型
class B final
{

};

class C
{
public:
	//default: 让编译器显式生成一个默认函数
	C() = default;
	//C(){}
	//delete: 把一个函数声明成已删除函数，不能再被使用
	// 拷贝构造声明为delete: 防拷贝
	C(const C& c) = delete;
	C& operator=(const C& c) = delete;
private:
	int _c;
};

//C::C(const C& c)
//:_c(c._c)
//{
//	cout << "C(const C&)" << endl;
//}

void testDelete()
{
	C c;
	//C copy(c);
}

int getA(int a)
{
	return a;
}

void testLR()
{
	//简单理解：
	//左值：既可以出现在=的两边、或者可以取地址的
	int a = 10;
	int b = a;
	int* p = &a;
	int* p2 = &b;

	//右值：只能出现在=的右边，或者不可以取地址
	/*10 = 20;
	int* p3 = &10;*/

	//C++中： 右值
	//纯右值： 常量， 临时变量/匿名变量
	// 将亡值： 声明周期即将结束

	// 不是右值的都为左值

	//临时变量/匿名变量：函数以值返回的变量， 调用类的构造函数创建的变量
	/*getA(a) = b;
	int* p4 = &(getA());*/
}

class D
{

public:
	D(int d)
		:_d(d)
	{
		cout << "D(int)" << endl;
	}
	D(const D& d)
	{
		cout << "D(const D&)" << endl;
	}

private:
	int _d;
};

D getD(D d)
{
	return d;
}

void func3(const D& d)
{

}
void testRef()
{
	int a = 10;
	//左值引用：引用的实体既可以为左值，也可以为右值

	//ra ： 实体为左值
	int& ra = a;

	// : ri实体为右值
	const int& ri = 10;

	//右值引用：引用的实体只能是右值

	//右值引用： 实体为常量
	int&& lr = 10;
	//右值引用： 实体为临时变量
	int&& lr2 = getA(a);

	const int& r3 = getA(a);

	//右值引用： 不能引用左值
	//int&& r4 = a;
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

private:
	char* _str;
	int _size;
	int _capacity;
};

String getString()
{
	String str("123");
	return str;
}

void testString()
{
	////浅拷贝： 移动构造， 直接获取将亡值（右值）的资源
	//String ret = getString();

	////深拷贝： 拷贝构造， ret为左值
	//String copy(ret);

	//String ret2 = String("456");  // 构造 + 拷贝构造 ， 优化： 构造


	String ret;

	ret = String("789");

	int a = 10;
}

int main()
{
	//test();
	//testAuto();
	//testDelete();
	//testRef();
	testString();
	return 0;
}
int main(){
	map<string, string> m;

	auto it = m.begin();
	// decltype it = m.begin();错
	//auto(1 + 2) it; 错
	decltype(1 + 2) it;

}
