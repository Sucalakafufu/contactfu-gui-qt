#include "settings.h"

Settings::Settings(QWidget *parent) : QDialog(parent)
{
	ui.setupUi(this);
	ui.dFile_lineEdit->setReadOnly(true);
	openDcheck=ui.openDefaultAtStart_checkBox->isChecked(); openLcheck=ui.openLastAtStart_checkBox->isChecked();
	if (ui.openLastAtStart_checkBox->isChecked())
	{
		ui.dFile_label->setEnabled(false); ui.dFile_lineEdit->setEnabled(false); ui.dFile_pushButton->setEnabled(false);
	}
}

Settings::~Settings()
{
}

void Settings::on_openLastAtStart_checkBox_toggled()
{
	if (ui.openLastAtStart_checkBox->isChecked())
	{
		ui.dFile_label->setEnabled(false); ui.dFile_lineEdit->setEnabled(false); ui.dFile_pushButton->setEnabled(false);
		ui.openDefaultAtStart_checkBox->setChecked(false);
		lastFile=true; defaultFile=false;
	}
	else
		ui.openDefaultAtStart_checkBox->setChecked(true);
}
void Settings::on_openDefaultAtStart_checkBox_toggled()
{
	if (ui.openDefaultAtStart_checkBox->isChecked())
	{
		ui.dFile_label->setEnabled(true); ui.dFile_lineEdit->setEnabled(true); ui.dFile_pushButton->setEnabled(true);
		ui.openLastAtStart_checkBox->setChecked(false);
		defaultFile=true; lastFile=false;
	}
	else
		ui.openLastAtStart_checkBox->setChecked(true);
}
void Settings::on_dFile_pushButton_clicked()
{
	QString browse = QFileDialog::getOpenFileName(this, tr("Default File"),QString(),tr("DataBase Files(*.db)\0*.db\0"));
}
void Settings::on_okay_pushButton_clicked()
{
	openDefaultAtStart=defaultFile; openLastAtStart=lastFile;
	close();
}
void Settings::on_cancel_pushButton_clicked()
{
	ui.openDefaultAtStart_checkBox->setChecked(openDcheck); ui.openLastAtStart_checkBox->setChecked(openLcheck);
	close();
}

bool Settings::isDchecked()
{
	return ui.openDefaultAtStart_checkBox->isChecked();
}

bool Settings::isLchecked()
{
	return ui.openLastAtStart_checkBox->isChecked();
}