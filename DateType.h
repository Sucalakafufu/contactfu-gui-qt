#define LESS	-1
#define EQUAL	0
#define GREATER	1
//
#include <iostream>
#include <string>
#include <cctype>
#include <sstream>
#include <stdlib.h>
#include <qstring.h>
#include <qdebug.h>
using namespace std;

class DateType
{
 public:	
  DateType();
  QString YearIs();       
  QString MonthIs();      
  QString DayIs();        
  bool SetDate(QString, QString, QString);
  int ComparedTo(DateType aDate);
  bool IsValidDate(QString, QString, QString);
  bool isLeapYear(int);
  bool isEmptyNow();
  void SearchConvert(QString &);

  //friends
  friend ostream &operator<<(ostream &strm, DateType &obj);
  friend istream &operator>>(istream &strm, DateType &obj); 

 private: 
  QString year;
  QString month;
  QString day;
};




