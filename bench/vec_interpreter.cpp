/**
 * @file:vec_interpreter.cpp   -
 * @description:
 * @project: BENCH OnSemiconductor
 * @date: 2014\12\04 13-58-04
 *
 * //TODO implement do not update ui to increase speed
 *
 */


#include "vec_interpreter.h"
#include "ui_vec_interpreter.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QDateTime>
#include <QElapsedTimer>

VecInterpreter *gp_VecInterpreter;

const char info_pattern[] = "<b>TIME</b><br>"
        "<b>started: </b>%1<br>"
        "<b>elapsed: </b>%2<br>"
        "<b>Complete: </b>%3";

const char timedate_format[] = "dd:MM hh:mm:ss";

/****************************************************************************
 * @function name: r - ---
 *
 * @param:
 *
    QWidget *parent
 * @description:
 * @return: (  )
 ****************************************************************************/
VecInterpreter::VecInterpreter(QWidget *parent) :
    Applet(parent),
    ui(new Ui::VecInterpreter)
{
    ui->setupUi(this);
    /* custom ui*/

    mp_dialog = new DialogProgressBar(0);
    setName("VecInterpreter");

    ui->progressBar->setValue(0);
    m_ignoreErrors = false;
    mp_runner = new Runner(this);
    m_CurrentStep = 0;
    m_do_not_update_ui =false;
    connect(ui->pbStep, SIGNAL(clicked()), this, SLOT(slot_Step()));
    connect(ui->pbReset, SIGNAL(clicked()), this, SLOT(slot_ClearResults()));
    connect(ui->pbBreakpointsClearAll, SIGNAL(clicked()), this, SLOT(slot_BreakpointClearAll()));
    connect(ui->pbRestart, SIGNAL(clicked()), this, SLOT(slot_Restart()));
    connect(ui->pbToggle, SIGNAL(clicked()), this, SLOT(slot_BreakpointToggle()));
    connect(ui->pbGoto, SIGNAL(clicked()), this, SLOT(slot_Goto()));
    connect(mp_runner, SIGNAL(signal_UpdateCursor()), this, SLOT(slot_UpdateCursor()));
    connect(mp_runner, SIGNAL(signal_UpdateProgressBar(qint32 ,qint32)), this, SLOT(slot_UpdateProgressbar(qint32,qint32)));
    connect(ui->pbRun, SIGNAL(clicked()), this, SLOT(slot_Run()));
    connect(ui->pbErrUp, SIGNAL(clicked()), this, SLOT(slot_PrevError()));
    connect(ui->pbErrDwn, SIGNAL(clicked()), this, SLOT(slot_NextError()));
    connect(ui->pbRepeatedExecution, SIGNAL(clicked()), this, SLOT(slot_RepExec()));
    connect(ui->pbSetCurrentBegin, SIGNAL(clicked()), this, SLOT(slot_SetActiveStart()));
    connect(ui->pbSetCurrentEnd, SIGNAL(clicked()), this, SLOT(slot_SetActiveStop()));
    connect(ui->pbLoadFile, SIGNAL(clicked()), this, SLOT(slot_LoadFile()));
    connect(ui->pbSaveFile, SIGNAL(clicked()), this, SLOT(slot_SaveFile()));
    connect(ui->cbIgnoreErrors, SIGNAL(clicked()), this, SLOT(slot_IgnoreErrors()));
    connect(mp_runner,SIGNAL(signal_UpdateInfo(QString)),this,SLOT(slot_UpdateInfo(QString)));

}


/****************************************************************************
 * @function name: r - ---
 *
 * @param:
 *             void
 * @description:
 * @return: (  )
 ****************************************************************************/
VecInterpreter::~VecInterpreter()
{
    clearData();
    delete ui;
}


