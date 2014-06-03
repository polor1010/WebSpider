#include "WebSpider.h"

#include <QWebFrame>
#include <iostream>
#include <QtWebKit>
#include <QDebug>
#include <QtSql/QtSql>

WebSpider::WebSpider()
{
    m_stockIndex = 0;
    m_pageIndex = 1;
    m_view = new QWebView();

    getStocks();
    connect(m_view, SIGNAL(loadFinished(bool)),this, SLOT(loadFinished(bool)));

    int i;
    for( i  = 0 ; i < m_stockList.count() ; ++i )
    {
        //qDebug() << m_stockList.at(i);
    }
}
WebSpider::~WebSpider()
{
    delete m_view;
}

void WebSpider::start()
{
    QString stockID = m_stockList.at(m_stockIndex).split(" ")[0];
    QString url  = QString("http://bsr.twse.com.tw/bshtm/bsContent.aspx?StartNumber=%1&FocusIndex=%2").arg(stockID).arg(m_pageIndex);
    m_view->load(QUrl(url));

}

void WebSpider::wait(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        ;
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void WebSpider::loadPage(const QUrl& url)
{
    m_view->load(url);
}

void WebSpider::getStocks()
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
            //List.clear();

            while( query.next() )
            {
                /*QMessageBox qmb;
                qmb.setText( query.value(0).toString() );*/

                //List.append( query.value(0).toString() + " " + query.value(2).toString() + " "+ query.value(3).toString() + " "+ query.value(4).toString() + " "+ query.value(5).toString());
                m_stockList.append(query.value(0).toString() + " "  + query.value(1).toString());
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

void WebSpider::getBrokers()
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
            //List.clear();

            while( query.next() )
            {
                /*QMessageBox qmb;
                qmb.setText( query.value(0).toString() );*/

                //List.append( query.value(0).toString() + " " + query.value(2).toString() + " "+ query.value(3).toString() + " "+ query.value(4).toString() + " "+ query.value(5).toString());
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

void WebSpider::getNextIndex()
{
    m_pageIndex += 1;

    if( m_stockIndex < m_stockList.count() )
    {
        QString stockID = m_stockList.at(m_stockIndex).split(" ")[0];
        QString url  = QString("http://bsr.twse.com.tw/bshtm/bsContent.aspx?StartNumber=%1&FocusIndex=%2").arg(stockID).arg(m_pageIndex);

        qDebug() << url;

        wait(100);
        m_view->load(QUrl(url));
    }

}

void WebSpider::reLoad()
{
    if( m_stockIndex < m_stockList.count() )
    {
        QString stockID = m_stockList.at(m_stockIndex).split(" ")[0];
        QString url  = QString("http://bsr.twse.com.tw/bshtm/bsContent.aspx?StartNumber=%1&FocusIndex=%2").arg(stockID).arg(m_pageIndex);

        qDebug() << url;

        wait(5000);
        m_view->load(QUrl(url));
    }

}

void WebSpider::getNextStock()
{
    m_stockIndex += 1;
    m_pageIndex = 1;

    if( m_stockIndex < m_stockList.count() )
    {
        QString stockID = m_stockList.at(m_stockIndex).split(" ")[0];
        QString url  = QString("http://bsr.twse.com.tw/bshtm/bsContent.aspx?StartNumber=%1&FocusIndex=%2").arg(stockID).arg(m_pageIndex);

        qDebug() << url;

        wait(100);
        m_view->load(QUrl(url));
    }
    else
    {
        qDebug() << "finish..";
    }

}

void WebSpider::loadFinished(bool ok)
{
    if( ok )
    {
        QWebElementCollection elements = m_view->page()->mainFrame()->findAllElements("tr");

        int i,j;
        int trCounter = 0;
        for( i = 0 ; i < elements.count() ; ++i )
        {
            QString className = elements.at(i).attribute("class");
            if( className == "column_value_price_3" || className == "column_value_price_2" )
            {
                trCounter++;

                QWebElementCollection childElemetn = elements.at(i).findAll("td");
                qDebug() << childElemetn.count();
                for( j = 0 ; j < childElemetn.count() ; ++j )
                {
                    qDebug() << j <<  childElemetn.at(j).toInnerXml();
                }
            }
        }
        qDebug() << "trCounter : " << trCounter << m_stockIndex << m_pageIndex;

        if( trCounter < 100 )
            getNextStock();
        else
            getNextIndex();

    }
    else
    {
        qDebug() << "load time out";
        reLoad();
    }

}
