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
	//TODO Set default file name using product name + requestor name
	bookmapFile = QFileDialog::getSaveFileName(this, tr("Select Bookmap"), "maps/", tr("DITA Bookmap (*.ditamap);;All Files (*.*)"));
	if (bookmapFile != nullptr)
	{
		ui.bookmap_line->setText(bookmapFile);
		ui.confirm_button->setEnabled(true);
	}
}

QString SaveDialog::savedFiles()
{
	QString _files;
	_files = bookmapFile;
	return _files;
}

SaveDialog::~SaveDialog()
{
}
