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
    void writeHeader(PHOENIX_HEADER oHeader);

    /* 写5个通道的数据 */
    void writeScan(quint64 uiCnt);

    /* 空读，不做如何处理。只为满足Dr.Lee 的数据格式 */
    void readNothing(quint64 uiCnt);

    /* AMT TS2， 第一段、间隔采样，只存0.1s */
    void writeAMT_TS2(quint64 uiSlotCnt);

    /* AMT TS3， 第二段、间隔采样 */
    void writeAMT_TS3(quint64 uiSlotCnt);

    /* AMT TS4， 连续采样 */
    void writeAMT_TS4();

    /* MT TS3*/
    void writeMT_TS3(quint64 uiSlotCnt);

    /* MT TS4， 连续采样 */
    void writeMT_TS4(quint64 uiSlotCnt);

    /* MT TS4， 连续采样 */
    void writeMT_TS5();

    /*向二进制文件中写数据，每个数据是24bits */
    void writeNum(double number);

private:
    /* 通道&&文件名 对应上别弄混淆了 */
    QMap<quint8,QString> mapChFile;

    /* 广域文件头 */
    WFEM_HEADER goHeaderWFEM;

    /* 凤凰文件头 */
    PHOENIX_HEADER goHeaderPhoenix;

    /* Phoenix file */
    FILE *pFile;

    /* 暂存一个文件名，到时用这个来修正凤凰文件名 */
    QString oStrFileNamePhoenix;

    /* 广域文件 */
    QFile oFileCh1, oFileCh2, oFileCh3, oFileCh4, oFileCh5;

    QTextStream oStreamCh1,oStreamCh2,oStreamCh3,oStreamCh4,oStreamCh5;

private slots:
    void convert(AMTorMT eAMTorMT, TSn eTSn, quint32 uiSlicBase, quint32 uiSlicSample);

signals:
    void sigMsg(MSG_TYPE eType ,QString oStrMsg);

    void sigWFEM_HEADER(WFEM_HEADER oHeader);
};

#endif // TSNWORK_H
