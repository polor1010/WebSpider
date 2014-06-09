#ifndef STOCKDB_H
#define STOCKDB_H

#include <QStringList>

class StockDB
{
public:
    StockDB();

    static void getStocks(QStringList &list);
    static void insertDeals(QStringList str);

};

#endif // STOCKDB_H
