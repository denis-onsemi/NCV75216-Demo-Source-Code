/**
 * @file:logs.cpp
 * @description:
 *    This is Logs window applet
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-50-31
 *
 */
#include "logs.h"
#include <QDateTime>
#include "beeper.h"
#include "mainapp.h"
#include <QMessageBox>
#include <QFile>
#include <QClipboard>                              //

Logs *gp_Logs;


/****************************************************************************
 * @function name: CONSTRUCTOR
 *
 * @param:
 *
 * @description:
 ****************************************************************************/
Logs::Logs(QWidget *parent) :
    Applet(parent)

{        
    mp_log_file = NULL;
    createCustomUi();    
}

/****************************************************************************
 * @function name: DESTRUCTOR
 *
 * @description:
 ****************************************************************************/
Logs::~Logs()
{

}
QMenu *mp_menu;
/****************************************************************************
 * @function name: Logs::createCustomUi()
 *
 * @param:
 *
 * @description: user interface
 * @return: (void)
 ****************************************************************************/
void Logs::createCustomUi()
{
    setObjectName("Logs");
    resize(535, 367);
    QIcon icon;
    icon.addFile(":/buttons/log");
    setWindowIcon(icon);
    pleControl = new QLineEdit(this);
    gridLayout = new QGridLayout(this);
    pbClear = new QPushButton(this);
    mp_list = new CustomControls::HtmlListBox(this);
    mp_cbSaveLog = new QCheckBox(this);
    mp_cbMessageBoxes = new QCheckBox("MessageBox",this);
    mp_cbTranslater =  new QCheckBox("Translater",this);
    gridLayout->addWidget(mp_list, 1, 0, 1, 3);
    gridLayout->addWidget(mp_cbSaveLog, 0, 0, 1, 3);
    gridLayout->addWidget(pleControl,2,0,1,3);
    gridLayout->addWidget(pbClear, 3, 0, 1, 1);
    gridLayout->addWidget(mp_cbTranslater, 3, 1, 1, 1);
    gridLayout->addWidget(mp_cbMessageBoxes, 3, 2, 1, 1);
    mp_cbMessageBoxes->setChecked(1);
    setWindowTitle("Logs");
    pbClear->setText("Clear");
    mp_cbSaveLog->setText(QDate::currentDate().toString("yyyyMMdd")+".log");
    mp_sound_on = new QPushButton(QIcon(":/buttons/horn"),"");
    mp_sound_on->setCheckable(1);
    mp_sound_on->setChecked(1);
    mp_sound_on->setFixedSize(23,23);
    mp_status =  new QLabel();
    mp_status->setFixedWidth(120);
    createContainerWindow(DOCK,MAINAPP->mainWindow());
    mp_list->setContextMenuPolicy(Qt::CustomContextMenu);
    mp_menu = new QMenu();
    QAction *paction = new QAction("Copy",0);
    connect(paction,SIGNAL(triggered()),this,SLOT(slot_ActionCopy()));
    mp_menu->addAction(paction);
    paction = new QAction("Copy All",0);
    paction->setEnabled(1);
    connect(paction,SIGNAL(triggered()),this,SLOT(slot_ActionCopyAll()));
    mp_menu->addAction(paction);
    connect(pleControl,SIGNAL(returnPressed()),this,SLOT(slot_ControllerSend()));
    connect(this->pbClear,SIGNAL(clicked()),this,SLOT(slot_pbClear_clicked()));
    connect(this->mp_cbSaveLog,SIGNAL(toggled(bool)),this,SLOT(slot_cbSaveLog_toggled(bool)));
    connect(this->mp_list,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(slot_LogContextMenu(QPoint)));    
}

/****************************************************************************
 * @function name: Logs::runAppCommand()
 *
 * @param:
 *             const QString &name
 *             const QVariant &data
 * @description: only one command impleneted here is log message in format
 *                                                          " color;message "
 * @return: ( bool )
 ****************************************************************************/
QVariant Logs::runAppCommand(const QString &name, const QVariant &data)
{
    if (!name.isEmpty() && !data.isNull())
    {
        if ( name.toLower() == "log")
        {
            slot_addLog(LOG_NO_ERROR, data.toString());
            return true;
        }
    }
    return false;
}


/****************************************************************************
 * @function name: Logs::initialize()
 *
 * @param:
 *             void
 * @description:
 * @return: (bool)
 ****************************************************************************/
bool Logs::initialize()
{        
    MAINAPP->mainWindow()->addWidget(mp_sound_on);
    MAINAPP->mainWindow()->addWidget(mp_status);
    mp_list->clear();
    resetStatus();
    return true;
}

void Logs::assignSignalNames(QStringList tags, QStringList signal_names)
{
    QStringList::const_iterator it = signal_names.begin();
    foreach(const QString &tag, tags)
    {
        if(!tag.trimmed().isEmpty())
        {
            if(it != signal_names.end())
                m_dict[tag] = *it;
        }
        ++it;
    }
}



