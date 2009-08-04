#include "gVars.h"

//numContactInfoRecords=finding=foundEnd=fileSize=index=i=0;

vector<ContactInfo> contactDB;
ContactInfo contact;
int numContactInfoRecords=0, finding=0, foundEnd=0, fileSize=0, index=0;
QString aFirstName, aLastName, checkChoice, search;
DateType dBegin, dEnd;
QString cPath, cFile, dFile, dPath;

ifstream fin;
ofstream fout;

QHash<QListWidgetItem*,int> hash;
int i=0;
vector<QListWidgetItem*> items;
bool openLastAtStart=true, openDefaultAtStart=true;