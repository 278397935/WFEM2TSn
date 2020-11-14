#ifndef PUBLICFUNCTION_H
#define PUBLICFUNCTION_H

#include "PublicDef.h"



class PublicFunction
{
public:
    PublicFunction();

    /* 首行信息得到广域header */
    static WFEM_HEADER getHeaderFromFileName(QString oStrFileName);

    /* 由首行string解析出首行信息，塞进结构体 */
    static WFEM_HEADER pickHeader(QString oStrPreheader);

    /* 由 UTC 秒 得到 date&time */
    static QDateTime getDateTimeFromUTC(uint uiSecs);

    /* 一行信息，得出qint32 数据 */
    static double getNumber(QString oStrLine);

    /* 从凤凰Header中获取到时间 */
    static QDateTime getDateTimeFromHeaderPhoenix(PHOENIX_HEADER oHeader);

    /* 原有的Phoenix header 追加秒数，的得到新的header */
    static PHOENIX_HEADER getHeaderFromAddSecs(PHOENIX_HEADER oHeader, quint64 uiSecs);

    /* 由已知的广域时间域文件的首行信息来完善凤凰的header */
    static PHOENIX_HEADER getHeaderFromHeaderWFEM(WFEM_HEADER oHeader);
};

#endif // PUBLICFUNCTION_H
