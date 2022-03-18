#include "PropRow.h"

PropRow::PropRow(const propValueCollection& propsRow, QWidget *parent)
	: QWidget(parent), _myPropsRow(propsRow)
{
	ui.setupUi(this);

	// fill in UI from propRow
	for (auto& node : _myPropsRow.propsNodes)
	{
		//get all nodes inside this props
		auto allText = node.select_nodes(".//node()");
		for (auto& child : allText)
		{
			if (child.node().attribute("keyref"))
			{
				//remove line breaks from before and after these keyrefs
				//replace keyref value with the matching keyword value from map file
				ui.textBrowser->append(QString::fromStdString(child.node().attribute("keyref").value()));
			}
			else
			{
				ui.textBrowser->append(QString::fromStdString(child.node().value()));
			}
		}
	}

	ui.propRow_group->setTitle(QString::fromStdString(_myPropsRow.propsName));
}

PropRow::~PropRow()
{
}
