#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtGui>
#include "ui_settings.h"
#include "gVars.h"

class Settings : public QDialog
{
	Q_OBJECT

public:
	Settings(QWidget *parent=0);
	~Settings();
	bool openDcheck, openLcheck;
	bool isDchecked();
	bool isLchecked();

private:
	Ui::settingsDialog ui;
	bool lastFile, defaultFile;

private slots:
	void on_openLastAtStart_checkBox_toggled();
	void on_openDefaultAtStart_checkBox_toggled();
	void on_dFile_pushButton_clicked();
	void on_okay_pushButton_clicked();
	void on_cancel_pushButton_clicked();

public slots:
};

#endif