#ifndef STOCKDB_H
#define STOCKDB_H

#include <QStringList>

class StockDB
{
public:
    StockDB();

    static QString m_tableName;
    static QString m_dbPath;

    static void deleteDeals(QString stockID,QString timeNow);
    static void getStocks(QStringList &list);
    static void insertDeals(QStringList str);

};

#endif // STOCKDB_H
