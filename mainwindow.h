#pragma once
 
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

#include "inifile.h"
#include "inimodel.h"


class KeyReceiver : public QObject
{
    Q_OBJECT
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
public:
    void* user_data;
};


class MainWindow : public QWidget
{
    Q_OBJECT
    const char* getCurSectionName(void) const;
    const char* getCurParamName(void) const;
    QLabel lblMsg;
    QLabel lblSection;
    const char* outFileName;
    QVBoxLayout mainLayout;
    QTableView tblView;
    IniFile ini;
    InifileModel inimodel;
    KeyReceiver keyReceiver;
public:
    MainWindow(const char* fileName,  const char* outFileName, QWidget* parent = nullptr);
    void processKeyEvent(QKeyEvent* keyEvent);
public slots:
    void onDataChanged(QString& s);
};
