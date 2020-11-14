#include "TSnWork.h"


TSnWork::TSnWork(QObject *parent) : QObject(parent)
{
    QThread *poThread = new QThread;
    this->moveToThread(poThread);
    poThread->start();

    pFile = nullptr;
}

/* 生成 5 个文件，广域时间域文件 */
void TSnWork::openFiles()
{
    /* CH1 */
    oFileCh1.setFileName(mapChFile.value(CH_1));
    oStreamCh1.setDevice(&oFileCh1);
    if(!oFileCh1.open(QIODevice::ReadOnly | QIODevice::Text)){
        emit sigMsg(MSG_Normal, "Can't open Ch1 file:"+oFileCh1.errorString());
    }else{
        oStreamCh1.setDevice(&oFileCh1);
        oStreamCh1.setCodec("utf-8");
    }

    /* CH2 */
    oFileCh2.setFileName(mapChFile.value(CH_2));
    oStreamCh2.setDevice(&oFileCh2);
    if(!oFileCh2.open(QIODevice::ReadOnly | QIODevice::Text)){
        emit sigMsg(MSG_Normal, "Can't open Ch2 file:"+oFileCh2.errorString());
    }else{
        oStreamCh2.setDevice(&oFileCh2);
        oStreamCh2.setCodec("utf-8");
    }

    /* CH3 */
    oFileCh3.setFileName(mapChFile.value(CH_3));
    oStreamCh3.setDevice(&oFileCh3);
    if(!oFileCh3.open(QIODevice::ReadOnly | QIODevice::Text)){
        emit sigMsg(MSG_Normal, "Can't open Ch3 file:"+oFileCh3.errorString());
    }else{
        oStreamCh3.setDevice(&oFileCh3);
        oStreamCh3.setCodec("utf-8");
    }

    /* CH4 */
    oFileCh4.setFileName(mapChFile.value(CH_4));
    oStreamCh4.setDevice(&oFileCh4);
    if(!oFileCh4.open(QIODevice::ReadOnly | QIODevice::Text)){
        emit sigMsg(MSG_Normal, "Can't open the file:"+oFileCh4.errorString());
    }else{
        oStreamCh4.setDevice(&oFileCh4);
        oStreamCh4.setCodec("utf-8");
    }

    /* CH5 */
    oFileCh5.setFileName(mapChFile.value(CH_5));
    oStreamCh5.setDevice(&oFileCh5);
    if(!oFileCh5.open(QIODevice::ReadOnly | QIODevice::Text)){
        emit sigMsg(MSG_Normal, "Can't open the file:"+oFileCh1.errorString());
    }else{
        oStreamCh5.setDevice(&oFileCh5);
        oStreamCh5.setCodec("utf-8");
    }

    /* 首行读取出来，发个UI 显示 */
    emit sigMsg(MSG_Normal,"首行信息 Ch1："+  oStreamCh1.readLine());
    emit sigMsg(MSG_Normal,"首行信息 Ch2："+  oStreamCh2.readLine());
    emit sigMsg(MSG_Normal,"首行信息 Ch3："+  oStreamCh3.readLine());
    emit sigMsg(MSG_Normal,"首行信息 Ch4："+  oStreamCh4.readLine());
    emit sigMsg(MSG_Normal,"首行信息 Ch5："+  oStreamCh5.readLine());
}

/* 5个通道的数据文件全部关闭 */
void TSnWork::closeFile()
{
    oFileCh1.flush();
    oFileCh2.flush();
    oFileCh3.flush();
    oFileCh4.flush();
    oFileCh5.flush();

    oFileCh1.close();
    oFileCh2.close();
    oFileCh3.close();
    oFileCh4.close();
    oFileCh5.close();
}

/* 判断， 所选的文件是不是
 * 同一台仪器(仪器号)
 * 同一个时刻开始()
 * 同一个采样率()
 * 同一个采样时长()
 * 放到单独的类里面去判断，不和UI类有关联
 */
