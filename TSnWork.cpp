#include "TSnWork.h"


TSnWork::TSnWork(QObject *parent) : QObject(parent)
{
    QThread *poThread = new QThread;
    this->moveToThread(poThread);
    poThread->start();

    pFile = nullptr;
}

void TSnWork::openFiles()
{
    /* 生成 5 个文件 */
    /* CH1 */
    oFileCh1.setFileName(mapChFile.value(CH_1));
    if(!oFileCh1.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebugV5()<<"Can't open the file!"<<oFileCh1.errorString()<<endl;
    }

    /* CH2 */
    oFileCh2.setFileName(mapChFile.value(CH_2));
    if(!oFileCh2.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebugV5()<<"Can't open the file!"<<oFileCh2.errorString()<<endl;
    }

    /* CH3 */
    oFileCh3.setFileName(mapChFile.value(CH_3));
    if(!oFileCh3.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebugV5()<<"Can't open the file!"<<oFileCh3.errorString()<<endl;
    }

    /* CH4 */
    oFileCh4.setFileName(mapChFile.value(CH_4));
    if(!oFileCh4.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebugV5()<<"Can't open the file!"<<oFileCh4.errorString()<<endl;
    }

    /* CH5 */
    oFileCh5.setFileName(mapChFile.value(CH_5));
    if(!oFileCh5.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebugV5()<<"Can't open the file!"<<oFileCh5.errorString()<<endl;
    }

    /* 首行读取出来，发个UI 显示 */
    emit sigMsg(MSG_Normal,"首行信息 Ch1："+  QString(oFileCh1.readLine()));
    emit sigMsg(MSG_Normal,"首行信息 Ch2："+  QString(oFileCh2.readLine()));
    emit sigMsg(MSG_Normal,"首行信息 Ch3："+  QString(oFileCh3.readLine()));
    emit sigMsg(MSG_Normal,"首行信息 Ch4："+  QString(oFileCh4.readLine()));
    emit sigMsg(MSG_Normal,"首行信息 Ch5："+  QString(oFileCh5.readLine()));
}

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
        goHeaderWFEM = PublicFunction::getHeader(oStrFileName);

        goHeaderPhoenix = PublicFunction::getHeader(goHeaderWFEM);

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
        oStrPhoenixFileName = aoStrFileName.first();
        return true;
    }
}

/* 写Phoenix记录头 */
void TSnWork::writeHeader(PHOENIX_HEADER oHeader)
{
    //    QString oStrPhoenixHeader = QString("秒：%1/n"
    //                                        "分：%2/n"
    //                                        "时：%3/n"
    //                                        "日：%4/n"
    //                                        "月：%5/n"
    //                                        "年：%6/n"
    //                                        "星期：%7/n"
    //                                        "世纪：%8/n")
    //            .arg(oHeader.uiSec)
    //            .arg(oHeader.uiMin)
    //            .arg(oHeader.uiHour)
    //            .arg(oHeader.uiDay)
    //            .arg(oHeader.uiMonth)
    //            .arg(oHeader.uiYear)
    //            .arg(oHeader.uiDayOfWeek)
    //            .arg(oHeader.uiCentury);

    //    qDebugV0()<<oStrPhoenixHeader;

    QDataStream oStream;


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

    fwrite(&oHeader.acReserved, 6, 1, pFile);
}

/* 写32位的有符号整数 */
void TSnWork::writeScan()
{
    /* 从广域时间域文件中读取浮点数 */
    iCh1 = PublicFunction::getNumber(oFileCh1.readLine());
    iCh2 = PublicFunction::getNumber(oFileCh2.readLine());
    iCh3 = PublicFunction::getNumber(oFileCh3.readLine());
    iCh4 = PublicFunction::getNumber(oFileCh4.readLine());
    iCh5 = PublicFunction::getNumber(oFileCh5.readLine());

    /* 写入数据（每个数据占3个Bytes）*/
    fwrite(&iCh1, 3, 1, pFile);
    fwrite(&iCh2, 3, 1, pFile);
    fwrite(&iCh3, 3, 1, pFile);
    fwrite(&iCh4, 3, 1, pFile);
    fwrite(&iCh5, 3, 1, pFile);
}

void TSnWork::readNothing(quint64 uiCnt)
{
    for(quint64 i = 0; i < uiCnt; i++)
    {
        /* 从广域时间域文件中读取浮点数 */
        oFileCh1.readLine();
        oFileCh2.readLine();
        oFileCh3.readLine();
        oFileCh4.readLine();
        oFileCh5.readLine();
    }
}

