#include "brokerdb.h"
#include <QtSql/QtSql>

BrokerDB::BrokerDB()
{
}

void BrokerDB::getBrokers(QStringList &list)
{
    QSqlDatabase dbconn;

    if(QSqlDatabase::contains("Brokers"))
         dbconn = QSqlDatabase::database("Brokers");
    else
         dbconn = QSqlDatabase::addDatabase("QSQLITE", "Brokers");

    dbconn.setDatabaseName("./Stock.db");

    int i = 0;

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
        SELECT_DATA = QString("select id, name from Brokers");

        //qDebug() << SELECT_DATA << "Index" << Index << "OrderType" << OrderType;

        bool isOk = query.exec(SELECT_DATA);

        if( !isOk )
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

                list.append( query.value(0).toString() + " " + query.value(1).toString());// + " "+ query.value(3).toString() + " "+ query.value(4).toString() + " "+ query.value(5).toString());
                i++;

                qDebug() <<  query.value(0).toString() + " "  + query.value(1).toString();// + " " + query.value(3).toString() + " "+ query.value(4).toString() + " "+ query.value(5).toString();
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
