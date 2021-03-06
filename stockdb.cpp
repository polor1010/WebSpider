#include "stockdb.h"
#include <QtSql/QtSql>

QString StockDB::m_tableName = "Stocks";
QString StockDB::m_dbPath = "./Stock.db";


StockDB::StockDB()
{

}

void wait(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        ;
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void StockDB::deleteDeals(QString stockID,QString timeNow)
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
        //SELECT_DATA.sprintf("select * from %s order by %s %s" , vm_TableName.toAscii().data() , vm_ValueArray[Index].toAscii().data() , vm_OrderType[OrderType].toAscii().data() );
        command = QString("delete from Deals where stockID = '%1' and time = '%2';").arg(stockID).arg(timeNow);

        bool IsOk = query.exec(command);

        if( !IsOk )
        {
            qDebug() << query.lastError();//
            /*QMessageBox qmb;
            qmb.setText(SELECT_DATA);
            qmb.exec();*/
        }
    }

    QString connectionName;
    {
        connectionName = QSqlDatabase::database().connectionName();
        dbconn.close();
    }
    QSqlDatabase::removeDatabase(connectionName);


}

void StockDB::insertDeals(QStringList strList)
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
        //SELECT_DATA.sprintf("select * from %s order by %s %s" , vm_TableName.toAscii().data() , vm_ValueArray[Index].toAscii().data() , vm_OrderType[OrderType].toAscii().data() );
        //SELECT_DATA = QString("select id, name from Stocks");

        command = "insert into Deals(time , stockID, brokerID,price,buy,sell) values ";

        int i;
        for( i  = 0 ; i < strList.count() ; i++ )
        {
            QString str = strList.at(i);
            QString timeNow = str.split(" ")[5];
            QString stockID = str.split(" ")[0];
            QString brokerID = str.split(" ")[1];
            QString price = str.split(" ")[2];
            price.replace(",","");
            QString buy = str.split(" ")[3];
            buy.replace(",","");
            QString sell = str.split(" ")[4];
            sell.replace(",","");

            command += QString(" ('%1','%2','%3','%4','%5','%6') ").arg( timeNow).arg(stockID).arg( brokerID).arg( price).arg( buy).arg( sell);

            if( i == ( strList.count() - 1 ) )
            {
                command += ";";
            }
            else
            {
                command += ",";
            }
        }
        //qDebug() << command;
        bool IsOk = query.exec(command);

        if( !IsOk )
        {
            qDebug() << command;
            qDebug() << query.lastError();//
            /*QMessageBox qmb;
            qmb.setText(SELECT_DATA);
            qmb.exec();*/
            wait(8000);
        }
    }

    QString connectionName;
    {
        connectionName = QSqlDatabase::database().connectionName();
        dbconn.close();
    }
    QSqlDatabase::removeDatabase(connectionName);


}

void StockDB::getStocks(QStringList &list)
{
    QSqlDatabase dbconn;

    if(QSqlDatabase::contains("Stocks"))
         dbconn = QSqlDatabase::database("Stocks");
    else
         dbconn = QSqlDatabase::addDatabase("QSQLITE", "Stocks");

    dbconn.setDatabaseName("./Stock.db");

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

        QString SELECT_DATA;
        //SELECT_DATA.sprintf("select * from %s order by %s %s" , vm_TableName.toAscii().data() , vm_ValueArray[Index].toAscii().data() , vm_OrderType[OrderType].toAscii().data() );
        SELECT_DATA = QString("select id, name from Stocks");

        //qDebug() << SELECT_DATA << "Index" << Index << "OrderType" << OrderType;

        bool IsOk = query.exec(SELECT_DATA);

        if( !IsOk )
        {
            qDebug() << query.lastError();//
            /*QMessageBox qmb;
            qmb.setText(SELECT_DATA);
            qmb.exec();*/
        }
        else
        {
            list.clear();
            while( query.next() )
            {
                /*QMessageBox qmb;
                qmb.setText( query.value(0).toString() );*/

                //List.append( query.value(0).toString() + " " + query.value(2).toString() + " "+ query.value(3).toString() + " "+ query.value(4).toString() + " "+ query.value(5).toString());
                list.append(query.value(0).toString() + " "  + query.value(1).toString());
                //qDebug() <<  query.value(0).toString() + " "  + query.value(1).toString();// + " " + query.value(3).toString() + " "+ query.value(4).toString() + " "+ query.value(5).toString();
            }
        }
    }

    QString connectionName;
    {
        connectionName = QSqlDatabase::database().connectionName();
        dbconn.close();
    }
    QSqlDatabase::removeDatabase(connectionName);

}

