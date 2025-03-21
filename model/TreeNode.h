/*******************************************************************************
 *                                                                             *
 * Project: QtQmlTreeView Demo using a custom json to populate the tree        *
 * Filename: TreeNode.h                                                        *
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

#ifndef __TREE_NODE_H__
#define __TREE_NODE_H__

#include <QVariant>
#include <QString>
#include <QList>

class TreeNode
{
public:
    /**
     * @brief Constructs a TreeNode with given data and optional parent node.
     *
     * @param name The name of the node.
     * @param value The value of the node.
     * @param parentItem The parent node (defaults to nullptr for leaf nodes).
     */
    explicit TreeNode(const QString &name, const QVariant &value, TreeNode *parentItem = nullptr);

    /**
     * @brief Destructor for the TreeNode class.
     *
     * This ensures proper cleanup of dynamically allocated memory, including
     * child nodes.
     */
    ~TreeNode();

    /**
     * @brief Appends a child node to the current TreeNode.
     *
     * @param child The child node to append.
     */
    void appendChild(TreeNode *child);

    /**
     * @brief Returns the child node at the specified row (index).
     *
     * @param row The index of the child node.
     * @return The child node at the specified row.
     */
    TreeNode *child(int row);

    /**
     * @brief Returns the total number of child nodes.
     *
     * @return The number of child nodes.
     */
    int childCount() const;

    /**
     * @brief Returns the number of columns (data elements) stored in the node.
     *
     * @return The number of columns.
     */
    int columnCount() const;  // Fixed typo here: `coloumnCount()` -> `columnCount()`

    /**
     * @brief Returns the data at the specified column for this node.
     *
     * @param column The column index.
     * @return The data for the node at the specified column.
     */
    QVariant data(int column) const;

    /**
     * @brief Returns the row (index) of this node within its parent.
     *
     * @return The index of this node in its parent's child list.
     */
    int row() const;

    /**
     * @brief Returns a list of child nodes.
     *
     * @return A list of child nodes.
     */
    inline QList<TreeNode *>& children() { return _children; }

    /**
     * @brief Returns the name of the node.
     *
     * @return The name of the node.
     */
    inline QString name() const { return _name; }

    /**
     * @brief Returns the value assciated with the name of the node.
     *
     * @return The value of the node.
     */
    inline QVariant value() const { return _value; }

    /**
     * @brief Sets the value of the node.
     *
     * This function sets the value of the current `TreeNode` to the provided `value`.
     * The `value` can be of any type supported by `QVariant`, and it is stored in the
     * private member variable `_value`. This method allows updating the value stored
     * in the node.
     *
     * @param value The value to set for the current node. It can be any type that
     *              `QVariant` supports (e.g., int, double, QString, etc.).
     *
     * @see QVariant
     */
    inline void setValue(QVariant value) { _value = value; }

    /**
     * @brief Returns the parent node of this TreeNode.
     *
     * @return The parent TreeNode.
     */
    inline TreeNode *parentNode() const { return _parentNode; }

private:
    QList<TreeNode *> _children;
    QString _name;
    QVariant _value;
    TreeNode *_parentNode;
};

#endif // __TREE_NODE_H__
