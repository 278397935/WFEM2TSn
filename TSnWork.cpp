#include "TSnWork.h"


TSnWork::TSnWork(QObject *parent) : QObject(parent)
{
    QThread *poThread = new QThread;
    this->moveToThread(poThread);
    poThread->start();

    pFile = nullptr;

    uiSlicCnt = 0;

    uiSkipSecs = 0;

    uiAddSecs = 0;
}

/* 生成 5 个文件，广域时间域文件 */
void TSnWork::openFiles()
{
    /* Ex */qDebugV0()<<mapChFile.value(CH_Ex);

    oFileEx.setFileName(mapChFile.value(CH_Ex));
    oStreamEx.setDevice(&oFileEx);
    if(!oFileEx.open(QIODevice::ReadOnly | QIODevice::Text)){
        emit sigMsg(MSG_Normal, "Can't open Ex file:"+oFileEx.errorString());
    }else{
        oStreamEx.setDevice(&oFileEx);
        oStreamEx.setCodec("utf-8");
    }

    /* Ey */
    oFileEy.setFileName(mapChFile.value(CH_Ey));
    oStreamEy.setDevice(&oFileEy);
    if(!oFileEy.open(QIODevice::ReadOnly | QIODevice::Text)){
        emit sigMsg(MSG_Normal, "Can't open Ey file:"+oFileEy.errorString());
    }else{
        oStreamEy.setDevice(&oFileEy);
        oStreamEy.setCodec("utf-8");
    }

    /* Hx */
    oFileHx.setFileName(mapChFile.value(CH_Hx));
    oStreamHx.setDevice(&oFileHx);
    if(!oFileHx.open(QIODevice::ReadOnly | QIODevice::Text)){
        emit sigMsg(MSG_Normal, "Can't open Hx file:"+oFileHx.errorString());
    }else{
        oStreamHx.setDevice(&oFileHx);
        oStreamHx.setCodec("utf-8");
    }

    /* Hy */
    oFileHy.setFileName(mapChFile.value(CH_Hy));
    oStreamHy.setDevice(&oFileHy);
    if(!oFileHy.open(QIODevice::ReadOnly | QIODevice::Text)){
        emit sigMsg(MSG_Normal, "Can't open Hy file:"+oFileHy.errorString());
    }else{
        oStreamHy.setDevice(&oFileHy);
        oStreamHy.setCodec("utf-8");
    }

    /* Hz */
    oFileHz.setFileName(mapChFile.value(CH_Hz));
    oStreamHz.setDevice(&oFileHz);
    if(!oFileHz.open(QIODevice::ReadOnly | QIODevice::Text)){
        emit sigMsg(MSG_Normal, "Can't open Hz file:"+oFileHz.errorString());
    }else{
        oStreamHz.setDevice(&oFileHz);
        oStreamHz.setCodec("utf-8");
    }

    /* 首行读取出来，发个UI 显示 */
    emit sigMsg(MSG_Normal,"首行信息 Ch1："+  oStreamEx.readLine());
    emit sigMsg(MSG_Normal,"首行信息 Ch2："+  oStreamEy.readLine());
    emit sigMsg(MSG_Normal,"首行信息 Ch3："+  oStreamHx.readLine());
    emit sigMsg(MSG_Normal,"首行信息 Ch4："+  oStreamHy.readLine());
    emit sigMsg(MSG_Normal,"首行信息 Ch5："+  oStreamHz.readLine());
}

