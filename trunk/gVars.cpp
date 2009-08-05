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