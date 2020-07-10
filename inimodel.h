#pragma once
#include <QAbstractTableModel>
#include <QTableView>
#include <QWidget>
#include <QtGui>
#include <list>

#include "inifile.h"


class InifileModel : public QAbstractTableModel
{
    Q_OBJECT
    std::map<size_t, QString> mdata;

public:

    void rebuildMdata(const IniFile &ini, int lvl = 0, const char* section = nullptr);
    InifileModel(void) : QAbstractTableModel(nullptr) {}
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};
