#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_RegulatoryTemplate.h"

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
    QGroupBox *contentUi_group();
    std::vector<std::string> lines;
    //std::vector<Platform> platforms;
};
