#include "mainwindow.h"

#include <QGridLayout>
#include <QSqlRecord>
#include <QAbstractItemView>
#include <QHeaderView>
#include <QAction>
#include <QMenu>
#include <QMenuBar>

MainWindow::MainWindow(const QString &factoryTable, const QString &carTable, QFile *carDetails, QWidget *parent)
    : QMainWindow(parent)
{
    file = carDetails;
    readCarData();                                                              // 将xml文件数据读入QDomDocument实例保存
    carModel = new QSqlRelationalTableModel(this);                              // 为cars 创建一个数据库依赖表模型
    carModel->setTable(carTable);
    /*
     * model->setRelation(authorIdx, QSqlRelation("authors", "id", "name"));
     * setRelation()设置指定列与另一张表格的关联关系
     *  通过authoridx获取到名为author的字段在数据表中的索引，然后将该列与名为author的表格建立了
     *  外键关联，其中authors是另一张表格的名称，id是该表格中用于关联的列名，name是该表格中用于显
     *  示的列名。建立外键关联关系的目的是为了在QTableView中显示该列的数据时，可以将id转换为对应的
     *  name。
    */
    carModel->setRelation(2, QSqlRelation(factoryTable, "id", "manufactory"));
    carModel->select();
    factoryModel = new QSqlTableModel(this);
    factoryModel->setTable(factoryTable);
    factoryModel->select();

    /*------------------------------------------------------------------*/

    QGroupBox *factory = createFactoryGroupBox();
    QGroupBox *cars = createCarGroupBox();
    QGroupBox *details = createDetailGroupBox();


    // Layout 布局
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(factory, 0, 0);
    layout->addWidget(cars, 1, 0);
    layout->addWidget(details, 0, 1, 2, 1);
    layout->setColumnStretch(1, 1);
    layout->setColumnMinimumWidth(0, 500);
    QWidget *widge = new QWidget;
    widge->setLayout(layout);
    setCentralWidget(widge);
    createMenuBar();
    resize(850, 400);
    setWindowTitle(tr("主从视图"));
}

MainWindow::~MainWindow()
{
}

QGroupBox *MainWindow::createCarGroupBox()
{
    QGroupBox *box = new QGroupBox(tr("汽车"));
    carView = new QTableView;
    carView->setEditTriggers(QAbstractItemView::NoEditTriggers);    // 设置视图是否允许用户编辑
    carView->setSortingEnabled(true);
    carView->setSelectionBehavior(QAbstractItemView::SelectRows);
    carView->setSelectionMode(QAbstractItemView::SingleSelection);
    carView->setShowGrid(false);
    carView->verticalHeader()->hide();
    carView->setAlternatingRowColors(true);


    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(carView, 0, 0);
    box->setLayout(layout);
    return box;
}

QGroupBox *MainWindow::createFactoryGroupBox()
{
    factoryView = new QTableView;
    factoryView->setEditTriggers(QAbstractItemView::NoEditTriggers);    // 设置视图是否允许用户编辑
    factoryView->setSortingEnabled(true);
    factoryView->setSelectionBehavior(QAbstractItemView::SelectRows);
    factoryView->setSelectionMode(QAbstractItemView::SingleSelection);
    factoryView->setShowGrid(false);
    factoryView->setAlternatingRowColors(true);

    QGroupBox *box = new QGroupBox(tr("汽车制造厂商"));
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(factoryView, 0, 0);
    box->setLayout(layout);
    return box;
}

QGroupBox *MainWindow::createDetailGroupBox()
{
    QGroupBox *box = new QGroupBox(tr("详细信息"));

    profileLabel = new QLabel;
    profileLabel->setWordWrap(true);
    profileLabel->setAlignment(Qt::AlignBottom);
    titleLabel = new QLabel;
    titleLabel->setWordWrap(true);
    titleLabel->setAlignment(Qt::AlignBottom);
    attribList = new QListWidget;
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(profileLabel, 0, 0, 1, 2);
    layout->addWidget(titleLabel, 1, 0, 1, 2);
    layout->addWidget(attribList, 2, 0, 1, 2);
    layout->setRowStretch(2, 1);
    box->setLayout(layout);
    return box;

    return box;
}

void MainWindow::createMenuBar()
{
    QAction *addAction = new QAction(tr("添加"), this);
    QAction *deleteAction = new QAction(tr("删除"), this);
    QAction *quitAction = new QAction(tr("退出"), this);

    addAction->setShortcut(tr("Ctrl+A"));
    deleteAction->setShortcut(tr("Ctrl+D"));
    quitAction->setShortcut(tr("Ctrl+Q"));

    QMenu *fileMenu = menuBar()->addMenu(tr("操作菜单"));
    fileMenu->addAction(addAction);
    fileMenu->addAction(deleteAction);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);
}

void MainWindow::changeFactory(QModelIndex index)
{
    QSqlRecord record = factoryModel->record(index.row());  // getting the user select carfactory records
    QString factoryId = record.value("id").toString();      // getting the select carfactory primary key, only display the models of select carfactory
    carModel->setFilter("id = '"+ factoryId +"'");          // 在carModel中设置过滤器，使其只显示所选汽车制造商的车型
    showFactoryProfile(index);                              // 在详细信息中显示所选汽车制造商的信息
}
