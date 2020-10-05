#ifndef PUBLICFUNCTION_H
#define PUBLICFUNCTION_H

#include "PublicDef.h"



class PublicFunction
{
public:
    PublicFunction();


    static WFEM_HEADER getHeader(QString oStrFileName);

    static WFEM_HEADER pickHeader(QString oStrPreheader);

    static QDateTime getDateTime(uint uiSecs);

    static qint32 getNumber(QByteArray baLine);

    static QDateTime getDateTime(PHOENIX_HEADER oHeader);


    static PHOENIX_HEADER getHeader(PHOENIX_HEADER oHeader, quint64 uiSecs);

    static PHOENIX_HEADER getHeader(WFEM_HEADER oHeader);
};

#endif // PUBLICFUNCTION_H
