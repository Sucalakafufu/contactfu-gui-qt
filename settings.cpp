#include "settings.h"

Settings::Settings(QWidget *parent) : QDialog(parent)
{
	ui.setupUi(this); setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
	ui.dFile_lineEdit->setReadOnly(true);
	openDcheck=ui.openDefaultAtStart_checkBox->isChecked(); openLcheck=ui.openLastAtStart_checkBox->isChecked();
	if (ui.openLastAtStart_checkBox->isChecked())
	{
		ui.dFile_label->setEnabled(false); ui.dFile_lineEdit->setEnabled(false); ui.dFile_pushButton->setEnabled(false);
	}
}

Settings::~Settings()
{
	if (!settings_okayPushed)
	{
		ui.openDefaultAtStart_checkBox->setChecked(openDcheck); ui.openLastAtStart_checkBox->setChecked(openLcheck);
	}
}

void Settings::on_openLastAtStart_checkBox_toggled()
{
	if (ui.openLastAtStart_checkBox->isChecked())
	{
		ui.dFile_label->setEnabled(false); ui.dFile_lineEdit->setEnabled(false); ui.dFile_pushButton->setEnabled(false);
		ui.openDefaultAtStart_checkBox->setChecked(false); ui.doNothing_checkBox->setChecked(false);
		lastFile=true; defaultFile=false; doNothing=false; dFileSetting="<None>";
		ui.dFile_lineEdit->setText(dFileSetting);
		dFile=cFile; dPath=cPath;
	}
	else if (!ui.openDefaultAtStart_checkBox->isChecked()&&!ui.doNothing_checkBox->isChecked())
		ui.openLastAtStart_checkBox->setChecked(true);
}
void Settings::on_openDefaultAtStart_checkBox_toggled()
{
	if (ui.openDefaultAtStart_checkBox->isChecked())
	{
		ui.dFile_label->setEnabled(true); ui.dFile_lineEdit->setEnabled(true); ui.dFile_pushButton->setEnabled(true);
		ui.openLastAtStart_checkBox->setChecked(false); ui.doNothing_checkBox->setChecked(false);
		if (dFile.isEmpty()) dFileSetting="<None>";
		else dFileSetting=dFile; 
		ui.dFile_lineEdit->setText(dFileSetting);
		defaultFile=true; lastFile=false; doNothing=false;
	}
	else if (!ui.openLastAtStart_checkBox->isChecked()&&!ui.doNothing_checkBox->isChecked())
		ui.openDefaultAtStart_checkBox->setChecked(true);
}
void Settings::on_doNothing_checkBox_toggled()
{
	if (ui.doNothing_checkBox->isChecked())
	{
		ui.dFile_label->setEnabled(false); ui.dFile_lineEdit->setEnabled(false); ui.dFile_pushButton->setEnabled(false);
		ui.openDefaultAtStart_checkBox->setChecked(false); ui.openLastAtStart_checkBox->setChecked(false);
		lastFile=false; defaultFile=false; dFileSetting="<None>"; doNothing=true;
		ui.dFile_lineEdit->setText(dFileSetting);
		dPath.clear();
	}
	else if (!ui.openLastAtStart_checkBox->isChecked()&&!ui.openDefaultAtStart_checkBox->isChecked())
		ui.doNothing_checkBox->setChecked(true);
}
void Settings::on_dFile_pushButton_clicked()
{
	QString browse = QFileDialog::getOpenFileName(this, tr("Default File"),QString(),tr("DataBase Files(*.db)\0*.db\0"));
	if (!browse.isEmpty())
	{
		dPath=browse; dFileUpdate(); cfgUpdate(); dFileSetting=dFile;
		ui.dFile_lineEdit->setText(dFileSetting);
	}
}
void Settings::on_okay_pushButton_clicked()
{
	if (ui.openDefaultAtStart_checkBox->isChecked()&&dFileSetting=="<None>")
		QMessageBox::information(this,tr("Error"),tr("Must Select Default File"));
	else
	{
		openDefaultAtStart=defaultFile; openLastAtStart=lastFile;
		cfgUpdate(); settings_okayPushed=true;
		close();
	}
}
void Settings::on_cancel_pushButton_clicked()
{
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

void Settings::setChecked()
{
	if (openLcheck)
		ui.openLastAtStart_checkBox->setChecked(true);
	else if (openDcheck)
		ui.openDefaultAtStart_checkBox->setChecked(true);
	else
		ui.doNothing_checkBox->setChecked(true);
}

void Settings::setDFileEdit(QString name)
{
	ui.dFile_lineEdit->setText(name);
}