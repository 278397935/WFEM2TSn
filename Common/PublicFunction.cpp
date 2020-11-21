#include "PublicFunction.h"

PublicFunction::PublicFunction()
{

}

/* 由首行string解析出首行信息，塞进结构体 */
HEAD_WFEM PublicFunction::pickHeader(QString oStrPreheader)
{
    HEAD_WFEM oHeader;

    oHeader.uiDataCnt = oStrPreheader.section(',', WFEM_DATA_CNT, WFEM_DATA_CNT).trimmed().toULongLong();

    oHeader.oStrTaskName = oStrPreheader.section(',', WFEM_TASK_NAME, WFEM_TASK_NAME).trimmed();

    oHeader.oStrIP = oStrPreheader.section(',', WFEM_IP, WFEM_IP).trimmed();
    oHeader.oStrLineId = oStrPreheader.section(',', WFEM_LINE, WFEM_LINE).trimmed();
    oHeader.oStrSiteId = oStrPreheader.section(',', WFEM_SITE, WFEM_SITE).trimmed();
    oHeader.uiSiteDistance = oStrPreheader.section(',', WFEM_SITE_DIST, WFEM_SITE_DIST).trimmed().toUInt();

    oHeader.uiDevId = oStrPreheader.section(',', WFEM_DEV, WFEM_DEV).trimmed().toUInt();
    oHeader.uiDevCh = oStrPreheader.section(',', WFEM_CH, WFEM_CH).trimmed().toUInt();
    oHeader.uiV2M = oStrPreheader.section(',', WFEM_V2M, WFEM_V2M).trimmed().toUInt();

    oHeader.uiChopSw = oStrPreheader.section(',', WFEM_CHOP_SW, WFEM_CHOP_SW).trimmed().toUInt();

    oHeader.oStrTag = oStrPreheader.section(',', WFEM_COMP_ID, WFEM_COMP_ID).trimmed().toUInt();

    /* 极距、角度 */
    oHeader.dPolar = oStrPreheader.section(',', WFEM_POLAR, WFEM_POLAR).trimmed().toDouble();
    oHeader.dAngle = oStrPreheader.section(',', WFEM_ANGLE, WFEM_ANGLE).trimmed().toDouble();

    oHeader.oStrSensorId = oStrPreheader.section(',', WFEM_SENSOR_ID, WFEM_SENSOR_ID).trimmed();
    oHeader.uiMethod = oStrPreheader.section(',', WFEM_METHOD, WFEM_METHOD).trimmed().toUInt();

    /* 坐标 */
    oHeader.dLatitude = oStrPreheader.section(',', WFEM_LATI, WFEM_LATI).trimmed().toDouble();
    oHeader.dLongitude = oStrPreheader.section(',', WFEM_LONG, WFEM_LONG).trimmed().toDouble();

    /* 开始时间、采样时长、采样率 */
    oHeader.uiStartTime = oStrPreheader.section(',', WFEM_START_TIME,WFEM_START_TIME).trimmed().toULongLong()- 8*3600;
    oHeader.uiFS = oStrPreheader.section(',', WFEM_SAMP_RATE, WFEM_SAMP_RATE).trimmed().toULongLong();
    oHeader.uiSampleLength =  oHeader.uiDataCnt/oHeader.uiFS;//oStrPreheader.section(',', WFEM_SAMP_TIME, WFEM_SAMP_TIME).trimmed().toULongLong();

    oHeader.uiPB_Type = oStrPreheader.section(',', WFEM_PB_TYPE, WFEM_PB_TYPE).trimmed().toULongLong();

    /* 采样名，CS模式的采样名统一为Root */
    oHeader.oStrSampleName = oStrPreheader.section(',', WFEM_SAMP_NAME, WFEM_SAMP_NAME).trimmed();

    /* 老广域工作模式参数 */
    oHeader.uiFreqWave = oStrPreheader.section(',', WFEM_FREQ_WAVE, WFEM_FREQ_WAVE).trimmed().toUInt();
    oHeader.uiFreqGroup = oStrPreheader.section(',', WFEM_FREQ_GROUP, WFEM_FREQ_GROUP).trimmed().toUInt();
    oHeader.uiPeriodCnt = oStrPreheader.section(',', WFEM_PERIOD_CNT, WFEM_PERIOD_CNT).trimmed().toUInt();
    oHeader.uiWorkCnt = oStrPreheader.section(',', WFEM_WORK_CNT, WFEM_WORK_CNT).trimmed().toUInt();
    oHeader.uiGain= oStrPreheader.section(',', WFEM_WORK_GAIN, WFEM_WORK_GAIN).trimmed().toUInt();

    /* 间隔采样 */
    oHeader.uiSlicBase = oStrPreheader.section(',', WFEM_SLICE_BASE, WFEM_SLICE_BASE).trimmed().toULongLong();
    oHeader.uiSliceSample = oStrPreheader.section(',', WFEM_SLICE_SAMP, WFEM_SLICE_SAMP).trimmed().toULongLong();

    /* 高低频板 */
    oHeader.uiXpf= oStrPreheader.section(',', WFEM_XPF, WFEM_XPF).trimmed().toUInt();

    return oHeader;
}