/****************************************************************************
 * @function name: VecInterpreter::slot_UpdateCursor - ---
 *
 * @param:
 *             void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::slot_UpdateCursor()
{
    if( !this->doNotUpdateUi() )
    {
        ui->leErrorsCount->setText(QString::number(m_errors_count));
        ui->leCurrentVector->setText(QString::number(m_CurrentStep));
        ui->listView->setCurrentRow((int)m_CurrentStep);
    }
}


/****************************************************************************
 * @function name: VecInterpreter::slot_UpdateAllUi - ---
 *
 * @param:
 *             void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::slot_UpdateAllUi()
{
    m_errors_count = 0;

    for (qint32 i = 0; i < ui->listView->count(); i++)
    {
        ui->listView->setRowText(i, generateItemText(i));
        Vector *pvc = m_VectorsList.at(i);
        m_errors_count += pvc->result == Vector::VEC_ERROR;
    }
    ui->leTotalVectors->setText(QString::number(m_VectorsList.size()));
}


/****************************************************************************
 * @function name: VecInterpreter::isVectorStepError - ---
 *
 * @param:
 *             void
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool VecInterpreter::isVectorStepError() const
{
    return m_VectorsList.at(m_CurrentStep)->result == Vector::VEC_ERROR;
}


/****************************************************************************
 * @function name: VecInterpreter::isVectorStepBreak - ---
 *
 * @param:
 *             void
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool VecInterpreter::isVectorStepBreak() const
{
    return m_VectorsList.at(m_CurrentStep)->breakpoint;
}



/****************************************************************************
 * @function name: VecInterpreter::compareResult - ---
 *
 * @param:
 *
    qint32 no
 * @description:
 * @return: ( Vector::Result )
 ****************************************************************************/
Vector::Result VecInterpreter::compareResult(qint32 no)
{
    Vector::Result result = Vector::VEC_NOTHING;
    // todo take current and comapre it
    Vector *p = m_VectorsList.at(no);
    for (qint32 i = 0; i < m_Signals.size(); i++)
    {
        if (m_Signals.at(i).direction == Signal::OUTPUT)
        {
            if ( p->ctrlVector[i] == p->outVector[i]
                 || p->ctrlVector[i] == VECTOR_UNKNOWN)
            {
                result = Vector::VEC_SUCCESS;
            }
            else
            {
                result = Vector::VEC_ERROR;
                break;
            }
        }
    }
    return result;
}

/****************************************************************************
 * @function name: VecInterpreter::keyPressEvent()
 *
 * @param:
 *      QKeyEvent *pev)
 * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::keyPressEvent(QKeyEvent *pev)
{
    if (pev->key() == Qt::Key_F5)
    {
        slot_Step();
    }
    else if (pev->key() == Qt::Key_F9)
    {
        slot_Run();
    }
    else if (pev->key() == Qt::Key_F6)
    {
        slot_Restart();
    }
}


/* Slots*/


