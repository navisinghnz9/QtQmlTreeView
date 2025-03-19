/*******************************************************************************
 *                                                                             *
 * Project: QtQmlTreeView Demo using a custom json to populate the tree        *
 * Filename: TreeModel.h                                                       *
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

#ifndef __TREE_MODEL_H__
#define __TREE_MODEL_H__

#include <QAbstractItemModel>
#include <QVariant>

#include "TreeNode.h"


class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    /**
     * @brief Constructs the TreeModel with an optional parent object.
     *
     * This constructor initializes the model and sets up the root node (TreeNode).
     * The model is used for managing a tree structure of JSON data, which can
     * be displayed in views like QTreeView.
     *
     * @param parent The parent QObject, default is nullptr.
     */
    explicit TreeModel(TreeNode *rootNode, QObject *parent = nullptr);

    /**
     * @brief Enum for custom roles used in the model.
     *
     * This enum defines the roles that are used to access node's data - name and value in the model.
     */
    enum Roles {
        NameRole = Qt::UserRole + 1,   // role for accesing name of node
        ValueRole                      // role for accesing value of the node
    };

    /**
     * @brief Returns the number of rows under a given parent index.
     *
     * This method is part of the QAbstractItemModel interface. It returns the
     * number of child nodes under the given parent index. For a root node,
     * it returns the number of top-level nodes.
     *
     * @param parent The parent index (default is QModelIndex()).
     * @return The number of rows (child nodes) under the given parent.
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief Returns the number of columns under a given parent index.
     *
     * This method is part of the QAbstractItemModel interface. It returns
     * the number of columns in the model. In this case, it's a single column
     * representing the node names.
     *
     * @param parent The parent index (default is QModelIndex()).
     * @return The number of columns (in this case, 1).
     */
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief Returns the data for a given index and role.
     *
     * This method is part of the QAbstractItemModel interface. It provides
     * the actual data for a given node at the specified index and role.
     * It uses the custom roles, like `NameRole`, to retrieve node name data.
     *
     * @param index The model index.
     * @param role The role that specifies what data to retrieve.
     * @return The data for the specified index and role (a QVariant).
     */
    QVariant data(const QModelIndex &index, int role) const override;

    /**
     * @brief Returns the index of the node at the given row and column.
     *
     * This method is part of the QAbstractItemModel interface. It returns the
     * index for the node at the specified row and column under the given parent.
     *
     * @param row The row index.
     * @param column The column index.
     * @param parent The parent index (default is QModelIndex()).
     * @return The model index for the specified node.
     */
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief Returns the parent index of the specified index.
     *
     * This method is part of the QAbstractItemModel interface. It returns the
     * parent index of the node at the specified index, allowing navigation
     * from child nodes back to the root or parent nodes.
     *
     * @param index The model index.
     * @return The parent index of the specified node.
     */
    QModelIndex parent(const QModelIndex &index) const override;

    /**
     * @brief Returns a hash of the roles used by the model.
     *
     * This method is part of the QAbstractItemModel interface. It maps custom
     * role enums (like `NameRole`) to human-readable strings, which can be used
     * by views for accessing specific data in the model.
     *
     * @return A hash of roles and their corresponding QByteArray names.
     */
    QHash<int, QByteArray> roleNames() const override;

    /**
     * @brief Recursively serializes a tree structure into a QJsonValue.
     *
     * This function recursively serializes the tree structure rooted at the given `TreeNode*`
     * into a `QJsonValue`. The value can either be a `QJsonObject` if the node has children (e.g., an object or array),
     * or a primitive JSON value (e.g., string, number, boolean) if the node is a leaf.
     *
     * @param node The root node of the tree to be serialized.
     *
     * @return A `QJsonValue` representing the serialized JSON structure of the tree.
     * It can be either a `QJsonObject` or a primitive JSON value.
     *
     * @note This function does not modify the tree structure, it only serializes it.
     */
    QJsonValue serializeTree(TreeNode* node);

    /**
     * @brief Serializes the entire tree structure to a QJsonDocument.
     *
     * This function serializes the entire tree structure starting from the root node into a `QJsonDocument`.
     * The resulting document can be written to a file or used for other purposes where a JSON format is required.
     *
     * @return A `QJsonDocument` containing the entire serialized tree structure.
     * The document will represent the entire tree as a JSON object or array.
     */
    QJsonDocument serializeTreeToJson();

    /**
     * @brief Saves the tree structure to a JSON file at the specified file path.
     *
     * This function serializes the entire tree structure starting from the root node and saves it as a JSON file
     * at the specified file path. The tree is serialized into a `QJsonDocument` and written to the file.
     *
     * @param filePath The path of the file where the tree structure will be saved. The path should include the file name and extension (e.g., "path/to/file.json").
     *
     * @return void
     *
     * @note If the file at the given path does not exist, it will be created. If the file already exists, it will be overwritten.
     */
    void saveToJsonFile(const QString& filePath);

private:
    TreeNode *_rootNode;
};

#endif // __TREE_MODEL_H__
