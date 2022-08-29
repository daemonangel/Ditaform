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
	bookmapFile = QFileDialog::getSaveFileName(this, tr("Select Bookmap"), "maps/", tr("DITA Bookmap (*.ditamap);;All Files (*.*)"));
	ui.bookmap_line->setText(bookmapFile);
	ui.confirm_button->setEnabled(true);
}


std::pair<QString, QString> SaveDialog::savedFiles()
{
	std::pair<QString, QString> _files;
	_files.first = bookmapFile;
	//_files.second = ditavalFile;
	return _files;
}

SaveDialog::~SaveDialog()
{
}
