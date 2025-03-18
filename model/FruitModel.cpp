#include "FruitModel.h"


FruitModel::FruitModel(QObject *parent) : QAbstractItemModel(parent)
{
    setupModelData();
}

int FruitModel::rowCount(const QModelIndex &parent) const
{
    int rowCount = 0;
    if (!parent.isValid()) {
        rowCount = _rootItem->children().count();
    } else {
        rowCount = static_cast<TreeNode *>(parent.internalPointer())->children().count();
    }
    return rowCount;
}

int FruitModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    // we have only one column (i.e. name of the fruit)
    return 1;
}

QVariant FruitModel::data(const QModelIndex &index, int role) const
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

QModelIndex FruitModel::index(int row, int column, const QModelIndex &parent) const
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

QModelIndex FruitModel::parent(const QModelIndex &index) const
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

QHash<int, QByteArray> FruitModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    // mapping NameRole to the property "name"
    roles[NameRole] = "name";

    return roles;
}

void FruitModel::setupModelData()
{
    _rootItem = new TreeNode("Fruits");

    // adding citrus category with its child nodes
    TreeNode *citrusCategory = new TreeNode("Citrus", _rootItem);
    citrusCategory->children().append(new TreeNode("Apple", citrusCategory));
    citrusCategory->children().append(new TreeNode("Orange", citrusCategory));
    citrusCategory->children().append(new TreeNode("Kiwi", citrusCategory));
    _rootItem->children().append(citrusCategory);

    // adding berries category with its child nodes
    TreeNode *berryCategory = new TreeNode("Berries", _rootItem);
    berryCategory->children().append(new TreeNode("Strawberry", berryCategory));
    berryCategory->children().append(new TreeNode("Blueberry", berryCategory));
    berryCategory->children().append(new TreeNode("Raspberry", berryCategory));
    _rootItem->children().append(berryCategory);

    // adding drupes category with its child nodes
    TreeNode *drupesCategory = new TreeNode("Drupes", _rootItem);
    drupesCategory->children().append(new TreeNode("Plums", drupesCategory));
    drupesCategory->children().append(new TreeNode("Peaches", drupesCategory));
    drupesCategory->children().append(new TreeNode("Olives", drupesCategory));
    _rootItem->children().append(drupesCategory);
}
