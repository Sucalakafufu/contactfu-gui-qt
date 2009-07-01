#include "DateType.h" 
#include <iostream>
#include <conio.h>
#include <map>
#include <utility>
#include <string>

DateType::DateType()
{
  year = "1"; month = "1"; day = "1";
}

bool DateType::SetDate(QString newMonth, QString newDay, QString newYear)
{
	if (newMonth.isEmpty() || newDay.isEmpty() || newYear.isEmpty())
	{
		year = "\0"; month = "\0"; day = "\0";
		return true;
	}
	if (IsValidDate(newMonth, newDay, newYear)) //checks to see if date is valid
	{
		year = newYear;
		month = newMonth;
		day = newDay;
		return true;
	}
	else
		return false;
}

bool DateType::IsValidDate(QString aMonth, QString aDay, QString aYear)
{
	int dayArray[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; //array with days of each month

	string newMonth, newDay, newYear;
	newMonth = aMonth.toStdString(); newDay = aDay.toStdString(); newYear = aYear.toStdString();
	if (newMonth.length() > 2 || newDay.length() > 2)
		return false;
	unsigned int count;
	for (count = 0; count < newMonth.size(); count++)
	{
		if (!isdigit(*newMonth.begin()+count))
			return false;
	}
	for (count = 0; count < newDay.size(); count++)
	{
		if (!isdigit(*newDay.begin()+count))
			return false;
	}
	if (!newYear.empty())
	{
		if (*newYear.begin() != '-' && !isdigit(*newYear.begin()))
			return false;
	}
	for (count = 1; count < newYear.size(); count++)
	{
		if (!isdigit(*newYear.begin()+count))
			return false;
	}
	int intMonth, intYear, intDay; 
	intMonth = aMonth.toInt(); intDay = aDay.toInt(); intYear = aYear.toInt();
	
	if (isLeapYear(intYear)) //if is a leap year february has 29 days
		dayArray[1] = 29;
	if (intMonth < 1 || intMonth > 12)
		return false;
	else if (intDay <= dayArray[intMonth-1] && intDay > 0) //if newDay is a valid day in its month, the date is valid
		return true;
	else
		return false;	
}

bool DateType::isLeapYear(int year) //checks to see if a year is a leap year, returns true if it is
{	
	if (year % 4 == 0)		
	{
		if (year % 100 == 0)
		{
			if (year % 400 == 0)
				return true;
		}
		else
			return true;
	}
	return false;
}

bool DateType::isEmptyNow()
{
	if (year.isEmpty())
		return true;
	else
		return false;
}

QString DateType::MonthIs()
// Accessor function for data member month.
{
  return month;
}

QString DateType::YearIs()
// Accessor function for data member year.
{
  return year;
}


QString DateType::DayIs()
// Accessor function for data member day.
{
    return day;
}

int DateType::ComparedTo(DateType aDate)
{
	string year1, year2, month1, month2, day1, day2;
	year1 = year.toStdString(); year2 = aDate.year.toStdString();
	month1 = month.toStdString(); month2 = aDate.month.toStdString();
	day1 = day.toStdString(); day2 = aDate.day.toStdString();
	if (atoi(year1.c_str()) < atoi(year2.c_str())) //checks year
		return LESS; //if year is less, the whole date is less
	else if (atoi(year1.c_str()) > atoi(year2.c_str()))
		return GREATER; //if year is greater, the whole date is greater
	else if (atoi(month1.c_str()) < atoi(month2.c_str())) //checks month if year was equal
		return LESS; //if month is less, the date is less
	else if (atoi(month1.c_str()) > atoi(month2.c_str()))
		return GREATER; //if month is greater, the date is greater
	else if (atoi(day1.c_str()) < atoi(day2.c_str())) //checks day if month was equal
		return LESS; //if day is less, date is less
	else if (atoi(day1.c_str()) > atoi(day2.c_str()))
		return GREATER; //if day is greater, date is greater
	return EQUAL; //if all are equal, the dates are equal
}

//converts the QString to uppercase
void DateType::SearchConvert(QString &name)
{
	name = name.toUpper();
}

ostream &operator<<(ostream &strm, DateType &obj)
{ 
	return strm;
}

//overloaded input stream; prompts user and takes input for the date
istream &operator>>(istream &strm, DateType &obj)
{
	string theMonths[12] = {"JANUARY","FEBRUARY","MARCH","APRIL","MAY","JUNE","JULY","AUGUST","SEPTEMBER","OCTOBER","NOVEMBER","DECEMBER"};
	string theNumbers[12] = {"1","2","3","4","5","6","7","8","9","10","11","12"};
	map<string,string> months;
	for (int count=0;count<12;count++)
	{
		months.insert(make_pair(theMonths[count],theNumbers[count]));
	}

	//local variables
	QString finMonth, finDay, finYear; 
	string aMonth, aDay, aYear;
	int count = 0;

repeat:
	if (count > 5) //user entered over 5 invalid dates
	{
		cout << "\nOver 5 invalid dates entered.\n";
		cout << "Returning to Menu.\n";
		_getch(); //pauses for user
		return strm;
	}

	if (strm != cin) //if filestreaming input
	{ //stream date
		strm >> aMonth;
		strm >> aDay;
		strm >> aYear;
		
		finMonth = QString::fromStdString(aMonth); finDay = QString::fromStdString(aDay); finYear = QString::fromStdString(aYear);
		if (aMonth == "\\0" || aDay == "\\0" || aYear == "\\0")
		{
			aMonth = aDay = aYear = "\0";
			finMonth = QString::fromStdString(aMonth); finDay = QString::fromStdString(aDay); finYear = QString::fromStdString(aYear);
		}
		else
		{
			obj.SearchConvert(finMonth); aMonth = qPrintable(finMonth);
			map<string, string>::iterator matchMonth = months.find(aMonth);
			if (matchMonth != months.end())
			{
				aMonth = matchMonth->second;
			}
		}
		finMonth = QString::fromStdString(aMonth); finDay = QString::fromStdString(aDay); finYear = QString::fromStdString(aYear);
		if (obj.SetDate(finMonth, finDay, finYear)) //if date was set correctly
			return strm;
	}
	else
	{ //takes input from user
		cout << "Month: ";
		getline(cin, aMonth);
		cout << "Day: ";
		getline(cin, aDay);
		cout << "Year: ";
		getline(cin, aYear);
	}

	finMonth = QString::fromStdString(aMonth);
	obj.SearchConvert(finMonth); aMonth = qPrintable(finMonth);
	map<string,string>::iterator matchMonth = months.find(aMonth);
	if (matchMonth != months.end())
		aMonth = matchMonth->second;

	if (obj.SetDate(finMonth, finDay, finYear) && strm == cin) //if user input valid date
	{
		return strm;
	}
	else
	{
		if (strm == cin) //if not filestreaming input
		{
			cout << "\nThat was an invalid date. Please try again.\n";
			_getch(); //pauses for user
			system("CLS"); //clear screen
			count++; //counting invalid inputs
			goto repeat; //try to input again
		}
		else //if filestreaming input
		{
			cout << "\nThere is an invalid date in your file or the file syntax is incorrect.\n";
			cout << "Please check your file and try again.\n";
		}
	}
	return strm;
}