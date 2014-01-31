#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include "flubit_api.h"

void test_ptr(QString hola)
{
    qDebug()<<"Custom callback function if these need to be expanded change the typedef in request_list header to suit ur needs \n Response :" << hola;
};

int main(int argc, char *argv[])
{


    QCoreApplication a(argc, argv);
    flubit_api *b= new flubit_api(0,"2937-4101-5371","dxgvt1mmntsgscokckgw0kokwc08gkkcskcwc48o","api.sandbox.weflubit.com/");


    //b->showSettings();

    b->getAccountStatus("test2",test_ptr);

    //b->getOrders("test3",test_ptr,"2012-01-01 00:01:00","");

    //b->getProducts("test4",test_ptr, "true", "2", "1", NULL );

    //b->getProductsFeed("test 5",test_ptr,"5340");

    //b->dispatchOrderByFlubitId("test_6",test_ptr,"12","2012-01-01T00:01:00","{\"courier\":\"Home\",\"consignmentNumber\":\"lcome\",\"trackingUrl\":\"http://o2.pl\"}");

    //b->dispatchOrderByMerchantOrderId("test_7",test_ptr,"12","2012-01-01T00:01:00","{\"courier\":\"Home\",\"consignmentNumber\":\"lcome\",\"trackingUrl\":\"http://o2.pl\"}");

    //b->cancelOrderByFlubitId("test_8",test_ptr,"12","some");

    //b->cancelOrderByMerchantOrderId("test_9",test_ptr,"13","some");

    //b->refundOrderByFlubitId("test_10",test_ptr,"1");

    //b->refundOrderByMerchantOrderId("test_11",test_ptr,"1");

    //b->updateProducts("test_12",test_ptr,"<xml></xml>");

    //b->createProducts("test_13",test_ptr,"<xml></xml>");



    return a.exec();
}
