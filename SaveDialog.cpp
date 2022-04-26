#include "SaveDialog.h"
#include "qfiledialog.h"

QString SaveDialog::bookmapFile;
QString SaveDialog::ditavalFile;

SaveDialog::SaveDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

void SaveDialog::saveBookmap()
{
	bookmapFile = QFileDialog::getSaveFileName(this, tr("Select Bookmap"), "", tr("DITA Bookmap (*.ditamap);;All Files (*.*)"));
	ui.bookmap_line->setText(bookmapFile);
}

void SaveDialog::saveDitaval()
{
	ditavalFile = QFileDialog::getSaveFileName(this, tr("Select Ditaval"), "", tr("DITA Ditaval (*.ditaval);;All Files (*.*)"));
	ui.ditaval_line->setText(ditavalFile);
}

std::pair<QString, QString> SaveDialog::savedFiles()
{
	std::pair<QString, QString> _files;
	_files.first = bookmapFile;
	_files.second = ditavalFile;
	return _files;
}

SaveDialog::~SaveDialog()
{
}
