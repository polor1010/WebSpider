#include "summarydb.h"
#include <QtSql/QtSql>

QString SummaryDB::m_tableName = "Summary";
QString SummaryDB::m_dbPath = "./Stock.db";

SummaryDB::SummaryDB()
{
}



void SummaryDB::insert(QString str)
{
    QSqlDatabase dbconn;

    if(QSqlDatabase::contains(m_tableName))
         dbconn = QSqlDatabase::database(m_tableName);
    else
         dbconn = QSqlDatabase::addDatabase("QSQLITE", m_tableName);

    dbconn.setDatabaseName(m_dbPath);

    if( !dbconn.open( ))
    {
        qDebug() << "load db fail";
       // QMessageBox qmb;
       // qmb.setText("DB File Open Fail..");
       // qmb.exec();
    }
    else
    {
        QSqlQuery query(dbconn);

        QString command;
        command = "create table if not exists Summary ( time datetime , ID text , tradeReceive int , tradeAmount int , tradeQuantity int );";
        bool IsOk = query.exec(command);

        if( !IsOk )
        {
            qDebug() << command;
            qDebug() << query.lastError();
        }

        QString timeNow = str.split(" ").at(0);
        QString stockID = str.split(" ").at(1);
        QString tradeReceive = str.split(" ").at(2);
        QString tradeAmount = str.split(" ").at(3);
        QString tradeQuantity = str.split(" ").at(4);

        command =  QString( "insert into Summary ( time , ID , tradeReceive , tradeAmount , tradeQuantity) values('%1','%2','%3','%4','%5') ").arg(timeNow).arg(stockID).arg(tradeReceive).arg(tradeAmount).arg(tradeQuantity);
        //qDebug() << command;
        //qDebug() << command;

        IsOk = query.exec(command);

        if( !IsOk )
        {
            qDebug() << command;
            qDebug() << query.lastError();
            /*QMessageBox qmb;
            qmb.setText(SELECT_DATA);
            qmb.exec();*/
            //wait(8000);
        }
    }

    QString connectionName;
    {
        connectionName = QSqlDatabase::database().connectionName();
        dbconn.close();
    }
    QSqlDatabase::removeDatabase(connectionName);

}
