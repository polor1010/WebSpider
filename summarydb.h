#ifndef SUMMARYDB_H
#define SUMMARYDB_H

#include <QString>

class SummaryDB
{
public:
    SummaryDB();

    static QString m_tableName;
    static QString m_dbPath;

    static void insert(QString data);

};

#endif // SUMMARYDB_H
