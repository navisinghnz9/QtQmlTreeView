# QtQmlTreeView
QtQmlTreeView is a demo to rendering/managing a dynamic tree from JSON data with nested nodes of various types.

## How to use the TreeModel

Instantiate the tree model with given json file

```
    TreeModel treeModel("./test.json");

    // pass tree model to QML
    engine.rootContext()->setContextProperty("treeModel", &treeModel);
```

## TreeView Create From JSON file
![Alt text](docs/TreeViewDemo-1.png)

