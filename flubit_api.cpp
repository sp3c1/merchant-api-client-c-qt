#include "flubit_api.h"

/*====== Constructor & Destructor =======*/

flubit_api::flubit_api(QObject *parent, QString key, QString secret, QString webdomain) :
    QObject(parent)
{

    qsrand(QDateTime::currentDateTime().currentMSecsSinceEpoch());

    version = new QString("1");

    timestampFormat = new QString(); /* Y-m-d\TH:i:sO   =>  2013-05-30T14:43:07Z"*/

    apiKey =  new QString(key);
    apiSecret = new QString(secret);
    domain = new QString(webdomain);

    client = new QNetworkAccessManager(this);
    connect(client, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));

    getDate();
}


/*======== debug ========*/

void flubit_api::showSettings()
{

    qDebug("==SHOW DEBUG==");
    qDebug(apiKey->toUtf8());
    qDebug(apiSecret->toUtf8());
    qDebug(domain->toUtf8());
    qDebug() << *apiKey;
    qDebug("==END DEBUG==");
    qDebug("\n\n");

}

/*======== public methods =========*/

void flubit_api::getAccountStatus(QString user_txn_id,callback_flubit fnc_ptr)
{

    call(getRequest("account/status.xml"),"","get","getAccountStatus",user_txn_id,fnc_ptr);
}

void flubit_api::dispatchOrderByFlubitId(QString user_txn_id, callback_flubit fnc_ptr, QString id, QString dateTime, QString params)
{
    QByteArray data;
    QUrlQuery params_;

    params_.addQueryItem("flubit_order_id",id);

    data.append( params_.query());

    QByteArray data2 = generateDispatchOrderPayload(dateTime,params).toUtf8();

    call(getRequest(QString("orders/dispatch.xml?").append(data)),data2,"post","dispatchOrderByFlubitId",user_txn_id,fnc_ptr);
}

void flubit_api::dispatchOrderByMerchantOrderId(QString user_txn_id, callback_flubit fnc_ptr, QString id, QString dateTime, QString params)
{
    QByteArray data;
    QUrlQuery params_;

    params_.addQueryItem("merchant_order_id",id);

    data.append( params_.query());

    QByteArray data2 = generateDispatchOrderPayload(dateTime,params).toUtf8();

    call(getRequest(QString("orders/dispatch.xml?").append(data)),data2,"post","dispatchOrderByMerchantOrderId",user_txn_id,fnc_ptr);
}

void flubit_api::cancelOrderByFlubitId(QString user_txn_id, callback_flubit fnc_ptr, QString id, QString reason)
{
    QByteArray data;
    QUrlQuery params_;

    params_.addQueryItem("flubit_order_id",id);

    data.append( params_.query());

    QByteArray data2 = generateCancelOrderPayload(reason).toUtf8();


    call(getRequest(QString("orders/cancel.xml?").append(data)),data2,"post","cancelOrderByFlubitId",user_txn_id,fnc_ptr);
}

void flubit_api::cancelOrderByMerchantOrderId(QString user_txn_id, callback_flubit fnc_ptr, QString id, QString reason)
{
    QByteArray data;
    QUrlQuery params_;

    params_.addQueryItem("merchant_order_id",id);

    data.append( params_.query());

    QByteArray data2 = generateCancelOrderPayload(reason).toUtf8();


    call(getRequest(QString("orders/cancel.xml?").append(data)),data2,"post","cancelOrderByMerchantOrderId",user_txn_id,fnc_ptr);
}

void flubit_api::refundOrderByFlubitId(QString user_txn_id, callback_flubit fnc_ptr, QString id)
{
    QByteArray data;
    QUrlQuery params;


    params.addQueryItem("flubit_order_id",id);
    data.append( params.query());

    call(getRequest(QString("orders/refund.xml?").append(data)),"","post","refundOrderByFlubitId",user_txn_id,fnc_ptr);
}

void flubit_api::refundOrderByMerchantOrderId(QString user_txn_id, callback_flubit fnc_ptr, QString id)
{
    QByteArray data;
    QUrlQuery params;


    params.addQueryItem("merchant_order_id",id);
    data.append( params.query());

    call(getRequest(QString("orders/refund.xml?").append(data)),"","post","refundOrderByMerchantOrderId",user_txn_id,fnc_ptr);
}

