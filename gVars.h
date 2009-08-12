#ifndef G_VARS_H
#define G_VARS_H

#include "ContactInfo.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <qstring.h>
#include <qhash.h>
#include <QListWidgetItem>

extern vector<ContactInfo> contactDB;
extern ContactInfo contact;
extern int numContactInfoRecords, finding, foundEnd, fileSize, index;
extern QString aFirstName, aLastName, checkChoice, search;
extern DateType dBegin, dEnd;
extern QString cPath, cFile, dFile, dPath, dFileSetting;

extern ifstream fin;
extern ofstream fout;

extern QHash<QListWidgetItem*,int> hash;
extern int i;
extern vector<QListWidgetItem*> items;
extern bool openLastAtStart, openDefaultAtStart, doNothing, cSave, settings_okayPushed;

//gFunctions
extern void dFileUpdate();
extern void cFileUpdate();
extern void cfgUpdate();
extern bool mergeTwoSortedVectors(vector<ContactInfo>&,vector<ContactInfo>&,vector<ContactInfo>&);
extern bool mergeSort(vector<ContactInfo>&);

#endif