/****************************************************************************
 * @function name: Logs::slot_addLog()
 *
 * @param:
 *             int type
 *             const QString text
 * @description: adds log message to window
 * @return: ( void )
 ****************************************************************************/
void Logs::slot_addLog(int type , const QString text)
{

    QString color;
    QString datetime_text;
    QString html;
    // errors
    if( type <0)
    {
        color = "red";
        if( mp_sound_on->isChecked() )
            new Beeper("1000:100;1200:200;2000:100");
        html = QString("C(%1) [%2]").arg(type).arg(QDateTime::currentDateTime().toString());
        if(mp_cbMessageBoxes->isChecked()) QMessageBox::critical(0,"Error",text);
        m_error_count++;
    }
    // information
    else if( type ==0 )
    {
        m_inf_count++;
        color = "black";
    }
    // some message with color(no error)
    else if( type >100 )
    {
        color = QColor((Qt::GlobalColor)(type-100)).name();
    }
    else {
        color="magenta";
        m_warn_count++;
    }

    //errors will be added always (type<0) all errors code <0
    if(MAINAPP->boardLogMode() || type<0)
    {
        html += QString("<font color=\"%1\"> %2</font> <br>")
                .arg(color)
                .arg(retranslateByDict(text))            ;

        if(mp_log_file && mp_log_file->isOpen())
        {
            mp_log_file->write(QString("C(%1) %2 ::: %3 \n")
                               .arg(type)
                               .arg(datetime_text)
                               .arg(text).toLatin1() );
            mp_log_file->flush();
        }
        mp_list->addRow(html);
        mp_list->setCurrentRow(mp_list->count()-1);
    }
    updateStatus();
}

/****************************************************************************
 * @function name: Logs::slot_pbClear_clicked()
 *
 * @param:
 *
 * @description: clear clicked
 * @return: ( void )
 ****************************************************************************/
void Logs::slot_pbClear_clicked()
{
    mp_list->clear();
}

/****************************************************************************
 * @function name: Logs::slot_cbSaveLog_toggled()
 *
 * @param:
 *
 * @description: save log checkbox clicked
 * @return: ( void )
 ****************************************************************************/
void Logs::slot_cbSaveLog_toggled(bool checked)
{
    if(mp_log_file == NULL)
    {
        mp_log_file= new QFile(mp_cbSaveLog->text());
    }
    if(checked && !mp_log_file->isOpen())
    {
        if(QFile::exists(mp_cbSaveLog->text()))
        {
            mp_log_file->open(QFile::Append);
        }
        else
        {
            mp_log_file->open(QFile::WriteOnly);
        }
    }
    else if(!checked)
    {
        mp_log_file->close();
    }
}

void Logs::slot_LogContextMenu(QPoint pos)
{
    mp_menu->exec(this->mapToGlobal(pos));
}

void Logs::slot_ActionCopy()
{
    QString text = mp_list->rowText(mp_list->currentRow());
    int l = text.indexOf('>')+1;
    int r = text.indexOf("</");
    qApp->clipboard()->setText(QString(text.mid(l,r-l)).replace("&lt;cr&gt;","<cr>"));
}

void Logs::slot_ActionCopyAll()
{
    QString clipboard;
    int rows = mp_list->count();
    for(int i=0;i<rows;i++)
    {
        QString text = mp_list->rowText(i);
        int l = text.indexOf('>')+1;
        int r = text.indexOf("</");
        clipboard += QString(text.mid(l,r-l)).replace("&lt;cr&gt;","<cr>")+"\n";
    }
    qApp->clipboard()->setText(clipboard);
}

void Logs::slot_ControllerSend()
{
#ifdef USE_CONTROLLER
    if(pleControl->text().contains("<cr>"))
        MCU.transferData(QString(pleControl->text()).replace("<cr>","\n").toLatin1());
    else MCU.transferData(pleControl->text().toLatin1()+"\n");

#elif USE_MPP
    if(pleControl->text().contains("<cr>"))
        MPP.transferData(QString(pleControl->text()).replace("<cr>","\n").toLatin1());
    else MPP.transferData(pleControl->text().toLatin1()+"\n");
#endif
}

void Logs::slot_ResetStatus()
{
    resetStatus();
}

void Logs::updateStatus()
{
    mp_status->setText(QString("Err:<font color=red>%1</font> Warn:<font color=magenta>%2</font> Inf:<font color=black>%3</font>")
                       .arg(m_error_count).arg(m_warn_count).arg(m_inf_count));
    mp_status->setToolTip(mp_status->text());
}

void Logs::resetStatus()
{
    m_error_count=m_warn_count=m_inf_count=0;
    updateStatus();
}


QString Logs::retranslateByDict(const QString &text)
{
    QString result = text;
    if(mp_cbTranslater->isChecked())
    {
        QStringList sorted_tags = m_dict.keys();
        qSort(sorted_tags.begin(),sorted_tags.end(),qGreater<QString>());
        foreach(const QString &tag,sorted_tags)
        {
            result.replace(tag,m_dict[tag]);
        }
    }
    return result;
}

