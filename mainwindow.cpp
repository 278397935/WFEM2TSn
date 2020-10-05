#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebugV0()<<QThread::currentThreadId();


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
    qRegisterMetaType<WFEM_HEADER>("WFEM_HEADER");
    qRegisterMetaType<AMTorMT>("AMTorMT");
    qRegisterMetaType<TSn>("TSn");

    poTSnWork = nullptr;

    poTSnWork = new TSnWork();

    connect(poTSnWork, SIGNAL(sigMsg(QString)), this, SLOT(updateMsg(QString)));

    connect(poTSnWork, SIGNAL(sigWFEM_HEADER(WFEM_HEADER)), this, SLOT(slotWFEM_HEADER(WFEM_HEADER)));

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

    if( !oDir.cdUp() )
    {
        //qDebugV0()<<"After switching to the first level Dir:"<<oDir.absolutePath();
        qDebugV0()<<"The previous directory of the current directory does not exist!";
        return;
    }

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

void MainWindow::updateMsg(QString oStrMsg)
{
    ui->textBrowser->append(oStrMsg);
}

/* 收到CS的首行信息，header，显示在UI上供User查看和修改 */
void MainWindow::slotWFEM_HEADER(WFEM_HEADER oWfemHeader)
{
    QString oStrWfemHeader;
    oStrWfemHeader.clear();

    /* 将 WFEM header 显示到 TextBrowser */
    QDateTime oDateTime = PublicFunction::getDateTime(oWfemHeader.uiStartTime);

    QString oStrDateTime = oDateTime.toString("yyyy年MM月dd日 hh:mm:ss dddd");

    this->updateMsg("\n开始采样时刻："+oStrDateTime+"\n");

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
            .arg(oWfemHeader.uiDataCnt)
            .arg(oWfemHeader.oStrTaskName)
            .arg(oWfemHeader.oStrIP)
            .arg(oWfemHeader.oStrLineId)
            .arg(oWfemHeader.oStrSiteId)
            .arg(oWfemHeader.uiSiteDistance)
            .arg(oWfemHeader.uiDevId)
            .arg(oWfemHeader.uiDevCh)
            .arg(mapV2M.value(oWfemHeader.uiV2M))
            .arg(mapChopSW.value(oWfemHeader.uiChopSw))
            .arg(oWfemHeader.oStrTag)
            .arg(oWfemHeader.dPolar)
            .arg(oWfemHeader.dAngle)
            .arg(oWfemHeader.oStrSensorId)
            .arg(mapMethod.value(oWfemHeader.uiMethod))
            .arg(oWfemHeader.dLatitude)
            .arg(oWfemHeader.dLongitude)
            .arg(oWfemHeader.uiStartTime)
            .arg(oWfemHeader.uiSampleLength)
            .arg(oWfemHeader.uiFS)
            .arg(oWfemHeader.uiPB_Type)
            .arg(oWfemHeader.oStrSampleName)
            .arg(oWfemHeader.uiFreqWave)
            .arg(oWfemHeader.uiFreqGroup)
            .arg(oWfemHeader.uiPeriodCnt)
            .arg(oWfemHeader.uiWorkCnt)
            .arg(oWfemHeader.uiGain)
            .arg(oWfemHeader.uiSlicBase)
            .arg(oWfemHeader.uiSliceSample)
            .arg(mapFormat.value(oWfemHeader.uiFormat))
            .arg(oWfemHeader.uiXpf);


    this->updateMsg(oStrWfemHeader);

    /* Phoenix header */
    ui->tableWidget->setItem(0,3,new QTableWidgetItem(oStrDateTime));
    ui->tableWidget->setItem(1,3,new QTableWidgetItem(QString::number(oWfemHeader.uiDevId, 10)));
    ui->tableWidget->setItem(2,3,new QTableWidgetItem(QString::number(oWfemHeader.uiFS, 10)));
    ui->tableWidget->setItem(8,3,new QTableWidgetItem(QString::number(oWfemHeader.uiFS, 10)));

    /* 内容自适应 */
    ui->tableWidget->resizeColumnsToContents();

    /* Dr.Lee  设计，采样片段时长 */
    ui->spinBoxSlot->setValue(oWfemHeader.uiSlicBase);

    /* Dr.Lee  设计，采样记录个数*/
    ui->spinBoxRecord->setValue(oWfemHeader.uiSliceSample);
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
        QMessageBox::information(NULL, "警告", "未选中CS时间域文件！",
                                 QMessageBox::Yes );
        return;
    }

    if(aoStrFileName.count() != CH_CNT)
    {
        QMessageBox::information(NULL, "警告",
                                 QString("请选%1个同类型CS时间域文件！")
                                 .arg(CH_CNT),
                                 QMessageBox::Yes );
        return;
    }

    ui->textBrowser->clear();

    this->updateMsg("打开文件：");
    for(int i = 0; i < CH_CNT; i++)
    {
        this->updateMsg(QString("No.%1\u058E %2")
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
        return;
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
