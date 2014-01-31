#ifndef FLUBIT_API_H
#define FLUBIT_API_H

#include <QObject>
#include <QString>
#include <QList>
#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>
#include <QUrlQuery>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QCryptographicHash>
#include <QTime>
#include <QDateTime>
#include <QtNetwork/QHostInfo>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>


#include "request_list.h"



class flubit_api : public QObject
{
    Q_OBJECT
public:
    explicit flubit_api(QObject *parent = 0, QString key="" , QString secret="", QString webdomain="api.weflubit.com");
    ~flubit_api();

    /* debug */
    void showSettings();

    /* functions */
    void getAccountStatus(QString user_txn_id="", callback_flubit fnc_ptr=NULL);
    void dispatchOrderByFlubitId(QString user_txn_id="",callback_flubit fnc_ptr=NULL, QString id="", QString dateTime="", QString params="{}");
    void dispatchOrderByMerchantOrderId(QString user_txn_id="",callback_flubit fnc_ptr=NULL,QString id="", QString dateTime="", QString params="{}");
    void cancelOrderByFlubitId(QString user_txn_id="",callback_flubit fnc_ptr=NULL,QString id="", QString reason="");
    void cancelOrderByMerchantOrderId(QString user_txn_id="",callback_flubit fnc_ptr=NULL,QString id="",QString reason="");
    void refundOrderByFlubitId(QString user_txn_id="",callback_flubit fnc_ptr=NULL, QString id="");
    void refundOrderByMerchantOrderId(QString user_txn_id="",callback_flubit fnc_ptr=NULL, QString id="");
    void getOrders(QString user_txn_id="", callback_flubit fnc_ptr=NULL, QString from="", QString status="");
    void getProducts(QString user_txn_id="",callback_flubit fnc_ptr=NULL, QString isActive="", QString limit="", QString page="",  QString sku = NULL );
    void getProductsFeed(QString user_txn_id="",callback_flubit fnc_ptr=NULL,QString feedID="");
    void createProducts(QString user_txn_id="",callback_flubit fnc_ptr=NULL,QString xml="");
    void updateProducts(QString user_txn_id="",callback_flubit fnc_ptr=NULL,QString xml="");
    QString buildUriQuery(QList<QString>* params);





private:
     /* Attributes */
    QNetworkAccessManager *client;
    QString *timestampFormat;
    QString *apiKey;
    QString *apiSecret;
    QString *domain;
    QUrl url;

    QList<request_list*> req_list;
    QString *version;

    QString timeZ;

    /* Methods */
    QString generateDispatchOrderPayload(QString dateTime, QString params);
    QString generateCancelOrderPayload(QString reason);
    QString call(QNetworkRequest* request,QByteArray  data, QString request_type,QString method,QString user_txn_id, callback_flubit fnc_ptr=NULL);
    QNetworkRequest* getRequest(QString uri); // get
    QString generateAuthToken();
    QString generateNonce();
    QString getDate();
    QString dateHelper(QString date);

/* signals and slots */
signals:



public slots:
    void replyFinished(QNetworkReply *reply);

};

#endif // FLUBIT_API_H
