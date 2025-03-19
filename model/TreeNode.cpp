/*******************************************************************************
 *                                                                             *
 * Project: QtQmlTreeView Demo using a custom json to populate the tree        *
 * Filename: TreeNode.cpp                                                      *
 *                                                                             *
 * Description:                                                                *
 * Header file for the TreeNode class, which represents a node in a tree       *
 * structure. Each TreeNode stores a name, a parent, & a list of child nodes,  *
 * allowing the creation of a tree data structure.                             *
 *                                                                             *
 * Author(s): Navi Singh                                                       *
 * License: GPL-3.0 License                                                    *
 * Copyright (c) 2025 Navi Singh                                               *
 *                                                                             *
 * This file is part of QtQmlTreeView Demo.                                    *
 *                                                                             *
 * QtQmlTreeView Demo is free software: you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by        *
 * the Free Software Foundation, either version 3 of the License, or           *
 * (at your option) any later version.                                         *
 *                                                                             *
 * QtQmlTreeView Demo is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 * GNU General Public License for more details.                                *
 *                                                                             *
 * You should have received a copy of the GNU General Public License           *
 * along with QtQmlTreeView Demo. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                             *
 ******************************************************************************/

#include "TreeNode.h"

TreeNode::TreeNode(const QString &name, const QVariant &value, TreeNode *parent)
    : _name{name},
    _value{value},
    _parentItem{parent} {}

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
        return _parentItem->children().indexOf(const_cast<TreeNode *>(this));
    }
    return 0;
}


