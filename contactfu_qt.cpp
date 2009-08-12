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
	setWindowTitle(tr("ContactFU Beta 0.6.2"));
	ui.addContactButton->setShortcut(Qt::CTRL+Qt::Key_A);
	ui.deleteContactButton->setShortcut(Qt::Key_Delete);
	settings_window = new Settings(this); 
	ui.saveButton1->setShortcut(Qt::CTRL+Qt::Key_D);

	QFile file("ContactFU.cfg");
	if (!file.exists())
	{
		file.open(QFile::WriteOnly);
		file.close();
		cfgUpdate();
	}
	else
	{
		file.open(QIODevice::ReadOnly);
		QTextStream check_in(&file);
		dPath = check_in.readLine();
		openLastAtStart=check_in.readLine().toUInt();
		openDefaultAtStart=check_in.readLine().toUInt();
		doNothing=check_in.readLine().toUInt();
		if (!openDefaultAtStart&&!openLastAtStart&&!doNothing) openLastAtStart=true;
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
			mergeSort(contactDB);
			updateList();
		}
		if (contactDB.empty()) dPath.clear();
		cfgUpdate();
	}
}

ContactFU_QT::~ContactFU_QT()
{
	this->show(); fileChanged(); close();
}

void ContactFU_QT::on_actionOpen_triggered()
{
	openFile();
}
void ContactFU_QT::on_actionSave_triggered()
{
	saveFile();
}
void ContactFU_QT::on_actionSaveAs_triggered()
{
	saveFileAs();
}
void ContactFU_QT::on_actionNew_triggered()
{
	fileChanged();
	ui.listing->clear(); clearInfo(); cFile.clear(); cPath.clear(); contactDB.clear(); items.clear(); hash.clear();
	if (openLastAtStart)
	{
		dPath=cPath; dFile=cFile; cfgUpdate();
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
	fileChanged();
	close();
}
void ContactFU_QT::on_actionSortBy_triggered()
{
}
void ContactFU_QT::on_actionSettings_triggered()
{
	settings_okayPushed=false;
	if (openLastAtStart)
	{
		settings_window->openLcheck=true; settings_window->openDcheck=false;
	}
	else if (openDefaultAtStart)
	{
		settings_window->openDcheck=true; settings_window->openLcheck=false;
	}
	else if (doNothing)
	{
		settings_window->openDcheck=false; settings_window->openLcheck=false;
	}
	settings_window->show(); settings_window->setChecked();  dFileSetting=dFile;
	if (dFileSetting.isEmpty()) dFileSetting="<None>"; 
	if (settings_window->isDchecked()) settings_window->setDFileEdit(dFileSetting);
	else settings_window->setDFileEdit(tr("<None>"));
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
		cSave=false;
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
	cSave=false;
	ContactInfo tempContact;
	contactDB.push_back(tempContact); QListWidgetItem *tempItem = new QListWidgetItem(tr("New Contact"),ui.listing);
	items.push_back(tempItem); int added = items.size()-1;
	hash.insert(items[added],added); ui.listing->sortItems();
	ui.listing->setCurrentItem(items[added]); contactClicked();
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
	cSave=false;
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

void ContactFU_QT::saveFile()
{
	if (cPath.isEmpty())
	{
		QString file_save = QFileDialog::getSaveFileName(this, tr("Save"),QString(),tr("DataBase Files(*.db)\0*.db\0"));
		if (!file_save.isEmpty())
		{
			cPath = file_save; cFileUpdate();
			if (openLastAtStart)
			{
				dPath = cPath; dFile = cFile; cfgUpdate();
			}
			fout.open(cPath.toStdString().c_str());
			fout<<contactDB.size(); fout<<endl<<endl;
			for (unsigned int count=0;count<contactDB.size();count++)
				fout<<contactDB[count];
			fout.close();
			QString info = "Contacts Saved in "+cFile;
			QMessageBox::information(this,tr("Save Successful"),info);
			cSave=true;
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
		cSave=true;
	}
}

void ContactFU_QT::saveFileAs()
{
	QString file_save = QFileDialog::getSaveFileName(this, tr("Save As..."),QString(),tr("DataBase Files(*.db)\0*.db\0"));
	if (!file_save.isEmpty())
	{
		cPath = file_save; cFileUpdate();
		if (openLastAtStart)
		{
			dPath = cPath; dFile = cFile; cfgUpdate();
		}
		fout.open(cPath.toStdString().c_str());
		fout<<contactDB.size(); fout<<endl<<endl;
		for (unsigned int count=0;count<contactDB.size();count++)
			fout<<contactDB[count];
		fout.close();
		QString info = "Contacts Saved in "+cFile;
		QMessageBox::information(this,tr("Save Successful"),info);
		cSave=true;
	}
}

void ContactFU_QT::openFile()
{
	QString file_open = QFileDialog::getOpenFileName(this, tr("Open"),QString(), tr("DataBase Files(*.db)\0*.db\0"));
	if (!file_open.isEmpty())
	{
		contactDB.clear(); cPath = file_open; cFileUpdate();
		if (openLastAtStart)
		{
			dPath=cPath; dFile=cFile; cfgUpdate();
		}
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

void ContactFU_QT::fileChanged()
{
	if (!cSave)
	{
		int answer;
		if (!cFile.isEmpty())
			answer = QMessageBox::question(this,tr("ContactFU"),tr("Information in ")+cFile+tr(" has been changed.\n\nWould you like to save the changes?"),tr("&Yes"),tr("&No"));
		else
			answer = QMessageBox::question(this,tr("ContactFU"),tr("Information has been changed.\n\nWould you like to save the changes?"),tr("&Yes"),tr("&No"));
		if (answer == 0)
			saveFile();
	}
	cSave=true;
}