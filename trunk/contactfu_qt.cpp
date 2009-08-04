#include "contactfu_qt.h"
#include "gVars.h"
#include <windows.h>
#include <sstream>
#include <qfiledialog.h>
#include <qobject.h>
#include <qlabel.h> 
#include <qwidget.h>
#include <qfile.h>
#include <qdir.h>
#include <qtextstream.h>
#include <qdatastream.h>
#include <qworkspace.h>
#include <qlistview.h>
#include <qtextedit.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <QVBoxLayout>
#include <qframe.h>
#include <qlineedit.h>
#include <QFormLayout>
#include <qwidget.h>
#include <QListWidget>
#include <QHash>
#include <qdatetime.h>
#include <qmessagebox.h>
#include <qshortcut.h>

//bool mergeTwoSortedVectors(vector<ContactInfo> &, vector<ContactInfo> &, vector<ContactInfo> &);
//bool mergeSort(vector<ContactInfo> &);
//void GrammarCheck(int);

ContactFU_QT::ContactFU_QT(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint);
	setWindowTitle(tr("ContactFU Beta 0.5"));
	ui.addContactButton->setShortcut(Qt::CTRL+Qt::Key_A);
	ui.deleteContactButton->setShortcut(Qt::Key_Delete);
	settings_window = new Settings(this); 
	/*ui.saveButton1->setShortcut(Qt::SHIFT+Qt::Key_S);
	ui.saveButton2->setShortcut(Qt::SHIFT+Qt::Key_S);*/

	QFile file("ContactFU.cfg");
	if (!file.exists())
	{
		file.open(QFile::WriteOnly);
		file.close();
	}
	else
	{
		file.open(QIODevice::ReadOnly);
		QTextStream check_in(&file);
		dPath = check_in.readAll();
		file.close();
		if (!dPath.isEmpty())
		{
			cPath.resize(dPath.size());		
			for(finding=0;*(dPath.end()-finding)!='.'&&finding<dPath.length();finding++){}
			foundEnd = finding;
			for(;*(dPath.end()-finding)!='/'&&finding<dPath.length();finding++){}
			dFile.resize(finding-foundEnd-1); cFile.resize(dFile.size());
			for(int index=0;index<dPath.length();index++) *(cPath.begin()+index)=*(dPath.begin()+index);
			for(int count=finding-1,index = 0;count>foundEnd;index++,count--) *(dFile.begin()+index)=*(dPath.end()-count);
			for(int index=0;index<dFile.length();index++) *(cFile.begin()+index)=*(dFile.begin()+index);
			fin.open(dPath.toStdString().c_str());
			fin >> numContactInfoRecords;
			for (int count = 0; count < numContactInfoRecords; count++) //loops for number of contacts specified
			{
				fin >> contact;
				contactDB.push_back(contact); //stores new contact in DB
			}
			fin.close(); fin.clear();
			updateList();
		}
	}
	
}

ContactFU_QT::~ContactFU_QT()
{
}

