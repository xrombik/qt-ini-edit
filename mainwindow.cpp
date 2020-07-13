#include "mainwindow.h"


bool KeyReceiver :: eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        MainWindow* mainWindow = (MainWindow*) this->user_data;
        mainWindow->processKeyEvent(keyEvent);
        return false;
    }
    return QObject :: eventFilter(obj, event);
}


const char* MainWindow :: getCurSectionName(void) const
{
    static char buf[INI_LINELEN_MAX];
    QModelIndex index = tblView.selectionModel()->currentIndex();
    QVariant value = index.sibling(index.row(), index.column()).data();
    QString s1 = value.toString().remove('\n').remove(']').remove('[');
    strcpy(buf, s1.toStdString().c_str());
    return &buf[0];
}


void MainWindow :: processKeyEvent(QKeyEvent *keyEvent)
{
    switch (keyEvent->key())
    {
    case Qt::Key_Return:
        if (lvl == 0)
        {
           lvl = 1;
           const char* section = getCurSectionName();
           inimodel.rebuildMdata(ini, lvl, section);
           this->tblView.reset();
        }
        break;
    case Qt::Key_Escape:
    case Qt::Key_Backspace:
        if (lvl == 1)
        {
            lvl = 0;
            inimodel.rebuildMdata(ini);
        }
        break;
    case Qt::Key_Delete:
        if (lvl == 0)
        {
            const char* section = getCurSectionName();
            ini.delete_section(section);
            inimodel.rebuildMdata(ini);
        }
        break;
    case Qt::Key_F2: // редактировать
        break;
    case Qt::Key_F5: // сохранить
        const char* path = lblPath.text().toStdString().c_str();
        ini.store(path);
        break;
    }
}


MainWindow :: MainWindow(const char* fileName, const char* outFileName, QWidget *parent) : QWidget(parent)
{
    lvl = 0;
    ini.load(fileName);
    inimodel.rebuildMdata(ini);
    lblPath.setText(fileName);
    lblOutPath.setText(outFileName);
    tblView.setModel(&inimodel);
    mainLayout.addWidget(&lblPath);
    mainLayout.addWidget(&lblOutPath);
    mainLayout.addWidget(&tblView);
    setLayout(&mainLayout);
    tblView.installEventFilter(&keyReceiver);
    keyReceiver.user_data = (void*) this;
}
