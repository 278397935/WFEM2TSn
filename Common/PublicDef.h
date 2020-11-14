#ifndef PUBLICDEF_H
#define PUBLICDEF_H

#include <QMetaType>
#include <QtCore>

#include <assert.h>


/****************************************************************/
/* Debug define*/
/****************************************************************/
#define qDebugV0() (qDebug()<<"[INFO]["<<__FUNCTION__<<":"<<__LINE__<<"] ")

/* Some debug info in test */
//#define qDebugV1() (qWarning()<<"[Debug]["<<__FUNCTION__<<":"<<__LINE__<<"] ")

/* Critical */
#define qDebugV5() (qCritical()<<"[ERR]["<<__FILE__"|"<<__FUNCTION__<<":"<<__LINE__<<"] ")

/* Last Dir log */
#define LASTDIR    "LastDirLog.ini"

#define CH_CNT 5
#define HEADER_LENGTH 32
#define BIT_WIDTH 3


#define CH_1 1
#define CH_2 2
#define CH_3 3
#define CH_4 4
#define CH_5 5

/* 广域 时间域数据 首行信息 枚举 */
enum WFEM_HEADER_INDEX
{
    WFEM_DATA_CNT,       //WFEM data count
    WFEM_TASK_NAME,      //Task name
    WFEM_IP,             //IP
    WFEM_LINE,           //Line
    WFEM_SITE,           //Site
    WFEM_SITE_DIST,      //Site distance
    WFEM_DEV,            //Dev ID
    WFEM_CH,             // Channel ID
    WFEM_V2M,            //V2M or M2V
    WFEM_CHOP_SW,        //Chop switch
    WFEM_COMP_ID,        //Component
    WFEM_POLAR,          //Polar distance
    WFEM_ANGLE,          //Angle
    WFEM_SENSOR_ID,      //Sensor ID
    WFEM_METHOD,         //Work method ID
    WFEM_LATI,           //Latitude
    WFEM_LONG,           //Longitude
    WFEM_START_TIME,     //Start time
    WFEM_SAMP_TIME,      //Sample time
    WFEM_SAMP_RATE,      //Sample rate
    WFEM_PB_TYPE,        //PB type
    WFEM_SAMP_NAME,      //Sample name
    WFEM_FREQ_WAVE,      //Freq Wave
    WFEM_FREQ_GROUP,     //Freq Group
    WFEM_PERIOD_CNT,     //Period count
    WFEM_WORK_CNT,       //Work conunt
    WFEM_WORK_GAIN,      //Gain
    WFEM_SLICE_BASE,     //CS time Slice BaseTime
    WFEM_SLICE_SAMP,     //CS time Slice SampTime
    WFEM_FORMAT,         //WFEM format
    WFEM_XPF,            //HPF or LPF
    WFEM_MAX
};

/* 广域 时间域数据 首行信息 结构体 */
struct WFEM_HEADER
{
    quint64 uiDataCnt;//WFEM_DATA_CNT,       //WFEM data count
    QString oStrTaskName;//WFEM_TASK_NAME,      //Task name
    QString oStrIP;// WFEM_IP,             //IP
    QString oStrLineId;//WFEM_LINE,           //Line
    QString oStrSiteId;//WFEM_SITE,           //Site
    quint32 uiSiteDistance;//WFEM_SITE_DIST,      //Site distance
    quint32 uiDevId;//WFEM_DEV,            //Dev ID
    quint32 uiDevCh;//WFEM_CH,             // Channel ID
    quint8  uiV2M;//WFEM_V2M,            //V2M or M2V
    quint8  uiChopSw;//WFEM_CHOP_SW,        //Chop switch
    QString oStrTag;//WFEM_COMP_ID,        //Component
    double  dPolar;//WFEM_POLAR,          //Polar distance
    double  dAngle;//WFEM_ANGLE,          //Angle
    QString oStrSensorId;//WFEM_SENSOR_ID,      //Sensor ID
    quint8  uiMethod;//WFEM_METHOD,         //Work method ID
    double  dLatitude;//WFEM_LATI,           //Latitude
    double  dLongitude;//WFEM_LONG,           //Longitude
    uint    uiStartTime;//WFEM_START_TIME,     //Start time
    uint    uiSampleLength;//WFEM_SAMP_TIME,      //Sample time
    quint16 uiFS;//WFEM_SAMP_RATE,      //Sample rate,凤凰仪器的采样率是固定的，存放采样率的看见大小16个二进制位，最大65535
    quint8  uiPB_Type;//WFEM_PB_TYPE,        //PB type
    QString oStrSampleName;//WFEM_SAMP_NAME,      //Sample name
    quint8  uiFreqWave;//WFEM_FREQ_WAVE,      //Freq Wave
    quint8  uiFreqGroup;//WFEM_FREQ_GROUP,     //Freq Group
    quint32 uiPeriodCnt;//WFEM_PERIOD_CNT,     //Period count
    quint32 uiWorkCnt;//WFEM_WORK_CNT,       //Work conunt
    quint32 uiGain;//WFEM_WORK_GAIN,      //Gain
    quint32 uiSlicBase;//WFEM_SLICE_BASE,     //CS time Slice BaseTime
    quint32 uiSliceSample;//WFEM_SLICE_SAMP,     //CS time Slice SampTime
    quint8  uiFormat;//WFEM_FORMAT,         //WFEM format
    quint8  uiXpf;//WFEM_XPF,            //HPF or LPF
    //WFEM_MAX
};