void flubit_api::getOrders(QString user_txn_id,callback_flubit fnc_ptr, QString from, QString status)
{
    QByteArray data;
    QUrlQuery params;


    params.addQueryItem("from",dateHelper(from));
    params.addQueryItem("status",status);
    data.append( params.query());

    call(getRequest(QString("orders/filter.xml?").append(data)),"","get","getOrders",user_txn_id,fnc_ptr);

}

void flubit_api::getProducts(QString user_txn_id,callback_flubit fnc_ptr, QString isActive, QString limit, QString page,  QString sku )
{
    QByteArray data;
    QUrlQuery params;

    if( sku!= NULL)
    {
        params.addQueryItem("isActive",isActive);
        params.addQueryItem("sku",sku);
    }else
    {
        params.addQueryItem("isActive",isActive);
        params.addQueryItem("limit",limit);
        params.addQueryItem("page",page);
    }

    data.append( params.query());

    call(getRequest(QString("products/filter.xml?").append(data)),"","get","getProducts",user_txn_id,fnc_ptr);
}

void flubit_api::getProductsFeed(QString user_txn_id, callback_flubit fnc_ptr, QString feedID)
{

    call(getRequest(QString("products/feed/%1.xml").arg(feedID)),"","get","getProductsFeed",user_txn_id,fnc_ptr);
}

void flubit_api::updateProducts(QString user_txn_id, callback_flubit fnc_ptr, QString xml)
{
    call(getRequest(QString("products/feed.xml")),xml.toUtf8(),"post","getProductsFeed",user_txn_id,fnc_ptr);
}

void flubit_api::createProducts(QString user_txn_id, callback_flubit fnc_ptr, QString xml)
{
    QByteArray data;
    QUrlQuery params;

    params.addQueryItem("type","create");

    data.append( params.query());

    call(getRequest(QString("products/feed.xml?").append(data)),xml.toUtf8(),"post","getProductsFeed",user_txn_id,fnc_ptr);
}

/*======= private methods =========*/

