#include "ContactInfo.h"

ContactInfo::ContactInfo()
{
	FirstName = sFirstName = "New";
	LastName = sLastName = "Contact";
	Email = "";
	Phone = "";
	Birthday.SetDate("", "", "");
	SearchConvert(sFirstName); SearchConvert(sLastName);
}

ContactInfo::ContactInfo(QString aFirstName, QString aLastName, QString aEmail, QString aPhone, DateType aBirthday)
{
	FirstName = sFirstName = aFirstName;
	LastName = sLastName = aLastName;
	Email = aEmail;
	Phone = aPhone;
	Birthday = aBirthday;
	SearchConvert(sFirstName); SearchConvert(sLastName);
}

void ContactInfo::SetContactInfo(QString newFirstName, QString newLastName, QString newEmail, QString newPhone, DateType newBirthday)
{
	FirstName = sFirstName = newFirstName;
	LastName = sLastName = newLastName;
	Email = newEmail;
	Phone = newPhone;
	Birthday = newBirthday;
	SearchConvert(sFirstName); SearchConvert(sLastName);
}

int ContactInfo::CompareFullName(const ContactInfo & aContact)
{
	if (sLastName < aContact.sLastName || (sLastName == aContact.sLastName && sFirstName < aContact.sFirstName))
		return LESS;
	else if (sLastName == aContact.sLastName && sFirstName == aContact.sFirstName)
		return EQUAL;
	else
		return GREATER;
}

int ContactInfo::CompareFullName(QString aFirstName, QString aLastName)
{
	SearchConvert(aLastName); SearchConvert(aFirstName);
	if (sLastName < aLastName || (sLastName == aLastName && sFirstName < aFirstName))
		return LESS;
	else if (sLastName == aLastName && sFirstName == aFirstName)
		return EQUAL;
	else
		return GREATER;
}

bool ContactInfo::SameFullName(const ContactInfo & aContact)
{
	if (sLastName == aContact.sLastName && sFirstName == aContact.sFirstName)
		return true;
	else
		return false;
}

bool ContactInfo::SameFullName(QString aFirstName, QString aLastName)
{
	SearchConvert(aLastName); SearchConvert(aFirstName);
	if (sLastName == aLastName && sFirstName == aFirstName)
		return true;
	else
		return false;
}

int ContactInfo::CompareBirthday(const ContactInfo & aContact)
{
	return Birthday.ComparedTo(aContact.Birthday);
}

int ContactInfo::CompareBirthday(DateType aBirthday)
{
	return Birthday.ComparedTo(aBirthday);
}

bool ContactInfo::SameBirthday(const ContactInfo & aContact)
{
	if (Birthday.ComparedTo(aContact.Birthday) == EQUAL)
		return true;
	else
		return false;
}

bool ContactInfo::SameBirthday(DateType aBirthday)
{
	if (Birthday.ComparedTo(aBirthday) == EQUAL)
		return true;
	else
		return false;
}

bool ContactInfo::operator>(const ContactInfo & aContact)
{
	if (CompareFullName(aContact) == GREATER)
		return true;
	else
		return false;
}

bool ContactInfo::operator>=(const ContactInfo & aContact)
{
	if (CompareFullName(aContact) == GREATER || CompareFullName(aContact) == EQUAL)
		return true;
	else
		return false;

}

bool ContactInfo::operator< (const ContactInfo & aContact)
{
	if (CompareFullName(aContact) == LESS)
		return true;
	else
		return false;
}

bool ContactInfo::operator<= (const ContactInfo & aContact)
{
	if (CompareFullName(aContact) == LESS || CompareFullName(aContact) == EQUAL)
		return true;
	else
		return false;
}

bool ContactInfo::operator== (const ContactInfo & aContact)
{
	if (CompareFullName(aContact) == EQUAL)
		return true;
	else
		return false;
}

bool ContactInfo::operator!= (const ContactInfo & aContact)
{
	if (CompareFullName(aContact) != EQUAL)
		return true;
	else
		return false;
}

QString ContactInfo::showFirstName()
{
	return FirstName;
}

QString ContactInfo::showLastName()
{
	return LastName;
}