bool TSnWork::judgeKind(QStringList aoStrFileName)
{
    QList<quint32> auiDevId;
    QList<uint> auiStartTime;
    QList<quint16> auiFS;
    QList<uint> quiSampleLength;

    /* */
    foreach(QString oStrFileName, aoStrFileName)
    {
        goHeaderWFEM = PublicFunction::getHeaderFromFileName(oStrFileName);

        goHeaderPhoenix = PublicFunction::getHeaderFromHeaderWFEM(goHeaderWFEM);

        emit sigWFEM_HEADER(goHeaderWFEM);

        mapChFile.insert(goHeaderWFEM.uiDevCh, oStrFileName);

        /*  */
        auiDevId.append(goHeaderWFEM.uiDevId);

        auiStartTime.append(goHeaderWFEM.uiStartTime);

        auiFS.append(goHeaderWFEM.uiFS);

        quiSampleLength.append(goHeaderWFEM.uiSampleLength);
    }

    /* 判断里面的项是不是只有一个(参数相同) */
    if( auiDevId.toSet().toList().count() > 1 ||
            auiStartTime.toSet().toList().count() > 1 ||
            auiFS.toSet().toList().count() > 1 ||
            quiSampleLength.toSet().toList().count() > 1 )
    {
        return false;
    }
    else
    {
        oStrFileNamePhoenix = aoStrFileName.first();
        return true;
    }
}

/* 写Phoenix记录头 */
void TSnWork::writeHeader(PHOENIX_HEADER oHeader)
{
    fwrite(&oHeader.uiSec,1, 1, pFile);
    fwrite(&oHeader.uiMin,1, 1, pFile);
    fwrite(&oHeader.uiHour,1, 1,pFile);
    fwrite(&oHeader.uiDay,1, 1, pFile);
    fwrite(&oHeader.uiMonth,1, 1, pFile);
    fwrite(&oHeader.uiYear,1, 1, pFile);
    fwrite(&oHeader.uiDayOfWeek,1, 1, pFile);
    fwrite(&oHeader.uiCentury,1, 1, pFile);

    fwrite(&oHeader.uiDevSN,2, 1, pFile);
    fwrite(&oHeader.uiSampleCntPerRecordPerCh,2, 1, pFile);
    fwrite(&oHeader.uiChCntPerScan,1, 1, pFile);
    fwrite(&oHeader.uiRecoedHeaderLength,1, 1, pFile);
    fwrite(&oHeader.uiRecStatus,1, 1, pFile);
    fwrite(&oHeader.uiBitSaturationCode,2, 1, pFile);
    fwrite(&oHeader.uiBitWidth,1, 1, pFile);
    fwrite(&oHeader.uiFS,2, 1, pFile);
    fwrite(&oHeader.uiFSUnit,1, 1, pFile);
    fwrite(&oHeader.uiClockStatus,1, 1, pFile);

    fwrite(&oHeader.iClockErr,4, 1, pFile);

    fwrite(oHeader.acReserved, 6, 1, pFile);

    QString oStrPhoenixHeader = QString("%1世纪   %2/%3/%4/ %5:%6:%7 星期%8")
            .arg(oHeader.uiCentury)
            .arg(oHeader.uiYear)
            .arg(oHeader.uiMonth)
            .arg(oHeader.uiDay)
            .arg(oHeader.uiHour)
            .arg(oHeader.uiMin)
            .arg(oHeader.uiSec)
            .arg(oHeader.uiDayOfWeek);

    //emit sigMsg(MSG_Normal, "写Phoenix记录头时间: \n"+oStrPhoenixHeader);

    qDebugV0()<<"写Phoenix记录头时间: "+oStrPhoenixHeader;
}

/* 写32位的有符号整数 */
void TSnWork::writeScan(quint64 uiCnt)
{
    emit sigMsg(MSG_Normal, QString("写%1行")
                .arg(uiCnt));

    /* 从广域时间域文件中读取浮点数 */
    for(quint64 i = 0; i < uiCnt; i++)
    {
        this->writeNum(PublicFunction::getNumber(oStreamCh1.readLine()));
        this->writeNum(PublicFunction::getNumber(oStreamCh2.readLine()));
        this->writeNum(PublicFunction::getNumber(oStreamCh3.readLine()));
        this->writeNum(PublicFunction::getNumber(oStreamCh4.readLine()));
        this->writeNum(PublicFunction::getNumber(oStreamCh5.readLine()));
    }
}