QString flubit_api::call(QNetworkRequest* request, QByteArray  data ,QString request_type, QString method,QString user_txn_id,callback_flubit fnc_ptr)
{


    if(QString::compare(user_txn_id,"")==0)
        user_txn_id = QString(QCryptographicHash::hash(QString(qrand()).toUtf8(),QCryptographicHash::Md5).toHex());


    if(request_type=="post")
    {
        req_list.append( new request_list(client->post(*request,data) , request , method ,user_txn_id,fnc_ptr));
    }
    else
    {
       req_list.append( new request_list(client->get(*request) , request , method ,user_txn_id,fnc_ptr));
    }
    \
    return user_txn_id;
}

 QNetworkRequest* flubit_api::getRequest(QString uri)
 {\
     url.setUrl(QString("http://").append(domain).append(version).append("/").append(uri));
     QNetworkRequest * request = new QNetworkRequest(url); // right for api

    request->setRawHeader("Content-type","application/atom+xml");
    request->setRawHeader("Accept","application/xml");


    qDebug();

    request->setRawHeader("Auth-Token",generateAuthToken().toUtf8());
    request->setRawHeader("User-Agent","FLU API Qt C++ Async");

    return request;
 }

 QString flubit_api::generateDispatchOrderPayload(QString dateTime, QString params)
 {
     QJsonDocument d = QJsonDocument::fromJson(params.toUtf8());

     QString courier             =(d.object().find("courier").value().isUndefined()?"":d.object().find("courier").value().toString());
     QString consignmentNumber   =(d.object().find("consignmentNumber").value().isUndefined()?"":d.object().find("consignmentNumber").value().toString());
     QString trackingUrl         =(d.object().find("trackingUrl").value().isUndefined()?"":d.object().find("trackingUrl").value().toString());


     return QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dispatch><dispatched_at>%1</dispatched_at><courier>%2</courier><consignment_number>%3</consignment_number><tracking_url>%4</tracking_url></dispatch>")
             .arg(dateHelper(dateTime))
             .arg(courier)
             .arg(consignmentNumber)
             .arg(trackingUrl);
 }

 QString flubit_api::generateCancelOrderPayload(QString reason)
 {
     return QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?><cancel><reason>%1</reason></cancel>").arg(reason);
 }

 QString flubit_api::generateAuthToken()
 {
     QString timestamp = getDate();
     QString once = generateNonce();


     return QString("key=\"%1\", signature=\"%2\", nonce=\"%3\", created=\"%4\"")
             .arg(*apiKey)
             .arg( QString( QCryptographicHash::hash( QByteArray::fromBase64(QString(once).toUtf8()).append(timestamp).append(*apiSecret),   QCryptographicHash::Sha1).toBase64() )  )
             .arg(once)
             .arg(timestamp);
 }

 QString flubit_api::getDate()
 {
     QDateTime time_local,time_utc;

     time_local=QDateTime::currentDateTime().toLocalTime();
     time_utc=time_local.toUTC();


     qint64 unix_a, unix_b,unix_c,unix_d;
     unix_a=  time_local.toString("hh").toInt();
     unix_b=  time_utc.toString("hh").toInt();

     unix_c=  time_local.toString("dd").toInt()+time_local.toString("MM").toInt()*31;
     unix_d=  time_utc.toString("dd").toInt()+time_utc.toString("MM").toInt()*31;

     if(unix_c==unix_d)
         unix_a = unix_a-unix_b;
     else
         if(unix_c<unix_d) // local smaller
            unix_a= unix_a - 24 - unix_b;
         else // local larger
            unix_a= 24 + unix_b -unix_a ;


     unix_a*=100;

    QString sign;

    if( unix_a>=0 )
        sign.append("+");
    else
        if(unix_a<0)
           sign.append("-");

    if(timeZ.length()==0)
        timeZ.append(sign).append(QString("%1").arg(unix_a, 4, 10, QChar('0')).toUpper());
    else
    {
        QString timeZ_tmp="";
        timeZ_tmp.append(sign).append(QString("%1").arg(unix_a, 4, 10, QChar('0')).toUpper());
        timeZ=timeZ_tmp;
    }


    return time_utc.toString("yyyy-MM-ddThh:mm:ss").append(timeZ);

 }

 QString flubit_api::generateNonce(){

     qint64 *a =new qint64();

     QString my;

     my = QCryptographicHash::hash(my.sprintf("%p_%i_%i", ( QString().sprintf("%p",a).toUtf8().toHex() ), QDateTime::currentDateTime().currentMSecsSinceEpoch() , rand() ).toUtf8(),QCryptographicHash::Md5).toHex();

     return my;
 }

 QString flubit_api::dateHelper(QString date)
 {
     date.replace(" ","T");
     date.append(timeZ);
     return date;
 }

 flubit_api::~flubit_api()
 {
      //request_list.~request_list();

      timeZ.~QString();
      timestampFormat->~QString();
      apiKey->~QString();

      apiSecret->~QString();
      domain->~QString();
      url.~QUrl();
      version->~QString();

      client->deleteLater();

      req_list.~QList();



 }

/*======= SLOT ========*/

void flubit_api::replyFinished(QNetworkReply *reply)
{

    if(reply)
        if( reply->error()== QNetworkReply::NoError)
        {
            qint64 v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

            if( v>=200 && v<307 )
            {
                for(qint32 i=0;i<req_list.size();i++)
                {
                    if(req_list.at(i)->rep->isFinished() == true && req_list.at(i)->rep==reply)
                    {
                        if(req_list.at(i)->callback!=NULL)
                        {
                           return req_list.at(i)->callback(req_list.at(i)->rep->readAll());
                        }
                        /*else
                        {
                            // no callback function supplied
                        }
                         */

                        req_list.at(i)->~request_list();
                        req_list.removeAt(i);
                        break;
                    }
                }
            }/*else
            {
               // http  response error code
            }*/
        }
        /*else
        {
          //  Encountered some QNetworkError
        }*/
    /*else
      {
          // unxepected network reply is null
      }
     */

}

