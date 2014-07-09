#include "summarydb.h"
#include <QtSql/QtSql>

QString SummaryDB::m_tableName = "Summaries";
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
        command = "create table if not exists Summaries ( time datetime , stockID text , tradeReceive flaot64 , tradeAmount flaot64 , tradeQuantity flaot64 );";
        bool IsOk = query.exec(command);

        if( !IsOk )
        {
            qDebug() << command;
            qDebug() << query.lastError();
        }

        QString timeNow = str.split(" ").at(0);
        QString stockID = str.split(" ").at(1);
        QString tradeReceive = str.split(" ").at(2);
        tradeReceive.replace(",","");
        QString tradeAmount = str.split(" ").at(3);
        tradeAmount.replace(",","");
        QString tradeQuantity = str.split(" ").at(4);
        tradeQuantity.replace(",","");

        command =  QString( "insert into Summaries ( time , stockID , tradeReceive , tradeAmount , tradeQuantity) values('%1','%2','%3','%4','%5') ").arg(timeNow).arg(stockID).arg(tradeReceive).arg(tradeAmount).arg(tradeQuantity);
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