void ContactFU_QT::on_actionOpenProject_triggered()
{
	QString file_open = QFileDialog::getOpenFileName(this, tr("Open"),QString(), tr("DataBase Files(*.db)\0*.db\0"));
	if (!file_open.isEmpty())
	{
		contactDB.clear(); cPath = file_open; cFileUpdate();
		dPath=cPath; dFile=cFile; cfgUpdate();
		fin.open(file_open.toStdString().c_str());
		fin >> numContactInfoRecords;
		for (int count = 0; count < numContactInfoRecords; count++) //loops for number of contacts specified
		{
			fin >> contact;
			contactDB.push_back(contact); //stores new contact in DB
		}
		fin.close(); fin.clear();
		ui.listing->clear();
		if (contactDB.empty()) clearInfo();
		else updateList();
	}
}
void ContactFU_QT::on_actionSave_triggered()
{
	if (cPath.isEmpty())
	{
		QString file_save = QFileDialog::getSaveFileName(this, tr("Save"),QString(),tr("DataBase Files(*.db)\0*.db\0"));
		if (!file_save.isEmpty())
		{
			cPath = file_save; cFileUpdate();
			dPath = cPath; dFile = cFile; cfgUpdate();
			fout.open(cPath.toStdString().c_str());
			fout<<contactDB.size(); fout<<endl<<endl;
			for (unsigned int count=0;count<contactDB.size();count++)
				fout<<contactDB[count];
			fout.close();
			QString info = "Contacts Saved in "+cFile;
			QMessageBox::information(this,tr("Save Successful"),info);
		}
	}
	else
	{
		fout.open(cPath.toStdString().c_str());
		fout<<contactDB.size(); fout<<endl<<endl;
		for (unsigned int count=0;count<contactDB.size();count++)
			fout<<contactDB[count];
		fout.close();
		QString info = "Contacts Saved in "+cFile;
		QMessageBox::information(this,tr("Save Successful"),info);
	}
}
//void ContactFU_QT::on_actionToggleMaxWindow_triggered() //toggles between normal and maximized
//{
//	if (QWidget::isMaximized())
//		QWidget::showNormal();
//	else
//	{
//		QWidget::showMaximized();
//	}
//}
void ContactFU_QT::on_actionQuit_triggered() //quits
{
	close();
}
void ContactFU_QT::on_actionSortBy_triggered()
{
}
void ContactFU_QT::on_actionSettings_triggered()
{
	if (settings_window->isLchecked())
	{
		settings_window->openLcheck=true; settings_window->openDcheck=false;
	}
	else if (settings_window->isDchecked())
	{
		settings_window->openDcheck=true; settings_window->openLcheck=false;
	}
	settings_window->show();
}
void ContactFU_QT::on_listing_currentItemChanged() //if selected contact changes
{
	contactClicked();
}

void ContactFU_QT::on_saveButton1_clicked()
{
	if (!contactDB.empty())
		saveChanges();
}
void ContactFU_QT::on_saveButton2_clicked()
{
	if (!contactDB.empty())
		saveChanges();
}
void ContactFU_QT::on_addContactButton_clicked()
{
	addContact();
}
void ContactFU_QT::on_deleteContactButton_clicked()
{
	if (!contactDB.empty())
	{
		i=hash.value(ui.listing->currentItem());
		delete items[i]; 
		items.erase(items.begin()+i);
		contactDB.erase(contactDB.begin()+i);
		itemHashUpdate();
		if (contactDB.empty()) clearInfo();
	}
}

void ContactFU_QT::addContact()
{
	ContactInfo tempContact;
	contactDB.push_back(tempContact); QListWidgetItem *tempItem = new QListWidgetItem(tr("New Contact"),ui.listing);
	items.push_back(tempItem); int added = items.size()-1;
	hash.insert(items[added],added); ui.listing->sortItems();
	ui.listing->setCurrentItem(items[added]); contactClicked();
}

void ContactFU_QT::cfgUpdate()
{
	fout.open("ContactFU.cfg"); fout<<qPrintable(dPath); fout.close();
}

void ContactFU_QT::clearInfo()
{
	ui.Fname_lineEdit->clear(); ui.Lname_lineEdit->clear(); ui.Email_lineEdit->clear(); ui.Phone_lineEdit->clear();
	ui.month_lineEdit->clear(); ui.day_lineEdit->clear(); ui.year_lineEdit->clear();
}

void ContactFU_QT::itemHashUpdate()
{
	hash.clear();
	for (unsigned int count=0;count<items.size();count++)
	{
		hash.insert(items[count],count);
	}
}

