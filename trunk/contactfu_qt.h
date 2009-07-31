#ifndef CONTACTFU_QT_H
#define CONTACTFU_QT_H

#include <QtGui/QMainWindow>
#include <string>
#include <vector>
#include <conio.h>
#include <fstream>
#include "ui_contactfu_qt.h"
#include "ContactInfo.h"

class ContactFU_QT : public QMainWindow
{
	Q_OBJECT

public:
	ContactFU_QT(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ContactFU_QT();

private:
	Ui::ContactFU_QTClass ui;

private slots:
	void on_actionOpenProject_triggered();
	void on_actionQuit_triggered();
	//void on_actionToggleMaxWindow_triggered();
	void on_action_Sorting_triggered();
	void on_listing_currentItemChanged();
	void on_saveButton1_clicked();
	void on_saveButton2_clicked();

public slots:
	void contactClicked();
	void saveChanges();
	void updateList();
};

#endif // CONTACTFU_QT_H
