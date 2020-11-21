#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebugV0()<<QThread::currentThreadId();

    /* 初始状态，转换按钮是不能按 */
    ui->actionConvert->setEnabled(false);

    /* 方法 */
    mapMethod.insert(METHOD_CS, "连续采样");
    mapMethod.insert(METHOD_WFEM, "CS手动采样");
    mapMethod.insert(METHOD_AWFEM, "CS自动采样");
    mapMethod.insert(METHOD_AUTO, "全自动采样");

    /*  斩波开关 */
    mapChopSW.insert(CHOP_OFF, "斩波关(CAS-90B，中低频段用)");
    mapChopSW.insert(CHOP_ON,  "斩波开(CAS-90B，高频段用)");


    /* 正常使用、磁道用作电道、电道用作磁道 */
    mapV2M.insert(CH_NORMAL,"正常");
    mapV2M.insert(CH_M2V,"磁转电");
    mapV2M.insert(CH_V2M,"电转磁");

    /* 数据格式 */
    mapFormat.insert(RAW_BIN, "二进制");
    mapFormat.insert(RAW_STR, "文本");


    /* 内容自适应 */
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableWidget->setWindowTitle("Phoenix Header");

    this->on_comboBoxAMTorMT_currentIndexChanged(0);

    /* 注册 信号槽 传递 类型 */
    qRegisterMetaType<HEAD_WFEM>("HEAD_WFEM");
    qRegisterMetaType<AMTorMT>("AMTorMT");
    qRegisterMetaType<TSn>("TSn");
    qRegisterMetaType<MSG_TYPE>("MSG_TYPE");

    poTSnWork = nullptr;

    poTSnWork = new TSnWork();

    connect(poTSnWork, SIGNAL(sigMsg(MSG_TYPE, QString)), this, SLOT(updateMsg(MSG_TYPE, QString)));

    connect(poTSnWork, SIGNAL(sigHEAD_WFEM(HEAD_WFEM)), this, SLOT(slotHEAD_WFEM(HEAD_WFEM)));

    connect(this, SIGNAL(sigParameter(AMTorMT,TSn,quint32, quint32)), poTSnWork, SLOT(convert(AMTorMT,TSn, quint32, quint32)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**************************************************************************************************
 * Read last Dir log file, get last Dir(Previous directory)
 */
QString MainWindow::LastDirRead()
{
    QString oStrLastDir;
    oStrLastDir.clear();

    //qDebugV0()<<"Read last time Dir.";

    QFile oFileLastDir(LASTDIR);

    if( oFileLastDir.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        QTextStream oTextStreamIn(&oFileLastDir);
        oStrLastDir = oTextStreamIn.readLine();

        if(oStrLastDir.isNull())
        {
            /* Default Dir */
            oStrLastDir = "D:/";
        }

        //qDebugV0()<<"Last Dir:"<<oStrLastDir;
    }
    else
    {
        qDebugV0()<<"Can't open the file! Return default Dir.";

        /* Default Dir */
        oStrLastDir = "D:/";
    }

    oFileLastDir.close();

    return oStrLastDir;
}

/**************************************************************************************************
 * Writes the last valid path to the configuration file.
 */
void MainWindow::LastDirWrite(QString oStrFileName)
{
    QFileInfo oFileInfoLastDir(oStrFileName);

    //qDebugV0()<<"Current project Dir:"<<oFileInfoLastDir.absoluteDir().absolutePath();

    QDir oDir = oFileInfoLastDir.absoluteDir();

//    if( !oDir.cdUp() )
//    {
//        //qDebugV0()<<"After switching to the first level Dir:"<<oDir.absolutePath();
//        qDebugV0()<<"The previous directory of the current directory does not exist!";
//        return;
//    }

    QFile oFileLastDir(LASTDIR);

    if( !oFileLastDir.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text) )
    {
        qDebugV0()<<"Open last dir file false!";
        return;
    }

    QTextStream oTextStreamOut(&oFileLastDir);

    oTextStreamOut<<oDir.absolutePath();

    oFileLastDir.close();
}

void MainWindow::updateMsg(MSG_TYPE eType, QString oStrMsg)
{
    switch (eType) {
    case MSG_Normal:
        ui->textBrowser->append(oStrMsg);
        break;

    case MSG_Err:;
        QMessageBox::information(NULL, "错误", oStrMsg);
        break;

    case MSG_Over:
        ui->textBrowser->append(oStrMsg);
        ui->actionConvert->setEnabled(true);
        break;

    default:
        break;
    }
}

/* 收到CS的首行信息，header，显示在UI上供User查看和修改 */
void MainWindow::slotHEAD_WFEM(HEAD_WFEM oHeadWFEM)
{
    QString oStrWfemHeader;
    oStrWfemHeader.clear();

    /* 将 WFEM header 显示到 TextBrowser */
    QDateTime oDateTime = PublicFunction::getDateTimeFromUTC(oHeadWFEM.uiStartTime);

    QString oStrDateTime = oDateTime.toString("yyyy年MM月dd日 hh:mm:ss dddd");

    this->updateMsg(MSG_Normal, "\n开始采样时刻："+oStrDateTime+"\n");

    oStrWfemHeader = QString("数据总长度：%1\n"
                             "任务名：%2\n"
                             "设备IP：%3\n"
                             "线号：%4\n"
                             "点号：%5\n"
                             "点号递增：%6\n"
                             "设备号：%7\n"
                             "通道号：%8\n"
                             "正常、磁转电、电转磁：%9\n"
                             "斩波开关：%10\n"
                             "分量标识：%11\n"
                             "极距：%12\n"
                             "方位角：%13\n"
                             "传感器编号：%14\n"
                             "方法：%15\n"
                             "经度：%16\n"
                             "维度：%17\n"
                             "开始采样时刻：%18 "+oStrDateTime+"\n"
                                                        "采样时长:%19\n"
                                                        "采样率：%20Hz\n"
                                                        "高低频板：%21\n"
                                                        "采样名：%22\n"
                                                        "频率波形(传统CS采集)：%23\n"
                                                        "频组(传统CS采集)：%24\n"
                                                        "周期数(传统CS采集)：%25\n"
                                                        "工作次数(传统CS采集)：%26\n"
                                                        "增益(传统CS采集)：%27\n"
                                                        "片段时长(间隔采样)：%28\n"
                                                        "片段数目(间隔采样)：%29\n"
                                                        "数据格式：%30\n"
                                                        "高低频滤波：%31\n")
            .arg(oHeadWFEM.uiDataCnt)
            .arg(oHeadWFEM.oStrTaskName)
            .arg(oHeadWFEM.oStrIP)
            .arg(oHeadWFEM.oStrLineId)
            .arg(oHeadWFEM.oStrSiteId)
            .arg(oHeadWFEM.uiSiteDistance)
            .arg(oHeadWFEM.uiDevId)
            .arg(oHeadWFEM.uiDevCh)
            .arg(mapV2M.value(oHeadWFEM.uiV2M))
            .arg(mapChopSW.value(oHeadWFEM.uiChopSw))
            .arg(oHeadWFEM.oStrTag)
            .arg(oHeadWFEM.dPolar)
            .arg(oHeadWFEM.dAngle)
            .arg(oHeadWFEM.oStrSensorId)
            .arg(mapMethod.value(oHeadWFEM.uiMethod))
            .arg(oHeadWFEM.dLatitude)
            .arg(oHeadWFEM.dLongitude)
            .arg(oHeadWFEM.uiStartTime)
            .arg(oHeadWFEM.uiSampleLength)
            .arg(oHeadWFEM.uiFS)
            .arg(oHeadWFEM.uiPB_Type)
            .arg(oHeadWFEM.oStrSampleName)
            .arg(oHeadWFEM.uiFreqWave)
            .arg(oHeadWFEM.uiFreqGroup)
            .arg(oHeadWFEM.uiPeriodCnt)
            .arg(oHeadWFEM.uiWorkCnt)
            .arg(oHeadWFEM.uiGain)
            .arg(oHeadWFEM.uiSlicBase)
            .arg(oHeadWFEM.uiSliceSample)
            .arg(mapFormat.value(oHeadWFEM.uiFormat))
            .arg(oHeadWFEM.uiXpf);


    this->updateMsg(MSG_Normal, oStrWfemHeader);

    /* Phoenix header */
    ui->tableWidget->setItem(0,3,new QTableWidgetItem(oStrDateTime));
    ui->tableWidget->setItem(1,3,new QTableWidgetItem(QString::number(oHeadWFEM.uiDevId, 10)));
    ui->tableWidget->setItem(2,3,new QTableWidgetItem(QString::number(oHeadWFEM.uiFS, 10)));
    ui->tableWidget->setItem(8,3,new QTableWidgetItem(QString::number(oHeadWFEM.uiFS, 10)));

    /* 内容自适应 */
    ui->tableWidget->resizeColumnsToContents();

    ui->spinBoxRecord->setValue(oHeadWFEM.uiSliceSample);
    ui->spinBoxRecord->setRange(1, oHeadWFEM.uiSliceSample);

    ui->spinBoxSlot->setValue(oHeadWFEM.uiSlicBase);

    if(oHeadWFEM.uiSlicBase == oHeadWFEM.uiSliceSample)
    {
        ui->spinBoxSlot->setEnabled(true);
        ui->spinBoxSlot->setRange(1, oHeadWFEM.uiSlicBase);
    }
    else//广域间隔采样
    {
        ui->spinBoxSlot->setEnabled(false);
    }
}

/* 打开文件*/
void MainWindow::on_actionOpenFile_triggered()
{
    QStringList aoStrFileName = QFileDialog::getOpenFileNames(
                this,
                QString("请选%1个同类型CS时间域文件！")
                .arg(CH_CNT),
                this->LastDirRead(),
                "CS时间域 (*.dat)");

    if(aoStrFileName.isEmpty())
    {
        QMessageBox::information(nullptr, "警告", "未选中CS时间域文件！",
                                 QMessageBox::Yes );
        return;
    }

    if(aoStrFileName.count() != CH_CNT)
    {
        QMessageBox::information(nullptr, "警告",
                                 QString("请选%1个同类型CS时间域文件！")
                                 .arg(CH_CNT),
                                 QMessageBox::Yes );
        return;
    }

    this->LastDirWrite(aoStrFileName.first());

    ui->textBrowser->clear();

    this->updateMsg(MSG_Normal, "打开文件：");
    for(int i = 0; i < CH_CNT; i++)
    {
        this->updateMsg(MSG_Normal, QString("No.%1\u058E %2")
                        .arg(i)
                        .arg(aoStrFileName.at(i)));
    }

    /** 判断， 所选的文件是不是
      * 同一台仪器、
      * 同一个时刻开始，
      * 同一个采样率、
      * 同一个采样时长
      * 放到单独的类里面去判断，不和UI类有关联
      */

    if(!poTSnWork->judgeKind(aoStrFileName))
    {
        QMessageBox::information(NULL, "警告", "选中的CS时间域文件，\n不属于同一参数文件。\n请重新选择！",
                                 QMessageBox::Yes );
        ui->textBrowser->clear();

        return;
    }
    else
    {
        ui->actionConvert->setEnabled(true);
    }
}

/* 开始转换·转换函数放在线程里面工作· */
void MainWindow::on_actionConvert_triggered()
{
    AMTorMT eAMTorMT;

    TSn eTSn;

    switch (ui->comboBoxAMTorMT->currentIndex()) {
    case 0:
        eAMTorMT = AMT;
        switch (ui->comboBoxBand->currentIndex()) {
        case 0:
            eTSn = TS2;
            break;

        case 1:
            eTSn = TS3;
            break;

        case 2:
            eTSn = TS4;
            break;

        default:
            break;
        }
        break;
    case 1:
        eAMTorMT = MT;
        switch (ui->comboBoxBand->currentIndex()) {
        case 0:
            eTSn = TS3;
            break;

        case 1:
            eTSn = TS4;
            break;

        case 2:
            eTSn = TS5;
            break;

        default:
            break;
        }
        break;
    default:
        break;
    }

    /* 在界面上设置好的参数，传递给work thread */
    emit sigParameter(eAMTorMT, eTSn, ui->spinBoxSlot->value(), ui->spinBoxRecord->value());
}

/* 打开转换文件 */
void MainWindow::on_actionConsult_triggered()
{

}

/* 由方法决定 TSn 文件选项 */
void MainWindow::on_comboBoxAMTorMT_currentIndexChanged(int index)
{
    ui->comboBoxBand->clear();

    QStringList aoStrItem;

    aoStrItem.clear();

    switch (index) {
    case 0:
        aoStrItem.append("TS2(采样率：24KHz， AMT高频)");
        aoStrItem.append("TS3(采样率：2.4KHz，AMT中频)");
        aoStrItem.append("TS4(采样率：150Hz， AMT低频)");
        break;
    case 1:
        aoStrItem.append("TS3(采样率：2.4KHz， MT高频)");
        aoStrItem.append("TS4(采样率：150KHz， MT中频)");
        aoStrItem.append("TS5(采样率：15 Hz，  MT低频)");
        break;
    default:
        break;
    }

    ui->comboBoxBand->addItems(aoStrItem);
}

/* 时间片段长度改变，则要重新限制每个片段内的记录数 */
void MainWindow::on_spinBoxSlot_valueChanged(int arg1)
{
    ui->spinBoxRecord->setRange(1, arg1);
}