/* 广域时间域的文件名和首行信息map上 */
//QMap<QString, WFEM_HEADER> MapFileNameHeader;


enum SAMP_METHOD {
    METHOD_CS,
    METHOD_WFEM,
    METHOD_AWFEM,
    METHOD_AUTO,
    METHOD_MAX
};

/* Chop switch */
enum CHOP_SW
{
    CHOP_OFF = 0,
    CHOP_ON
};

/* Channel convert */
enum CH_ATTR
{
    CH_NORMAL = 0,
    CH_M2V,
    CH_V2M,
    CH_ATTR_MAX
};

/* Raw format */
enum RAW_FORMAT
{
    RAW_BIN = 0,
    RAW_STR,
};


/****************************************************************/
/* Geophysical method: AMT, MT */
/****************************************************************/
typedef enum _GEO_METHOD
{
    GEO_AMT = 1,
    GEO_MT,
}GEO_METHOD;

/****************************************************************/
/* Band: TS2, TS3, TS4, TS5 */
/****************************************************************/
typedef enum _BAND
{
    Band2 = 2,
    Band3,
    Bnad4,
    Band5
}BAND;

/* Record format phoenix   list */
typedef struct _PHOENIX_HEADER
{
    quint8  uiSec;                                //0:     秒
    quint8  uiMin;                               //1:     分
    quint8  uiHour;                             //2:     时
    quint8  uiDay;                             //3:     日
    quint8  uiMonth;                          //4:     月
    quint8  uiYear;                          //5：    年（后两位）
    quint8  uiDayOfWeek;                    //6:     星期几 dayOfWeek
    quint8  uiCentury;                     //7:     世纪

    quint16 uiDevSN;                    //8~9:   盒子序列号（16位整数）
    quint16 uiSampleCntPerRecordPerCh; //10~11: 记录中scan的个数（16位整数）
    quint8  uiChCntPerScan;           //12:    每次scan的道数
    quint8  uiRecoedHeaderLength;    //13:    tag的长度（TSn）或tag的长度代码（TSH，TSL）
    quint8  uiRecStatus;            //14:    状态代码
    quint8  uiBitSaturationCode;   //15~16:    位饱和码
    quint8  uiBitWidth;           //17:    采样长度（单位：Bytes）24bit, 3个字节 固定
    quint16 uiFS;                //18~19: 采样率（单位：20Byte）
    quint8  uiFSUnit;           //20:    采样率单位 固定， 秒 0
    quint8  uiClockStatus;     //21:    时钟状态
    qint32  iClockErr;        //22~25: 时钟误差（单位：微秒）
    char    acReserved[6];   //26~31: 保留（必须为0）
}PHOENIX_HEADER;


enum AMTorMT
{
    AMT = 0,
    MT
};

enum TSn
{
    TS2 = 2,
    TS3,
    TS4,
    TS5
};

enum MSG_TYPE
{
    MSG_Normal = 0,
    MSG_Over,
    MSG_Err,
};

#endif // PUBLICDEF_H