/* 由 UTC 秒 得到 date&time */
QDateTime PublicFunction::getDateTimeFromUTC(uint uiSecs)
{
    QDateTime oDateTime;

    oDateTime = QDateTime::fromTime_t(uiSecs);

    return oDateTime;
}

/* 从String，获取到浮点数 */
float PublicFunction::getNumber(QString oStrLine)
{
    float fData = 0;

    oStrLine.remove("\n");

    QStringList aoStrLine = oStrLine.split(",", QString::SkipEmptyParts);

    if(aoStrLine.isEmpty())
    {
        return 0;
    }

    QString oStrData = aoStrLine.first();

    fData = oStrData.toFloat();

    return fData;
}

/* 从凤凰Header中获取到时间 */
QDateTime PublicFunction::getDateTimeFromHeaderPhoenix(HEAD_PHOENIX oHeader)
{
    QDateTime oDateTime;

    QTime oTime;
    QDate oDate;

    oTime.setHMS( oHeader.uiHour, oHeader.uiMin, oHeader.uiSec);

    oDate.setDate( (oHeader.uiCentury - 1)*100 + oHeader.uiYear, oHeader.uiMonth,oHeader.uiDay);

    oDateTime.setTime(oTime);
    oDateTime.setDate(oDate);

    return oDateTime;
}

/* 更新哈时间 */
HEAD_PHOENIX PublicFunction::getHeaderFromAddSecs(HEAD_PHOENIX oHeader, quint64 uiSecs)
{
    QDateTime oDateTimeOld = getDateTimeFromHeaderPhoenix(oHeader);

    QDateTime oDateTime = oDateTimeOld.addSecs(uiSecs);

    QTime oTime = oDateTime.time();
    QDate oDate = oDateTime.date();

    oHeader.uiSec = oTime.second();
    oHeader.uiMin = oTime.minute();
    oHeader.uiHour = oTime.hour();

    oHeader.uiDay = oDate.day();
    oHeader.uiMonth = oDate.month();
    oHeader.uiDayOfWeek = oDate.dayOfWeek();

    int iYear = oDate.year();

    oHeader.uiYear = iYear%100;
    oHeader.uiCentury = iYear/100+1;

    return oHeader;
}

/* 由已知的广域时间域文件的首行信息来完善凤凰的header */
HEAD_PHOENIX PublicFunction::getHeaderFromHeaderWFEM(HEAD_WFEM oHeader)
{
    HEAD_PHOENIX oHeaderPhoenix;

    /* 时间,已验证过，确实可行 */
    QDateTime oDateTime = QDateTime::fromTime_t(oHeader.uiStartTime);

    QTime oTime = oDateTime.time();
    QDate oDate = oDateTime.date();

    oHeaderPhoenix.uiSec  = oTime.second();
    oHeaderPhoenix.uiMin  = oTime.minute();
    oHeaderPhoenix.uiHour = oTime.hour();

    oHeaderPhoenix.uiDay       = oDate.day();
    oHeaderPhoenix.uiMonth     = oDate.month();
    oHeaderPhoenix.uiDayOfWeek = oDate.dayOfWeek();

    int iYear = oDate.year();

    oHeaderPhoenix.uiYear    = iYear%100;
    oHeaderPhoenix.uiCentury = iYear/100+1;

    /* 设备编号 */
    oHeaderPhoenix.uiDevSN = oHeader.uiDevId;
    oHeaderPhoenix.uiSampleCntPerRecordPerCh = oHeader.uiFS;
    oHeaderPhoenix.uiChCntPerScan = CH_CNT;

    oHeaderPhoenix.uiRecoedHeaderLength = HEADER_LENGTH;

    oHeaderPhoenix.uiRecStatus = 0;

    oHeaderPhoenix.uiBitSaturationCode = 0;

    oHeaderPhoenix.uiBitWidth = BIT_WIDTH;

    oHeaderPhoenix.uiFS = oHeader.uiFS;

    oHeaderPhoenix.uiFSUnit = 0;

    oHeaderPhoenix.uiClockStatus = 3;
    oHeaderPhoenix.iClockErr = 0;

    memset(oHeaderPhoenix.acReserved, 0, 6);

    return oHeaderPhoenix;
}

/* 读广域时间域文件首行，得到首行信息，塞进结构体中 */
HEAD_WFEM PublicFunction::getHeaderFromFileName(QString oStrFileName)
{
    HEAD_WFEM oHeader;

    QString oStrPreheader;
    oStrPreheader.clear();

    QFile oFile(oStrFileName);

    /* Read Raw*.dat */
    if( oFile.open(QIODevice::ReadOnly) )
    {
        QTextStream oStream(&oFile);

        /* 首行信息 */
        oStrPreheader = oStream.readLine();

        oHeader = pickHeader(oStrPreheader);
    }

    oFile.close();

    return oHeader;
}