/* 5个通道的数据文件全部关闭 */
void TSnWork::closeFile()
{
    oFileEx.flush();
    oFileEy.flush();
    oFileHx.flush();
    oFileHy.flush();
    oFileHz.flush();

    oFileEx.close();
    oFileEy.close();
    oFileHx.close();
    oFileHy.close();
    oFileHz.close();
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

        goHeadPhoenix = PublicFunction::getHeaderFromHeaderWFEM(goHeaderWFEM);

        emit sigHEAD_WFEM(goHeaderWFEM);

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
void TSnWork::writeHeader()
{
    fwrite(&goHeadPhoenix.uiSec,1, 1, pFile);
    fwrite(&goHeadPhoenix.uiMin,1, 1, pFile);
    fwrite(&goHeadPhoenix.uiHour,1, 1,pFile);
    fwrite(&goHeadPhoenix.uiDay,1, 1, pFile);
    fwrite(&goHeadPhoenix.uiMonth,1, 1, pFile);
    fwrite(&goHeadPhoenix.uiYear,1, 1, pFile);
    fwrite(&goHeadPhoenix.uiDayOfWeek,1, 1, pFile);
    fwrite(&goHeadPhoenix.uiCentury,1, 1, pFile);

    fwrite(&goHeadPhoenix.uiDevSN,2, 1, pFile);
    fwrite(&goHeadPhoenix.uiSampleCntPerRecordPerCh,2, 1, pFile);
    fwrite(&goHeadPhoenix.uiChCntPerScan,1, 1, pFile);
    fwrite(&goHeadPhoenix.uiRecoedHeaderLength,1, 1, pFile);
    fwrite(&goHeadPhoenix.uiRecStatus,1, 1, pFile);
    fwrite(&goHeadPhoenix.uiBitSaturationCode,2, 1, pFile);
    fwrite(&goHeadPhoenix.uiBitWidth,1, 1, pFile);
    fwrite(&goHeadPhoenix.uiFS,2, 1, pFile);
    fwrite(&goHeadPhoenix.uiFSUnit,1, 1, pFile);
    fwrite(&goHeadPhoenix.uiClockStatus,1, 1, pFile);

    fwrite(&goHeadPhoenix.iClockErr,4, 1, pFile);

    fwrite(goHeadPhoenix.acReserved, 6, 1, pFile);

    QString oStrPhoenixHeader = QString("%1世纪   %2/%3/%4/ %5:%6:%7 星期%8")
            .arg(goHeadPhoenix.uiCentury)
            .arg(goHeadPhoenix.uiYear)
            .arg(goHeadPhoenix.uiMonth)
            .arg(goHeadPhoenix.uiDay)
            .arg(goHeadPhoenix.uiHour)
            .arg(goHeadPhoenix.uiMin)
            .arg(goHeadPhoenix.uiSec)
            .arg(goHeadPhoenix.uiDayOfWeek);

    emit sigMsg(MSG_Normal, "写Phoenix记录头时间: "+ oStrPhoenixHeader);
}

/* 写32位的有符号整数 */
void TSnWork::writeScan(quint64 uiCnt)
{
    emit sigMsg(MSG_Normal, QString("写%1个数据")
                .arg(uiCnt));

    /* 从广域时间域文件中读取浮点数 */
    for(quint64 i = 0; i < uiCnt; i++)
    {
        this->writeNum(PublicFunction::getNumber(oStreamEx.readLine()));
        this->writeNum(PublicFunction::getNumber(oStreamEy.readLine()));
        this->writeNum(PublicFunction::getNumber(oStreamHx.readLine()));
        this->writeNum(PublicFunction::getNumber(oStreamHy.readLine()));
        this->writeNum(PublicFunction::getNumber(oStreamHz.readLine()));
    }
}

/* QDataStream readLine() ， 空读，不做任何操作 */
void TSnWork::skipRows(quint64 uiCnt)
{
    emit sigMsg(MSG_Normal, QString("弃%1个数据")
                .arg(uiCnt));

    for(quint64 i = 0; i <uiCnt; i++)
    {
        /* 从广域时间域文件中读取浮点数 */
        oStreamEx.readLine();
        oStreamEy.readLine();
        oStreamHx.readLine();
        oStreamHy.readLine();
        oStreamHz.readLine();
    }
}

/* AMT TS2， 间隔采样，只存0.1s */
void TSnWork::writeAMT_TS2(quint32 uiHSMP, quint32 uiL2NS)
{
    /* 区别对待广域设备连续采集和间隔采集 */
    if(goHeaderWFEM.uiSlicBase == goHeaderWFEM.uiSliceSample)
    {
        uiSlicCnt = goHeaderWFEM.uiDataCnt/uiHSMP/goHeaderWFEM.uiFS/2;
        uiSkipSecs = 2*uiHSMP - uiL2NS ;
        uiAddSecs =  2*uiHSMP - uiL2NS;
    }
    else
    {
        uiSlicCnt = goHeaderWFEM.uiDataCnt/goHeaderWFEM.uiSliceSample/goHeaderWFEM.uiFS/2;
        uiSkipSecs = 2*goHeaderWFEM.uiSliceSample - uiL2NS;
        uiAddSecs =  2*goHeaderWFEM.uiSlicBase - uiL2NS ;
    }

    for(quint64 i = 0; i < uiSlicCnt; i++)
    {
        for(quint32 j = 0; j < uiL2NS; j++)
        {
            this->writeHeader();

            this->writeScan( goHeaderWFEM.uiFS/10 );
            this->skipRows( goHeaderWFEM.uiFS - goHeaderWFEM.uiFS/10 );

            this->updateTime( 1 );

            emit sigMsg(MSG_Normal, QString("AMT TS2 间隔采样，\u058E记录：%1/%2\u058E片段：%3/%4 \n")
                        .arg(j+1)
                        .arg(uiL2NS)
                        .arg(i+1)
                        .arg(uiSlicCnt));
        }
        this->skipRows( goHeaderWFEM.uiFS*uiSkipSecs );
        this->updateTime( uiAddSecs );
    }

    fflush(pFile);
    fclose(pFile);

    emit sigMsg(MSG_Over, "AMT TS2 间隔采样写入完毕！");
}

/* 间隔采样， 第一个片段丢弃，第二个片段留着指定的 L3NS record个数 */
void TSnWork::writeAMT_TS3(quint32 uiHSMP, quint32 uiL3NS)
{
        /* 区别对待广域设备连续采集和间隔采集 */
    if(goHeaderWFEM.uiSlicBase == goHeaderWFEM.uiSliceSample)
    {
        uiSlicCnt = goHeaderWFEM.uiDataCnt/uiHSMP/goHeaderWFEM.uiFS/2;
        uiSkipSecs = 2*uiHSMP - uiL3NS ;
        uiAddSecs =  2*uiHSMP - uiL3NS;
    }
    else
    {
        uiSlicCnt = goHeaderWFEM.uiDataCnt/goHeaderWFEM.uiSliceSample/goHeaderWFEM.uiFS/2;
        uiSkipSecs = 2*goHeaderWFEM.uiSliceSample - uiL3NS;
        uiAddSecs =  2*goHeaderWFEM.uiSlicBase - uiL3NS ;
    }

    for(quint64 i = 0; i < uiSlicCnt; i++)
    {
        for(quint32 j = 0; j < uiL3NS; j++)
        {
            this->writeHeader();

            this->writeScan( goHeaderWFEM.uiFS );

            this->updateTime( 1 );

            emit sigMsg(MSG_Normal, QString("AMT TS3 间隔采样，\u058E记录：%1/%2\u058E片段：%3/%4 \n")
                        .arg(j+1)
                        .arg(uiL3NS)
                        .arg(i+1)
                        .arg(uiSlicCnt));
        }
        this->skipRows( goHeaderWFEM.uiFS*uiSkipSecs );
        this->updateTime( uiAddSecs );
    }

    fclose(pFile);

    emit sigMsg(MSG_Over, "AMT TS3 间隔采样写入完毕！");
}

/* 连续采样 */
void TSnWork::writeAMT_TS4()
{
    quint64 uiSamplingTimeLength = goHeaderWFEM.uiDataCnt/goHeaderWFEM.uiFS;

    qDebugV0()<<uiSamplingTimeLength<< goHeaderWFEM.uiDataCnt<<goHeaderWFEM.uiFS;

    for(quint64 i = 0; i < uiSamplingTimeLength; i++)
    {
        /* 朝TSn文件中写phoenix 的 Header */
        this->writeHeader();

        this->writeScan( goHeaderWFEM.uiFS );
        this->updateTime(1);

        emit sigMsg(MSG_Normal, QString("AMT TS4 连续采样\u058E%1/%2(写入记录数/总记录数)")
                    .arg(i+1)
                    .arg(uiSamplingTimeLength));
    }

    fclose(pFile);

    emit sigMsg(MSG_Over,"AMT TS4 连续采样写入完毕！");
}

/* MT TS3 L3NS */
void TSnWork::writeMT_TS3(quint32 uiHSMP, quint32 uiL3NS)
{
       /* 区别对待广域设备连续采集和间隔采集 */
    if(goHeaderWFEM.uiSlicBase == goHeaderWFEM.uiSliceSample)
    {
        uiSlicCnt = goHeaderWFEM.uiDataCnt/uiHSMP/goHeaderWFEM.uiFS/2;
        uiSkipSecs = 2*uiHSMP - uiL3NS ;
        uiAddSecs =  2*uiHSMP - uiL3NS;
    }
    else
    {
        uiSlicCnt = goHeaderWFEM.uiDataCnt/goHeaderWFEM.uiSliceSample/goHeaderWFEM.uiFS/2;
        uiSkipSecs = 2*goHeaderWFEM.uiSliceSample - uiL3NS;
        uiAddSecs =  2*goHeaderWFEM.uiSlicBase - uiL3NS ;
    }

    for(quint64 i = 0; i < uiSlicCnt; i++)
    {
        for(quint32 j = 0; j < uiL3NS; j++)
        {
            this->writeHeader();
            this->writeScan( goHeaderWFEM.uiFS );
            this->updateTime( 1 );

            emit sigMsg(MSG_Normal, QString("MT TS3 间隔采样，\u058E记录：%1/%2\u058E片段：%3/%4\n")
                        .arg(j+1)
                        .arg(uiL3NS)
                        .arg(i+1)
                        .arg(uiSlicCnt));
        }
        this->skipRows( goHeaderWFEM.uiFS*uiSkipSecs );
        this->updateTime( uiAddSecs );
    }

    fclose(pFile);

    emit sigMsg(MSG_Over, "MT TS3 间隔采样写入完毕！");
}

/* MT TS4 L4NS */
void TSnWork::writeMT_TS4(quint32 uiHSMP, quint32 uiL4NS)
{
       /* 区别对待广域设备连续采集和间隔采集 */
    if(goHeaderWFEM.uiSlicBase == goHeaderWFEM.uiSliceSample)
    {
        uiSlicCnt = goHeaderWFEM.uiDataCnt/uiHSMP/goHeaderWFEM.uiFS/2;
        uiSkipSecs = 2*uiHSMP - uiL4NS ;
        uiAddSecs =  2*uiHSMP - uiL4NS;
    }
    else
    {
        uiSlicCnt = goHeaderWFEM.uiDataCnt/goHeaderWFEM.uiSliceSample/goHeaderWFEM.uiFS/2;
        uiSkipSecs = 2*goHeaderWFEM.uiSliceSample - uiL4NS;
        uiAddSecs =  2*goHeaderWFEM.uiSlicBase - uiL4NS ;
    }

    for(quint64 i = 0; i < uiSlicCnt; i++)
    {
        for(quint32 j = 0; j < uiL4NS; j++)
        {
            this->writeHeader();
            this->writeScan( goHeaderWFEM.uiFS );
            this->updateTime( 1 );

            emit sigMsg(MSG_Normal, QString("AMT TS4 间隔采样，\u058E记录：%1/%2\u058E片段：%1/%2\n")
                        .arg(j+1)
                        .arg(uiL4NS)
                        .arg(i+1)
                        .arg(uiSlicCnt));
        }
        this->skipRows( goHeaderWFEM.uiFS*uiSkipSecs );
        this->updateTime( uiAddSecs );
    }

    fclose(pFile);

    emit sigMsg(MSG_Over, "AMT TS2 间隔采样写入完毕！");
}

/* 连续采样 */
void TSnWork::writeMT_TS5()
{
    quint64 uiSamplingTimeLength = goHeaderWFEM.uiDataCnt/goHeaderWFEM.uiFS;

    for(quint64 i = 0; i < uiSamplingTimeLength; i++)
    {
        /* 朝TSn文件中写phoenix 的 Header */
        this->writeHeader();
        this->writeScan( goHeaderWFEM.uiFS );
        this->updateTime(1);

        emit sigMsg(MSG_Normal, QString("MT TS5 连续采样\u058E%1/%2(写入记录数/总记录数)")
                    .arg(i+1)
                    .arg(uiSamplingTimeLength));
    }

    fclose(pFile);

    emit sigMsg(MSG_Over,"MT TS5 连续采样写入完毕！");
}

/*向二进制文件中写数据，每个数据是24bits */
void TSnWork::writeNum(float value)
{
    unsigned char high;
    unsigned char middle;
    unsigned char low;

    unsigned char buffer[3];

    unsigned int intNumber;

    float number = value*1000;//微伏

    if(number >= 0)
    {
        intNumber = int(std::abs(floor(number)));
    }
    else
    {
        intNumber = ~int(std::abs(floor(number))) + 1;
    }

    high   = intNumber/65536;
    middle = (intNumber%65536)/256;
    low    = intNumber%256;

    buffer[0] = low;
    buffer[1] = middle;
    buffer[2] = high;

    fwrite(buffer, BIT_WIDTH, 1, pFile);
}

/* 转换函数 */
void TSnWork::convert(AMTorMT eAMTorMT, TSn eTSn, quint32 uiHSMP, quint32 uiLXNS)
{
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

    pFile = fopen( poFileName, "wb" );

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
                                   "采样率：%2sps\n"
                                   "片段长度：%3\n"
                                   "片段个数：%4\n"
                                   "记录数：%5\n")
                .arg(goHeaderWFEM.uiSampleLength)
                .arg(goHeaderWFEM.uiFS)
                .arg(goHeaderWFEM.uiSlicBase)
                .arg(uiSlotCnt)
                .arg(uiLXNS));

    qDebugV0()<<"test:"<<QString("方法：%1,TSn：%2")
                .arg(eAMTorMT)
                .arg(eTSn);

    switch (eAMTorMT) {
    case AMT:
        switch (eTSn) {
        case TS2:
            this->writeAMT_TS2(uiHSMP, uiLXNS);
            break;
        case TS3:
            this->writeAMT_TS3(uiHSMP, uiLXNS);
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
            this->writeMT_TS3(uiHSMP, uiLXNS);
            break;
        case TS4:
            this->writeMT_TS4(uiHSMP, uiLXNS);
            break;
        case TS5:
            this->writeMT_TS5();
            break;
        default:
            break;
        }
        break;
    }

    this->closeFile();
}


/* 更新哈时间 */
void TSnWork::updateTime(quint64 uiSecs)
{
    QDateTime oDateTimeOld = PublicFunction::getDateTimeFromHeaderPhoenix(goHeadPhoenix);

    QDateTime oDateTime = oDateTimeOld.addSecs(uiSecs);

    QTime oTime = oDateTime.time();
    QDate oDate = oDateTime.date();

    goHeadPhoenix.uiSec = oTime.second();
    goHeadPhoenix.uiMin = oTime.minute();
    goHeadPhoenix.uiHour = oTime.hour();

    goHeadPhoenix.uiDay = oDate.day();
    goHeadPhoenix.uiMonth = oDate.month();
    goHeadPhoenix.uiDayOfWeek = oDate.dayOfWeek();

    int iYear = oDate.year();

    goHeadPhoenix.uiYear = iYear%100;
    goHeadPhoenix.uiCentury = iYear/100+1;

    emit sigMsg(MSG_Normal, QString("时间秒：累加%1s").arg(uiSecs));
}
