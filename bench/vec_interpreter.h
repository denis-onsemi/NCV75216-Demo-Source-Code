/**
 * @file:vec_interpreter.h   -
 * @description:
 * @project: BENCH OnSemiconductor
 * @date: 2014\12\04 13-58-04
 *
 */


#ifndef VEC_INTEPRETER_H
#define VEC_INTEPRETER_H
#include "applet.h"

#define MAX_SIGNAL_SIZE       (50)
#define VECTOR_UNKNOWN        ('X')
#define VECTOR_ZERO           ('0')
#define VECTOR_ONE            ('1')
#define VECTOR_PULLDOWN       ('L')
#define VECTOR_PULLUP         ('H')
#define VECTOR_HIGHIMPEDANCE  ('Z')
#define VECTOR_UNKNOWN_U      ('U')
#define VECTOR_IO             ('-')


#include <QWidget>
#include <QThread>
#include <QFile>
#include <QDateTime>
#include "customdialogs.h"

namespace Ui
{
class VecInterpreter;
}


class VecInterpreter;
class Runner;
struct Vector;
struct Signal;

/* vector is a step to do in list */
struct Vector
{

    typedef enum
    {
        VEC_NOTHING,
        VEC_SUCCESS,
        VEC_ERROR
    } Result;

    Vector(const char *vector_data, qint32 size);
    char ctrlVector[MAX_SIGNAL_SIZE];
    char outVector[MAX_SIGNAL_SIZE];
    bool breakpoint;
    Result result;
};

/* signal is a setting for each signal in step*/
struct Signal
{
    typedef enum
    {
        EDGE,
        WINDOW,
        RTO,
        COMP,
        MWINDOW,
        MRTO
    } TgeType;

    typedef enum
    {
        INPUT,
        OUTPUT,
        IO,
        CONTROL,
        CLOCK
    } Direction;

    QString name;
    Direction direction;
    double t1;
    double t2;
    double mclockrate;
    TgeType tge_type;
    qint32 pin_number;
    bool notest;
};


/****************************************************************************
 * @class :
 * VecInterpreter
 *
 ****************************************************************************/

class VecInterpreter: public Applet
{
    Q_OBJECT

public:
    explicit VecInterpreter(QWidget *parent = 0);
    ~VecInterpreter();
    /* if vector interpreter step makes error*/
    bool isVectorStepError() const;
    /* if vectr interpreter step is break flag*/
    bool isVectorStepBreak() const;
    /* apply result line no, result is data*/
    void applyResult(qint32 no, const QByteArray result);
    /* signals available. actually charactetrs in step line*/
    typedef QList<Signal> Signals;    

    /* add item to vector steps list size == signals.size*/
    void addItem( const QByteArray &control_vector);
    /* count of vector steps*/
    qint32 count() const;
    /* if need to ignore errors*/
    bool ignoreErrors() const;
    /* set flag to ignore errors*/
    void setIgnoreErrors(bool ignoreErrors);
    /* current cursor */
    qint32 cursor();
    /* sets current cursor position*/
    void setCursor(qint32 position);
    /* cursor start for multiple run mode*/
    qint32 cursorStart() const;
    /* sets cursor start multiple run mode*/
    void setCursorStart(const qint32 &multipleRunStart);
    /* cursor stop for multiple mode*/
    qint32 cursorStop() const;
    /* set cursor stop for multiple mode*/
    void setCursorStop(const qint32 &multipleRunStop);
    /* multiple run count */
    qint32 multipleRunCount() const;
    /* sets multiple run count*/
    void setMultipleRunCount(const qint32 &multipleRunCount);
    /* for external cmmunication */
    virtual QVariant runAppCommand(const QString &, const QVariant &){return false;}
    /* makes one step cursor position incremented */
    void step();
    /* binds applet */
    void bindApplet(Applet *papplet);
    /* one multiple mode . useful to stop updateUi */
    void setDoNotUpdateUi(bool on);
    /* not updateUi flag getter*/
    const bool doNotUpdateUi()const;

public slots:
    /* updates all lines */
    void slot_UpdateAllUi();
    /* updates cursor position */
    void slot_UpdateCursor();
    /* clears all breakpoints*/
    void slot_BreakpointClearAll(void);
    /* toggles breakpoint under cursor(current line)*/
    void slot_BreakpointToggle(void);
    /* loads file with vectors*/
    void slot_LoadFile(void);
    /* saves file with vectors*/
    void slot_SaveFile(void);
    /* slot runs to break*/
    void slot_Run();
    /* makes single step */
    void slot_Step();
    /* restarts from first step*/
    void slot_Restart();
    /* run multiple times from start to stop*/
    void slot_RepExec();
    /* clears results of vector*/
    void slot_ClearResults();
    /* jumps to next error */
    void slot_NextError();
    /* jumps to previous error */
    void slot_PrevError();
    /* goto  line with number */
    void slot_Goto();
    /* sets cursor to stop line number*/ 
    void slot_SetActiveStop();
    /* sets cursor to start line number*/
    void slot_SetActiveStart();
    /* update progress bar fro thread*/
    void slot_UpdateProgressbar(qint32 cur, qint32 max);
    /* if ignore errors checkbox clicked*/
    void slot_IgnoreErrors();
    /* updates information :time start, elapsed, stop */
    void slot_UpdateInfo(const QString & info);
    /* pushbutton stop running clicked slot*/
    void on_pbStop_clicked();
signals:
    void signal_transferVecData(QByteArray &);

private:
    
