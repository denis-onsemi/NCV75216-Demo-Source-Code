#include "customdialogs.h"
#include <QRadioButton>
#include <QPushButton>
#include <Windows.h>
#include <QApplication>

/****************************************************************************
 * CONSTRUCTOR
 *          
 * @description:  Creates check boxes list 
 ****************************************************************************/
DialogCheckBoxes::DialogCheckBoxes(Type type, QWidget *parent) :
    QDialog(parent)
{

    mp_type = type;
    mp_list_widget =  new QListWidget(this);
    mp_layout = new QVBoxLayout(this);
    mp_buttonbox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    mp_checkboxes = new QList<QAbstractButton *>();
    mp_widgetitems = new QList<QListWidgetItem *>();
    mp_layout->addWidget(mp_list_widget);
    mp_layout->addWidget(mp_buttonbox);
    setLayout(mp_layout);

    if (type != RADIOBUTTON)
    {
        QPushButton *ppbOn = mp_buttonbox->addButton("All On", QDialogButtonBox::ActionRole);
        QPushButton *ppbOff = mp_buttonbox->addButton("All Off", QDialogButtonBox::ActionRole);
        connect(ppbOn, SIGNAL(clicked()), this, SLOT(slot_SetAllOn()));
        connect(ppbOff, SIGNAL(clicked()), this, SLOT(slot_SetAllOff()));
    }
    
    connect(mp_buttonbox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(mp_buttonbox, SIGNAL(rejected()), this, SLOT(close()));
}


/****************************************************************************
 * @function name: inflate
 *
 * @param:
 *          const QStringList &list
 * @description:
 * @return
 ****************************************************************************/
void DialogCheckBoxes::inflate(const QStringList &list)
{
    qint32 count = list.size();
    qint32 i = 0;
    while (count--)
    {
        QAbstractButton *pbutton = NULL;
        if (mp_type == CHECKBOX)
        {
            pbutton = new QCheckBox;
        }
        else if (mp_type == RADIOBUTTON)
        {
            pbutton = new QRadioButton;
            if(i==0) pbutton->setChecked(1);
        }
        else if( mp_type == BUTTON)
        {
            pbutton = new QPushButton;
            if(i==0) pbutton->setChecked(1);
        }
        QListWidgetItem *pwidgetitem = new QListWidgetItem;
        if (pbutton)
        {
            QStringList l = list[i].split('=');
            if(l.count() == 2)
            {
                pbutton->setText(l.at(0));
                pbutton->setChecked(l.at(1).toInt());
            }
            else {
                pbutton->setText(l.at(0));
            }
            pbutton->setFixedSize(QSize(150, 15));
            mp_widgetitems->append(pwidgetitem);
            mp_checkboxes->append(pbutton);
            mp_list_widget->addItem(pwidgetitem);
            mp_list_widget->setItemWidget(pwidgetitem, pbutton);
            i++;
        }
    }
}




/****************************************************************************
 * @function name: addWidget
 *
 * @param:
 *          QWidget *pwidget
 * @description:
 * @return: void
 ****************************************************************************/
void DialogCheckBoxes::addWidget(QWidget *pwidget)
{
    mp_layout->addWidget(pwidget);
}


/****************************************************************************
 * @function name: readStringList
 *
 * @param:
 *          QStringList &list
 * @description:
 * @return
 ****************************************************************************/
qint32 DialogCheckBoxes::readStringList(QStringList &list)
{
    qint32 i = 0;
    list.clear();
    for (i = 0; i < mp_checkboxes->count(); i++)
    {
        if ((*mp_checkboxes)[i]->isChecked())
        { list.append((*mp_checkboxes)[i]->text()); }
    }
    return list.count();
}


/****************************************************************************
 * @function name: readIndexList
 *
 * @param:
 *          QList<qint32> &list
 * @description:
 * @return:
 ****************************************************************************/
qint32 DialogCheckBoxes::readIndexList(QList<qint32> &list)
{
    qint32 i = 0;
    for (i = 0; i < mp_checkboxes->count(); i++)
    {
        if ((*mp_checkboxes)[i]->isChecked())
        { list.append(i); }
    }
    return list.count();
}


/****************************************************************************
 * @function name: slot_SetAllOn
 *
 * @param:
 *          
 * @description:
 * @return: void
 ****************************************************************************/
void DialogCheckBoxes::slot_SetAllOn()
{
    qint32 i = 0;
    for (i = 0; i < mp_checkboxes->count(); i++)
    {
        (*mp_checkboxes)[i]->setChecked(true);
    }
}


/****************************************************************************
 * @function name: slot_SetAllOff
 *
 * @param:
 *          
 * @description:
 * @return: void
 ****************************************************************************/
void DialogCheckBoxes::slot_SetAllOff()
{
    qint32 i = 0;
    for (i = 0; i < mp_checkboxes->count(); i++)
    {
        (*mp_checkboxes)[i]->setChecked(false);
    }
}



DialogCheckBoxes::~DialogCheckBoxes()
{
    for (qint32 i = 0; i < mp_checkboxes->count(); i++)
    {
        delete mp_checkboxes->at(i);
        delete mp_widgetitems->at(i);
    }
    mp_checkboxes->clear();
    mp_widgetitems->clear();
    delete mp_buttonbox;
    delete mp_layout;
    delete mp_list_widget;
    delete mp_widgetitems;
    delete mp_checkboxes;
}



/****************************************************************************
 * CONSTRUCTOR
 *          
 * @description: Create progressbar dialog
 ****************************************************************************/
DialogProgressBar::DialogProgressBar(QWidget *parent, const QPixmap &pixmap)
    : QDialog(parent)
{
    setLayout(&m_layout);
    this->setModal(1);
    m_layout.addWidget(&m_progress_bar);
    m_layout.addWidget(&m_label);
    if (!pixmap.isNull() )
    {
        m_image.setPixmap(pixmap);
        m_image.setFixedSize(200, 200);
    }
    m_layout.addWidget(&m_image);
}

DialogProgressBar::~DialogProgressBar()
{
}

/****************************************************************************
 * @function name: addWidget
 *
 * @param:
 *          
 * @description:
 * @return: void
 ****************************************************************************/
void DialogProgressBar::addWidget(QWidget *pwidget)
{

    m_layout.addWidget(pwidget);
}

/****************************************************************************
 * @function name: setProgress
 *
 * @param:
 *          
 * @description:
 * @return: void
 ****************************************************************************/
void DialogProgressBar::setProgress(qint32 value, qint32 maximum)
{
    m_progress_bar.setValue(value);
    m_progress_bar.setMaximum(maximum);
    QApplication::processEvents();
}


/****************************************************************************
 * @function name: progress
 *
 * @param:
 *          
 * @description:
 * @return: qint32
 ****************************************************************************/
qint32 DialogProgressBar::progress()
{
    return m_progress_bar.value();
}


/****************************************************************************
 * @function name: setText
 *
 * @param:
 *          
 * @description:
 * @return: void
 ****************************************************************************/
void DialogProgressBar::setText(const QString &text)
{
    m_label.setText(text);
    QApplication::processEvents();
}


/****************************************************************************
 * @function name: sleep
 *
 * @param:
 *          
 * @description:
 * @return: void
 ****************************************************************************/
void DialogProgressBar::sleep(qint32 sleep)
{
    Sleep(sleep);
}
