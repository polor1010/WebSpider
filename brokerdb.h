#ifndef BROKERDB_H
#define BROKERDB_H

#include <QStringList>

class BrokerDB
{
public:
    BrokerDB();

    void getBrokers(QStringList &List);
};

#endif // BROKERDB_H
