#pragma once
#include "my_string.h"
/*
Contructor
*/
MyString::MyString()
{
	this->str = nullptr;
	this->cap = 0;
	this->len = 0;
}
MyString::MyString(const MyString& other)
{
	this->cap = set_capacity(my_strlen(other.str));
	this->str = new char[this->cap];
	my_strcpy(this->str, other.str);
	this->len = my_strlen(this->str);
}
MyString::MyString(const std::string& other)
{
	this->cap = set_capacity(other.length());
	this->str = new char[this->cap];
	my_strcpy(this->str, other.c_str());
	this->len = my_strlen(this->str);
}
MyString::MyString(const char* new_str)
{
	this->cap = set_capacity(my_strlen(new_str));
	this->str = new char[this->cap];
	my_strcpy(this->str, new_str);
		this->len = my_strlen(this->str);
}
MyString::~MyString()
{
	//delete[] this->str;
	//this->str = nullptr;
	//this->cap = 0;
}
MyString::MyString(std::initializer_list<char>& il)
{
	this->cap = set_capacity(my_strlen(il.begin()));
	this->str = new char[this->cap];
	my_strcpy(str, il.begin());
	this->len = my_strlen(this->str);
}
MyString::MyString(char* new_str, int count)
{
	this->cap = set_capacity(count);
	this->str = new char[this->cap];
	char* tmp = str;
	for (int i = 0; i < count; i++)
		*tmp++ = *new_str++;
	*tmp = 0;
	this->len = my_strlen(this->str);
}
MyString::MyString(int count, char s)
{
	this->cap = set_capacity(count);
	int tmp = count - 1;
	this->str = new char[cap];
	while (tmp-- != 0)
		this->str[tmp] = s;
	this->str[count - 1] = 0;
	this->len = my_strlen(this->str);
}
/*
No class Functions
*/
char* MyString::my_strcpy(char* dst, const char* src)
{
	char* d = dst;
	while (*d++ = *src++);
	return dst;
}
char* MyString::my_strcat(const char* frst, const char* scnd)
{
	int cap = set_capacity(my_strlen(frst) + my_strlen(scnd));
	char* tmp = new char[cap];
	int i = 0;
	for (; i < my_strlen(frst); i++)
		tmp[i] = frst[i];
	for (int j = 0; j < my_strlen(scnd); i++, j++)
		tmp[i] = scnd[j];
	tmp[i] = '\0';
		return tmp;
}
size_t MyString::my_strlen(const char* str)
{
	if (str == nullptr)
		return 0;
	size_t i = 0;
	while (str[i] != '\0')
		++i;
	return i;
}
int my_strcmp(char* frst, const char* scnd)
{
	while (*frst && *scnd && *frst == *scnd) { ++frst; ++scnd; }
	return (unsigned char)(*frst) - (unsigned char)(*scnd);
}
int MyString::set_capacity(int size)
{
	return (((size / BLOCK) + 1) * BLOCK);
}
/*
Simple operations with data
*/
size_t MyString::length() const
{
	return this->len;
}
size_t MyString::size() const
{
	return this->len;
}
char* MyString::data() const
{
	return this->str;
}
char* MyString::c_str() const
{
	return data();
}
size_t MyString::capacity() const
{
	return this->cap;
}
void MyString::clear()
{
	this->c_str()[0] = '\0';
	this->len = 0;
}
bool MyString::empty() const
{
	return length() == 0;
}
void MyString::shrink_to_fit()
{
	this->cap = this->len + 1;
	char* tmp = new char[this->cap];
	my_strcpy(tmp, this->str);
	delete[] this->str;
	str = new char[this->cap];
	my_strcpy(this->str, tmp);
	delete[] tmp;
}
/*
Operators
*/
MyString& MyString::operator = (const char* new_str)
{
	if (this->str != nullptr)
		delete[] str;
	this->len = my_strlen(new_str);
	this->cap = set_capacity(this->len + 1);
	this->str = new char[this->cap];
	my_strcpy(this->str, (char*)new_str);
	this->str[this->len] = '\0';
	return *this;
}
MyString& MyString::operator = (const MyString& other)
{
	if (this->str != nullptr)
	{
		delete[] str;
		this->str = nullptr;
		this->len = 0;
		this->cap = 0;
	}
	if (other.str == nullptr)
		return *this;
	this->len = my_strlen(other.str);
	this->cap = set_capacity(this->len + 1);
	this->str = new char[this->cap];
	my_strcpy(this->str, (char*)other.str);
	return *this;
}
MyString& MyString::operator = (const std::string& other)
{
	if (this->str != nullptr)
	{
		delete[] str;
		this->str = nullptr;
		this->len = 0;
		this->cap = 0;
	}
	if (other.c_str() == nullptr)
		return *this;
		this->len = my_strlen(other.c_str());
	this->cap = set_capacity(this->len + 1);
	this->str = new char[this->cap];
	my_strcpy(this->str, (char*)other.c_str());
	this->str[this->len] = '\0';
	return *this;
}
MyString& MyString::operator = (const char symbol)
{
	if (this->str != nullptr)
	{
		delete[] str;
		this->str = nullptr;
		this->len = 0;
		this->cap = 0;
	}
	if (symbol == NULL)
		return *this;
	this->len = 1;
	this->cap = set_capacity(this->len + 1);
	this->str = new char[this->cap];
	this->str[0] = symbol;
	this->str[1] = '\0';
	return *this;
}
MyString MyString::operator + (const MyString& other)
{
	return my_strcat(this->str, (char*)other.str);
}
MyString MyString::operator + (const char* new_str)
{
	return my_strcat(this->str, (char*)new_str);
}
MyString MyString::operator + (const std::string& other)
{
	return my_strcat(this->str, (char*)other.c_str());
}
MyString& MyString::operator += (const char* str)
{
	this->str = my_strcat(this->str, str);;
	return *this;
}
MyString& MyString::operator += (const std::string& other)
{
	this->str = my_strcat(this->str, other.c_str());
	return *this;
}
MyString& MyString::operator += (const MyString& other)
{
	this->str = my_strcat(this->str, other.str);
	return *this;
}
char MyString::operator[] (int index)
{
	if (index > this->len || this->str == nullptr)
		return 0;
	else
		return this->str[index];
}
bool MyString::operator > (const MyString& other)
{
	return my_strcmp(this->str, other.str) > 0;
}
bool MyString::operator < (const MyString& other)
{
	return my_strcmp(this->str, other.str) < 0;
}
bool MyString::operator >= (const MyString& other)
{
	return my_strcmp(this->str, other.str) >= 0;
}
bool MyString::operator <= (const MyString& other)
{
	return my_strcmp(this->str, other.str) <= 0;
}
/*
Lexicographically comparing operators
*/
bool MyString::operator !=(const MyString& other)
{
	return !(this->operator==(other));
}
bool MyString::operator ==(const MyString& other)
{
	if (this->len != my_strlen(other.str))
	{
		return false;
	}
	for (int i = 0; i < this->len; i++)
	{
		if (this->str[i] != other.str[i])
		{
			return false;
		}
	}
	return true;
}
/*
Insert
*/
void MyString::insert(int index, int count, char symbol)
{
	char* src = new char[count + 1];
		for (int i = 0; i < count; ++i)
			src[i] = symbol;
	src[count] = '\0';
	this->insert(index, src, count);
	delete[] src;
}
void MyString::insert(int index, const char* src)
{
	this->insert(index, src, my_strlen(src));
}
void MyString::insert(int index, const char* src, int count)
{
	int len_before = this->len;
	char* tmp = nullptr;
	if (len_before)
	{
		tmp = new char[len_before + 1];
		my_strcpy(tmp, this->str);
		delete[] this->str;
	}
	this->len += my_strlen(src);
	this->cap = set_capacity(this->len);
	this->str = new char[this->cap];
	int i = 0, j = 0;
	for (; i <= this->len; ++i)
	{
		if (i == index)
		{
			for (; j < count; ++j, ++i)
			{
				this->str[i] = src[j];
			}
		}
		if (len_before)
			this->str[i] = tmp[i - j];
		else
			this->str[i] = '\0';
	}
	delete[] tmp;
}
void MyString::insert(int index, const std::string& other)
{
	this->insert(index, other.c_str(), my_strlen(other.c_str()));
}
void MyString::insert(int index, const std::string& other, int count)
{
	this->insert(index, other.c_str(), count);
}
/*
Erase
*/
void MyString::erase(int index, int count)
{
	int i = 0, j = 0;
	for (; i < this->len; ++i)
	{
		if (i == index)
			j += count;
		this->str[i] = this->str[i + j];
	}
	this->str[i] = '\0';
	this->len -= count;
}
/*
Append
*/
void MyString::append(int count, char symbol)
{
	this->insert(my_strlen(this->str), count, symbol);
}
void MyString::append(const char* new_str)
{
	this->insert(my_strlen(this->str), new_str);
}
void MyString::append(const char* new_str, int index, int count)
{
	char* tmp = new char[count + 1];
	for (int i = 0; i < count; ++i)
		tmp[i] = new_str[index + i];
	tmp[count] = '\0';
	this->insert(my_strlen(this->str), (const char*)tmp);
	delete[] tmp;
}
void MyString::append(const std::string& other)
{
	this->insert(my_strlen(other.c_str()), other.c_str());
}
void MyString::append(const std::string& other, int index, int count)
{
	this->insert(index, other.c_str(), count);
}
/*
Replace
*/
void MyString::replace(int index, int count, const char* new_str)
{
	this->erase(index, count);
	this->insert(index, new_str);
}
void MyString::replace(int index, int count, const std::string& other)
{
	this->erase(index, count);
	this->insert(index, other.c_str());
}
/*
Substr
*/
void MyString::substr(int index)
{
	this->erase(0, index);
}
void MyString::substr(int index, int count)
{
	this->erase(index + count, my_strlen(this->str) - index - count);
	this->erase(0, index);
}
/*
Find
*/
int MyString::find(const char* substr)
{
	return this->find(substr, 0);
}
int MyString::find(const char* substr, int index)
{
	int len_substr = my_strlen(substr);
	int i = index, j = 0;
	for (; i < this->len - len_substr + 1; ++i)
	{
		if (this->str[i] == substr[0])
		{
			j = 0;
			for (; j < len_substr; ++j)
			{
				if (this->str[i + j] != substr[j])
					break;
			}
			if (j == len_substr)
				return i;
		}
	}
	return -1;
}
int MyString::find(const std::string& other)
{
	return this->find(other.c_str(), 0);
}
int MyString::find(const std::string& other, int index)
{
	return this->find(other.c_str(), index);
}
/*
Shift ostream and istream
*/
std::ostream& operator << (std::ostream& os, MyString& other)
{
	return (os << other.c_str());
	
}
std::istream& operator >> (std::istream& is, MyString& other)
{
	other.clear();
	char c;
	do {
		c = is.get();
		if (c == '\n' || c == ' ')
			break;
		other.append(1, c);
	} while (true);
	return is;
}