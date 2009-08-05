#ifndef CONTACTFU_QT_H
#define CONTACTFU_QT_H

#include <QtGui/QMainWindow>
#include <string>
#include <vector>
#include <conio.h>
#include <fstream>
#include "ui_contactfu_qt.h"
#include "ContactInfo.h"
#include "settings.h"

class ContactFU_QT : public QMainWindow
{
	Q_OBJECT

public:
	ContactFU_QT(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ContactFU_QT();

private:
	Ui::ContactFU_QTClass ui;
	Settings *settings_window;

private slots:
	void on_actionOpen_triggered();
	void on_actionQuit_triggered();
	//void on_actionToggleMaxWindow_triggered();
	void on_actionSortBy_triggered();
	void on_actionSave_triggered();
	void on_actionSaveAs_triggered();
	void on_actionNew_triggered();
	void on_actionSettings_triggered();
	void on_listing_currentItemChanged();
	void on_saveButton1_clicked();
	void on_saveButton2_clicked();
	void on_addContactButton_clicked();
	void on_deleteContactButton_clicked();

public slots:
	void contactClicked();
	void saveChanges();
	void updateList();
	void itemHashUpdate();
	void clearInfo();
	void addContact();
	void saveFile();
	void saveFileAs();
	void openFile();
	void fileChanged();
};

#endif // CONTACTFU_QT_H
