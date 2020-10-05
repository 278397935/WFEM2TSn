#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#include <QFileDialog>
#include <QMessageBox>

#include "Common/PublicDef.h"
#include "Common/PublicFunction.h"

#include "TSnWork.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void slotWFEM_HEADER(WFEM_HEADER oWfemHeader);

    void updateMsg(QString oStrMsg);

    void on_actionOpenFile_triggered();

    void on_actionConvert_triggered();

    void on_actionConsult_triggered();

    void on_comboBoxAMTorMT_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;

    TSnWork *poTSnWork;


    QMap<quint8, QString> mapMethod;
    QMap<quint8, QString> mapChopSW;
    QMap<quint8, QString> mapV2M;

    QMap<quint8, QString> mapFormat;


    /* Read last Dir log file, get last Dir(Previous directory) */
    QString LastDirRead();

    /* Writes the last valid path to the configuration file. */
    void LastDirWrite(QString oStrFileName);


signals:
    void sigWFEM_HEADER(WFEM_HEADER oHeader);

    void sigParameter(AMTorMT eAMTorMT, TSn eTSn, quint32 uiSlicBase, quint32 uiSlicSample);

};

#endif // MAINWINDOW_H
