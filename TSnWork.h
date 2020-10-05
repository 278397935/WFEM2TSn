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

    void openFiles();

    void closeFile();

    bool judgeKind(QStringList aoStrFileName);

    void writeHeader(PHOENIX_HEADER oHeader);



    void writeScan();

    /* 空读，不做如何处理。只为满足Dr.Lee 的数据格式 */
    void readNothing(quint64 uiCnt);

    /* AMT TS2， 间隔采样，只存0.1s */
    void writeTS2(quint64 uiSlotCnt);

    /* AMT TS3 & MT TS3 、TS4 间隔采样，存1s */
    void writeGap(quint64 uiSlotCnt);

    /* AMT TS4 & MT TS5, 连续采集 */
    void writeNoGap();

private:
    QMap<quint8,QString> mapChFile;

    WFEM_HEADER goHeaderWFEM;

    PHOENIX_HEADER goHeaderPhoenix;

    /* Phoenix file */
    FILE *pFile;

    QString oStrPhoenixFileName;

    QFile oFileCh1, oFileCh2, oFileCh3, oFileCh4, oFileCh5;

private slots:
    void convert(AMTorMT eAMTorMT, TSn eTSn, quint32 uiSlicBase, quint32 uiSlicSample);


signals:
    void sigMsg(QString oStrMsg);

    void sigWFEM_HEADER(WFEM_HEADER oHeader);

};

#endif // TSNWORK_H