bool ContactInfo::Search(QString search)
{
	unsigned int matches, first, last, email, phone, aMatch = 1;
	matches = first = last = email = phone = 0;
	QString sName = search;
	QString sEmail = Email;
	QString sPhone = Phone;
	SearchConvert(sName); SearchConvert(sEmail); SearchConvert(sPhone);

	if (search.isEmpty() && sFirstName.isEmpty())
	{
		matches++; goto noFmatch;
	}
	else if ((search.isEmpty() && !sFirstName.isEmpty()) || (!search.isEmpty() && sFirstName.isEmpty()))
		goto noFmatch;
	else
		aMatch = search.length();
	
	while ((first < sFirstName.length() && matches != aMatch) || (sFirstName.isEmpty() && matches != aMatch))
	{
		if (*(sFirstName.begin()+first) == *(sName.begin()+matches))
			matches++;
		else
		{
			if (matches > 0 && *(sFirstName.begin()+first) == *(sName.begin()))
				matches = 1;
			else
				matches = 0;
		}
		first++;
	}
noFmatch:
	if (matches == aMatch)
		return true;
	else
		matches = 0;

	if (search.isEmpty() && sLastName.isEmpty())
	{
		matches++; goto noLmatch;
	}
	else if ((search.isEmpty() && !sLastName.isEmpty()) || (!search.isEmpty() && sLastName.isEmpty()))
		goto noLmatch;
	while ((last < sLastName.length() && matches != aMatch) || (sLastName.isEmpty() && matches != aMatch))
	{
		if (*(sLastName.begin()+last) == *(sName.begin()+matches))
			matches++;
		else
		{
			if (matches > 0 && *(sLastName.begin()+last) == *(sName.begin()))
				matches = 1;
			else
				matches = 0;
		}
		last++;
	}
noLmatch:
	if (matches == aMatch)
		return true;
	else
		matches = 0;

	if (search.isEmpty() && sEmail.isEmpty())
	{
		matches++; goto noEmatch;
	}
	else if ((search.isEmpty() && !sEmail.isEmpty()) || (!search.isEmpty() && sEmail.isEmpty()))
		goto noEmatch;
	while ((email < sEmail.length() && matches != aMatch) || (sEmail.isEmpty() && matches != aMatch))
	{
		if (*(sEmail.begin()+email) == *(sName.begin()+matches))
			matches++;
		else
		{
			if (matches > 0 && *(sEmail.begin()+email)== *(sName.begin()))
				matches = 1;
			else
				matches = 0;
		}
		email++;
	}
noEmatch:
	if (matches == aMatch)
		return true;
	else
		matches = 0;

	if (search.isEmpty() && sPhone.isEmpty())
	{
		matches++; goto noPmatch;
	}
	else if ((search.isEmpty() && !sPhone.isEmpty()) || (!search.isEmpty() && sPhone.isEmpty()))
		goto noPmatch;
	while ((phone < sPhone.length() && matches != aMatch) || (sPhone.isEmpty() && matches != aMatch))
	{
		if (*(sPhone.begin()+phone) == *(sName.begin()+matches))
			matches++;
		else
		{
			if (matches > 0 && *(sPhone.begin()+phone)== *(sName.begin()))
				matches = 1;
			else
				matches = 0;
		}
		phone++;
	}
noPmatch:
	if (matches == aMatch)
		return true;
	else
		return false;
}

bool ContactInfo::SearchName(QString searchFirst, QString searchLast)
{
	unsigned int matches, aMatch, first, last;
	matches = first = last = 0; aMatch = 1;
	QString sFname = searchFirst, sLname = searchLast;
	SearchConvert(sFname); SearchConvert(sLname);
	bool Fmatch = false;

	if (searchFirst.isEmpty() && sFirstName.isEmpty())
	{
		matches++; goto noFirstMatch;
	}
	else if (searchFirst.isEmpty() && !sFirstName.isEmpty() || (!searchFirst.isEmpty() && sFirstName.isEmpty()))
		goto noFirstMatch;
	else
		aMatch = searchFirst.length();
	while ((first < sFirstName.length() && matches != aMatch))
	{
		if (*(sFirstName.begin()+first) == *(sFname.begin()+matches))
			matches++;
		else if (first == 0)
		{
			matches = 0;
			goto noFirstMatch;
		}
		first++;
	}
noFirstMatch:
	if ((matches != aMatch && !sFname.isEmpty()) || (sFname.isEmpty() && sLname.isEmpty() && matches != aMatch))
		return false;
	else	
	{
		Fmatch = true;
		matches = 0;
	}

	if (searchLast.isEmpty() && sLastName.isEmpty())
	{
		matches++; goto noLastMatch;
	}
	else if (searchLast.isEmpty() && !sLastName.isEmpty() || (!searchLast.isEmpty() && sLastName.isEmpty()))
		goto noLastMatch;
	else
		aMatch = searchLast.length();

	while (last < sLastName.length() && matches != aMatch)
	{
		if (searchLast.isEmpty())
		{
			if (sLastName.isEmpty())
				matches++;
			else
				goto noLastMatch;
		}
		else if (*(sLastName.begin()+last) == *(sLname.begin()+matches))
			matches++;
		else if (last == 0)
		{
			matches = 0;
			goto noLastMatch;
		}
		last++;
	}
noLastMatch:
	if (sFname.isEmpty() && sLname.isEmpty() && Fmatch && matches != aMatch)
		return false;
	if (matches == aMatch || (searchLast.isEmpty() && Fmatch))
		return true;
	else
		return false;
}

