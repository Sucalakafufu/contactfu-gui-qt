#ifndef CONTACT_INFO_H
#define CONTACT_INFO_H

#include "DateType.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <qstring.h>
#include <qdebug.h>
using namespace std;

class ContactInfo
{
	public:
		ContactInfo();
		ContactInfo(QString, QString, QString, QString, DateType);
		void SetContactInfo(QString, QString, QString, QString, DateType);
		void PrintInfo();
		int CompareFullName(const ContactInfo &);
		int CompareFullName(QString, QString);
		bool SameFullName(const ContactInfo &);
		bool SameFullName(QString, QString);
		int CompareBirthday(const ContactInfo &);
		int CompareBirthday(DateType);
		bool SameBirthday(const ContactInfo &);
		bool SameBirthday(DateType);
		bool operator> (const ContactInfo &);
		bool operator< (const ContactInfo &);
		bool operator== (const ContactInfo &);
		bool operator!= (const ContactInfo &);
	    bool operator>= (const ContactInfo &);
	    bool operator<= (const ContactInfo &);
		QString showFirstName(), showLastName(), showEmail(), showPhone();
		bool emptyBday();
		bool Search(QString); bool SearchName(QString, QString);
		int nameLength();

		void SearchConvert(QString &); //converts a string to all caps
		QString sFirstName, sLastName; //to store converted QStrings

		QString showYear();
		QString showMonth();
		QString showDay();

		friend ostream & operator<< (ostream & output, ContactInfo & aContact);
		friend istream & operator>> (istream & input, ContactInfo & aContact);

	private:		
		QString FirstName;
		QString LastName;
		QString Email;
		QString Phone;
		DateType Birthday;
};


#endif

