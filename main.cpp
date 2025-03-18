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
    citrusCategory->children().append(new TreeNode("Kiwi", citrusCategory));
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

TreeNode* setupJsonModelData() {

    QFile jsonFile("/home/nav/navi/experiments/QtQmlTreeView/data/test.json");
    if (!jsonFile.open(QIODevice::ReadOnly)) {
        qDebug() << "failed to open json file";
        return nullptr; // FIXME later
    }

    QByteArray jsonData = jsonFile.readAll();

    TreeNode* rootItem = new TreeNode("Config");

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    QJsonObject jsonObj = jsonDoc.object();

    for (auto it = jsonObj.begin(); it != jsonObj.end(); ++it)
    {
        QString name = it.key();
        QString value = it.value().toString();

        TreeNode *obj = new TreeNode(name, rootItem);
        obj->children().append(new TreeNode(value, obj));
        rootItem->children().append(obj);
    }

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
