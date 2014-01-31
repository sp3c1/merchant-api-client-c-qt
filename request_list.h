#ifndef REQUEST_LIST_H
#define REQUEST_LIST_H



typedef void (*callback_flubit)(QString);


class request_list
{
    public:
    QNetworkReply *rep;
    QNetworkRequest *req;
    QString *method_name;
    QString *index;
    callback_flubit callback;

    request_list(QNetworkReply *rep_ , QNetworkRequest *req_, QString method="default", QString ind="default", void (*fnc_ptr)(QString)=NULL )
    {
        rep=rep_;
        req=req_;
        method_name = new QString(method);
        index = new QString(ind);
        callback = fnc_ptr;
    }

    request_list( )
    {
    }

    ~request_list()
    {
        rep->deleteLater();
        req->~QNetworkRequest();
        method_name->~QString();
        index->~QString();

        qDebug()<<"oi";
    }
};

#endif // REQUEST_LIST_H
