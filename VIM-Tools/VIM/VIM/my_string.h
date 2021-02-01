#pragma once
#define MY_STRING_H
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
// Allocate in blocks
#define BLOCK 64
using namespace std;
int my_strcmp(char* frst, const char* scnd);
class MyString
{
	// Char array
private:
	char* str;
	int cap;
	int len;
	// Constructor
public:
	MyString();
	MyString(const MyString& other);
	MyString(const char* new_str);
	MyString(std::initializer_list<char>& il);
	MyString(const std::string& other);
	MyString(char* new_str, int count);
	MyString(int count, char s);
	// Shift ostream and istream
public:
	friend std::ostream & operator << (std::ostream & os, MyString & other);
	friend std::istream& operator >> (std::istream& is, MyString& other);
	// Destructor
public:
	~MyString();
	// Simple operations with data
public:
	char* data() const;
	char* c_str() const;
	size_t length() const;
	size_t size() const;
	void clear();
	bool empty() const;
	void shrink_to_fit();
	size_t capacity() const;
	// Operators
public:
	MyString& operator = (const char* new_str);
	MyString& operator = (const MyString& other);
	MyString& operator = (const std::string& other);
	MyString& operator = (const char symbol);
	MyString operator + (const MyString& other);
	MyString operator + (const char* new_str);
	MyString operator + (const std::string& other);
	char operator[] (int index);
	bool operator != (const MyString& other);
	bool operator == (const MyString& other);
	bool operator > (const MyString& other);
	bool operator < (const MyString& other);
	bool operator >= (const MyString& other);
	bool operator <= (const MyString& other);
	MyString& operator += (const char* str);
	MyString& operator += (const std::string& other);
	MyString& operator += (const MyString& other);
	// Insert method
public:
	void insert(int index, int count, char symbol);
	void insert(int index, const char* new_str);
	void insert(int index, const char* new_str, int count);
	void insert(int index, const std::string& other);
	void insert(int index, const std::string& other, int count);
	// Erase
public:
	void erase(int index, int count);
	// Append
public:
	void append(int count, char symbol);
	void append(const char* new_str);
	void append(const char* new_str, int index, int count);
	void append(const std::string& other);
	void append(const std::string& other, int index, int count);
	// Replace
public:
	void replace(int index, int count, const char* new_str);
	void replace(int index, int count, const std::string& other);
	// Substr
public:
	void substr(int index);
	void substr(int index, int count);
	// Find
public:
	int find(const char* substr);
	int find(const char* substr, int index);
	int find(const std::string& other);
	int find(const std::string& other, int index);
	// Help functions
	char* my_strcpy(char* dst, const char* src);
	char* my_strcat(const char* frst, const char* scnd);
	size_t my_strlen(const char* str);
	// Count the capacity we need
	int set_capacity(int size);
};