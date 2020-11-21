#ifndef PUBLICFUNCTION_H
#define PUBLICFUNCTION_H

#include "PublicDef.h"



class PublicFunction
{
public:
    PublicFunction();

    /* 首行信息得到广域header */
    static HEAD_WFEM getHeaderFromFileName(QString oStrFileName);

    /* 由首行string解析出首行信息，塞进结构体 */
    static HEAD_WFEM pickHeader(QString oStrPreheader);

    /* 由 UTC 秒 得到 date&time */
    static QDateTime getDateTimeFromUTC(uint uiSecs);

    /* 一行信息，得出qint32 数据 */
    static float getNumber(QString oStrLine);

    /* 从凤凰Header中获取到时间 */
    static QDateTime getDateTimeFromHeaderPhoenix(HEAD_PHOENIX oHeader);

    /* 原有的Phoenix header 追加秒数，的得到新的header */
    static HEAD_PHOENIX getHeaderFromAddSecs(HEAD_PHOENIX oHeader, quint64 uiSecs);

    /* 由已知的广域时间域文件的首行信息来完善凤凰的header */
    static HEAD_PHOENIX getHeaderFromHeaderWFEM(HEAD_WFEM oHeader);
};

#endif // PUBLICFUNCTION_H
