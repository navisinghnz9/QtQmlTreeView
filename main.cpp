#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include "model/TreeModel.h"


TreeNode* setupTreeModelData()
{
    TreeNode* _rootItem = new TreeNode("Fruits", "");

    // adding citrus category with its child nodes
    TreeNode *citrusCategory = new TreeNode("Citrus", "", _rootItem);
    citrusCategory->children().append(new TreeNode("Apple", "", citrusCategory));
    citrusCategory->children().append(new TreeNode("Orange", "", citrusCategory));

    TreeNode* kiwiNode = new TreeNode("Kiwi", "", citrusCategory);
    kiwiNode->children().append(new TreeNode("Type 1", "", kiwiNode));
    kiwiNode->children().append(new TreeNode("Type 2", "", kiwiNode));

    citrusCategory->children().append(kiwiNode);
    _rootItem->children().append(citrusCategory);

    // adding berries category with its child nodes
    TreeNode *berryCategory = new TreeNode("Berries", "", _rootItem);
    berryCategory->children().append(new TreeNode("Strawberry", "", berryCategory));
    berryCategory->children().append(new TreeNode("Blueberry", "", berryCategory));
    berryCategory->children().append(new TreeNode("Raspberry", "", berryCategory));
    _rootItem->children().append(berryCategory);

    // adding drupes category with its child nodes
    TreeNode *drupesCategory = new TreeNode("Drupes", "", _rootItem);
    drupesCategory->children().append(new TreeNode("Plums", "", drupesCategory));
    drupesCategory->children().append(new TreeNode("Peaches", "", drupesCategory));
    drupesCategory->children().append(new TreeNode("Olives", "", drupesCategory));
    _rootItem->children().append(drupesCategory);

    return _rootItem;
}


void traverseJson(TreeNode* rootItem, QJsonObject &jsonObj) {

    for (auto it = jsonObj.begin(); it != jsonObj.end(); ++it)
    {
        QString name = it.key();


        if (it.value().isString()) {
            TreeNode *obj = new TreeNode(name, it.value().toString(), rootItem);
            rootItem->children().append(obj);
            continue;
        }

        if (it.value().isBool()) {
            TreeNode *obj = new TreeNode(name, it.value().toBool(), rootItem);
            rootItem->children().append(obj);
            continue;
        }

        if (it.value().isDouble()) {
            TreeNode *obj = new TreeNode(name, it.value().toDouble(), rootItem);
            rootItem->children().append(obj);
            continue;
        }

        if (it.value().isArray()) {
            TreeNode *obj = new TreeNode(name, "", rootItem);
            rootItem->children().append(obj);

            QJsonArray jsonArray = it.value().toArray();
            for (int i = 0; i < jsonArray.size(); ++i) {
                QJsonValue value = jsonArray.at(i);
                QJsonObject childObj = value.toObject();
                traverseJson(obj, childObj);
            }
            continue;
        }

        if (it.value().isObject()) {
            TreeNode *obj = new TreeNode(name, "-", rootItem); // FIXME value param
            rootItem->children().append(obj);

            QJsonObject childObj = it.value().toObject();
            traverseJson(obj, childObj);
            continue;
        }

        if (it.value().isNull()) {
            TreeNode *obj = new TreeNode(name, "", rootItem); // FIXME value param
            rootItem->children().append(obj);

            qDebug() << "TODO - Need to handle null value for node: " << name;
        }

    }
}

TreeNode* setupJsonModelData() {

    TreeNode* rootItem = new TreeNode("Config", "");

    QFile jsonFile(":/data/test.json");
    if (!jsonFile.open(QIODevice::ReadOnly)) {
        qDebug() << "ERROR - failed to open json file";
        return rootItem;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonFile.readAll());
    QJsonObject jsonObj = jsonDoc.object();
    traverseJson(rootItem, jsonObj);

    return rootItem;
}

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    TreeModel treeModel(setupJsonModelData());
    engine.rootContext()->setContextProperty("treeModel", &treeModel);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
