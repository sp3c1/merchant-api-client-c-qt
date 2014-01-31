Flubit Merchant API C++

1. General Notes.
Qt 5.1 C++ adpaptation of Flubit Merchant API. Library follows Qt paradigms and asynchornous architecture.
Implementation is based on libraries Core & Network, using extensivley QtNetworkManager, QtNetworkRequest & QtNetworkReply.




2. Points of Interest
There are several places that looks different to the originall php implementation


2.1 Asynchronous Build [callback function]
As the Qt is asynchronous framework with any call function the pointer to callback function is supplied, which will be executed when request response is ready.
By default the callback should be returning void and taking QString with HTTP response as argument.

    typedef void (*callback_flubit)(QString);

This type definition can be ammended to need. If the additional arguments are needed that can be passed in replyFinished slot. Note, there can be many handling functions
and implementations, for example simpliest way

    void test_ptr(QString hola)
    {
        qDebug()<<"This is your Response : \n" << ola;
    };

2.2 Custom Call Identifier
As in the asynchronous workflow there might be many calls waiting for answer, user might want to pass his own text (QString) reference to call. If supplied empty string, library will generate
and give back the random md5 hashed id on call (otherwise it will return the string provided).

2.3 Request List Container
All the waiting request are passed to QList based on request_list class. Uppon reciving finished, callback is triggered. After that the entry is removed form list. All the objects are cleared with
propriate methods to ensure proper memory handling and events detachments. Reason is also, failed request are not cleared of the list. To see node structure see header request_list.h .

2.4. Handling Exceptions
As of historic (and some praticall) reasons the Qt is not fully following expections, instead it uses enum error codes (which means it wont crash on error).
If any additional handling of request is needed before callback, slot (replyFinished) function is the place to put the code in.

2.5. Passing Associative Arrays
While those could be handled as custom templates, its easier to handle array arguments with JSON notation. Please use following template

    "{\"courier\":\"DHL",\"consignmentNumber\":\"123",\"trackingUrl\":\"http://some.url.com\"}"
Class uses QJson Library.

2.6 Passing Dates
Please you do following format: "yyyy-mm-dd hh:mm:ss", it will automatically got translated to "yyyy-mm-dd\Thh:mm:ssO" based on calculated system timezone (of local machine).






4. Other Notes
Main library extends QObject, which on add (saying we go multithread) will require to clean all, run qmake, build all, then run. This is due to QObject macro creation.




5. Future Work
    - Unit testing
    - Possibly custom slot function can be additionally provided, to overide the replyFinish slot and make improvements handling (thou some initial checks and places of interest can be found in comments in code)
    - It would be quite easy to multihread this library, as the main object inherits from QObject.
    - Some of the methods could be overrided to handle more then QString, but its more to need of platform on which library might be used.

