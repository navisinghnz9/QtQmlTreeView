#include "TreeNode.h"

TreeNode::TreeNode(const QString &data, TreeNode *parent)
    : _name{data}
    , _parentItem{parent} { }

TreeNode::~TreeNode()
{
    qDeleteAll(_children);
}

void TreeNode::appendChild(TreeNode *child)
{
    _children.append(child);
}

TreeNode *TreeNode::child(int row)
{
    if (row < 0 || row >= _children.count()) {
        return nullptr;
    }
    return _children.at(row);
}

int TreeNode::childCount() const
{
    return _children.count();
}

int TreeNode::columnCount() const
{
    // Note - we are having 1 column at the moment,
    // for more columns, we will return name.length() in future, if needed.
    return 1;
}

QVariant TreeNode::data(int column) const
{
    if (column != 1) {
        return QVariant();
    }
    return _name.at(column);
}

int TreeNode::row() const
{
    if (_parentItem) {
        return _parentItem->_children.indexOf(const_cast<TreeNode *>(this));
    }
    return 0;
}