/*  */
void TSnWork::writeGap(quint64 uiSlotCnt, RETAIN_SWITCH eSwitch)
{
    /* 采样时间小于采样间隔， 错误！❌ */
    if(goHeaderWFEM.uiSampleLength <goHeaderWFEM.uiSlicBase)
    {
        emit sigMsg(MSG_Err, "❌,采样间隔比采样时间长！");
        return;
    }

    for(int i = 0; i < uiSlotCnt; i++)
    {
        if(i&1)//奇数,第二端
        {
            switch (eSwitch) {
            case RETAIN_First:
                this->readNothing(goHeaderWFEM.uiSliceSample);

                break;
            case RETAIN_Second:
                /* 奇数个slot，读进去指定的n个记录 */
                for(int j = 0; j < goHeaderWFEM.uiSliceSample; j++)
                {
                    /* 更新记录头的时间，写记录头 */
                    PHOENIX_HEADER oHeaderNew = PublicFunction::getHeader(goHeaderPhoenix,
                                                                          goHeaderWFEM.uiSlicBase*2*i + j);

                    this->writeHeader(oHeaderNew);

                    /*  */
                    for(int k = 0; k < goHeaderPhoenix.uiSampleCntPerRecordPerCh; k++)
                    {
                        this->writeScan();
                    }
                    emit sigMsg(MSG_Normal,QString("间隔采样\u058E：%1/%2, %3/%4")
                                .arg(i/2+1)
                                .arg(uiSlotCnt/2)
                                .arg(j)
                                .arg(goHeaderWFEM.uiSliceSample));

                }
                break;
            default:
                break;
            }
        }
        else//偶数,第一段
        {
            switch (eSwitch) {

            case RETAIN_First:
                /* 奇数个slot，读进去指定的n个记录 */
                for(int j = 0; j < goHeaderWFEM.uiSliceSample; j++)
                {
                    /* 更新记录头的时间，写记录头 */
                    PHOENIX_HEADER oHeaderNew = PublicFunction::getHeader(goHeaderPhoenix,
                                                                          goHeaderWFEM.uiSlicBase*i + j);

                    this->writeHeader(oHeaderNew);

                    /*  */
                    for(int k = 0; k < goHeaderPhoenix.uiSampleCntPerRecordPerCh; k++)
                    {
                        this->writeScan();
                    }
                    emit sigMsg(MSG_Normal,QString("间隔采样\u058E：%1/%2, %3/%4")
                                .arg(i/2+1)
                                .arg(uiSlotCnt/2)
                                .arg(j)
                                .arg(goHeaderWFEM.uiSliceSample));

                }
                break;

            case RETAIN_Second:
                this->readNothing(goHeaderWFEM.uiSliceSample);
                break;

            default:
                break;
            }
        }
    }

    fclose(pFile);

    emit sigMsg(MSG_Over,"间隔采样写入完毕！");
}

/* AMT TS2， 间隔采样，只存0.1s */
void TSnWork::writeTS2(quint64 uiSlotCnt)
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
                PHOENIX_HEADER oHeaderNew = PublicFunction::getHeader(goHeaderPhoenix,
                                                                      goHeaderWFEM.uiSlicBase*i + j);

                this->writeHeader(oHeaderNew);

                for(int k = 0; k < goHeaderPhoenix.uiSampleCntPerRecordPerCh*0.1; k++)
                {
                    this->writeScan();
                }

                this->readNothing(goHeaderWFEM.uiSliceSample*0.9);
            }
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

/* AMT TS4 & MT TS5, 连续采集 */
void TSnWork::writeNoGap()
{
    for(int i = 0; i < goHeaderWFEM.uiSampleLength; i++)
    {
        /* 更新记录头的时间，写记录头 */
        PHOENIX_HEADER oHeaderNew = PublicFunction::getHeader(goHeaderPhoenix, i);

        /* 朝TSn文件中写phoenix 的 Header */
        this->writeHeader(oHeaderNew);

        /* 写扫描数据，是连续采集还是间隔采集，区分开来 */
        for(int j = 0; j < goHeaderPhoenix.uiSampleCntPerRecordPerCh; j++)
        {
            this->writeScan();
        }

        emit sigMsg(MSG_Normal, QString("连续采样\u058E%1/%2(写入记录数/总记录数)")
                    .arg(i+1)
                    .arg(goHeaderWFEM.uiSampleLength));
    }

    fclose(pFile);

    emit sigMsg(MSG_Over,"连续采样写入完毕！");
}

/* 转换函数 */
void TSnWork::convert(AMTorMT eAMTorMT, TSn eTSn, quint32 uiSlicBase, quint32 uiSlicSample)
{
    goHeaderWFEM.uiSlicBase = uiSlicBase;
    goHeaderWFEM.uiSliceSample = uiSlicSample;

    qDebugV0()<<"Convert Thread ID:"<<QThread::currentThreadId();

    /* 凤凰格式的文件 */
    QFileInfo oFileInfo(oStrPhoenixFileName);

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
        emit sigMsg(MSG_Err, "钗头凤·红酥手\n"
                             "【作者】陆游 【朝代】宋\n"
                             "红酥手。黄縢酒。满城春色宫墙柳。东风恶。欢情薄。一怀愁绪，几年离索。错错错。\n"
                             "春如旧。人空瘦。泪痕红浥鲛绡透。桃花落。闲池阁。山盟虽在，锦书难托。莫莫莫。");
        return;
    }

    this->openFiles();

    /* 多少个slot */
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
            this->writeTS2(uiSlotCnt);
            break;

        case TS3:
            this->writeGap(uiSlotCnt, RETAIN_Second);
            break;

        case TS4:
            this->writeNoGap();
            break;

        default:
            break;
        }

        break;

    case MT:
        switch (eTSn) {
        case TS3:
            this->writeGap(uiSlotCnt, RETAIN_First);
            break;

        case TS4:
            this->writeGap(uiSlotCnt, RETAIN_Second);
            break;

        case TS5:
            this->writeNoGap();
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