/****************************************************************************
 * @function name: VecInterpreter::slot_BreakpointClearAll - ---
 *
 * @param:
 *
    void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::slot_BreakpointClearAll(void)
{
    for (qint32 i = 0; i < m_VectorsList.size(); i++)
    {
        m_VectorsList.at(i)->breakpoint = false;
    }
    slot_UpdateAllUi();
}


/****************************************************************************
 * @function name: VecInterpreter::slot_BreakpointToggle - ---
 *
 * @param:
 *
    void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::slot_BreakpointToggle(void)
{
    m_VectorsList.at(ui->listView->currentRow())->breakpoint ^= true;
    slot_UpdateAllUi();
}

/****************************************************************************
 * @function name: VecInterpreter::fileReadLine - ---
 *
 * @param:
 *
    QByteArray &line
 * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::fileReadLine(QByteArray &line)
{
    char buf[100];
    if (m_file.readLine(buf, 100) > 0 )
    {
        line = QByteArray(buf).replace("\r", "").replace("\n", "");
    }
}


/****************************************************************************
 * @function name: VecInterpreter::fileParseSignalLine - ---
 *
 * @param:
 *
 *      Signal* psignal
 *    const QByteArray &line
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool VecInterpreter::fileParseSignalLine(Signal *psignal, const QByteArray &line)
{
    QList<QByteArray> l = line.split(' ');
    if (l.count() >= 6)
    {
        qint32 index = 0;
        bool ok;
        psignal->name = l[index++];
        if (l[index] == QByteArray("input"))
            psignal->direction = Signal::INPUT;
        else if (l[index] == QByteArray("output"))
            psignal->direction = Signal::OUTPUT;
        else if (l[index] == QByteArray("io"))
            psignal->direction = Signal::IO;
        else if (l[index] == QByteArray("control"))
            psignal->direction = Signal::CONTROL;
        else if (l[index] ==  QByteArray("clock"))
            psignal->direction = Signal::CLOCK;
        index++;

        psignal->t1 = l[index++].toDouble(&ok);
        psignal->t2 = l[index++].toDouble(&ok);

        if (l[index] == QByteArray("edge"))
            psignal->tge_type = Signal::EDGE;
        else if (l[index] == QByteArray("window"))
            psignal->tge_type  = Signal::WINDOW;
        else if (l[index] == QByteArray("rto"))
            psignal->tge_type  = Signal::RTO;
        else if (l[index] == QByteArray("comp"))
            psignal->tge_type  = Signal::COMP;
        else if (l[index] == QByteArray("mwindow"))
            psignal->tge_type  = Signal::MWINDOW;
        else if (l[index] == QByteArray("mrto"))
            psignal->tge_type  = Signal::MRTO;
        index++;

        if (psignal->direction == Signal::MRTO || psignal->direction == Signal::MWINDOW)
        {
            psignal->mclockrate = l[index++].toDouble();
        }

        psignal->pin_number =  l[index++].toInt();
        if ( index < l.count())
        {
            psignal->notest = l[index].toInt();
        }
        return true;
    }
    return false;
}


/****************************************************************************
 * @function name: VecInterpreter::clearData - ---
 *
 * @param:
 *             void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::clearData()
{
    m_Signals.clear();
    qint32 i = m_VectorsList.count();
    while (i--)
    {
        delete m_VectorsList.at(i);
    }
    m_VectorsList.clear();
    m_clockRate = 0;
    m_CurrentStep = 0;
    ui->listView->clear();
}


/****************************************************************************
 * @function name: VecInterpreter::populateSignals - ---
 *
 * @param:
 *             void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::populateSignals()
{
    qint32 i;
    QString format;
    Signal *psignal ;
    ui->texSignals->clear();
    format = "<span style='background-color:#f1f1f1'>"
            "<p style='color:red'>NUMBER: %1</p>"
            "<p>NAME: %2</p>"
            "<p>TYPE: %3</p>"
            "<p>TGE1/STR1: %4</p>"
            "<p>TGE2/STR2: %5</p>"
            "<p>TGE/STR type: %6</p>"
            "<p>Out mode: %7</p>"
            "<p>Control Signal: %8</p>"
            "</span>";

    for (i = 0; i < m_Signals.size(); i++)
    {
        psignal = const_cast<Signal*>(&m_Signals.at(i));
        QString text = format.arg(m_Signals.at(i).pin_number)
                .arg(psignal->name)
                .arg(psignal->direction)
                .arg(QString::number(psignal->t1 * 1e+06) + "uS")
                .arg(QString::number(psignal->t1 * 1e+06) + "uS")
                .arg((qint32)psignal->tge_type)
                .arg(0)
                .arg(psignal->notest);
        ui->texSignals->append(text);
    }

}


/****************************************************************************
 * @function name: VecInterpreter::slot_LoadFile - ---
 *
 * @param:
 *
    void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::slot_LoadFile(void)
{
    QString filename = QFileDialog::getOpenFileName(0, "Load vec file",
                                                    "",
                                                    "Files (*.vec)");

    enum
    {
        FILE_HEADER,
        FILE_SIGNALS,
        FILE_VECTORS,
        FILE_STOP
    } m_fileStep;
    
    if (!filename.isEmpty())
    {
        clearData();
        mp_dialog->show();
        mp_dialog->setText("Parsing Vec file");

        m_file.setFileName(filename);

        if (m_file.open(QFile::ReadOnly))
        {
            QByteArray line;
            qint32 line_no = 0;
            m_fileStep = FILE_HEADER;
            do
            {
                fileReadLine(line);
                if (line.size() <= 0)
                {
                    m_fileStep = FILE_STOP;
                }
                switch (m_fileStep)
                {
                case FILE_HEADER:
                    if (line_no == 0)
                    {
                        if (!line.contains("ADS"))
                        {
                            // check
                            m_fileStep = FILE_STOP;
                        }//header
                    }
                    else if (line_no == 1)
                    {
                        m_clockRate = line.toDouble();
                        ui->leClockRate->setText(QString::number(m_clockRate * 1e+06) + "uS");
                        m_fileStep = FILE_SIGNALS;
                    }
                    break;

                case  FILE_SIGNALS:
                    if (line.contains("#beginvectors"))
                    {
                        m_fileStep = FILE_VECTORS;
                    }
                    else
                    {
                        Signal *psignal = new Signal;
                        if (fileParseSignalLine(psignal, line))
                        {
                            m_Signals.append(*psignal);
                        }
                        else
                        {
                            delete psignal;
                            psignal = NULL;
                        }
                    }

                    break;

                case FILE_VECTORS:
                    if (!line.contains("#endvectors"))
                    {
                        addItem(line);
                        // todo add log
                    }
                    else
                    {
                        m_fileStep = FILE_STOP;
                    }
                    break;

                default:
                    break;
                }
                line_no++;
                mp_dialog->setProgress(line_no, 10000);
            }
            while (!m_file.atEnd() && m_fileStep != FILE_STOP);
            m_file.close();
        }

        /* populate * signals table*/
        populateSignals();
        slot_UpdateAllUi();
        slot_UpdateCursor();
        mp_dialog->hide();
    }
}


