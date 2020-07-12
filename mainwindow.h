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
    int lvl;
public:
    MainWindow(const char* fileName,  const char* outFileName, QWidget* parent = nullptr);
    void processKeyEvent(QKeyEvent* keyEvent);
private:
    const char* getCurSectionName(void) const;
    QLabel lblPath;
    QLabel lblOutPath;
    QVBoxLayout mainLayout;
    QTableView tblView;
    IniFile ini;
    InifileModel inimodel;
    KeyReceiver keyReceiver;
};
