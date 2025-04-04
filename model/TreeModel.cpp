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

TreeModel::TreeModel(const QString jsonFile) : QAbstractItemModel(), _jsonFile(jsonFile){
    _rootNode = setupJsonModelData();
}

void TreeModel::traverseJsonObject(TreeNode* rootNode, QJsonObject &jsonObj) {

    for (auto it = jsonObj.begin(); it != jsonObj.end(); ++it)
    {
        QString name = it.key();

        if (it.value().isString()) {
            TreeNode *obj = new TreeNode(name, it.value().toString(), rootNode);
            rootNode->children().append(obj);
            continue;
        }

        if (it.value().isBool()) {
            TreeNode *obj = new TreeNode(name, it.value().toBool(), rootNode);
            rootNode->children().append(obj);
            continue;
        }

        if (it.value().isDouble()) {
            TreeNode *obj = new TreeNode(name, it.value().toDouble(), rootNode);
            rootNode->children().append(obj);
            continue;
        }

        if (it.value().isArray()) {
            TreeNode *obj = new TreeNode(name, "", rootNode);
            rootNode->children().append(obj);
            QJsonArray jsonArray = it.value().toArray();
            traverseJsonArray(obj, jsonArray);
            continue;
        }

        if (it.value().isObject()) {
            TreeNode *obj = new TreeNode(name, "", rootNode);
            rootNode->children().append(obj);
            QJsonObject childObj = it.value().toObject();
            traverseJsonObject(obj, childObj);
            continue;
        }

        if (it.value().isNull()) {
            TreeNode *obj = new TreeNode(name, "", rootNode);
            rootNode->children().append(obj);
        }

        if(it.value().isUndefined()) {
            qWarning() << "[WARNING] :: undefined type for node: " << name;
        }
    }
}

void TreeModel::traverseJsonArray(TreeNode* obj, QJsonArray &jsonArray) {
    for (int i = 0; i < jsonArray.size(); ++i) {
        QJsonValue value = jsonArray.at(i);
        if (value.isObject()) {
            QJsonObject childObj = value.toObject();
            traverseJsonObject(obj, childObj);
            continue;
        }

        if (value.isArray()) {
            // ToDo
            continue;
        }

        obj->children().append(new TreeNode("", value, obj));
    }
}

TreeNode* TreeModel::setupJsonModelData() {

    TreeNode* rootNode = new TreeNode("Config", "");
    QFile jsonFile(_jsonFile);

    if (!jsonFile.open(QIODevice::ReadOnly)) {
        qDebug() << "ERROR - failed to open json file";
        return rootNode;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonFile.readAll());
    QJsonObject jsonObj = jsonDoc.object();
    traverseJsonObject(rootNode, jsonObj);

    return rootNode;
}

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

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    // we are not using role, as we are simply editing value at given index
    Q_UNUSED(role);

    TreeNode* node = static_cast<TreeNode*>(index.internalPointer());
    if (index.isValid()) {
        node->setValue(value);
        emit dataChanged(index, index, {Qt::EditRole});
        saveToJsonFile(_jsonFile);
        return true;
    }
    return false;
}
