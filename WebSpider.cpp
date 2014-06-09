#include "WebSpider.h"

#include <QWebFrame>
#include <iostream>
#include <QtWebKit>
#include <QDebug>
#include <QtSql/QtSql>
#include "stockdb.h"

WebSpider::WebSpider()
{
    m_stockIndex = 0;
    m_pageIndex = 1;
    m_view = new QWebView();

    connect(m_view, SIGNAL(loadFinished(bool)),this, SLOT(loadFinished(bool)));

    StockDB::getStocks(m_stockList);

    QString str = "10,234.83";
    qDebug() << str << currencyToNumber(str);

}

WebSpider::~WebSpider()
{
    delete m_view;
}

void WebSpider::start()
{
    if( m_stockList.count() > 0 )
    {
        QString stockID = m_stockList.at(m_stockIndex).split(" ")[0];
        QString url  = QString("http://bsr.twse.com.tw/bshtm/bsContent.aspx?StartNumber=%1&FocusIndex=%2").arg(stockID).arg(m_pageIndex);
        m_view->load(QUrl(url));
    }
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

double WebSpider::currencyToNumber(QString str)
{
    int i;
    QString intPart, floatPart;

    double sum = 0;

    if( str.contains(".") )
    {
        intPart = str.split(".").at(0);
        floatPart = str.split(".").at(1);

        int floatPartLength = floatPart.count();
        for( i = 0 ; i < floatPartLength ; ++i )
        {
            float value = QString(floatPart.at(i)).toInt() * pow(0.1,i+1);
            sum += value;
        }
    }
    else
    {
        intPart = str;

    }

    int intPartLength = intPart.count();
    int dec = 0;
    for( i = intPartLength - 1 ; i >= 0 ; --i )
    {
        if( intPart.at(i) != ',' )
        {
            sum += QString(intPart.at(i)).toInt() * pow(10,dec);
            dec++;
        }
    }

    return sum;
}

void WebSpider::loadFinished( bool ok )
{
    if( ok )
    {
        QWebElementCollection elements = m_view->page()->mainFrame()->findAllElements("tr");

        int i;
        int trCounter = 0;
        QStringList strList;
        for( i = 0 ; i < elements.count() ; ++i )
        {
            QString className = elements.at(i).attribute("class");
            if( className == "column_value_price_3" || className == "column_value_price_2" )
            {
                trCounter++;

                QWebElementCollection childElemetn = elements.at(i).findAll("td");
                qDebug() << childElemetn.count();
                if( childElemetn.count() == 5 )
                {
                    QString stockID = m_stockList.at(m_stockIndex).split(" ")[0];;
                    QString brokerID = childElemetn.at(1).toInnerXml().simplified().split(" ").at(0);
                    QString price = childElemetn.at(2).toInnerXml().simplified();
                    QString buy = childElemetn.at(3).toInnerXml().simplified();
                    QString sell = childElemetn.at(4).toInnerXml().simplified();

                    if( !brokerID.contains("&nbsp") )
                    {
                        QString str = stockID + " " + brokerID + " "+ price + " "+ buy + " "+sell;
                        strList << str;
                        qDebug() << stockID << price << buy  << currencyToNumber(buy) << sell << currencyToNumber(sell);

                    }
                }
            }

        }
        if( strList.count() > 0 )
            StockDB::insertDeals(strList);

        qDebug() << "trCounter : " << trCounter << m_stockIndex << m_pageIndex;
        qDebug() << QDateTime::currentDateTime().toString();

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
