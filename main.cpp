#include <QCoreApplication>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

int main(int argc, char *argv[])
{
    int errorLevel = 0;
    QCoreApplication app(argc, argv);

    QStringList list = QSqlDatabase::drivers();
    qInfo() << "Available SQL drivers:";
    for (int x = 0; x < list.length(); x++)
    {
        qInfo().noquote() << "-" << list.at(x);
    }
    qInfo() << "";

    const QString sqlDriverName = "QMYSQL";
    QSqlDatabase db = QSqlDatabase::addDatabase(sqlDriverName);
    if (db.isValid())
    {
        qInfo().noquote() << "The" << sqlDriverName << "driver seems to be OK";
        qInfo() << "";

        db.setHostName("127.0.0.1");
        db.setPort(3306);
        db.setDatabaseName("YOUR-DATABASE-NAME");
        db.setUserName("YOUR-DATABASE-USERNAME");
        db.setPassword("YOUR-DATABASE-PASSWORD");
        if (!db.open())
        {
            qCritical().noquote() << "[ERROR] Could not connect to the database."
                        << db.lastError().databaseText();//.driverText()
            errorLevel = 2;
        }
        else
        {
            QSqlQuery query;
            if (query.exec("SELECT something FROM sometable LIMIT 1;"))
            {
                if (query.first())
                {
                    // results are printed in quotes, just in case, since we are forcing string from QVariant
                    qInfo() << "Query results:" << query.value(0).toString();
                }
                else
                {
                    // for example, if there was a WHERE clause which would not match anything
                    qWarning() << "Query has succeeded, but there seem to be no results for it";
                }
            }
            else
            {
                qCritical() << "[ERROR] Executing query has failed."
                            << query.lastError().databaseText();//.driverText()
                errorLevel = 3;
            }
        }
    }
    else
    {
        qCritical().noquote() << "[ERROR]" << sqlDriverName << "driver is missing, as it seems";
        errorLevel = 1;
    }

    QCoreApplication::quit();
    return errorLevel;
}
