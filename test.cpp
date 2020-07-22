/*
98��֧���Զ������͵������ʼ��, 
C++11: ֧���������ͺ��Զ������͵��б��ʼ��
	vector<int>array{1,2,3}��vector<int>array = {1,2,3}��һ����
	map<int, int> a = {{}};, �ڲ�ÿ��Ԫ����pair<int, int>����. �ⲿ��map����
auto: ����ʱ���ݳ�ʼ�����ʽ���������Ƶ�
decltype: ����ʱ����ʶ��. typeid���Բ鿴����
		���û�в����б�,ֻ�к�����ʱ, �Ƶ�Ϊ�����Ľӿ�����
		����в����б�, �Ƶ�Ϊ����ֵ����
����һ�����ܱ��̳е���,��final�������� ����麯�� 
override ��ʾ����Ҫ��д������麯��, 
����: ��ֵ����, ��ֵ���� ---> �﷨����: ���Ǳ����ı���
	��ֵ����: �ȿ���������ֵ, Ҳ����������ֵ
			���������ֵ, ��ҪΪconst��ֵ���� const int& a = 10;
	��ֵ����: ֻ��������ֵ int&& a = 10; ��������ʱ�����ֵ����ָ���ʵ��(����ҲΪ��ֵ)����Դ���Ա�ֱ������, �Ӷ���߿�����Ч��, ����Ϊǳ����.��ֵ��ʵ�����Ͳ�����ʹ����,�൱�ڶ��ֳ���������, ����ֵ���õĹ�����൱���³����˺���Ҫ����,�˼һ�Ҫ�õ�
	��ֵ����ָ���ʵ���������ڽ���ʱ(����һ��ʱ�����)ֻ�ǵ�������,���Ǳ���û���ͷ���Դ,�ѽ���ֵ����Դ������,�ƶ�����, ������ֵ���ò����ͷ���Դ, 
	��ֵ���ý���ʱҪ�ͷ���Դ, ��������

	�ƶ�����: ��������Ϊ��ֵ����, ��߿�����Ч��
		1. ����ڿ�������, ������ʵ��ǳ�����������, ����������
		2. ��ֵ����ָ���ʵ��һ����������������Ҫ������ʵ�� ---> ����ֵ, ����ֱ�ӻ�ȡ��ֵ������ָ���ʵ����Դ, ����Ҫ���.
	 �ƶ���ֵ: ��������Ϊ��ֵ����
		Ҳ��ǳ����, ԭ��ͬ�ƶ�����

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

	//C++11�� ֧���������͵��б��ʼ��
	int a = 1;
	int b = { 1 };
	int c{ 1 };
	float d = { 1.2f };

	//C++11�� ֧���Զ������͵��б��ʼ��
	vector<int> array3{ 1, 2, 3, 4, 5 };
	vector<int> array2 = { 1, 2 };

	pair<int, int> p = { 1, 1 };
	map<int, int>   m = { { 1, 1 }, { 2, 2 }, { 3, 3 } };

	//�Զ������ͣ�������������
	A a3 = { 1, 2 };
	A a2(3, 4);
	//���ι��캯������ʽ����ת��
	A a4 = 5;

	//�Զ������ͣ���ʼ�����Ԫ��  ----> ������Ȼ֧���б��ʼ��
	// ��Ҫ��ʾ�����������Ϊinitializer_list�Ĺ��캯��

	// ����Ԫ��
	Vector<int> vec = { 1, 2, 3 };

	//ƥ���һ�������������Ĺ��캯��
	//û��ʵ��Ԫ�صĿյ�Vector
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
	cout << "func2����" << endl;
}

void testAuto()
{
	map<string, string> m;
	m["123"] = "456";
	m["789"] = "012";

	std::map<std::string, std::string>::iterator it = m.begin();
	//auto: ����ʱ���ݳ�ʼ�����ʽ���������Ƶ�
	// auto: ����ʱ����ʶ��
	auto it2 = m.begin();

	//decltype: ����ʱ����ʶ��
	decltype(1 + 2) a;

	decltype(func(10)) b;
	//decltype: ����в����б��Ƶ�����ֵ����
	cout << "b: type: " << typeid(b).name() << endl;
	//decltype: ���û�в����б�ֻ�к��������Ƶ�Ϊ�����Ľӿ�����
	cout << typeid(decltype(func)).name() << endl;
}

//����һ�����ܱ��̳е�����
class B final
{

};

class C
{
public:
	//default: �ñ�������ʽ����һ��Ĭ�Ϻ���
	C() = default;
	//C(){}
	//delete: ��һ��������������ɾ�������������ٱ�ʹ��
	// ������������Ϊdelete: ������
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
	//����⣺
	//��ֵ���ȿ��Գ�����=�����ߡ����߿���ȡ��ַ��
	int a = 10;
	int b = a;
	int* p = &a;
	int* p2 = &b;

	//��ֵ��ֻ�ܳ�����=���ұߣ����߲�����ȡ��ַ
	/*10 = 20;
	int* p3 = &10;*/

	//C++�У� ��ֵ
	//����ֵ�� ������ ��ʱ����/��������
	// ����ֵ�� �������ڼ�������

	// ������ֵ�Ķ�Ϊ��ֵ

	//��ʱ����/����������������ֵ���صı����� ������Ĺ��캯�������ı���
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
	//��ֵ���ã����õ�ʵ��ȿ���Ϊ��ֵ��Ҳ����Ϊ��ֵ

	//ra �� ʵ��Ϊ��ֵ
	int& ra = a;

	// : riʵ��Ϊ��ֵ
	const int& ri = 10;

	//��ֵ���ã����õ�ʵ��ֻ������ֵ

	//��ֵ���ã� ʵ��Ϊ����
	int&& lr = 10;
	//��ֵ���ã� ʵ��Ϊ��ʱ����
	int&& lr2 = getA(a);

	const int& r3 = getA(a);

	//��ֵ���ã� ����������ֵ
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
	////ǳ������ �ƶ����죬 ֱ�ӻ�ȡ����ֵ����ֵ������Դ
	//String ret = getString();

	////����� �������죬 retΪ��ֵ
	//String copy(ret);

	//String ret2 = String("456");  // ���� + �������� �� �Ż��� ����


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
	// decltype it = m.begin();��
	//auto(1 + 2) it; ��
	decltype(1 + 2) it;

}
