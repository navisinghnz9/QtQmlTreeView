#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include "model/TreeModel.h"

// dummy data of some fruits nested in categories, prices and attributes
// used for initial testing of tree.
// Later on, we will be populating tree with the data from JSON file.
TreeNode* setupFruitsTreeModelData()
{
    TreeNode* rootNode = new TreeNode("Fruits", "");

    // adding citrus category with its child nodes
    TreeNode *citrusCategory = new TreeNode("Citrus", 1, rootNode);
    citrusCategory->children().append(new TreeNode("Apple", 2, citrusCategory));
    citrusCategory->children().append(new TreeNode("Orange", 3, citrusCategory));

    TreeNode* kiwiNode = new TreeNode("Kiwi", "", citrusCategory);
    kiwiNode->children().append(new TreeNode("Type 1", "Expensive", kiwiNode));
    kiwiNode->children().append(new TreeNode("Type 2", "Cool", kiwiNode));

    citrusCategory->children().append(kiwiNode);
    rootNode->children().append(citrusCategory);

    // adding berries category with its child nodes
    TreeNode *berryCategory = new TreeNode("Berries", "", rootNode);
    berryCategory->children().append(new TreeNode("Strawberry", 1.5, berryCategory));
    berryCategory->children().append(new TreeNode("Blueberry", "Detox", berryCategory));
    berryCategory->children().append(new TreeNode("Raspberry", "Smoothies", berryCategory));
    rootNode->children().append(berryCategory);

    // adding drupes category with its child nodes
    TreeNode *drupesCategory = new TreeNode("Drupes", "", rootNode);
    drupesCategory->children().append(new TreeNode("Plums", 12, drupesCategory));
    drupesCategory->children().append(new TreeNode("Peaches", "Hot", drupesCategory));
    drupesCategory->children().append(new TreeNode("Olives", "Subway", drupesCategory));
    rootNode->children().append(drupesCategory);

    return rootNode;
}

void traverseJson(TreeNode* rootNode, QJsonObject &jsonObj) {

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
            for (int i = 0; i < jsonArray.size(); ++i) {
                QJsonValue value = jsonArray.at(i);
                QJsonObject childObj = value.toObject();
                traverseJson(obj, childObj);
            }
            continue;
        }

        if (it.value().isObject()) {
            TreeNode *obj = new TreeNode(name, "", rootNode);
            rootNode->children().append(obj);

            QJsonObject childObj = it.value().toObject();
            traverseJson(obj, childObj);
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

TreeNode* setupJsonModelData() {

    TreeNode* rootNode = new TreeNode("Config", "");
    QFile jsonFile("./test.json");

    if (!jsonFile.open(QIODevice::ReadOnly)) {
        qDebug() << "ERROR - failed to open json file";
        return rootNode;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonFile.readAll());
    QJsonObject jsonObj = jsonDoc.object();
    traverseJson(rootNode, jsonObj);

    return rootNode;
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
