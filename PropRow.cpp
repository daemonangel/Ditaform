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
				ui.textBrowser->setFontPointSize(12);
				ui.textBrowser->setTextColor(QColor(255, 0, 0));
				ui.textBrowser->setFontWeight(QFont::Normal);
				ui.textBrowser->insertPlainText(child.node().attribute("keyref").value());

				//add keyref to the group box
				QFrame* line = new QFrame(this);
				line->setFrameStyle(QFrame::NoFrame);
				line->setLineWidth(5);
				QFont labelFont("Normal", 10, QFont::Bold);
				QLabel* label = new QLabel(this);
				label->setFrameStyle(QFrame::NoFrame);
				label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
				label->setFont(labelFont);
				label->setText(child.node().attribute("keyref").value());
				QTextEdit* input = new QTextEdit(this);
				input->setObjectName(child.node().attribute("keyref").value());

				//connect signal textchanged from input object to function updateKeyref in RegulatoryTemplate class
				_RegulatoryTemplate = std::make_unique<RegulatoryTemplate>();
				connect(input, &QTextEdit::textChanged, _RegulatoryTemplate.get(), &RegulatoryTemplate::updateKeyref);

				ui.verticalLayout->addWidget(line);
				ui.verticalLayout->addWidget(label);
				ui.verticalLayout->addWidget(input);	
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
				ui.textBrowser->insertHtml("<p>"); //line break
			}
		}
	}

	//set title of groupbox to the props node name
	ui.propRow_group->setTitle(QString::fromStdString(_myPropsRow.propsName));
}

PropRow::~PropRow()
{
}