    Signals m_Signals;
    /* errors accumulator*/
    qint32 m_errors_count;
    /* clock rate */
    double m_clockRate;
    /* vectors step list */
    QList<Vector *> m_VectorsList;
    /* current step number*/
    qint32 m_CurrentStep;
    /* multiple run start position variable*/
    qint32 m_multipleRunStart;
    /* multiple run stop variable*/
    qint32 m_multipleRunStop;
    /* multiple run count variable*/
    qint32 m_multipleRunCount;
    /* flag do not update ui*/
    bool m_do_not_update_ui;
    /* runner pointer */
    Runner *mp_runner;
    /* dialog progressbar*/
    DialogProgressBar *mp_dialog;
    /* current opened file */  
    QFile m_file;
    /* ui window*/
    Ui::VecInterpreter *ui;
    /* flag ignore errors */
    bool m_ignoreErrors;
    /* file parse signal line*/
    bool fileParseSignalLine(Signal *psignal, const QByteArray &line);
    /* file read single lne*/
    void fileReadLine(QByteArray &line);
    /* generates item text */
    const QString generateItemText(qint32 no);
    /* populates signals */
    void populateSignals();
    /* clears all data from vector steps list and results*/
    void clearData();
    /* signal length (equal to vector steo data size) */
    qint32 signalLength() const;
    /* compare result */
    Vector::Result compareResult(qint32 no);

protected:
    /* key press event for shortcuts*/
    void keyPressEvent(QKeyEvent *pev);

};


/****************************************************************************
 * @class :
 * Runner
 * this is only runner thread for vector interpreter
 ****************************************************************************/
class Runner: public QThread
{
    Q_OBJECT
    friend class VecInterpreter;
public:
    /* commands list*/
    typedef enum
    {
        CMD_STOP,
        CMD_MAKE_ONE_STEP,
        CMD_RUN_TO_ERROR,
        CMD_RUN_TO_BREAK,
        CMD_RUN_MANY_TIMES
    } Command;

    Runner(VecInterpreter *pvi);
    ~Runner();
    /* sleep time */
    qint32 SleepTime() const;
    /* sets sleep time */
    void setSleepTime(const qint32 &SleepTime);
    /* command to do */
    Command command() const;
    /* sets command to do */
    void setCommand(const Command &command);

protected:
    /* back link to vector ui intepreter class */
    VecInterpreter *m_pvi;
    /* run - thread function*/
    void run();

private:
    /* command to do variable*/
    Command m_command;
    /* sleep time between steps*/
    qint32 m_SleepTime;
    /* time when multiple run has started */
    QDateTime m_timeStart;
    /* time when elapsed till now*/
    QDateTime m_timeElapse;
    /* time when complete */
    QDateTime m_timeComplete;
    /* run many times operation*/
    inline void runManyTimes();
    /* runs one line */
    inline void runToBreak();
    /* runs till bfirst break met*/
    inline void runOneLine();
    /* check if end occurs */
    inline void checkEnd();

signals:    
    /* signal update cursor */
    void signal_UpdateCursor();
    /* updates progress bar */
    void signal_UpdateProgressBar(qint32 current, qint32 maximum);
    /* updates information about time elapses, time complete,...*/
    void signal_UpdateInfo(const QString & info);
};

extern VecInterpreter *gp_VecInterpreter;

#endif // VEC_INTEPRETER_H