/* QDataStream readLine() */
void TSnWork::readNothing(quint64 uiCnt)
{   
    emit sigMsg(MSG_Normal, QString("跳%1行")
                .arg(uiCnt));

    for(quint64 i = 0; i < uiCnt; i++)
    {
        /* 从广域时间域文件中读取浮点数 */
        oStreamCh1.readLine();
        oStreamCh2.readLine();
        oStreamCh3.readLine();
        oStreamCh4.readLine();
        oStreamCh5.readLine();
    }
}

/* AMT TS2， 间隔采样，只存0.1s */
void TSnWork::writeAMT_TS2(quint64 uiSlotCnt)
{
    /* 采样时间小于采样间隔， 错误！❌ */
    if(goHeaderWFEM.uiSampleLength <goHeaderWFEM.uiSlicBase)
    {
        emit sigMsg(MSG_Err, "❌,采样间隔比采样时间长！");
        return;
    }
    /* 第一段留着，第二段舍弃 */
    for(int i = 0; i < uiSlotCnt; i++)
    {
        /* 偶数、奇数 */
        if(i%2 == 0)
        {
            for(int j = 0; j <  goHeaderWFEM.uiSliceSample; j++)
            {
                PHOENIX_HEADER oHeaderNew = PublicFunction::getHeaderFromAddSecs(goHeaderPhoenix,
                                                                                 goHeaderWFEM.uiSlicBase*i + j);

                this->writeHeader(oHeaderNew);

                /* 写0.1 跳0.9，保证1s的数据掠过去了 */
                this->writeScan(goHeaderPhoenix.uiFS*0.1);
                this->readNothing(goHeaderWFEM.uiFS*0.9);
            }
            /* 保留的那个片段，要舍弃 */
            this->readNothing(goHeaderWFEM.uiSlicBase*goHeaderWFEM.uiFS -
                              goHeaderWFEM.uiSliceSample*goHeaderWFEM.uiFS);
        }
        else
        {
            this->readNothing(goHeaderWFEM.uiSliceSample);
        }

        emit sigMsg(MSG_Normal,QString("AMT TS2 间隔采样\u058E %1/%2")
                    .arg(i+1)
                    .arg(uiSlotCnt));
    }

    fclose(pFile);
    emit sigMsg(MSG_Over,"AMT TS2 间隔采样写入完毕！");
}

void TSnWork::writeAMT_TS3(quint64 uiSlotCnt)
{
    /* 采样时间小于采样间隔， 错误！❌ */
    if(goHeaderWFEM.uiSampleLength <goHeaderWFEM.uiSlicBase)
    {
        emit sigMsg(MSG_Err, "❌,采样间隔比采样时间长！");
        return;
    }

    for(int i = 0; i < uiSlotCnt; i++)
    {
        if(i%2 == 0)//第一片，skip!
        {
            this->readNothing(goHeaderWFEM.uiSlicBase*goHeaderWFEM.uiFS);
        }
        else//第二片，write
        {
            /* 记录 */
            for(int j = 0; j < goHeaderWFEM.uiSliceSample; j++)
            {
                PHOENIX_HEADER oHeaderNew = PublicFunction::getHeaderFromAddSecs(goHeaderPhoenix,
                                                                                 goHeaderWFEM.uiSlicBase*i+j);

                this->writeHeader(oHeaderNew);
                this->writeScan(goHeaderPhoenix.uiFS);
            }

            this->readNothing(goHeaderWFEM.uiSlicBase*goHeaderWFEM.uiFS -
                              goHeaderWFEM.uiSliceSample*goHeaderWFEM.uiFS);
        }

        emit sigMsg(MSG_Normal, "=====\n");
    }

    fclose(pFile);

    emit sigMsg(MSG_Over, "AMT TS3 间隔采样写入完毕！");
}

