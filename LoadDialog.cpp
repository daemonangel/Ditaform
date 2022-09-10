#include "LoadDialog.h"
#include "qfiledialog.h"

QString LoadDialog::bookmapFile;
QString LoadDialog::ditavalFile;

LoadDialog::LoadDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

void LoadDialog::openBookmap()
{
	bookmapFile = QFileDialog::getOpenFileName(this, tr("Select Bookmap"), "", tr("DITA Bookmap (*.ditamap);;All Files (*.*)"));
	if (bookmapFile != nullptr)
	{
		ui.bookmap_line->setText(bookmapFile);
		if (ditavalFile != nullptr)
		{
			ui.confirm_button->setEnabled(true);
		}
	}
}

void LoadDialog::openDitaval()
{
	ditavalFile = QFileDialog::getOpenFileName(this, tr("Select Ditaval"), "", tr("DITA Ditaval (*.ditaval);;All Files (*.*)"));
	if (ditavalFile != nullptr)
	{
		ui.ditaval_line->setText(ditavalFile);
		if (bookmapFile != nullptr)
		{
			ui.confirm_button->setEnabled(true);
		}
	}
}

std::pair<QString, QString> LoadDialog::openedFiles()
{
	std::pair<QString, QString> _files;
	_files.first = bookmapFile;
	_files.second = ditavalFile;
	return _files;
}

LoadDialog::~LoadDialog()
{
}
