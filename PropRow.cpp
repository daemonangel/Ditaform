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
				//replace keyref value with the matching keyword value from map file
				ui.textBrowser->setFontPointSize(12);
				ui.textBrowser->setTextColor(QColor(255, 0, 0));
				ui.textBrowser->setFontWeight(QFont::Normal);
				ui.textBrowser->insertPlainText(child.node().attribute("keyref").value());
			}
			else if (child.node().name() == std::string("title") || child.parent().name() == std::string("title"))
			{
				ui.textBrowser->setFontPointSize(16);
				ui.textBrowser->setTextColor(QColor(0, 0, 0));
				ui.textBrowser->setFontWeight(QFont::Bold);
				ui.textBrowser->insertPlainText(child.node().value());
				ui.textBrowser->insertHtml("<br>"); //line break
			}
			else
			{
				ui.textBrowser->setFontPointSize(12);
				ui.textBrowser->setTextColor(QColor(0, 0, 0));
				ui.textBrowser->setFontWeight(QFont::Normal);
				ui.textBrowser->insertPlainText(child.node().value());
			}
		}
	}

	ui.propRow_group->setTitle(QString::fromStdString(_myPropsRow.propsName));
}

PropRow::~PropRow()
{
}
