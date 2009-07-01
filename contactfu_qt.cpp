#include "contactfu_qt.h"
#include <windows.h>
#include <iostream>
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

bool mergeTwoSortedVectors(vector<ContactInfo> &, vector<ContactInfo> &, vector<ContactInfo> &);
bool mergeSort(vector<ContactInfo> &);
void GrammarCheck(int);

vector<ContactInfo> contactDB;
ContactInfo contact;
int numContactInfoRecords=0, finding = 0, foundEnd = 0, fileSize=0, index=0;
QString aFirstName, aLastName, checkChoice, search;
DateType dBegin, dEnd;
QString cPath, cFile, dFile, dPath;

ifstream fin;
ofstream fout;

QString buffer;
QObject *display;

ContactFU_QT::ContactFU_QT(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this); setWindowTitle(tr("ContactFU Alpha 0.5"));
	//connect(ui.actionQuit,SIGNAL(triggered()),this,SLOT(close())); //might be needed later
	QWorkspace *workspace = new QWorkspace(this);
	setCentralWidget(workspace);
	QFile file("ContactFU.cfg");
	if (!file.exists())
	{
		file.open(QFile::WriteOnly);
		QTextStream fout(&file);
		QDir path; cPath = path.currentPath()+"/";
		fout << cPath;
		file.close();
	}
	else
	{
		file.open(QIODevice::ReadOnly);
		QTextStream check_in(&file);
		dPath = check_in.readAll();
		cPath.resize(dPath.size());
		file.close();
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
	}
	string test = qPrintable(contactDB[0].showFirstName());
	QTextEdit *text = new QTextEdit; text->setParent(workspace); text->setReadOnly(true);
	text->setText(contactDB[0].showFirstName()+" "+contactDB[0].showLastName()+"\n"+contactDB[0].showEmail()); 
	text->show();
}

ContactFU_QT::~ContactFU_QT()
{
}

void ContactFU_QT::on_actionOpenProject_triggered()
{
	QString file_open = QFileDialog::getOpenFileName(this, tr("Open"),QString(), tr("Data Base Files(*.db)\0*.db\0"));
}
void ContactFU_QT::on_actionToggleMaxWindow_triggered()
{
	if (QWidget::isMaximized())
		QWidget::showNormal();
	else
		QWidget::showMaximized();
}
void ContactFU_QT::on_actionQuit_triggered()
{
	close();
}

bool mergeTwoSortedVectors(vector<ContactInfo> &vecA, vector<ContactInfo> &vecB, vector<ContactInfo> &vecC)
{
	//variables
	ContactInfo check;
	unsigned int countA, countB, countC;
	countA = countB = countC = 0;

	if (vecA.size() < 0 || vecB.size() < 0) //if negative amount of contacts
	{
		system("CLS"); //clear screen
		cout << "Could not complete operation due to a negative series.\n";
		_getch(); //pauses for user
		return false;
	}

	check = vecA[0];
	for (unsigned int count = 0; count < vecA.size(); count++) //makes sure 1st series is sorted
	{
		if (check > vecA[count]) //if series is not sorted
		{
			system("CLS"); //clear screen
			cout << "A series was not entered in ascending order.";
			_getch(); //pauses for user
			return false;
		}
		check = vecA[count];
	}

	check = vecB[0];
	for (unsigned int count = 0; count < vecB.size(); count++) //makes sure 2nd series is sorted
	{
		if (check > vecB[count]) //if series is not sorted
		{
			system("CLS"); //clear screen
			cout << "A series was not entered in ascending order.";
			_getch(); //pause for user
			return false;
		}
		check = vecB[count];
	}

	while (countC < vecB.size() + vecA.size() && countA < vecA.size() && countB < vecB.size()) //loops while both series aren't merged
	{
		if (vecA[countA] < vecB[countB])//merges the lower number
		{
			vecC[countC] = vecA[countA];
			countA++; countC++;
		}
		else
		{
			vecC[countC] = vecB[countB]; //merges the lower number
			countB++; countC++;
		}
	}

	if (countA < vecA.size()) //checks if to see if the 1st series is already merged
	{
		while (countA < vecA.size()) //merges the rest of the series
		{
			vecC[countC] = vecA[countA];
			countA++; countC++;
		}
	}
	else if (countB < vecB.size()) //checks to see if the 2nd series is already merged
	{
		while (countB < vecB.size()) //merges the rest of the series
		{
			vecC[countC] = vecB[countB];
			countB++; countC++;
		}
	}
	return true;
}

bool mergeSort(vector<ContactInfo> &vecToSort)
{
	//variables
	ContactInfo low;
	//vectors
	vector<ContactInfo> vec1;
	vector<ContactInfo> vec2;

	if (vecToSort.size() < 0) //if negative amount of numbers
	{
		system("CLS"); //clear screen
		cout << "Could not complete operation due to a negative series.\n";
		_getch(); //pause for user
		return false;
	}
	else if (vecToSort.size() <= 1) //series already sorted
		return true;
	else if (vecToSort.size() == 2) //only 2 numbers
	{
		if (vecToSort[0] < vecToSort[1] || vecToSort[0] == vecToSort[1]) //check the value of the numbers
			return true;
		else //switch the numbers if necessary
		{
			low = vecToSort[1];
			vecToSort[1] = vecToSort[0];
			vecToSort[0] = low;
			return true;
		}
	}
	else //if series is long enough to sort
	{
		vec1.resize(vecToSort.size()/2); vec2.resize(vecToSort.size() - vecToSort.size()/2); //resize vectors to split main vector in half
		for (unsigned int count = 0; count < vecToSort.size()/2; count++) //copy first half to vec1
		{
			vec1[count] = vecToSort[count];
		}
		int count2 = 0; //count2 variable for vec2, copy second half to vec2
		for (unsigned int count = vecToSort.size() - (vecToSort.size() - vecToSort.size()/2); count < vecToSort.size(); count++)
		{
			vec2[count2] = vecToSort[count];
			count2++;
		}

		mergeSort(vec1); mergeSort(vec2); //sort both vector halves
		mergeTwoSortedVectors(vec1, vec2, vecToSort); //merge into one vector
	}
	return true;
}

void GrammarCheck(int integer) //checks grammar
{
	if (integer == 1 || (integer > 11 && integer % 100 != 11 && integer % 10 == 1) || integer == -1 || (integer < -11 && integer % 100 != -11 && integer % 10 == -1))
		cout << "st";
	else if (integer == 2 || (integer % 10 == 2 && (integer > 12 || integer < -12)) || integer % 10 == -2)
		cout << "nd";
	else if (integer == 3 || (integer % 10 == 3 && (integer > 13 || integer < -13)) || integer % 10 == -3)
		cout << "rd";
	else
		cout << "th";
}