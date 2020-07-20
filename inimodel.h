#pragma once

#include <QAbstractTableModel>
#include <QTableView>
#include <QWidget>
#include <QtGui>
#include <unordered_map>

#include "inifile.h"


class InifileModel : public QAbstractTableModel
{
    Q_OBJECT
    std::unordered_map<size_t, QString> mdata;
    int lvl;
public:
    int get_lvl() { return lvl; }
    void set_lvl(int lvl) { this->lvl = lvl; }
    const QString* getMRow(int row);
    bool setData(const QModelIndex& index, const QVariant& value, int role);
    Qt::ItemFlags flags(const QModelIndex& index) const;
    void rebuildMdata(const IniFile& ini, const char* section = nullptr);
    InifileModel(void) : QAbstractTableModel(nullptr), lvl(0) { }
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
signals:
    void editCompleted(QString& s);
};
