/*******************************************************************************
 *                                                                             *
 * Project: QtQmlTreeView Demo using a custom json to populate the tree        *
 * Filename: TreeModel.cpp                                                     *
 *                                                                             *
 * Description:                                                                *
 * Header file for TreeModel class, which inherits from QAbstractItemModel     *
 * This model manages a hierarchical tree structure of JSON data, using a      *
 * TreeNode class for storage.                                                 *
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
 *
 * QtQmlTreeView Demo is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 * GNU General Public License for more details.                                *
 *                                                                             *
 * You should have received a copy of the GNU General Public License           *
 * along with QtQmlTreeView Demo. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                             *
 ******************************************************************************/

#include <QFile>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include "TreeModel.h"

TreeModel::TreeModel(TreeNode *rootNode, QObject *parent) : QAbstractItemModel(parent), _rootNode(rootNode) {}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    int rowCount = 0;
    if (!parent.isValid()) {
        rowCount = _rootNode->children().count();
    } else {
        rowCount = static_cast<TreeNode *>(parent.internalPointer())->children().count();
    }
    return rowCount;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    TreeNode *node = static_cast<TreeNode *>(index.internalPointer());
    if (role == NameRole) {
        return node->name();
    }

    if (role == ValueRole) {
        return node->value();
    }

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    TreeNode *parentNode;
    if (!parent.isValid()) {
        parentNode = _rootNode;
    } else {
        parentNode = static_cast<TreeNode *>(parent.internalPointer());
    }

    TreeNode *childItem = parentNode->children().value(row);
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
    TreeNode *parentNode= childItem->parentNode();
    if (parentNode == _rootNode) {
        return QModelIndex();
    }

    return createIndex(parentNode->row(), 0, parentNode);
}

QHash<int, QByteArray> TreeModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    // mapping NameRole to the property "name"
    roles[NameRole] = "name";
    roles[ValueRole] = "value";

    return roles;
}

QJsonValue TreeModel::serializeTree(TreeNode* item) {
    if (item->children().isEmpty()) {
        // we reached the leaf node, now return its value
        return QJsonValue::fromVariant(item->value());
    }

    QJsonObject jsonObject;
    for (TreeNode* child : item->children()) {
        // recursively serialize the tree for each child node
        QString key = child->name();
        jsonObject[key] = serializeTree(child);
    }
    return jsonObject;
}

QJsonDocument TreeModel::serializeTreeToJson() {
    QJsonValue rootValue = serializeTree(_rootNode);
    return QJsonDocument(rootValue.toObject());
}

void TreeModel::saveToJsonFile(const QString& filePath) {
    QJsonDocument doc = serializeTreeToJson();
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}