void ContactFU_QT::updateList()
{
	items.clear();
	items.resize(contactDB.size());
	for (unsigned int count=0;count<contactDB.size();count++)
	{
		if (contactDB[count].showLastName().isEmpty())
		{
			if (contactDB[count].showFirstName().isEmpty())
				items[count] = new QListWidgetItem(tr("Name Unknown"), ui.listing);
			else
				items[count] = new QListWidgetItem(contactDB[count].showFirstName(), ui.listing);
		}
		else if (contactDB[count].showFirstName().isEmpty())
			items[count] = new QListWidgetItem(contactDB[count].showLastName(), ui.listing);
		else
			items[count] = new QListWidgetItem(contactDB[count].showLastName()+", "+contactDB[count].showFirstName(), ui.listing);
		hash.insert(items[count],count);
	}
	ui.listing->setSortingEnabled(true); ui.listing->sortItems(); //sorts names of contacts
	ui.listing->setCurrentRow(0); contactClicked();
}

void ContactFU_QT::contactClicked() //displays info of the currently selected contact
{
	if (!contactDB.empty())
	{
		int clicked=hash.value(ui.listing->currentItem());
		ui.Fname_lineEdit->setText(contactDB[clicked].showFirstName());
		ui.Lname_lineEdit->setText(contactDB[clicked].showLastName());
		ui.Email_lineEdit->setText(contactDB[clicked].showEmail());
		ui.Phone_lineEdit->setText(contactDB[clicked].showPhone());
		ui.month_lineEdit->setText(contactDB[clicked].showMonth());
		ui.day_lineEdit->setText(contactDB[clicked].showDay());
		ui.year_lineEdit->setText(contactDB[clicked].showYear());
	}
}

void ContactFU_QT::saveChanges()
{
	i=hash.value(ui.listing->currentItem());DateType aBirthday;
	if (!aBirthday.SetDate(ui.month_lineEdit->text(),ui.day_lineEdit->text(),ui.year_lineEdit->text()))
	{
		if (aBirthday.datePartEmpty)
		{
			QString temp = "Birthday must have full date or no date";
			QMessageBox::information(this,tr("Syntax Error"),temp);
			contactClicked();
			return;
		}
		else
		{
			QString temp = "Birthday is invalid";
			QMessageBox::information(this,tr("Syntax Error"),temp);
			contactClicked();
			return;
		}
	}
	contactDB[i].SetContactInfo(ui.Fname_lineEdit->displayText(),ui.Lname_lineEdit->text(),ui.Email_lineEdit->text(),ui.Phone_lineEdit->text(),aBirthday);
	if (contactDB[i].showLastName().isEmpty())
	{
		if (contactDB[i].showFirstName().isEmpty())
			ui.listing->currentItem()->setText(tr("Name Unknown"));
		else
			ui.listing->currentItem()->setText(contactDB[i].showFirstName());
	}
	else if (contactDB[i].showFirstName().isEmpty())
		ui.listing->currentItem()->setText(contactDB[i].showLastName());
	else
		ui.listing->currentItem()->setText(contactDB[i].showLastName()+", "+contactDB[i].showFirstName());
}

void ContactFU_QT::cFileUpdate()
{
	for(finding=0;*(cPath.end()-finding)!='.'&&finding<cPath.length();finding++){}
	foundEnd = finding;
	for(;*(cPath.end()-finding)!='/'&&finding<cPath.length();finding++){}
	cFile.resize(finding-foundEnd-1);
	for(int count=finding-1,index = 0;count>foundEnd;index++,count--) *(cFile.begin()+index)=*(cPath.end()-count);
}

