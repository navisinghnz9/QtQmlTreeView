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
    TreeNode* _rootItem = new TreeNode("Fruits");

    // adding citrus category with its child nodes
    TreeNode *citrusCategory = new TreeNode("Citrus", _rootItem);
    citrusCategory->children().append(new TreeNode("Apple", citrusCategory));
    citrusCategory->children().append(new TreeNode("Orange", citrusCategory));

    TreeNode* kiwiNode = new TreeNode("Kiwi", citrusCategory);
    kiwiNode->children().append(new TreeNode("Type 1", kiwiNode));
    kiwiNode->children().append(new TreeNode("Type 2", kiwiNode));

    citrusCategory->children().append(kiwiNode);
    _rootItem->children().append(citrusCategory);

    // adding berries category with its child nodes
    TreeNode *berryCategory = new TreeNode("Berries", _rootItem);
    berryCategory->children().append(new TreeNode("Strawberry", berryCategory));
    berryCategory->children().append(new TreeNode("Blueberry", berryCategory));
    berryCategory->children().append(new TreeNode("Raspberry", berryCategory));
    _rootItem->children().append(berryCategory);

    // adding drupes category with its child nodes
    TreeNode *drupesCategory = new TreeNode("Drupes", _rootItem);
    drupesCategory->children().append(new TreeNode("Plums", drupesCategory));
    drupesCategory->children().append(new TreeNode("Peaches", drupesCategory));
    drupesCategory->children().append(new TreeNode("Olives", drupesCategory));
    _rootItem->children().append(drupesCategory);

    return _rootItem;
}

void traverseJson(TreeNode* rootItem, QJsonObject &jsonObj) {

    for (auto it = jsonObj.begin(); it != jsonObj.end(); ++it)
    {
        QString name = it.key();
        TreeNode *obj = new TreeNode(name, rootItem);
        rootItem->children().append(obj);
        qDebug() << "checking node: " << name;

        if (it.value().isString()) {
            QString value = it.value().toString();
            obj->children().append(new TreeNode(value, obj));
            continue;
        }

        if (it.value().isBool()) {
            QString value = QString::number(it.value().toBool());
            obj->children().append(new TreeNode(value, obj));
            continue;
        }

        if (it.value().isDouble()) {
            QString value = QString::number(it.value().toDouble()); // TODO - fix double conversion
            obj->children().append(new TreeNode(value, obj));
            continue;
        }

        if (it.value().isArray()) {
            QJsonArray jsonArray = it.value().toArray();
            for (int i = 0; i < jsonArray.size(); ++i) {
                QJsonValue value = jsonArray.at(i);
                QJsonObject childObj = value.toObject();
                traverseJson(obj, childObj);
            }
            continue;
        }

        if (it.value().isObject()) {
            QJsonObject childObj = it.value().toObject();
            traverseJson(obj, childObj);
            continue;
        }

        if (it.value().isNull()) {
            qDebug() << "TODO - Need to handle null value for node: " << name;
        }

    }
}

TreeNode* setupJsonModelData() {

    TreeNode* rootItem = new TreeNode("Config");

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

    TreeModel TreeModel(setupJsonModelData());
    engine.rootContext()->setContextProperty("treeModel", &TreeModel);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
