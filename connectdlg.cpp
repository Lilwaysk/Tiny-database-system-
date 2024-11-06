#include "connectdlg.h"
#include "ui_connectdlg.h"

#include <QSqlDatabase>
#include <QtSql>

ConnDlg::ConnDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnDlg)
{
    ui->setupUi(this);

    QStringList drivers = QSqlDatabase::drivers();      // return all allow use dbdriver
    ui->comboDriver->addItems(drivers);                 // dbdriver name join the combox
    // when the user select different dbdriver in combox, the slot function will be called
    connect(ui->comboDriver, SIGNAL(currentIndexChanged(const QStirng &)), this, SLOT(driverChanged(const QString &)));
    ui->status_label->setText(tr("准备连接数据库！"));     // set current program running status
}

ConnDlg::~ConnDlg()
{
    delete ui;
}

void ConnDlg::driverChanged(const QString &text)
{
    if(text == "QSQLITE")                               // QSQLITE no need to set up
    {
        ui->editDatabase->setEnabled(false);
        ui->editUsername->setEnabled(false);
        ui->editPassword->setEnabled(false);
        ui->editHostname->setEnabled(false);
        ui->portSpinBox->setEnabled(false);
    }
    else
    {
        ui->editDatabase->setEnabled(true);
        ui->editUsername->setEnabled(true);
        ui->editPassword->setEnabled(true);
        ui->editHostname->setEnabled(true);
        ui->portSpinBox->setEnabled(true);
    }
}

QSqlError ConnDlg::addConnetion(const QString &driver, const QString &dbName, const QString &host, const QString &user, const QString &passwd, int port)
{
    QSqlError err;
    QSqlDatabase db = QSqlDatabase::addDatabase(driver);
    db.setDatabaseName(dbName);
    db.setHostName(host);
    db.setPort(port);

    if(!db.open(user, passwd))      // if failed to open db, record the last error and delete the connected
        err = db.lastError();

    return err;
}

void ConnDlg::creatDB()
{
    QSqlQuery query;
    query.exec("create table factory (id int primary key, manufactory varchar(40), address varchar(40))");
    query.exec(QObject::tr("insert into factory values(1, '一汽大众', '长春')"));
    query.exec(QObject::tr("insert into factory values(2, '华晨宝马', '武汉')"));
    query.exec(QObject::tr("insert into factory values(3, '梅赛德斯', '上海')"));
    // set the foreign key factoryid into factory
    query.exec("create table cars (carid int primary key, name varchar(50), factoryid int, year int, foreign key(factoryid) references factory)");

    query.exec(QObject::tr("insert into cars values(1, 'Macan', 1, 2005)"));
    query.exec(QObject::tr("insert into cars values(1, 'Taycan', 1, 2015)"));
    query.exec(QObject::tr("insert into cars values(1, 'Cayenne', 1, 2001)"));
    query.exec(QObject::tr("insert into cars values(1, 'Panamera', 2, 2001)"));
    query.exec(QObject::tr("insert into cars values(1, 'Ghibli', 2, 2002)"));
    query.exec(QObject::tr("insert into cars values(1, '总裁', 2, 2011)"));
    query.exec(QObject::tr("insert into cars values(1, 'Urus', 3, 2005)"));
    query.exec(QObject::tr("insert into cars values(1, 'Revuelto', 3, 2009)"));
}

void ConnDlg::addSqliteConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("databasefile");
    if(!db.open())
    {
        ui->status_label->setText(db.lastError().text());
        return ;
    }
    ui->status_label->setText(tr("创建sqlite数据库成功！"));
}

QString ConnDlg::driverName() const
{
    return ui->comboDriver->currentText();
}

QString ConnDlg::databaseName() const
{
    return ui->editDatabase->text();
}

QString ConnDlg::userName() const
{
    return ui->editUsername->text();
}

QString ConnDlg::password() const
{
    return ui->editPassword->text();
}

QString ConnDlg::hostName() const
{
    return ui->editHostname->text();
}

int ConnDlg::port() const
{
    return ui->portSpinBox->value();
}

void ConnDlg::on_okButton_clicked()
{
    if(ui->comboDriver->currentText().isEmpty())                // check user whether to choose a dbdriver
    {
        ui->status_label->setText(tr("请选择一个数据库驱动！"));
        ui->comboDriver->setFocus();
    }
    else if(ui->comboDriver->currentText() == "QSQLITE")        // if choose qsqlite, recall addSqliteConnection() funtional
    {
        addSqliteConnection();

        // if table not exist ,create table
        creatDB();
        accept();
    }
    else
    {
        QSqlError err = addConnetion(driverName(),              // if choose other dbdriver, recall addSqliteConnection() create another type db connecting
                                     databaseName(),
                                     hostName(),
                                     userName(),
                                     password(),
                                     port());
        if(err.type() != QSqlError::NoError)
            ui->status_label->setText(err.text());              // display the error message
        else
            ui->status_label->setText(tr("链接数据库成功!"));

        // if table not exist ,create table
        accept();
    }
}