/****************************************************************************
 * @function name: VecInterpreter::slot_SaveFile - ---
 *
 * @param:
 *   void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::slot_SaveFile(void)
{
    QString filename = QFileDialog::getSaveFileName(0, "Save RESULTS to csv",
                                                    "",
                                                    "Files (*.csv)");
    m_file.setFileName(filename);
    if (m_file.open(QFile::ReadWrite))
    {
        m_file.write("vector;result\n");
        qint32 i;
        for (i = 0; i < m_VectorsList.count(); i++)
        {
            m_file.write(QByteArray(m_VectorsList.at(i)->ctrlVector) + ";");
            m_file.write(QByteArray(m_VectorsList.at(i)->outVector) + "\n");
        }
        m_file.close();
    }
}



/****************************************************************************
 * @function name: VecInterpreter::slot_Run - ---
 *
 * @param:
 *             void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::slot_Run()
{    
    mp_runner->setCommand(Runner::CMD_RUN_TO_BREAK);
}


/****************************************************************************
 * @function name: VecInterpreter::slot_Step - ---
 *
 * @param:
 *             void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::slot_Step()
{
    mp_runner->setCommand(Runner::CMD_MAKE_ONE_STEP);
}


/****************************************************************************
 * @function name: VecInterpreter::slot_Restart - ---
 *
 * @param:
 *             void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::slot_Restart()
{
    m_CurrentStep = 0;
    slot_UpdateCursor();
}


/****************************************************************************
 * @function name: VecInterpreter::slot_RepExec - ---
 *
 * @param:
 *             void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::slot_RepExec()
{
    m_multipleRunCount = ui->leRepeat->text().toInt();
    m_multipleRunStart = ui->leBegin->text().toInt();
    m_multipleRunStop = ui->leEnd->text().toInt();
    m_CurrentStep = m_multipleRunStart;
    ui->progressBar->setMaximum(m_multipleRunCount);
    slot_UpdateCursor();
    setDoNotUpdateUi(true);
    mp_runner->setCommand(Runner::CMD_RUN_MANY_TIMES);
}


/****************************************************************************
 * @function name: VecInterpreter::slot_ClearResults - ---
 *
 * @param:
 *             void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::slot_ClearResults()
{
    qint32 i = m_VectorsList.size();
    while (i--)
    {
        Vector *p = m_VectorsList.at(i);
        //p->breakpoint = false;
        p->result = Vector::VEC_NOTHING;
        qint32 j = signalLength();
        while (j--)
            p->outVector[j] = 0;
    }
    slot_UpdateAllUi();
}


/****************************************************************************
 * @function name: VecInterpreter::slot_NextError - ---
 *
 * @param:
 *             void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::slot_NextError()
{
    qint32 i = m_VectorsList.size() - 1;
    for (i = m_CurrentStep + 1; i < m_VectorsList.size(); i++)
    {
        if (m_VectorsList.at(i)->result == Vector::VEC_ERROR)
        {
            m_CurrentStep = i;
            break;
        }
    }
    slot_UpdateCursor();
}


/****************************************************************************
 * @function name: VecInterpreter::slot_PrevError - ---
 *
 * @param:
 *             void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::slot_PrevError()
{
    qint32 i = m_VectorsList.size() - 1;
    for (i = m_CurrentStep - 1; i > 0; i--)
    {
        if (m_VectorsList.at(i)->result == Vector::VEC_ERROR)
        {
            m_CurrentStep = i;
            break;
        }
    }
    slot_UpdateCursor();
}


/****************************************************************************
 * @function name: VecInterpreter::slot_Goto - ---
 *
 * @param:
 *             void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::slot_Goto()
{
    setCursor(ui->leCurrentVector->text().toInt());
    slot_UpdateCursor();
}


/****************************************************************************
 * @function name: VecInterpreter::slot_SetActiveStop - ---
 *
 * @param:
 *             void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::slot_SetActiveStop()
{
    setCursor(ui->leEnd->text().toInt());
    slot_UpdateCursor();
}


/****************************************************************************
 * @function name: VecInterpreter::slot_SetActiveStart - ---
 *
 * @param:
 *             void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::slot_SetActiveStart()
{
    setCursor(ui->leBegin->text().toInt());
    slot_UpdateCursor();
}


/****************************************************************************
 * @function name: VecInterpreter::slot_UpdateProgressbar - ---
 *
 * @param:
 *             void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::slot_UpdateProgressbar(qint32 cur, qint32 max)
{
    ui->progressBar->setValue(cur);
    ui->progressBar->setMaximum(max);
}


/****************************************************************************
 * @function name: VecInterpreter::slot_IgnoreErrors - ---
 *
 * @param:
 *             void * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::slot_IgnoreErrors()
{
    this->setIgnoreErrors( ui->cbIgnoreErrors->isChecked());
}

/****************************************************************************
 * @function name: VecInterpreter::slot_UpdateInfo()
 *
 * @param:
 *             const QString &info
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::slot_UpdateInfo(const QString &info)
{
    this->ui->lbInfo->setText(info);
}

/****************************************************************************
 * @function name: VecInterpreter::multipleRunCount - ---
 *
 * @param:
 *             void
 * @description:
 * @return: ( qint32 )
 ****************************************************************************/
