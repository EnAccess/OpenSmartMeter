#ifndef STRING_H
#define STRING_H
#define _CRT_SECURE_NO_WARNINGS

#define CAPACITY_CONSTANT 1.7

#include <iostream>
#include <gtest\gtest.h>

class String
{
public:
	class Substring
	{
		int offset;
		int length;
		String *str;
	public:
		Substring(String &, int, int);
		Substring operator=(const String &);
		Substring operator=(const char *);
		operator String();
	};
	friend class Substring;

	class Buffer
	{
		friend class Substring;
		friend class String;

		FRIEND_TEST(StringTest, PerformanceSubstringTest);
		char *string;
		int references;
		int length;
		int capacity;
	public:
		Buffer();
		Buffer(const char*, int);
		~Buffer();
		Buffer& operator++();
		Buffer& operator--();
		operator int();
	};

	class Char
	{
		char &chr;
		void operator &();
	public:
		Char(char &);
		Char& operator = (const char&);

		bool operator==(const char&) const;

		operator char();
	};

	Buffer *buf;

public:
	String();
	String(const char*);
	String(const char*, int);
	String(const String &);
	~String();

	int length() const;

	char* c_str() const;

	friend std::ostream& operator<<(std::ostream &out, const String &s);
	friend std::istream& operator>>(std::istream &in, String &s);

	String operator=(const String &);
	String operator+(const String &);
	String operator+=(const String &);

	Substring substr(int, int);
	void remove(int, int);
	void insert(int, const String &);

	void reserve(int res);
	int capacity() const;

	Substring operator()(int, int);
	Char operator[](int) const;

	bool operator!() const;

	bool operator==(const String &) const;
	bool operator!=(const String &) const;
	bool operator<(const String &) const;
	bool operator>(const String &) const;
	bool operator>=(const String &) const;
	bool operator<=(const String &) const;

	bool operator==(const char *) const;
	bool operator!=(const char *) const;
	bool operator<(const char *) const;
	bool operator>(const char *) const;
	bool operator>=(const char *) const;
	bool operator<=(const char *) const;

	int rc();
};

#endif	