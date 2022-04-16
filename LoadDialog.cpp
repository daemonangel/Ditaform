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
	bookmapFile = QFileDialog::getOpenFileName(this, tr("Select Bookmap"), "", tr("DITA Bookmap (*.ditamap)"));
	ui.bookmap_line->setText(bookmapFile);
}

void LoadDialog::openDitaval()
{
	ditavalFile = QFileDialog::getOpenFileName(this, tr("Select Ditaval"), "", tr("DITA Ditaval (*.ditaval)"));
	ui.ditaval_line->setText(ditavalFile);
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
