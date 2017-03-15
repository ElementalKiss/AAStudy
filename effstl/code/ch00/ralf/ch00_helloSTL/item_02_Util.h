#pragma once
#include "extern.h"

namespace item_02_Util
{
	class Customer
	{
	public:
		void SetID(int id) { _id = id; }
	private:
		int _id;
	};

	class CustomerList
	{
	public:
		// ...

	private:
		typedef list<Customer> CustomerContainer;
		typedef CustomerContainer::iterator CCIterator;
		CustomerContainer customers;
	};

	class Widget
	{
	public:
		inline void SetX(int x) { _x = x; }
		inline void SetY(int y) { _y = y; }

	public:
		bool operator==(Widget& rhs) {
			if (_x == rhs._x &&
				_y == rhs._y) {
				return true;
			}
			return false;
		}

	private:
		int _x;
		int _y;
	};

	template<typename T>
	class SpecialAllocator
	{
	public:
		SpecialAllocator(T data) : _data(data) {}
		inline T GetData() { return _data; }

	private:
		T _data;
	};

}