qint32 VecInterpreter::multipleRunCount() const
{
    return m_multipleRunCount;
}


/****************************************************************************
 * @function name: VecInterpreter::setMultipleRunCount - ---
 *
 * @param:
 *
    const qint32 &multipleRunCount
 * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::setMultipleRunCount(const qint32 &multipleRunCount)
{
    m_multipleRunCount = multipleRunCount;
}


/****************************************************************************
 * @function name: VecInterpreter::cursorStop - ---
 *
 * @param:
 *             void
 * @description:
 * @return: ( qint32 )
 ****************************************************************************/
qint32 VecInterpreter::cursorStop() const
{
    return m_multipleRunStop;
}


/****************************************************************************
 * @function name: VecInterpreter::setCursorStop - ---
 *
 * @param:
 *
 *   const qint32 &multipleRunStop
 * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::setCursorStop(const qint32 &multipleRunStop)
{
    m_multipleRunStop = multipleRunStop;
}


/****************************************************************************
 * @function name: VecInterpreter::cursorStart - ---
 *
 * @param:
 *             void
 * @description:
 * @return: ( qint32 )
 ****************************************************************************/
qint32 VecInterpreter::cursorStart() const
{
    return m_multipleRunStart;
}


/****************************************************************************
 * @function name: VecInterpreter::setCursorStart - ---
 *
 * @param:
 *
 *   const qint32 &multipleRunStart
 * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::setCursorStart(const qint32 &multipleRunStart)
{
    m_multipleRunStart = multipleRunStart;
}


/****************************************************************************
 * @function name: VecInterpreter::ignoreErrors - ---
 *
 * @param:
 *             void
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool VecInterpreter::ignoreErrors() const
{
    return m_ignoreErrors;
}


/****************************************************************************
 * @function name: VecInterpreter::setIgnoreErrors - ---
 *
 * @param:
 *    bool ignoreErrors
 * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::setIgnoreErrors(bool ignoreErrors)
{
    m_ignoreErrors = ignoreErrors;
}


/****************************************************************************
 * @function name: VecInterpreter::cursor - ---
 *
 * @param:
 *             void
 * @description:
 * @return: ( qint32 )
 ****************************************************************************/
