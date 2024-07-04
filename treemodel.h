#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>

/**

TreeModel class - helps to represent other QAbstractItemModel model which has flat structure as tree model.

for exmple:

// Using QStandardItemModel
auto standardModel = new QStandardItemModel(this);
// Populate standardModel with items and hierarchy

auto treeModelFromStandard = new TreeModel(standardModel, this);

// Using a Custom Model
class CustomModel : public QAbstractItemModel {
    // Implement custom model functionality
};

auto customModel = new CustomModel(this);

auto treeModelFromCustom = new TreeModel(customModel, this);

*/
class TreeModel : public QAbstractItemModel
{
public:
    explicit TreeModel(QAbstractItemModel *data = nullptr, QObject *parent = nullptr);

private:
    QAbstractItemModel *m_data;

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QModelIndex index(int row, int column, const QModelIndex &parent  = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;

    QList<int> findRows(const QString &field, const QVariant &value = "") const;
    QVariantMap getRow(const int &i = 0) const;

    static QString variantToString(const QVariant &value);
};

#endif // TREEMODEL_H