//bool mergeTwoSortedVectors(vector<ContactInfo> &vecA, vector<ContactInfo> &vecB, vector<ContactInfo> &vecC)
//{
//	//variables
//	ContactInfo check;
//	unsigned int countA, countB, countC;
//	countA = countB = countC = 0;
//
//	if (vecA.size() < 0 || vecB.size() < 0) //if negative amount of contacts
//	{
//		system("CLS"); //clear screen
//		cout << "Could not complete operation due to a negative series.\n";
//		_getch(); //pauses for user
//		return false;
//	}
//
//	check = vecA[0];
//	for (unsigned int count = 0; count < vecA.size(); count++) //makes sure 1st series is sorted
//	{
//		if (check > vecA[count]) //if series is not sorted
//		{
//			system("CLS"); //clear screen
//			cout << "A series was not entered in ascending order.";
//			_getch(); //pauses for user
//			return false;
//		}
//		check = vecA[count];
//	}
//
//	check = vecB[0];
//	for (unsigned int count = 0; count < vecB.size(); count++) //makes sure 2nd series is sorted
//	{
//		if (check > vecB[count]) //if series is not sorted
//		{
//			system("CLS"); //clear screen
//			cout << "A series was not entered in ascending order.";
//			_getch(); //pause for user
//			return false;
//		}
//		check = vecB[count];
//	}
//
//	while (countC < vecB.size() + vecA.size() && countA < vecA.size() && countB < vecB.size()) //loops while both series aren't merged
//	{
//		if (vecA[countA] < vecB[countB])//merges the lower number
//		{
//			vecC[countC] = vecA[countA];
//			countA++; countC++;
//		}
//		else
//		{
//			vecC[countC] = vecB[countB]; //merges the lower number
//			countB++; countC++;
//		}
//	}
//
//	if (countA < vecA.size()) //checks if to see if the 1st series is already merged
//	{
//		while (countA < vecA.size()) //merges the rest of the series
//		{
//			vecC[countC] = vecA[countA];
//			countA++; countC++;
//		}
//	}
//	else if (countB < vecB.size()) //checks to see if the 2nd series is already merged
//	{
//		while (countB < vecB.size()) //merges the rest of the series
//		{
//			vecC[countC] = vecB[countB];
//			countB++; countC++;
//		}
//	}
//	return true;
//}
//
//bool mergeSort(vector<ContactInfo> &vecToSort)
//{
//	//variables
//	ContactInfo low;
//	//vectors
//	vector<ContactInfo> vec1;
//	vector<ContactInfo> vec2;
//
//	if (vecToSort.size() < 0) //if negative amount of numbers
//	{
//		system("CLS"); //clear screen
//		cout << "Could not complete operation due to a negative series.\n";
//		_getch(); //pause for user
//		return false;
//	}
//	else if (vecToSort.size() <= 1) //series already sorted
//		return true;
//	else if (vecToSort.size() == 2) //only 2 numbers
//	{
//		if (vecToSort[0] < vecToSort[1] || vecToSort[0] == vecToSort[1]) //check the value of the numbers
//			return true;
//		else //switch the numbers if necessary
//		{
//			low = vecToSort[1];
//			vecToSort[1] = vecToSort[0];
//			vecToSort[0] = low;
//			return true;
//		}
//	}
//	else //if series is long enough to sort
//	{
//		vec1.resize(vecToSort.size()/2); vec2.resize(vecToSort.size() - vecToSort.size()/2); //resize vectors to split main vector in half
//		for (unsigned int count = 0; count < vecToSort.size()/2; count++) //copy first half to vec1
//		{
//			vec1[count] = vecToSort[count];
//		}
//		int count2 = 0; //count2 variable for vec2, copy second half to vec2
//		for (unsigned int count = vecToSort.size() - (vecToSort.size() - vecToSort.size()/2); count < vecToSort.size(); count++)
//		{
//			vec2[count2] = vecToSort[count];
//			count2++;
//		}
//
//		mergeSort(vec1); mergeSort(vec2); //sort both vector halves
//		mergeTwoSortedVectors(vec1, vec2, vecToSort); //merge into one vector
//	}
//	return true;
//}
//
//void GrammarCheck(int integer) //checks grammar
//{
//	if (integer == 1 || (integer > 11 && integer % 100 != 11 && integer % 10 == 1) || integer == -1 || (integer < -11 && integer % 100 != -11 && integer % 10 == -1))
//		cout << "st";
//	else if (integer == 2 || (integer % 10 == 2 && (integer > 12 || integer < -12)) || integer % 10 == -2)
//		cout << "nd";
//	else if (integer == 3 || (integer % 10 == 3 && (integer > 13 || integer < -13)) || integer % 10 == -3)
//		cout << "rd";
//	else
//		cout << "th";
//}