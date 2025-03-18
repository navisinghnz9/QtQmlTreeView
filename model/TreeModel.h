/*******************************************************************************
 *                                                                             *
 * Project: QtQmlTreeView Demo using a custom json to populate the tree        *
 * Filename: TreeModel.h                                                      *
 *                                                                             *
 * Description:                                                                *
 * Header file for TreeModel class, which inherits from QAbstractItemModel    *
 * This model manages a hierarchical tree structure of fruit data, using a     *
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
     * This constructor initializes the model and sets up the root item (TreeNode).
     * The model is used for managing a tree structure of fruit data, which can
     * be displayed in views like QTreeView.
     *
     * @param parent The parent QObject, default is nullptr.
     */
    explicit TreeModel(TreeNode *rootItem, QObject *parent = nullptr);

    /**
     * @brief Enum for custom roles used in the model.
     *
     * This enum defines the roles that are used to access fruit-related data
     * in the model. Currently, only `NameRole` is defined to access the name
     * of the fruit or category.
     */
    enum Roles {
        NameRole = Qt::UserRole + 1 ///< Custom role for accessing fruit names.
    };

    /**
     * @brief Returns the number of rows under a given parent index.
     *
     * This method is part of the QAbstractItemModel interface. It returns the
     * number of child items under the given parent index. For a root item,
     * it returns the number of top-level items.
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
     * representing the fruit names.
     *
     * @param parent The parent index (default is QModelIndex()).
     * @return The number of columns (in this case, 1).
     */
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief Returns the data for a given index and role.
     *
     * This method is part of the QAbstractItemModel interface. It provides
     * the actual data for a given item (node) at the specified index and role.
     * It uses the custom roles, like `NameRole`, to retrieve fruit name data.
     *
     * @param index The model index.
     * @param role The role that specifies what data to retrieve.
     * @return The data for the specified index and role (a QVariant).
     */
    QVariant data(const QModelIndex &index, int role) const override;

    /**
     * @brief Returns the index of the item at the given row and column.
     *
     * This method is part of the QAbstractItemModel interface. It returns the
     * index for the item at the specified row and column under the given parent.
     *
     * @param row The row index.
     * @param column The column index.
     * @param parent The parent index (default is QModelIndex()).
     * @return The model index for the specified item.
     */
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief Returns the parent index of the specified index.
     *
     * This method is part of the QAbstractItemModel interface. It returns the
     * parent index of the item at the specified index, allowing navigation
     * from child nodes back to the root or parent nodes.
     *
     * @param index The model index.
     * @return The parent index of the specified item.
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
     * @brief Sets up the data for the model, initializing the tree structure.
     *
     * This method initializes the model with a predefined set of fruit categories
     * and fruit items. The tree structure is built by adding child nodes to the
     * root item.
     */
    void setupModelData();

private:
    TreeNode *_rootItem;
};

#endif // __TREE_MODEL_H__
