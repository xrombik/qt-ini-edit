#include "mainwindow.h"


void MainWindow :: onDataChanged(QString& s)
{
    if (inimodel.get_lvl() == 1)
    {
        QStringList ss = s.split(" = ");
        if (ss.size() == 2)
        {
            ini.set_param(
                lblSection.text().toStdString().c_str(),
                ss[0].toStdString().c_str(),
                ss[1].toStdString().c_str());
        }
    }
    else if (inimodel.get_lvl() == 0)
    {
        QString s2 = s.remove('[').remove(']').remove('\n');
        ini.rename_section(lblSection.text().toStdString().c_str(), s2.toStdString().c_str());
    }
}


bool KeyReceiver :: eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        MainWindow* mainWindow = (MainWindow*) this->user_data;
        mainWindow->processKeyEvent(keyEvent);
        return false;
    }
    return QObject :: eventFilter(obj, event);
}


const char* MainWindow :: getCurSectionName(void) const
{
    static char buf[INI_LINELEN_MAX];
    buf[0] = '\0';
    QModelIndex index = tblView.selectionModel()->currentIndex();
    QVariant value = index.sibling(index.row(), index.column()).data();
    QString s1 = value.toString().remove('\n').remove(']').remove('[').remove('\r');
    strcpy(buf, s1.toStdString().c_str());
    return &buf[0];
}


const char* MainWindow::getCurParamName(void) const
{
    static char buf[INI_LINELEN_MAX];
    buf[0] = '\0';
    QModelIndex index = tblView.selectionModel()->currentIndex();
    QVariant value = index.sibling(index.row(), 0).data();
    QString s1 = value.toString();
    strcpy(buf, s1.toStdString().c_str());
    return &buf[0];
}


void MainWindow :: processKeyEvent(QKeyEvent* keyEvent)
{
    switch (keyEvent->key())
    {
    case Qt::Key_Return:
        if (inimodel.get_lvl() == 0)
        {
            inimodel.set_lvl(1);
            const char* section = getCurSectionName();
            lblSection.setText(section);
            inimodel.rebuildMdata(ini, section);
        }
        break;
    case Qt::Key_Escape:
    case Qt::Key_Backspace:
        if (inimodel.get_lvl() == 1)
        {
            inimodel.set_lvl(0);
            lblSection.setText("");
            inimodel.rebuildMdata(ini);
        }
        break;
    case Qt::Key_Delete:
        if (inimodel.get_lvl() == 0)
        {
            const char* section = getCurSectionName();
            ini.delete_section(section);
            inimodel.rebuildMdata(ini);
        }
        else if (inimodel.get_lvl() == 1)
        {
            const char* param = getCurParamName();
            ini.delete_param(lblSection.text().toStdString().c_str(), param);
            inimodel.rebuildMdata(ini, lblSection.text().toStdString().c_str());
        }
        break;
    case Qt::Key_F5: // сохранить
        ini.store(outFileName);
        break;
    case Qt::Key_Insert:
        if (inimodel.get_lvl() == 0)
        {
            int sc = ini.get_section_count();
            char buf[INI_LINELEN_MAX];
            sprintf(buf, "[NewSection%d]\n", sc);
            ini.append(buf);
            inimodel.rebuildMdata(ini);
        }
        else if (inimodel.get_lvl() == 1)
        {
            int pc = ini.get_params_count(lblSection.text().toStdString().c_str());
            char name[INI_LINELEN_MAX];
            sprintf(name, "NewParameter%d", pc);
            char value[INI_LINELEN_MAX];
            sprintf(value, "NewValue%d", pc);
            ini.set_param(lblSection.text().toStdString().c_str(), name, value);
            inimodel.rebuildMdata(ini, lblSection.text().toStdString().c_str());
        }
    }
}


MainWindow :: MainWindow(const char* fileName, const char* outFileName, QWidget* parent) : QWidget(parent)
{
    ini.load(fileName);
    inimodel.rebuildMdata(ini);
    lblMsg.setText(
        "Входной файл: \"" + QString(fileName) + "\"\n"
        "Выходной файл: \"" + QString(outFileName) + "\"\n"
        "Зайти в секцию: \"Return\"\n"
        "Удалить секцию или параметр: \"Del\"\n"
        "Добавить секцию или параметр: \"Insert\"\n"
        "Изменить: двойной клик\n"
        "Выйти из секции: \"Backspace\" или \"Esc\"\n"
        "Сохранить в выходной файл: \"F5\"\n");
    lblSection.setText("");
    tblView.setModel(&inimodel);
    mainLayout.addWidget(&lblMsg);
    mainLayout.addWidget(&lblSection);
    mainLayout.addWidget(&tblView);
    setLayout(&mainLayout);
    tblView.installEventFilter(&keyReceiver);
    keyReceiver.user_data = (void*) this;
    this->outFileName = outFileName;
    QObject::connect(&inimodel, &InifileModel::editCompleted, this, &MainWindow::onDataChanged);
}