qint32 VecInterpreter::cursor()
{
    return m_CurrentStep;
}


/****************************************************************************
 * @function name: VecInterpreter::setCursor - ---
 *
 * @param:
 *    qint32 position
 * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::setCursor(qint32 position)
{
    if (position >= 0 && position < m_VectorsList.size())
    {
        m_CurrentStep = position;
    }
}


/****************************************************************************
 * @function name: VecInterpreter::setResult - ---
 *
 * @param:
 *
    qint32 no
     const QByteArray result
 * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::applyResult(qint32 no, const QByteArray result)
{
    if (result.size() == signalLength())
    {
        Vector *pvc = m_VectorsList.at(no);
        memcpy(pvc->outVector, result.constData(), result.size());
        pvc->result = compareResult(no);
        QString text =  generateItemText(no);
        if( !doNotUpdateUi() )
            ui->listView->setRowText(no, text);
    }
}


/****************************************************************************
 * @function name: VecInterpreter::addItem - ---
 *
 * @param:
 *
    const QByteArray &control_vector
 * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::addItem(const QByteArray &control_vector)
{
    //add item to table
    if (control_vector.size() == signalLength())
    {
        Vector *pvc = new Vector(control_vector.constData(), control_vector.size());
        m_VectorsList.append(pvc);
        ui->listView->addRow(QString());
    }
    else
    {
        signal_log(0, "signal count <> signals between #endvector and #beginvector") ;
    }
}


/****************************************************************************
 * @function name: VecInterpreter::const()
 *
 * @param:
 *
 * @description:
 * @return: ( qint32 )
 ****************************************************************************/
qint32 VecInterpreter::count() const
{
    return m_VectorsList.count();
}


/****************************************************************************
 * @function name: VecInterpreter::generateItemText - ---
 *
 * @param:
 *
    qint32 no
 * @description:
 * @return: ( const )
 ****************************************************************************/
const QString VecInterpreter::generateItemText(qint32 no)
{
    QString result = "<p><b><font face='Courier New'>"; // generate colored output
    Vector *pvc = m_VectorsList.at(no);
    if (pvc->breakpoint)
    {
        result += QString("<span style='background-color:yellow'>B&nbsp;</span>");
    }
    else
    {
        result += QString("<span>&nbsp;&nbsp;</span>");
    }
    char a[10];
    SPRINTF(a, "%06u", no);
    result += "<font color='magenta'>" + QString(a) + "</font>";
    result += "&nbsp;&nbsp;";
    result += QString(pvc->ctrlVector);
    result += "&nbsp;&nbsp;";
    qint32 result_size = strlen(pvc->outVector);
    if (result_size < signalLength())
    {
        //no result
        for (qint32 i = 0; i < signalLength(); i++ )
        {
            result += QString('.');
        }
    }
    else
    {
        // result is available
        for (qint32 i = 0; i < signalLength(); i++ )
        {
            char c = pvc->outVector[i];
            if ((pvc->ctrlVector[i] != VECTOR_UNKNOWN)
                    && (pvc->ctrlVector[i] != c)
                    && (m_Signals.at(i).direction ==  Signal::OUTPUT))
            {
                result += "<span style='color:red'>" + QString(c) + "</span>";
            }
            else
            {
                result += "<span style='color:blue'>" + QString(c) + "</span>";
            }
        }
        result += "&nbsp;&nbsp;";
        if (pvc->result == Vector::VEC_ERROR)
        {
            result += QString("<span style='color:red'>ERROR</span>");
        }
        else
        {
            result += QString("<span style='color:green'>pass</span>");
        }
    }

    result += "</font></b></p>";
    return result;
}