void TSnWork::writeMT_TS3(quint64 uiSlotCnt)
{
    /* 采样时间小于采样间隔， 错误！❌ */
    if(goHeaderWFEM.uiSampleLength <goHeaderWFEM.uiSlicBase)
    {
        emit sigMsg(MSG_Err, "❌,采样间隔比采样时间长！");
        return;
    }

    for(int i = 0; i < uiSlotCnt; i++)
    {
        if(i%2 == 0)//第一片，skip!
        {
            this->readNothing(goHeaderWFEM.uiSlicBase*goHeaderWFEM.uiFS);
        }
        else//第二片，write
        {
            /* 记录 */
            for(int j = 0; j < goHeaderWFEM.uiSliceSample; j++)
            {
                PHOENIX_HEADER oHeaderNew = PublicFunction::getHeaderFromAddSecs(goHeaderPhoenix,
                                                                                 goHeaderWFEM.uiSlicBase*i+j);

                this->writeHeader(oHeaderNew);
                this->writeScan(goHeaderPhoenix.uiFS);
            }

            this->readNothing(goHeaderWFEM.uiSlicBase*goHeaderWFEM.uiFS -
                              goHeaderWFEM.uiSliceSample*goHeaderWFEM.uiFS);
        }

        emit sigMsg(MSG_Normal, "=====\n");
    }

    fclose(pFile);

    emit sigMsg(MSG_Over, "MT TS3 间隔采样写入完毕！");
}

void TSnWork::writeMT_TS4(quint64 uiSlotCnt)
{
    /* 采样时间小于采样间隔， 错误！❌ */
    if(goHeaderWFEM.uiSampleLength <goHeaderWFEM.uiSlicBase)
    {
        emit sigMsg(MSG_Err, "❌,采样间隔比采样时间长！");
        return;
    }

    for(int i = 0; i < uiSlotCnt; i++)
    {
        this->readNothing(goHeaderWFEM.uiSliceSample);

        /* 奇数个slot，读进去指定的n个记录 */
        for(int j = 0; j < goHeaderWFEM.uiSliceSample; j++)
        {
            /* 更新记录头的时间，写记录头 */
            PHOENIX_HEADER oHeaderNew = PublicFunction::getHeaderFromAddSecs(goHeaderPhoenix,
                                                                             goHeaderWFEM.uiSlicBase*2*i + j);

            this->writeHeader(oHeaderNew);


            this->writeScan(goHeaderPhoenix.uiSampleCntPerRecordPerCh*0.1);


            emit sigMsg(MSG_Normal ,QString("间隔采样\u058E：%1/%2, %3/%4")
                        .arg(i/2+1)
                        .arg(uiSlotCnt/2)
                        .arg(j)
                        .arg(goHeaderWFEM.uiSliceSample));
        }
    }

    fclose(pFile);

    emit sigMsg(MSG_Over, "间隔采样写入完毕！");
}

void TSnWork::writeMT_TS5()
{
    for(int i = 0; i < goHeaderWFEM.uiSampleLength; i++)
    {
        /* 更新记录头的时间，写记录头 */
        PHOENIX_HEADER oHeaderNew = PublicFunction::getHeaderFromAddSecs(goHeaderPhoenix, i);

        /* 朝TSn文件中写phoenix 的 Header */
        this->writeHeader(oHeaderNew);

        this->writeScan(goHeaderPhoenix.uiSampleCntPerRecordPerCh);

        emit sigMsg(MSG_Normal, QString("MT TS5 连续采样\u058E%1/%2(写入记录数/总记录数)")
                    .arg(i+1)
                    .arg(goHeaderWFEM.uiSampleLength));
    }

    fclose(pFile);

    emit sigMsg(MSG_Over,"MT TS5 连续采样写入完毕！");
}

