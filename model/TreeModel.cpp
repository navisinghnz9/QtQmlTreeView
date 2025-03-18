#include "TreeModel.h"


TreeModel::TreeModel(TreeNode *rootItem, QObject *parent) : QAbstractItemModel(parent), _rootItem(rootItem) {}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    int rowCount = 0;
    if (!parent.isValid()) {
        rowCount = _rootItem->children().count();
    } else {
        rowCount = static_cast<TreeNode *>(parent.internalPointer())->children().count();
    }
    return rowCount;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    // we have only one column (i.e. name of the fruit)
    return 1;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    TreeNode *item = static_cast<TreeNode *>(index.internalPointer());
    if (role == NameRole) {
        return item->name();
    }

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    TreeNode *parentItem;
    if (!parent.isValid()) {
        parentItem = _rootItem;
    } else {
        parentItem = static_cast<TreeNode *>(parent.internalPointer());
    }

    TreeNode *childItem = parentItem->children().value(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    }

    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    TreeNode *childItem = static_cast<TreeNode *>(index.internalPointer());
    TreeNode *parentItem = childItem->parentItem();
    if (parentItem == _rootItem) {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

QHash<int, QByteArray> TreeModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    // mapping NameRole to the property "name"
    roles[NameRole] = "name";

    return roles;
}