/****************************************************************************
 * @function name: VecInterpreter::setDoNotUpdateUi()
 *
 * @param:
 *             bool on
 * @description:
 * @return: ( void )
 ****************************************************************************/
void VecInterpreter::setDoNotUpdateUi(bool on)
{
    m_do_not_update_ui =on;
}

/****************************************************************************
 * @function name: VecInterpreter::doNotUpdateUi()
 *
 * @param:
 *
 * @description:
 * @return: ( const bool )
 ****************************************************************************/
const bool VecInterpreter::doNotUpdateUi() const
{
    return m_do_not_update_ui;
}

/****************************************************************************
 * @function name: VecInterpreter::signalLength - ---
 *
 * @param:
 *             void
 * @description:
 * @return: ( qint32 )
 ****************************************************************************/
qint32 VecInterpreter::signalLength() const
{
    return m_Signals.size();
}





/****************************************************************************
 * @function name: CONSTRUCTOR
 *
 * @param:
 *    VecInterpreter *pvi
 * @description:
 * @return: (  )
 ****************************************************************************/
Runner::Runner(VecInterpreter *pvi)
    : QThread(0)
{
    m_pvi = pvi;
    start(QThread::NormalPriority);
    m_command = CMD_STOP;
    m_SleepTime = 0;
}



/****************************************************************************
 * @function name: DESTRUCTOR
 *
 * @param:
 *             void
 * @description:
 * @return: (  )
 ****************************************************************************/
Runner::~Runner()
{

}


/****************************************************************************
 * @function name: Runner::run - ---
 *
 * @param:
 *             void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void Runner::run()
{
    while (1)
    {
        while (m_command != CMD_STOP)
        {
            checkEnd();
            switch (m_command )
            {
            case CMD_MAKE_ONE_STEP:
                runOneLine();
                emit signal_UpdateCursor();
                break;

            case CMD_RUN_TO_BREAK:
                runToBreak();
                emit signal_UpdateCursor();
                break;

            case CMD_RUN_MANY_TIMES:
                runManyTimes();
                break;

            default:
                break;
            }
        }
        msleep(200);
        if(m_SleepTime)
            msleep(m_SleepTime);
    }
}

/****************************************************************************
 * @function name: Runner::command - ---
 *
 * @param:
 *             void
 * @description:
 * @return: ( Runner::Command )
 ****************************************************************************/
Runner::Command Runner::command() const
{
    return m_command;
}


/****************************************************************************
 * @function name: Runner::setCommand - ---
 *
 * @param:
 *    const Command &command
 * @description:
 * @return: ( void )
 ****************************************************************************/
void Runner::setCommand(const Command &command)
{
    if(m_pvi->count()>0)
    {
        m_command = command;
        if(command == CMD_RUN_MANY_TIMES)
        {
            m_timeStart =  QDateTime::currentDateTime();
            emit signal_UpdateInfo(QString(info_pattern)
                                   .arg(m_timeStart.toString(timedate_format))
                                   .arg(0)
                                   .arg(0)
                                   );
        }
    }
}


