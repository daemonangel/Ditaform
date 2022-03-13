#pragma once

#include <QtWidgets/QMainWindow>
#include "XmlData.h"
#include "ui_RegulatoryTemplate.h"
#include "qwidget.h"

/*struct Platform
{
    std::string name;
    bool enabled;
};*/

class RegulatoryTemplate : public QMainWindow
{
    Q_OBJECT

public:
    RegulatoryTemplate(QWidget *parent = Q_NULLPTR);

private slots:
    void enableDisableContent(bool checked);
    void prodnameEdit(const QString& metadata);
    void fileOpen();
    
    //void thingClicked(const QString& selection);
    //void thingClicked(int row);

private:
    Ui::RegulatoryTemplateClass ui;
    std::vector<std::string> lines;
    std::unique_ptr<XmlData> _xmlData;
    //std::vector<Platform> platforms;
};
