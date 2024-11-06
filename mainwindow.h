#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGroupBox>
#include <QTableView>
#include <QListWidget>
#include <QLabel>
#include <QFile>
#include <QSqlRelationalTableModel>
#include <QSqlTableModel>
#include <QModelIndex>
#include <QDomNode>
#include <QDomDocument>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /* 构造函数: factorytable是要传入的汽车制造商表名
     *          cartable是汽车表名
     *          carDetails是需要传入的读取XML文件的QFile指针
     */
    MainWindow(const QString &factoryTable, const QString &carTable, QFile *carDetails, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addCar();
    void changeFactory(QModelIndex index);
    void delCar();
    void showCarDetails(QModelIndex index);
    void showFactoryProfile(QModelIndex index);

private:

    void decreaseCarCount(QModelIndex index);
    void getAttribList(QDomNode car);
    QModelIndex indexOfFactory(const QString &factory);
    void readCarData();
    void removeCarDatabase(QModelIndex index);
    void removeCarFromFile(int id);
    QDomDocument carData;
    QFile *file;
    QSqlRelationalTableModel *carModel;
    QSqlTableModel *factoryModel;

    QGroupBox *createCarGroupBox();
    QGroupBox *createFactoryGroupBox();
    QGroupBox *createDetailGroupBox();

    void createMenuBar();

    QTableView *carView;        // 显示汽车的视图
    QTableView *factoryView;    // 显示汽车制造商的视图
    QListWidget *attribList;     // 显示车辆的详细信息列表

    /* 声明相关的信息标签 */
    QLabel *profileLabel;
    QLabel *titleLabel;
};
#endif // MAINWINDOW_H
