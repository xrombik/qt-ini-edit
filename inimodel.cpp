#include "inimodel.h"


const QString* InifileModel::getMRow(int row)
{
    return &mdata.at(static_cast<size_t>(row));
}


bool InifileModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (role == Qt::EditRole)
    {
        size_t row = index.row();
        if (mdata.find(row) != mdata.end())
        {
            QStringList sl = mdata[row].split(" = ");
            if (sl.size() == 2)
            {
                sl[1] = value.toString();
                mdata[row] = sl[0] + " = " + sl[1];
                emit editCompleted(mdata.at(row));
            }
        }
    }
    return true;
}


Qt::ItemFlags InifileModel::flags(const QModelIndex& index) const
{
    if (index.column() == 1)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    else
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}


void InifileModel::rebuildMdata(const IniFile& ini, const char* section)
{
    mdata.clear();
    if (lvl == 0)
    {
        StrList sections;
        int count = ini.get_section_count(&sections);
        StrNode* cur = sections.top();
        for (int i = 0; i < count; i ++)
        {
            mdata[i] = QString(cur->data).remove('\n');
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
            mdata[i] = QString(cur->data).remove('\n');
            cur = cur->next;
        }
    }
    emit layoutChanged();
}


int InifileModel::rowCount(const QModelIndex& parent) const
{
   return mdata.size();
}


int InifileModel::columnCount(const QModelIndex& parent) const
{
    return lvl + 1;
}


QVariant InifileModel::data(const QModelIndex& index, int role) const
{
    switch (role) {
    case Qt::EditRole:
    case Qt::DisplayRole:
    {
        size_t row = index.row();
        size_t column = index.column();
        if (mdata.find(row) != mdata.end())
        {
            const QString* s = &mdata.at(row);
            QStringList ss = s->split(" = ");
            if (ss.size() == 2)
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
        break;
    } }
    return QVariant();
}
