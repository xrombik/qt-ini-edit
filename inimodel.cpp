#include "inimodel.h"



void InifileModel::rebuildMdata(const IniFile &ini, int lvl, const char* section)
{
    mdata.clear();
    if (lvl == 0)
    {
        StrList sections;
        int count = ini.get_section_count(&sections);
        StrNode* cur = sections.top();
        for (int i = 0; i < count; i ++)
        {
            mdata[i] = QString(cur->data);
            cur = cur->next;
        }
    }
    else if (lvl == 1)
    {
        if (!section)
            return;
        StrList params;
        int count = ini.get_params_count(section, &params);
        StrNode* cur = params.top();
        for (int i = 0; i < count; i ++)
        {
            mdata[i] = QString (cur->data);
            cur = cur->next;
        }
    }
    QModelIndex topLeft = createIndex(0, 0);
    QModelIndex bottomRight = createIndex(columnCount(), rowCount());
    emit dataChanged(topLeft, bottomRight);
}


int InifileModel::rowCount(const QModelIndex & /*parent*/) const
{
   return mdata.size();
}


int InifileModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 2;
}


QVariant InifileModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        size_t row = index.row();
        size_t column = index.column();
        const QString* s = &mdata.at(row);
        QStringList ss = s->split("=");
        if (ss.length() == 2)
        {
            return ss[column];
        }
        else
        {
            if (column == 0)
                return *s;
            else
                return QString("");
        }
    }
    return QVariant();
}
