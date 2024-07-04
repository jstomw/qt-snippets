#include "treemodel.h"

QString TreeModel::variantToString(const QVariant &value){
    if(value.canConvert<QString>()){
        return value.toString();
    }
    if(value.canConvert<int>()){
        return QString::number(value.toInt());
    }
    return "";
}

TreeModel::TreeModel(QAbstractItemModel *data, QObject *parent)
    : QAbstractItemModel{parent},
    m_data{data}
{

}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    if (!m_data || parent.column() > 0)
        return 0;

    if(!parent.isValid()){
        return findRows("parent_id", "").size(); // rows without parent_id
    }
    //rows where parent_id exists
    auto parent_id = getRow(parent.internalId()).value("id");
    return findRows("parent_id", parent_id).size();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();
    if (!parent.isValid()){
        auto children_indexes = findRows("parent_id", "");
        int child = 0;
        for(auto &i : children_indexes){ //internal id in list
            if(child == row){
                return createIndex(child, 0, i);
            }
            child++;
        }
    }else{
        auto parent_id = getRow(parent.internalId()).value("id");
        if(variantToString(parent_id).isEmpty()){
            return QModelIndex();
        }
        //items with parent_id
        auto children_indexes = findRows("parent_id", parent_id);
        int child = 0;
        for(auto &i : children_indexes){
            if(child == row){
                return createIndex(child, 0, i);
            }
            child++;
        }
    }
    return QModelIndex(); // rows with parent_id
}

QModelIndex TreeModel::parent(const QModelIndex &child) const
{
    if (child.column() > 0)
        return QModelIndex();
    //items with parent_id
    if (child.isValid()){
        auto parent_id = getRow(child.internalId()).value("parent_id");
        if(variantToString(parent_id).isEmpty()){
            return QModelIndex();
        }
        auto list_index = findRows("id", parent_id).at(0);
        return createIndex(list_index, 0, list_index);
    }
    //ROOT has no parent
    return QModelIndex();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    auto name = roleNames().value(role);
    return getRow(index.internalId()).value(name);
}

QList<int> TreeModel::findRows(const QString &field, const QVariant &value) const
{
    QList<int> result;
    if(!m_data){
        return result;
    }
    for(int i = 0; i != m_data->rowCount(); i++){
        if(getRow(i).value(field) == value){
            result << i;
        }
    }
    return result;
}

QVariantMap TreeModel::getRow(const int &i) const
{
    QVariantMap result;
    for(int j = 0; j != m_data->columnCount(); j++){
        auto index = m_data->index(i, j);
        if(!index.isValid()){
            continue;
        }
        const auto &key = m_data->headerData(j, Qt::Horizontal).toString(); //get field name
        const auto &value = m_data->data(index); // get field value
        result.insert(key, value);
    }
    return result;
}

QHash<int, QByteArray> TreeModel::roleNames() const
{
    QHash<int, QByteArray> result{m_data->roleNames()};
    for(int i = 0; i != m_data->columnCount(); i++){
        result.insert(Qt::UserRole + i + 1, qPrintable(m_data->headerData(i, Qt::Horizontal).toString()));
    }
    return result;
}