/****************************************************************************
 * @function name: Runner::runManyTimes()
 *
 * @param:
 *             void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void Runner::runManyTimes()
{

    QElapsedTimer elapse_timer;
    elapse_timer.start();

    m_pvi->step();
    if (m_pvi->cursor() >= m_pvi->cursorStop())
    {
        //restart
        m_pvi->setMultipleRunCount(m_pvi->multipleRunCount() - 1);
        if (m_pvi->multipleRunCount() == 0)
        {
            m_command = CMD_STOP;
            m_pvi->setDoNotUpdateUi(false);
            m_pvi->slot_UpdateAllUi();
        }
        else
        {
            m_pvi->setCursor(m_pvi->cursorStart());
        }
    }
    else
    {
        m_pvi->setCursor(m_pvi->cursor() + 1);
    }

    static quint32 update_time_to_complete = 100;
    if(--update_time_to_complete==0)
    {
        quint64 start = m_timeStart.toMSecsSinceEpoch();
        quint64 elapsed = QDateTime::currentMSecsSinceEpoch() - start;
        quint64 complete = start + elapsed+elapse_timer.elapsed() * (m_pvi->cursorStop()-m_pvi->cursor());
        emit signal_UpdateProgressBar(m_pvi->cursor() ,m_pvi->cursorStop());
        emit signal_UpdateInfo(QString(info_pattern)
                               .arg(this->m_timeStart.toString(timedate_format))
                               .arg(elapsed/1000)
                               .arg(QDateTime::fromMSecsSinceEpoch(complete).toString(timedate_format))
                               );
        update_time_to_complete = 100;
    }


}


/****************************************************************************
 * @function name: Runner::runToBreak()
 *
 * @param:
 *             void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void Runner::runToBreak()
{
    // break point
    if (m_pvi->isVectorStepBreak())
    {
        m_command = CMD_STOP;
    }
    m_pvi->step();
    //on error
    if ( m_pvi->isVectorStepError() && !m_pvi->ignoreErrors())
    {
        m_command = CMD_STOP;
    }
    //at end
    if ( m_pvi->cursor() == m_pvi->count() - 1)
    {
        m_command = CMD_STOP;
    }
    m_pvi->setCursor(m_pvi->cursor() + 1);
}

/****************************************************************************
 * @function name: Runner::runOneLine()
 *
 * @param:
 *             void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void Runner::runOneLine()
{
    m_pvi->step();
    m_command = CMD_STOP;
    m_pvi->setCursor(m_pvi->cursor() + 1);
}


/****************************************************************************
 * @function name: Runner::checkEnd()
 *
 * @param:
 *             void
 * @description:
 * @return: ( void )
 ****************************************************************************/
void Runner::checkEnd()
{
    if ( m_pvi->cursor() >= m_pvi->count())
    {
        m_command = CMD_STOP;
        m_pvi->setCursor(m_pvi->count() - 1);
    }
}

/****************************************************************************
 * @function name: Runner::SleepTime - ---
 *
 * @param:
 *             void
 * @description:
 * @return: ( qint32 )
 ****************************************************************************/
qint32 Runner::SleepTime() const
{
    return m_SleepTime;
}


/****************************************************************************
 * @function name: Runner::setSleepTime - ---
 *
 * @param:
 *   const qint32 &SleepTime
 * @description:
 * @return: ( void )
 ****************************************************************************/
void Runner::setSleepTime(const qint32 &SleepTime)
{
    m_SleepTime = SleepTime;
}



/****************************************************************************
 * @function name: r - ---
 *
 * @param:
 *    const char *vector_data
 *    qint32 size
 * @description:
 * @return: (  )
 ****************************************************************************/
Vector::Vector(const char *vector_data, qint32 size)
{
    memset(ctrlVector, 0x00, MAX_SIGNAL_SIZE);    
    memset(outVector, 0x00, MAX_SIGNAL_SIZE);
    memcpy(ctrlVector, vector_data , size);
    breakpoint = false;
    result = VEC_NOTHING;
}


/****************************************************************************
 * @function name: CustomMdiChild::step()
 *
 * @param:
 *             void
 * @description:
 * @return: (const QVariant)
 ****************************************************************************/
void VecInterpreter::step()
{

        QByteArray data = m_VectorsList.at(m_CurrentStep)->ctrlVector;
        emit signal_transferVecData(data);
        applyResult(m_CurrentStep, data);    
}

/****************************************************************************
 * @function name: CustomMdiChild::bindApplet()
 *
 * @param:
 *       Applet *papplet
 * @description:
 * @return: (void)
 ****************************************************************************/
void VecInterpreter::bindApplet(Applet *papplet)
{
    connect(this,SIGNAL(signal_transferVecData(QByteArray&))
		,papplet,SLOT(transferVecData(QByteArray&)),Qt::BlockingQueuedConnection);
    ui->lbAppletConnected->setText(papplet->name());
}

/****************************************************************************
 * @function name: CustomMdiChild::on_pbStop_clicked()
 *
 * @param:
 *
 * @description:
 * @return: (void)
 ****************************************************************************/
void VecInterpreter::on_pbStop_clicked()
{
    mp_runner->setCommand(Runner::CMD_STOP);
}
