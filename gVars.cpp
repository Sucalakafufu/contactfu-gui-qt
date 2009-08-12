#include "gVars.h"

vector<ContactInfo> contactDB;
ContactInfo contact;
int numContactInfoRecords=0, finding=0, foundEnd=0, fileSize=0, index=0;
QString aFirstName, aLastName, checkChoice, search;
DateType dBegin, dEnd;
QString cPath, cFile, dFile, dPath, dFileSetting;

ifstream fin;
ofstream fout;

QHash<QListWidgetItem*,int> hash;
int i=0;
vector<QListWidgetItem*> items;
bool openLastAtStart=true, openDefaultAtStart=false, doNothing=false, cSave=true, settings_okayPushed=false;

//gFunctions
void dFileUpdate()
{
	for(finding=0;*(dPath.end()-finding)!='.'&&finding<dPath.length();finding++){}
	foundEnd = finding;
	for(;*(dPath.end()-finding)!='/'&&finding<dPath.length();finding++){}
	dFile.resize(finding-foundEnd-1);
	for(int count=finding-1,index = 0;count>foundEnd;index++,count--) *(dFile.begin()+index)=*(dPath.end()-count);
}

void cFileUpdate()
{
	for(finding=0;*(cPath.end()-finding)!='.'&&finding<cPath.length();finding++){}
	foundEnd = finding;
	for(;*(cPath.end()-finding)!='/'&&finding<cPath.length();finding++){}
	cFile.resize(finding-foundEnd-1);
	for(int count=finding-1,index = 0;count>foundEnd;index++,count--) *(cFile.begin()+index)=*(cPath.end()-count);
}

void cfgUpdate()
{
	fout.open("ContactFU.cfg"); fout<<qPrintable(dPath)<<endl; fout<<openLastAtStart<<endl; fout<<openDefaultAtStart<<endl;
	fout<<doNothing<<endl;
	fout.close();
}

bool mergeTwoSortedVectors(vector<ContactInfo> &vecA, vector<ContactInfo> &vecB, vector<ContactInfo> &vecC)
{
	//variables
	ContactInfo check;
	unsigned int countA, countB, countC;
	countA = countB = countC = 0;

	if (vecA.size() < 0 || vecB.size() < 0) //if negative amount of contacts
		//system("CLS"); //clear screen
		//cout << "Could not complete operation due to a negative series.\n";
		////_getch(); //pauses for user
		return false;

	check = vecA[0];
	for (unsigned int count = 0; count < vecA.size(); count++) //makes sure 1st series is sorted
	{
		if (check > vecA[count]) //if series is not sorted
			//system("CLS"); //clear screen
			//cout << "A series was not entered in ascending order.";
			////_getch(); //pauses for user
			return false;
		check = vecA[count];
	}

	check = vecB[0];
	for (unsigned int count = 0; count < vecB.size(); count++) //makes sure 2nd series is sorted
	{
		if (check > vecB[count]) //if series is not sorted
			//system("CLS"); //clear screen
			//cout << "A series was not entered in ascending order.";
			////_getch(); //pause for user
			return false;
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
		//system("CLS"); //clear screen
		//cout << "Could not complete operation due to a negative series.\n";
		////_getch(); //pause for user
		return false;
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
