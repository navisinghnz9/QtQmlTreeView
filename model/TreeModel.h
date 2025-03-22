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

#include <QFile>
#include <QVariant>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QAbstractItemModel>

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
    explicit TreeModel(const QString jsonFile);

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

    /**
     * @brief Sets the data for a given index in the tree model.
     *
     * This function updates the value at the specified `index` in the model. It does
     * not use the provided `role` argument since the function is primarily concerned
     * with editing the value of the item at the given index.
     *
     * The data update triggers a `dataChanged` signal to notify any views that the data
     * has been updated. Additionally, it saves the updated model state to a JSON file
     * by calling the `saveToJsonFile` function.
     *
     * @param index The model index that identifies the item to modify.
     * @param value The new value to set at the specified index.
     * @param role The role for the data. This parameter is unused, but it is typically
     *             provided for compatibility with the Qt framework.
     *
     * @return `true` if the data was successfully set, `false` if the index is invalid.
     *
     * @note The `role` parameter is not used in this implementation as we are directly
     *       modifying the value at the given index. The `Qt::EditRole` is emitted by the
     *       `dataChanged` signal for compatibility with Qt's model/view framework.
     *
     * @see saveToJsonFile
     */
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

private:

    /**
     * @brief Recursively traverses a JSON object and creates a tree structure of TreeNode objects.
     *
     * This function takes a JSON object and recursively traverses its contents to build
     * a tree of `TreeNode` objects, starting from the given `rootNode`. It handles various
     * data types in the JSON object, including strings, booleans, doubles, arrays, objects,
     * and null values. The resulting tree structure is represented by `TreeNode` objects
     * where each `TreeNode` corresponds to a key-value pair from the JSON object.
     *
     * The function processes each JSON element based on its type:
     * - Strings are stored as values in the tree.
     * - Booleans are stored as values in the tree.
     * - Doubles are stored as values in the tree.
     * - Arrays are represented as parent nodes with their elements recursively added as children.
     * - Objects are represented as parent nodes with their key-value pairs added recursively as child nodes.
     * - Null values are represented by a `TreeNode` with an empty string value.
     *
     * The function will output a warning if an "undefined" type is encountered in the JSON object.
     *
     * @param rootNode The root node of the tree to which new nodes will be appended.
     * @param jsonObj The JSON object to be traversed.
     *
     * @note This function assumes that the `TreeNode` class can handle different data types
     *       (e.g., string, bool, double) and supports hierarchical relationships between nodes.
     *
     * @warning Undefined types in the JSON object are logged as warnings.
     *
     * @see TreeNode
     * */
    void traverseJson(TreeNode* rootNode, QJsonObject &jsonObj);

    /**
     * @brief Sets up the tree model data from a JSON file.
     *
     * This function reads a JSON file, parses its content, and sets up the tree structure
     * using `TreeNode` objects. It initializes the root node with the name "Config" and
     * an empty value. It then reads the JSON file specified by the member variable `_jsonFile`,
     * parses it into a `QJsonObject`, and recursively traverses the JSON structure to populate
     * the tree model.
     *
     * If the JSON file cannot be opened, an error message is logged, and the root node
     * with default values is returned. Otherwise, the function processes the JSON file
     * and builds a hierarchical structure of `TreeNode` objects.
     *
     * @return A pointer to the root `TreeNode` representing the top-level structure of
     *         the parsed JSON data.
     *
     * @note The `_jsonFile` member variable is expected to contain the path to a valid
     *       JSON file.
     *
     * @see traverseJson
     */
    TreeNode* setupJsonModelData();

private:
    TreeNode *_rootNode;
    QString _jsonFile;
};

#endif // __TREE_MODEL_H__