/*向二进制文件中写数据，每个数据是24bits */
void TSnWork::writeNum(double number)
{
    unsigned char high;
    unsigned char middle;
    unsigned char low;

    char buffer[3];

    unsigned int intNumber;

    if(number >= 0)
    {
        intNumber = int(abs(floor(number)));
    }
    else
    {
        intNumber = ~int(abs(floor(number))) + 1;
    }

    high   = intNumber/65536;
    middle = (intNumber%65536)/256;
    low    = intNumber%256;

    buffer[0] = low;
    buffer[1] = middle;
    buffer[2] = high;

    fwrite(buffer, BIT_WIDTH, 1, pFile);
}

void TSnWork::writeAMT_TS4()
{
    for(int i = 0; i < goHeaderWFEM.uiSampleLength; i++)
    {
        /* 更新记录头的时间，写记录头 */
        PHOENIX_HEADER oHeaderNew = PublicFunction::getHeaderFromAddSecs(goHeaderPhoenix, i);

        /* 朝TSn文件中写phoenix 的 Header */
        this->writeHeader(oHeaderNew);

        this->writeScan(goHeaderPhoenix.uiSampleCntPerRecordPerCh);

        emit sigMsg(MSG_Normal, QString("AMT TS4 连续采样\u058E%1/%2(写入记录数/总记录数)")
                    .arg(i+1)
                    .arg(goHeaderWFEM.uiSampleLength));
    }

    fclose(pFile);

    emit sigMsg(MSG_Over,"AMT TS4 连续采样写入完毕！");
}


/* 转换函数 */
void TSnWork::convert(AMTorMT eAMTorMT, TSn eTSn, quint32 uiSlicBase, quint32 uiSlicSample)
{
    goHeaderWFEM.uiSlicBase = uiSlicBase;
    goHeaderWFEM.uiSliceSample = uiSlicSample;

    qDebugV0()<<"Convert Thread ID:"<<QThread::currentThreadId();

    /* 凤凰格式的文件 */
    QFileInfo oFileInfo(oStrFileNamePhoenix);

    QString oStrFileName = QString("%1/%2.TS%3")
            .arg(oFileInfo.absolutePath())
            .arg(goHeaderWFEM.uiDevId)
            .arg(eTSn);

    emit sigMsg(MSG_Normal,"生成Phoenix文件名："+ oStrFileName);

    char* poFileName;
    QByteArray ba = oStrFileName.toLatin1();
    poFileName = ba.data();

    pFile = fopen( poFileName, "ab+" );

    if(pFile == nullptr)
    {
        emit sigMsg(MSG_Err, "err");
        return;
    }

    this->openFiles();

    /* 多少个slot */

    emit sigMsg(MSG_Normal, QString("数据长度：%1\n"
                                    "分段长度：%2")
                .arg(goHeaderWFEM.uiSampleLength)
                .arg(goHeaderWFEM.uiSlicBase));

    quint64 uiSlotCnt = goHeaderWFEM.uiSampleLength/goHeaderWFEM.uiSlicBase;

    emit sigMsg(MSG_Normal,QString("采样时长：%1s\n"
                                   "采样率：%2Hz\n"
                                   "片段长度：%3\n"
                                   "片段个数：%4\n"
                                   "记录数：%5\n")
                .arg(goHeaderWFEM.uiSampleLength)
                .arg(goHeaderWFEM.uiFS)
                .arg(goHeaderWFEM.uiSlicBase)
                .arg(uiSlotCnt)
                .arg(uiSlicSample));

    qDebugV0()<<"test:"<<QString("方法：%1,TSn：%2")
                .arg(eAMTorMT)
                .arg(eTSn);

    switch (eAMTorMT) {
    case AMT:
        switch (eTSn) {
        case TS2:
            this->writeAMT_TS2(uiSlotCnt);
            break;
        case TS3:
            this->writeAMT_TS3(uiSlotCnt);
            break;
        case TS4:
            this->writeAMT_TS4();
            break;
        default:
            break;
        }

        break;

    case MT:
        switch (eTSn) {
        case TS3:
            this->writeMT_TS3(uiSlotCnt);
            break;
        case TS4:
            this->writeMT_TS4(uiSlotCnt);
            break;
        case TS5:
            this->writeMT_TS5();
            break;
        default:
            break;
        }
        break;

    default:
        break;
    }

    this->closeFile();
}
