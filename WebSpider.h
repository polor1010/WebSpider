#ifndef FRAMECAPTURE_H
#define FRAMECAPTURE_H

#include <QtWebKit>
#include <QWebPage>
#include <QWebView>
#include <QString>

class WebSpider : public QObject
{
    Q_OBJECT

public:
    WebSpider();
    ~WebSpider();
    void loadPage(const QUrl&);
    void start();

public slots:
    void loadFinished(bool);

    void getNextIndex();
    void getNextStock();
    void reLoad();

private:
    void wait(int msec);
    double currencyToNumber(QString value);
    int getIndexOfStockID();
    //int getProgress();

private:
    QWebView* m_view;
    QString m_dbPath;
    QStringList m_stockList;

    QString m_stockID;
    int m_stockIndex;
    int m_pageIndex;

};


#endif
