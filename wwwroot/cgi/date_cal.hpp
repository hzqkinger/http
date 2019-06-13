#include<iostream>
using namespace std;
class Date{
public:
	Date(int year, int month, int day)/*{{{*/
	{
		_year = year;
		_month = month;
		_day = day;
	}/*}}}*/

	int GetMonth()/*{{{*/
	{
		static int months[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		if (_month == 2 && (_year % 4 == 0 && _year % 100 != 0 || _year % 400 == 0))
			return months[_month]+1;
		return months[_month];
	}/*}}}*/

	void Display()/*{{{*/
	{
		cout << _year << "-" << _month << "-" << _day << endl;
	}/*}}}*/

	void Display2()/*{{{*/
	{
		cout << _year << endl << endl;
		for (size_t i = 1; i <= 12; ++i)
		{
			cout << _month << endl;
			for (size_t j = 1; j <= GetMonth(); ++j)
			{
				cout << j << " ";
				if (j % 7 == 0)cout << endl;
			}
		}
	}/*}}}*/

	int operator>(const Date &d)/*{{{*/
	{
		if (_year > d._year)
			return 1;
		else if (_year == d._year)
		{
			if (_month > d._month)
				return 1;
			else if (_month == d._month)
				if(_day > d._day)
					return 1;
		}
		return 0;
	}/*}}}*/

	Date operator++()/*{{{*/
	{
		*this = *this + 1;
		return *this;
	}/*}}}*/

	Date operator++(int)/*{{{*/
	{
		Date tmp(*this);
		*this = *this + 1;
		return tmp;
	}/*}}}*/

	Date operator+(int day)/*{{{*/
	{
		if (day < 0)
			return *this - (-day);
		Date tmp(*this);
		tmp._day += day;
		while(tmp._day>tmp.GetMonth())
		{
			tmp._day -= tmp.GetMonth();
			tmp._month++;
			if (tmp._month > 12)
			{
				tmp._year++;
				tmp._month = 1;
			}
		}
		return tmp;
	}/*}}}*/

	Date operator-(int day)/*{{{*/
	{
		if (day < 0)
			return *this + (-day);
		Date tmp(*this);
		tmp._day -= day;
		while (tmp._day <= 0)
		{
			tmp._month--;
			tmp._day += tmp.GetMonth();
			if (tmp._month <= 0)
			{
				tmp._year--;
				tmp._month = 12;
			}
		}
		return tmp;
	}/*}}}*/
	
	int operator-(const Date &d)/*{{{*/
	{
		int num = 0;
		Date Great(*this);
		Date Small(d);
		if (Small > Great)
		{
			Great = d;
			Small = *this;
		}
		while (Great > Small++)
			num++;
		return num;
	}/*}}}*/

private:
	int _year;
	int _month;
	int _day;
};