QString ContactInfo::showEmail()
{
	return Email;
}

QString ContactInfo::showPhone()
{
	return Phone;
}

bool ContactInfo::emptyBday()
{
	return Birthday.isEmptyNow();
}

//converts the QString to uppercase
void ContactInfo::SearchConvert(QString &name)
{
	name = name.toUpper();
}

int ContactInfo::nameLength()
{
	return FirstName.length()+LastName.length()+1;
}

QString ContactInfo::showYear()
{
	return Birthday.YearIs();
}

QString ContactInfo::showMonth()
{
	return Birthday.MonthIs();
}

QString ContactInfo::showDay()
{
	return Birthday.DayIs();
}

bool ContactInfo::isDatePartEmpty()
{
	return Birthday.datePartEmpty;
}

ostream & operator<< (ostream & output, ContactInfo & aContact)
{
	if (output == cout)
	{
		if (!aContact.FirstName.isEmpty()) output << "First Name: " << qPrintable(aContact.FirstName);
		if (!aContact.LastName.isEmpty()) output << "\nLast Name: " << qPrintable(aContact.LastName);
		if (aContact.FirstName.isEmpty() && aContact.LastName.isEmpty()) output << "Name: Unknown";
		if (!aContact.Email.isEmpty()) output << "\nEmail: " << qPrintable(aContact.Email);
		if (!aContact.Phone.isEmpty()) output << "\nPhone: " << qPrintable(aContact.Phone);
		if (!aContact.Birthday.isEmptyNow()) output << "\nBirthday: " << aContact.Birthday;
	}
	else
	{
		if (aContact.FirstName.isEmpty()) output << "\\0 "; 
		else output << qPrintable(aContact.FirstName) << " ";
		if (aContact.LastName.isEmpty()) output << "\\0 ";
		else output << qPrintable(aContact.LastName) << " ";
		if (aContact.Email.isEmpty()) output << "\\0 ";
		else output << qPrintable(aContact.Email) << " ";
		if (aContact.Phone.isEmpty()) output << "\\0 ";
		else output << qPrintable(aContact.Phone) << " ";
		output << aContact.Birthday << endl;
	}
	return output;
}

istream & operator>> (istream & input, ContactInfo & aContact)
{
	string first, last, email, phone;
	if (input ==cin)
	{
		cout << "First Name: ";
		getline(cin, first);
		aContact.sFirstName = aContact.FirstName = QString::fromStdString(first);
		cout << "Last Name: ";
		getline(cin, last);
		aContact.sLastName = aContact.LastName = QString::fromStdString(last);
		cout << "Email: ";
		getline(cin, email);
		aContact.Email = QString::fromStdString(email);
		cout << "Phone: ";
		getline(cin, phone);
		aContact.Phone = QString::fromStdString(phone);
		cout << "Birthday\n";
		cin >> aContact.Birthday;
		aContact.SearchConvert(aContact.sFirstName); aContact.SearchConvert(aContact.sLastName);
	}
	else
	{
		input >> first >> last >> email >> phone >> aContact.Birthday;
		aContact.FirstName=QString::fromStdString(first); aContact.LastName=QString::fromStdString(last);
		aContact.Email=QString::fromStdString(email); aContact.Phone=QString::fromStdString(phone);
		if (aContact.FirstName == "\\0") aContact.FirstName = "\0";
		if (aContact.LastName == "\\0") aContact.LastName = "\0";
		if (aContact.Email == "\\0") aContact.Email = "\0";
		if (aContact.Phone == "\\0") aContact.Phone = "\0";
		aContact.sFirstName = aContact.FirstName; aContact.sLastName = aContact.LastName;
		aContact.SearchConvert(aContact.sFirstName); aContact.SearchConvert(aContact.sLastName);
	}
	return input;
}
