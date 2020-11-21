#ifndef TSNWORK_H
#define TSNWORK_H

#include <QObject>

#include "Common/PublicDef.h"
#include "Common/PublicFunction.h"

class TSnWork : public QObject
{
    Q_OBJECT
public:
    explicit TSnWork(QObject *parent = 0);

    /* 打开5个广域文件 */
    void openFiles();

    /* 关闭5个广域文件 */
    void closeFile();

    /* 判断文件是不是同一组 */
    bool judgeKind(QStringList aoStrFileName);

    /* 写记录头 */
    void writeHeader();

    /* 写5个通道的数据 */
    void writeScan(quint64 uiCnt);

    /* 空读，不做如何处理。只为满足Dr.Lee 的数据格式 */
    void skipRows(quint64 uiCnt);

    /* AMT TS2， 第一段、间隔采样，只存0.1s */
    void writeAMT_TS2(quint32 uiHSMP, quint32 uiL2NS);

    /* AMT TS3， 第二段、间隔采样 */
    void writeAMT_TS3(quint32 uiHSMP, quint32 uiL3NS);

    /* AMT TS4， 连续采样 */
    void writeAMT_TS4();

    /* MT TS3*/
    void writeMT_TS3(quint32 uiHSMP, quint32 uiL3NS);

    /* MT TS4， 连续采样 */
    void writeMT_TS4(quint32 uiHSMP, quint32 uiL4NS);

    /* MT TS4， 连续采样 */
    void writeMT_TS5();

    /*向二进制文件中写数据，每个数据是24bits */
    void writeNum(float number);

    /* 原有的Phoenix header 追加秒数，的得到新的header */
    void updateHeadPhoenix(quint64 uiSecs);

private:
    /* 通道&&文件名 对应上别弄混淆了 */
    QMap<quint32, QString> mapChFile;

    /* 广域文件头 */
    HEAD_WFEM goHeaderWFEM;

    /* 凤凰文件头 */
    HEAD_PHOENIX goHeadPhoenix;

    /* Phoenix file */
    FILE *pFile;

    /* 暂存一个文件名，到时用这个来修正凤凰文件名 */
    QString oStrFileNamePhoenix;

    /* 广域文件 */
    QFile oFileEx, oFileEy, oFileHx, oFileHy, oFileHz;

    QTextStream oStreamEx,oStreamEy,oStreamHx,oStreamHy,oStreamHz;

    quint64 uiSlicCnt = 0;

    quint64 uiSkipSecs = 0;

    quint64 uiAddSecs = 0;

private slots:
    void convert(AMTorMT eAMTorMT, TSn eTSn, quint32 uiHSMP, quint32 uiLXNS);

signals:
    void sigMsg(MSG_TYPE eType ,QString oStrMsg);

    void sigHEAD_WFEM(HEAD_WFEM oHeader);
};

#endif // TSNWORK_H
