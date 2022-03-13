#include "PropRow.h"

PropRow::PropRow(const propValueCollection& propsRow, QWidget *parent)
	: QWidget(parent), _myPropsRow(&propsRow)
{
	ui.setupUi(this);

	// fill in UI from propRow
	for (auto& node : _myPropsRow->propsNodes)
	{
		ui.textBrowser->append(QString::fromStdString(node.text().as_string()));
	}

	ui.propRow_group->setTitle(QString::fromStdString(_myPropsRow->propsName));
}

PropRow::~PropRow()
{
}
