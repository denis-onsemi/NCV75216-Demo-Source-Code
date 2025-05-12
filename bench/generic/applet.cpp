/**
 * @file:applet.cpp   -
 * @description: This is applpet for creating new windows
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-52-19
 *
 */

#include "compiler.h"
#include "applet.h"
#include "mainapp.h"
#include <customdialogs.h>


/****************************************************************************
 * @function name: constructor
 *
 * @param:
 *
 *   QWidget *parent
 *
 *  OPTIONS
 *
 *   fixed_size - when form has fixed size. user cannot resize it with mouse
 *   auto_bind  - autobind all registers to CustomControls
 *   use_context_menu - use context menu
 *   do_not_save - do not save this applet in save config from mainwindow
 *
 * @description:
 * @return: (  )
 ****************************************************************************/
Applet::Applet(QWidget *parent) :
    QWidget(parent)
{    
    mp_action = NULL;
    mp_MdiSubWindow = NULL;
    mp_DockWidget = NULL;
    m_call_from_ui = 0;
    mp_applet_helper = NULL;
    m_window_type = MDI; 
    setFixedWidth(0);
    setFixedHeight(0);
}

Applet::~Applet()
{

}

/****************************************************************************
 * @function name: Applet::widget()
 * @description: this function returns widget type pointer
 * @return: ( QWidget *)
 ****************************************************************************/
QWidget *Applet::widget()
{
    return dynamic_cast<QWidget*>(this);
}

/****************************************************************************
 * @function name: Applet::dragFunction()
 *
 * @param:
 *      QWidget *pwidget - widget to drag
 *      const char *namepath - namepath to access this element
 *      const char *parameter - parameter value
 * @description: pointers to objects for Drag/Drop functionality
 * @return: ( void)
 ****************************************************************************/
void Applet::dragFunction(QWidget *pwidget,const char *namepath, const char *parameter)
{
    static QMimeData *gp_MimeData =  NULL;
    static QDrag *gp_Drag = NULL;

    char tmp[4000];
    /* assembly MIME string*/
    gp_MimeData =  new QMimeData;
    SPRINTF(tmp, "Set @%s = GetObject(,\"%s.%s\") ", PROJECT_NAME
            ,PROJECT_NAME
            ,PROJECT_CLASS);
    gp_MimeData->setData(QString("VBScript Init Object")
                         , QByteArray(tmp));
    SPRINTF(tmp, "call @%s.run(\"%s\", \"%s\" ) "
            , PROJECT_NAME
            , namepath
            , parameter
            );
    gp_MimeData->setData(QString("VBScript Set Data")
                         , QByteArray(tmp));

    SPRINTF(tmp, "Set @%s = Nothing ", PROJECT_NAME);
    gp_MimeData->setData(QString("VBScript Clean-up")
                         , QByteArray(tmp));
    /* [ for my sweeper] */
    //TODO doesnot use parameter
    QString text =QString("%1 #%2: run").arg(namepath).arg(PROJECT_NAME);
    gp_MimeData->setText(text);

    /*** start dragging ***/
    gp_Drag = new QDrag(pwidget);
    gp_Drag->setMimeData(gp_MimeData);
    gp_Drag->exec(Qt::CopyAction );
    delete gp_Drag;
    gp_Drag= NULL;

}

/****************************************************************************
 * @function name: Applet::name() 
 * @description: returns Applet name
 * @return: ( const QString )
 ****************************************************************************/
const QString Applet::name()const
{
    return objectName();
}


/****************************************************************************
 * @function name: Applet::setName() 
 * @param:  const QString &name
 * @description: sets Applet name
 * @return: ( void )
 ****************************************************************************/
void Applet::setName(const QString &name)
{
    this->setObjectName(name);
    if(mp_action) mp_action->setText(name);
    setWindowTitle(name);
}


/****************************************************************************
 * @function name: Applet::closeEvent - ---
 *
 * @param:
 *      QCloseEvent *pevent
 * @description:
 * @return: ( void )
 ****************************************************************************/
void Applet::closeEvent(QCloseEvent *pevent)
{
    pevent->ignore();
    emit signal_HideWindow();
}


/****************************************************************************
 * @function name: Applet::initialize_applet()
 * @description:
 * @return: ( bool)
 ****************************************************************************/
bool Applet::initialize_applet()
{
    bool result = initialize();
    appletAction()->setEnabled(false);
    autoBind();
    Logs *plog = static_cast<Logs*>(MAINAPP->log_applet());
    if(plog)
    {
        connect(this,SIGNAL(signal_log(int,QString))
                ,plog,SLOT(slot_addLog(int,QString)));
    }
    QMetaObject::connectSlotsByName(this);
    slot_updateAllControls();        
    m_activated =false;
    return result;
}

/****************************************************************************
 * @function name: Applet::resume_applet()
 *
 * @description: resumes applet
 * @return: ( void )
 ****************************************************************************/
void Applet::resume_applet()
{
    if(isEnabled())
    {
      resume();
      appletAction()->setEnabled(true);
      m_activated = true;
    }
}

/****************************************************************************
 * @function name: Applet::suspend_applet()
 *
 * @description: suspends applet
 * @return: ( void )
 ****************************************************************************/
void Applet::suspend_applet()
{
    if(isEnabled())
    {
      appletAction()->setEnabled(false);
      suspend();
      m_activated = false;
    }
}

/****************************************************************************
 * @function name: Applet::registers_list()
 * @description: this will return list of data elements
 * @return: ( QStringList )
 ****************************************************************************/
QStringList Applet::registers_list() const
{
    return m_applet_data.keys();
}

/****************************************************************************
 * @function name: Applet::controls_list()
 * @description: custom controls list
 * @return: ( QStringList )
 ****************************************************************************/
QStringList Applet::controls_list() const
{
    QStringList result;
    foreach(UniversalControl *pctrl, m_applet_controls)
    {
        result.append(pctrl->name());
    }

    return result;
}

/****************************************************************************
 * @function name: Applet::functions_list()
 * @description: functions list (names)
 * @return: ( QStringList )
 ****************************************************************************/
QStringList Applet::functions_list() const
{
    return m_functions.keys();
}

/****************************************************************************
 * @function name: Applet::updateHardwareNonBlocking()
 * @param:
 *      Applet::HardwareUpdateDirection direction
 *      const QString &item
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool Applet::updateHardwareNonBlocking(Applet::Direction direction, const QString &item)
{
        bool result =false;
        if(m_update_list.contains(item) ) return true;
          m_update_list.append(item);
          result= updateHardware(direction , item);
          int index;
          while((index = m_update_list.indexOf(item))>=0)
          {
             m_update_list.removeAt(index);
          }
         return result;
}



/****************************************************************************
 * @function name: Applet::appletAction()
 *
 * @description: This function creates and returns window Action
 * @return: ( QAction * )
 ****************************************************************************/
QAction *Applet::appletAction()
{
    if (mp_action == NULL)
    {
        switch(m_window_type)
        {
        case (MDI):
        {
            /* creating action*/
            QString action_name = name();
            if(windowTitle() != name())
            {
                action_name = QString("%1(%2)").arg(windowTitle()).arg(name());
            }
            mp_action = new QAction(0);
            mp_action->setText(action_name);
            mp_action->setCheckable(true);
            mp_action->setWhatsThis(objectName());
            mp_action->setIcon(windowIcon());
            connect(mp_action, SIGNAL(triggered()), this, SLOT(slot_ActionTriggered()));
        }
            break;

        case (DOCK):
        {
            mp_action = mp_DockWidget->toggleViewAction();
            mp_action->setIcon(windowIcon());
            connect( mp_DockWidget,SIGNAL(visibilityChanged(bool)),this,SLOT(slot_SetInactiveWindow()));
        }
            break;
        }
    }    return mp_action;
}


/****************************************************************************
 * @function name: *Applet::createContainerWindow()
 *
 * @param:
 *             Type type
 *             QWidget *parent
 * @description:This function returns window mdi
 * @return: ( QWidget * )
 ****************************************************************************/
QWidget *Applet::createContainerWindow(Type type,QWidget *parent)
{
    m_window_type = type;
    switch(m_window_type)
    {
    case (MDI):
        if (mp_MdiSubWindow == NULL)
        {
            mp_MdiSubWindow =  new QMdiSubWindow(0);            
            mp_MdiSubWindow->setWidget(dynamic_cast<QWidget *>(this));

            QRect rect = this->geometry();
            rect.setWidth(rect.width() + 20);
            rect.setHeight(rect.height() + 40);
            mp_MdiSubWindow->setGeometry(rect);
            if( fixedWidth() )
            {
                mp_MdiSubWindow->setFixedWidth(width()+10);
            }
            if( fixedHeight() )
            {
                mp_MdiSubWindow->setFixedHeight(height()+35);
            }
            // disable maximize button
            mp_MdiSubWindow->setWindowFlags(windowFlags() & (~Qt::WindowMaximizeButtonHint) );
            //default state is hidden
            mp_MdiSubWindow->setVisible(0);            
            connect(this, SIGNAL(signal_HideWindow()), this, SLOT(slot_SetInactiveWindow()));
        }        
        return mp_MdiSubWindow;
        break;

    case (DOCK):
        if(mp_DockWidget == NULL)
        {
            mp_DockWidget = new QDockWidget(windowTitle(), parent);
            mp_DockWidget->setWidget(dynamic_cast<QWidget *>(this));
            mp_DockWidget->setVisible(0);//
        }
        return mp_DockWidget;
        break;
    }
    return NULL;
}

/****************************************************************************
 * @function name: Applet::containerWindow()
 *
 * @description: returns pointer on container window
 * @return: ( QWidget * )
 ****************************************************************************/
QWidget *Applet::containerWindow()
{
    switch(m_window_type)
    {
    case (MDI):
        return mp_MdiSubWindow;
    case (DOCK):
        return mp_DockWidget;
    default :
        return NULL;

    }
}

/****************************************************************************
 * @function name: Applet::removeContainerWindowAndAction()
 *
 * @description: this function removes container window action from MainWindow
 * @return: ( void )
 ****************************************************************************/
void Applet::removeContainerWindowAndAction()
{
    switch(m_window_type)
    {

    case (MDI):
        disconnect(this, SIGNAL(signal_HideWindow()), this, SLOT(slot_SetInactiveWindow()));
        break;

    case (DOCK):

        break;

    default :
        break;
    }
    // remove action
    if (mp_action != NULL)
    {
        switch(m_window_type)
        {
        case (MDI):
        {
            disconnect(mp_action, SIGNAL(triggered()), this, SLOT(slot_ActionTriggered()));
        }
            break;

        case (DOCK):
        {
            disconnect( mp_DockWidget,SIGNAL(visibilityChanged(bool)),this,SLOT(slot_SetInactiveWindow()));
        }
            break;
        }
        delete mp_action;
    }
}


/****************************************************************************
 * @function name: Applet::slot_ActionTriggered()
 *
 * @description: hide when window is triggered by its action
 * @return: ( void )
 ****************************************************************************/
void Applet::slot_ActionTriggered()
{
    containerWindow()->setVisible(mp_action->isChecked());
}


/****************************************************************************
 * @function name: Applet::slot_SetInactiveWindow()
 *
 * @description: hides window
 * @return: ( void )
 ****************************************************************************/
void Applet::slot_SetInactiveWindow()
{
    if(mp_action)
        mp_action->setChecked(false);
    if(m_window_type == MDI && mp_MdiSubWindow)
        mp_MdiSubWindow->setVisible(false);
}


/****************************************************************************
 * @function name: Applet::appData()
 * @param:
 *             const QString &name
 *             quint32 
 * @description: this function provides access to registers of applet
 * @return: (const QVariant)
 ****************************************************************************/
const QVariant Applet::appData(const QString &name, quint32 )
{
    QVariant result;
    if (!name.isEmpty())
    {
        QString regname;
        UniversalControl::extractParameter(name,0,&regname,0);
        //register        
        if (!regname.isEmpty() && m_applet_data.contains(regname))
        {
            // mutex.lock();
            updateHardwareNonBlocking(HARDWARE_GET, regname);
            DataNode *pnode = m_applet_data[regname];
            if (pnode->type == NODE_REGISTER)
            {
                result = pnode->pRegister->value(name);
            }
            else if (pnode->type == NODE_VARIABLE)
            {
                if(pnode->pVariable->type()== Variable::ARRAY)
                {
                    result = QString(pnode->pVariable->array());
                }
                else
                {                    
                    result = QVariant(pnode->pVariable->value());
                }
            }
            slot_updateControls(regname);
            //mutex.unlock();
        }
        else {
            TRACE("Requested reg %s fail ",name.toLatin1().constData());
        }
    }
    return result;
}



/****************************************************************************
 * @function name: Applet::setAppData()
 *
 * @param:
 *             const QString &name
 *             const QVariant &data
 * @description: This function sets applet data registers
 *               registers format is:
 *                 1.register[0:3] - bits 0-3
 *                 2.register[] - all register
 *                 3.register[1] - bit 1 of register
 *                 4.register -  all register
 *              Used for communication external and internal ring
 * @return: ( bool )
 ****************************************************************************/
bool Applet::setAppData(const QString &name, const QVariant &data,quint32 ring)
{
    bool result = false;
    if (!name.isEmpty() && !data.isNull())
    {
        QString regname ;
        QVariant value;
        UniversalControl::extractParameter(name,0,&regname,0);
        if (!regname.isEmpty() && m_applet_data.contains(regname))
        {
            DataNode *pnode =  m_applet_data[regname];
            if (pnode->type == NODE_REGISTER)
            {
                value = data.toUInt();
                result = pnode->pRegister->setValue(name, (quint32)data.toUInt());
            }
            else if (pnode->type == NODE_VARIABLE)
            {
                if(pnode->pVariable->type()== Variable::ARRAY)
                {
                    value = data.toByteArray();
                    result = pnode->pVariable->setValue(data.toByteArray(),ring);
                }
                else{
                    bool ok;
                    result = pnode->pVariable->setValue(data.toDouble(&ok),ring);
                    result &= ok;
                    if(ok) value = data.toDouble();
                }
            }            
            slot_updateControls(regname);                        
        }        
    }
    if(!result)
    {
       TRACE("Request %s fail ",name.toLatin1().constData());
    }
    return result;
}


/****************************************************************************
 * @function name: Applet::slot_ControlTriggered()  
 * @description: this is slot when some control is clicked
 * @return: ( void )
 ****************************************************************************/
void Applet::slot_ControlTriggered()
{
    QObject *pobj = QObject::sender();

    UniversalControl *pctrl = dynamic_cast<UniversalControl *>(pobj);
    if (pctrl->controlType() == UniversalControl::PUSHBUTTON)
    {
            m_call_from_ui  = true;
            runAppCommand(pctrl->name(UniversalControl::NODENAME)
                            ,pctrl->name(UniversalControl::PARAMETER)
                            );
            m_call_from_ui=false;
    }
    else
    {
        QString regname = pctrl->name(UniversalControl::NODENAME);
        if (m_applet_data.contains(regname))
        {
            DataNode *pnode =  m_applet_data[regname];
            //REGISTER
            if (pnode->type == NODE_REGISTER)
            {
                pnode->pRegister->setValue(pctrl->name(), (quint32)pctrl->editValue());
            }
            //VARIABLE
            else if (pnode->type == NODE_VARIABLE)
            {
                if(pnode->pVariable->type()== Variable::ARRAY)
                {
                    pnode->pVariable->setValue(pctrl->array(),RING_EXTERNAL);
                }
                else
                {
                    pnode->pVariable->setValue(pctrl->editValue(),RING_EXTERNAL);
                }
            }               
        }
        slot_updateControls(regname);
    }
}


/****************************************************************************
 * @function name: Applet::addDataNode() 
 * @param:
 *             Register *preg
 * @description: this adds data as Register to applet
 * @return: ( bool )
 ****************************************************************************/
bool Applet::addDataNode(Register *preg, qint32 control_size)
{
    // NULL pointers and empty registers are ignored
    if (preg && !preg->name().isEmpty())
    {
        if(control_size>0 && preg->size() != control_size)
        {
            qWarning()<<QString("Register size control fail : %1 .size %2 != %3")
                     .arg(preg->name())
                     .arg(preg->size())
                     .arg(control_size).toLatin1().constData();
        }
        return addDataNode(preg->name(), preg);
    }
    return false;
}

/****************************************************************************
 * @function name: Applet::addDataNode()
 *
 * @param:
 *             Variable *pvar
 * @description: this adds data as Variable to applet
 * @return: ( bool )
 ****************************************************************************/
bool Applet::addDataNode( Variable *pvar)
{
    if (pvar && !pvar->name().isEmpty())
    {
        return addDataNode(pvar->name(), pvar);
    }
    return false;
}

/****************************************************************************
 * @function name: Applet::addDataNode()
 *
 * @param:
 *             const QString name
 *             Register *preg
 * @description: this adds data as Register to applet
 * @return: ( bool)
 ****************************************************************************/
bool Applet::addDataNode(const QString name, Register *preg)
{

    if ( m_applet_data.contains(name))
    {
        qWarning()<<QString("Applet %1 already contains %2").arg(this->name()).arg(name);
    }
    else
        if ( preg)
        {
            DataNode *pnode = new DataNode;
            m_applet_data[name] = pnode;
            pnode->pRegister = preg;
            pnode->type = NODE_REGISTER;
            pnode->controls.clear();
            preg->bind(this,SLOT(slot_updateSet(QString)),SLOT(slot_updateGet(QString)));
            return true;
        }
    return false;
}

/****************************************************************************
 * @function name: Applet::addDataNode()
 *
 * @param:
 *             const QString name
 *             QVariant *pvar
 * @description: This adds data as Variant to aplpet
 * @return: ( bool )
 ****************************************************************************/
bool Applet::addDataNode(const QString name, Variable *pvar)
{    

    if ( m_applet_data.contains(name))
    {
        qWarning()<<QString("Applet %1 already contains %2").arg(this->name()).arg(name);
    }
    else
        if ( pvar)
        {
            DataNode *pnode = new DataNode;
            m_applet_data[name] = pnode;
            pnode->pVariable = pvar;
            pnode->type = NODE_VARIABLE;
            pnode->controls.clear();
            pvar->bind(this,SLOT(slot_updateSet(QString)),SLOT(slot_updateGet(QString)));
            return true;
        }
    return false;
}


/****************************************************************************
 * @function name: Applet::removeDataNode()
 *
 * @param:
 *            (I) Register *preg
 *            (II) Variable *pvar
 * @description:
 * @return: ( void )
 ****************************************************************************/
void Applet::removeDataNode(Register *preg)
{
    if(preg) removeDataNode(preg->name());
}
void Applet::removeDataNode(Variable *pvar)
{
    if(pvar) removeDataNode(pvar->name());
}

/****************************************************************************
 * @function name: Applet::removeDataNode()
 *
 * @param:
 *            const QString &node_name
 * @description: Removes Register/Variable from applet.
 * @return: ( void )
 ****************************************************************************/
void Applet::removeDataNode(const QString &node_name)
{
    unbindNode(node_name);
    if(m_applet_data.contains(node_name ))
    {
        //find register in list(map)
        DataNode *pnode = m_applet_data.take(node_name);
        delete pnode;
    }
}

/****************************************************************************
 * @function name: Applet::bindControl()
 *
 * @param:
 *             const QString &name - register name
 *             UniversalControl *pctrl -control pinter
 * @description: this binds conrol with data node.
 *               Register knows about control in the way it has pointer to it
 *               Control doesn't know about register but it is reverse connected
 *                      to applet on finish edit (signal/slot)
 *              When binding control control gets some  setup from register
 * @return: ( void )
 ****************************************************************************/
void Applet::bindControl(UniversalControl *pctrl)
{
    QString reg_name= pctrl->name(UniversalControl::NODENAME);
    if (pctrl->controlType() == UniversalControl::PUSHBUTTON )
    {
        QString text = QString("<font color=#0000ff>%1 (%2)</font><br>")
                .arg(pctrl->name(UniversalControl::NODENAME))
                .arg(pctrl->name(UniversalControl::PARAMETER));
        pctrl->setDescription(text);
        pctrl->bind(SLOT(slot_ControlTriggered()), this);
    }
    else
    {
        pctrl->construct();
        if (m_applet_data.contains(reg_name) )
        {
            QString caption;
            QString fieldname;
            DataNode *pnode = m_applet_data[reg_name];
            if ( pnode )
            {
                pnode->controls.append(pctrl);
                //button has no description
                if (pnode->type == NODE_REGISTER)
                {
                    //obtain fieldname and description for bit(s) from register
                    pnode->pRegister->field(&fieldname, &caption, pctrl->name().toLatin1());
    //                if(pctrl->caption().isEmpty()) pctrl->setCaption(fieldname);
                    if (pnode->pRegister->readOnly())
                        pctrl->setEditEnabled(false);
                }
                else if (pnode->type == NODE_VARIABLE)
                {
                    caption = pnode->pVariable->description();
                    if(pnode->pVariable->readOnly())
                        pctrl->setEditEnabled(false);
                    pctrl->setEditValueRange(pnode->pVariable->minimum(),pnode->pVariable->maximum());
                }

                QString text = QString("%1 <font color=#0000ff><br>%2</font><br>")
                        .arg(caption).arg(pctrl->name());
                pctrl->setDescription(text);
                /* bind slot*/                  
                pctrl->bind(SLOT(slot_ControlTriggered()), this);
            }
        }
    }
}

/****************************************************************************
 * @function name: Applet::updateControls() 
 * @param:
 *             const QString &name - name of register
 *
 * @description: Updates ALL controls of single register/data
 * @return: ( void )
 ****************************************************************************/
void Applet::slot_updateControls(const QString &name)
{
    if (name.isEmpty() || name.contains("*"))
    {
        slot_updateAllControls();
    }
    else
    {
        QStringList l = name.split(';');
        QStringList::const_iterator l_i = l.constBegin();
        while (l_i != l.constEnd())
        {
            qint32 i;
            if (m_applet_data.contains(*l_i))
            {
                DataNode *pnode = m_applet_data[*l_i];
                if ( pnode->type == NODE_REGISTER )
                {
                    for (i = 0; i < pnode->controls.size(); i++) // list controls
                    {
                        UniversalControl *pctrl = pnode->controls.at(i);
                        if ( pctrl->controlType() != UniversalControl::PUSHBUTTON )
                        {
                            double value = (double)pnode->pRegister->value(pctrl->name());
                            pctrl->setEditValue(value);
                        }
                    }
                }
                else if ( pnode->type == NODE_VARIABLE )
                {
                    for (i = 0; i < pnode->controls.size(); i++)
                    {
                        UniversalControl *pctrl = pnode->controls.at(i);
                        if ( pctrl->controlType() != UniversalControl::PUSHBUTTON )
                        {
                            if(pnode->pVariable->type()== Variable::ARRAY)
                            {
                                pctrl->setArray(pnode->pVariable->array());
                            }
                            else
                            {                               
                                pctrl->setEditValue(pnode->pVariable->value());
                            }
                        }
                    }
                }
            }
            ++l_i;
        }//while
    }
}


/****************************************************************************
 * @function name: Applet::slot_updateControls()
 * @param:
 *           const Register &reg - object which was changed
 * @description:
 * @return: ( void )
 ****************************************************************************/
void Applet::slot_updateControls(const Register &reg)
{
    slot_updateControls(reg.name());
}

/****************************************************************************
 * @function name: Applet::slot_updateControls()
 * @param:
 *           const Variable &var - object which was changed
 * @description:
 * @return: ( void )
 ****************************************************************************/
void Applet::slot_updateControls(const Variable &var)
{
    slot_updateControls(var.name());
}


/****************************************************************************
 * @function name: Applet::autoBind()
 * @description: this binds automatically all registers to all controlls
 *                  function gets one by one controls from the list and
 *              add it to register.
 * @return: ( void )
 ****************************************************************************/
void Applet::autoBind()
{    
    // finds all controls on form which are CustomControl::*
    // then control is pushed to the list m_applet_controls
    populateControlsList();
    // this scans controls list for unbinded one and bind control to its register
    foreach (UniversalControl *pctrl, m_applet_controls)
    {
        if( !pctrl->isBinded() )
        {
            bindControl(pctrl);
        }
    }
}

/****************************************************************************
 * @function name: Applet::unbindNode() 
 * @param:
 *           const QString &name
 * @description: unbinds all controls from given node_name
 * @return: ( void )
 ****************************************************************************/
void Applet::unbindNode(const QString &node_name)
{           
    if(m_applet_data.contains(node_name ))
    {
        //find register in list(map)
        DataNode *pnode =  m_applet_data[node_name];
        // clear all controls in list
        while(pnode->controls.size())
        {
            pnode->controls.first()->unbind();
            pnode->controls.removeFirst();
        }
    }
}




/****************************************************************************
 * @function name: Applet::enableControlsByMode() 
 * @param:
 *             const QString &mode
 * @description: enable controls on window by mode. Mode is text value.
 *              to define the mode when this control is enabled put
 *              arbitrary mode names to accessibleDescription property
 * @return: ( void )
 ****************************************************************************/
void Applet::enableControlsByMode(const QString &mode)
{
    foreach(QWidget *pwidget, m_sub_widgets)
    {
        bool updated=false;
        if(!pwidget->accessibleDescription().isEmpty() && pwidget->accessibleDescription().contains(mode))
        {
            QStringList l= pwidget->accessibleDescription().split(';');
            foreach(const QString &item, l)
            {
                if(item.mid(0,item.indexOf(':')) == mode)
                {
                    pwidget->setEnabled(item.mid(item.indexOf(':')+1,item.count()-1).toInt());
                    updated = true;
                }
            }
        }
        if(!updated)//default
        {
            pwidget->setEnabled(true);
        }
    }
}

/****************************************************************************
 * @function name: Applet::uploadHardware() 
 * @param:
 *             const QString &list_as_text
 * @description: This function will upload listed(separator';') registers to
 *              hardware. It issues updateHardware function in Applet.
 *              List may contain '*' or 'all' or empty string it means
 *              that take registers variables from applet data and upload
 *              hardware. For example it is allowed to read read something as first.
 *              Example: "MODE;*" -means write MODE and all registers
 *              !!!Upload here means send to hardware
 * @return: ( void )
 ****************************************************************************/
bool Applet::uploadHardware(const QString &list_as_text)
{
    bool result =true;
    CURSOR_WAIT();
    QStringList items_list;    
    if(list_as_text.isEmpty() || list_as_text=="all"|| list_as_text=="*")
        items_list = this->registers_list();
    else
        items_list = list_as_text.split(';');

    foreach(const QString &item, items_list)
    {
        bool ok ;
        bool readonly = false;
        if(item=="*" || item=="all") {uploadHardware(item );        continue;}
        if(m_applet_data.contains(item) && m_applet_data[item])
        {
            switch(m_applet_data[item]->type)
            {
            case NODE_REGISTER:
                readonly = m_applet_data[item]->pRegister->readOnly();
                break;
            case NODE_VARIABLE:
                readonly = m_applet_data[item]->pVariable->readOnly();
                break;
            default:
                break;
            }
        }

        if(!readonly)
        {            
            ok = updateHardwareNonBlocking(HARDWARE_SET, item);
            if(!ok)
            {
                QMessageBox::warning(0,"", QString("%1 Parameter %2 fail to write")
                                     .arg(name()).arg(item));
            }
            result &= ok;
        }    
    }while(0);
    CURSOR_RESTORE();
    return result;
}

/****************************************************************************
 * @function name: Applet::downloadHardware()
 *
 * @param:
 *             const QString &list_as_text
 * @description: This function will upload listed(separator';') registers to
 *              hardware. It issues updateHardware function in Applet.
 *              List may contain '*' or 'all' or empty string it means
 *              that take registers variables from applet data and upload
 *              hardware. For example it is allowed to read read something as first.
 *              Example: "MODE;*" -means read MODE and all registers
 *              !!! download here means read from hardware
 * @return: ( bool )
 ****************************************************************************/
bool Applet::downloadHardware(const QString &list_as_text)
{
    bool result =true;
    QStringList items_list;
    CURSOR_WAIT();
    if(list_as_text.isEmpty() || list_as_text=="all" || list_as_text=="*")
        items_list = this->registers_list();
    else
        items_list = list_as_text.split(';');
    foreach(const QString &item, items_list)
    {
        bool ok;
        if(item=="*" || item=="all") {downloadHardware(item );continue;}
        ok= updateHardwareNonBlocking(Applet::HARDWARE_GET,item);
        if(!ok) {
            QMessageBox::warning(0,"", QString("%1 Parameter %2 fail to read")
                                 .arg(name()).arg(item));
        }
        else{
            slot_updateControls(item);
        }
        result &=ok;
    }
    CURSOR_RESTORE();
    return result;
}



/****************************************************************************
 * @function name: Applet::saveAppData() 
 * @description: assemblies bytearray string
 *                  !!!  not allowed to use '\n' or '\r'   !!!
 * @return: ( QByteArray  ) this returns byte string to save to file
 ****************************************************************************/
const QByteArray Applet::saveAppData()
{
    QByteArray result;
    AppDataMapIterator i = m_applet_data.constBegin();
    while (i != m_applet_data.constEnd())
    {
        QByteArray tmp;
        DataNode *pnode =  static_cast<DataNode *>(i.value());
        Q_ASSERT(pnode);
        if (pnode->type == NODE_REGISTER &&  !pnode->pRegister->readOnly())
        {
            tmp = "0b"+pnode->pRegister->toBitString();
            result += i.key()+':'+tmp+"\n";
        }
        else if (pnode->type == NODE_VARIABLE &&  !pnode->pVariable->readOnly())
        {
            tmp = QByteArray::number(pnode->pVariable->value(),'f');
            result += i.key()+':'+tmp+"\n";
        }

        result += tmp +'\n'; // # as separator
        ++i;
    }
   return result;
}


/****************************************************************************
 * @function name: Applet::loadAppData()
 * @param:
 *             const QByteArray &data - data to load to registers
 * @description: loads data to registers -m_AppDataMap
 * @return: ( bool   )
 ****************************************************************************/
bool Applet::loadAppData(const QByteArray &data)
{
    bool result = false;
    QList<QByteArray> l = data.split('\n');
    if(l.size())
    {
        for(qint32 i=0;i<l.size();i++)
        {
            QList<QByteArray> l_entries = l.at(i).split(':');
            if(l_entries.size()==2
                    && !l_entries.at(0).isEmpty()
                    && m_applet_data.contains(l_entries.at(0)))
            {
                DataNode *pnode =  m_applet_data[l_entries.at(0)];
                if (pnode->type == NODE_REGISTER)
                {
                    //16
                    if(l_entries.at(1).startsWith("0x"))
                        pnode->pRegister->setUInt(l_entries.at(1).mid(2).toUInt(0,16));
                    //bool
                    else if(l_entries.at(1).startsWith("0b"))
                        pnode->pRegister->fromBitString(l_entries.at(1).mid(2));
                    //10
                    else
                        pnode->pRegister->setUInt(l_entries.at(1).toUInt());
                }
                else
                {
                    pnode->pVariable->setValue((double)l_entries.at(1).toDouble());
                }
            }
        }
        result = true;
    }
    slot_updateAllControls();
    return result;
}


/****************************************************************************
 * @function name: Applet::keyPressEvent()
 * @param:
 *             QKeyEvent *pev
 * @description:
 * @return: ( void   )
 ****************************************************************************/
void Applet::keyPressEvent(QKeyEvent *pev)
{
    if(pev->modifiers() == 0 && pev->key() == Qt::Key_F1)
    {
        slot_showHelp();
    }
}


/****************************************************************************
 * @function name: Applet::populateControlsList()  
 * @description: this function scans applet window
 *                      for CusomControls and makes list
 * @return: ( void   )
 ****************************************************************************/
void Applet::populateControlsList()
{
    /* finding applet controls*/
    QObject *pobj = this;
    qint32 levels = 10;
    QObjectList list_all = pobj->children();
    int i=0;
    int j = list_all.count();
    while(levels--)
    {
        for(j=list_all.count();i<j;i++)
        {
            QObject *pobj = list_all.at(i);
            if(pobj)
            {
                list_all.append(pobj->children());
            }
        }
    }
    m_applet_controls.clear();
    foreach(pobj,list_all)
    {
        if(QString(pobj->metaObject()->className()).contains("CustomControls"))
        {
            UniversalControl *pctrl = dynamic_cast<UniversalControl*>(pobj);
            if(pctrl)
            {
                    m_applet_controls.append(dynamic_cast<UniversalControl*>(pctrl));
            }
        }
        if(pobj->inherits("QWidget"))
        {
            m_sub_widgets.append(static_cast<QWidget*>(pobj));
        }
    }

}

/****************************************************************************
 * @function name: Applet::nodeReadOnly()
 * @param:
 *            const QString &name
 * @description: if data node is readonly
 * @return: ( bool   )  true if it  is  readonly
 ****************************************************************************/
bool Applet::nodeReadOnly(const QString &name)
{
    bool result =false;
    if(m_applet_data.contains(name)){
        DataNode *pnode = m_applet_data[name];
        if(pnode){
            result = (pnode->type==NODE_REGISTER) ? pnode->pRegister->readOnly():0
                        |(pnode->type==NODE_VARIABLE) ? pnode->pVariable->readOnly():0;
        }
    }
    return result;
}

/****************************************************************************
 * @function name: Applet::isActivated()
 * @description: returns applet is activated or not true/false
 * @return: ( bool   )
 ****************************************************************************/
bool Applet::isActivated() const
{
    return m_activated;
}


/****************************************************************************
 * @function name: Applet::updateAllControls() 
 * @description: updates all controls within all registers
 * @return: ( void   )
 ****************************************************************************/
void Applet::slot_updateAllControls()
{
    // update all data
    AppDataMapIterator i = m_applet_data.constBegin();
    while (i != m_applet_data.constEnd())
    {
        slot_updateControls(i.key());
        ++i;
    }
}


/****************************************************************************
 * @function name: Applet::dataNode() 
 * @param:
 *      const QString &name - name of Register or Variable
 * @description:
 * @return: ( Applet::DataNode * ) - data node pointer.
 *                                  If doesn't exist returns NULL
 ****************************************************************************/
Applet::DataNode *Applet::dataNode(const QString &name)
{
    Applet::DataNode *presult = NULL;
    if( m_applet_data.contains(name) )
        presult = m_applet_data[name];
    return presult ;
}


/****************************************************************************
 * @function name: static Applet::showHelp()
 * @description: Shows help window
 * @return: ( void )
 ****************************************************************************/
void Applet::slot_showHelp()
{

    if(mp_applet_helper == NULL)
    {
        mp_applet_helper = new AppletHelper(this,QString("HELP:") + windowTitle());
        connect(mp_applet_helper, SIGNAL(signal_update()),this,SLOT(slot_showHelp()));
    }    
    mp_applet_helper->makeHelp("functions+nodes");
    mp_applet_helper->show();
    mp_applet_helper->activateWindow();
    QApplication::setActiveWindow(mp_applet_helper);
}

void Applet::setEnabled(bool on)
{
    appletAction()->setEnabled(on);
    QWidget::setEnabled(on);
}

void Applet::slot_updateSet(const QString &name)
{
    if(isActivated())
    {
        slot_updateControls(name);
        updateHardwareNonBlocking(HARDWARE_SET,name);
        emit signal_RegisterChanged();
    }
}

void Applet::slot_updateGet(const QString &name)
{
    if(isActivated())
    {
        updateHardwareNonBlocking(HARDWARE_GET,name);
        slot_updateControls(name);
    }
}

/****************************************************************************
 * @function name:  Applet::open_window() 
 * @description: Opens window and make it visible
 * @related: isOpened(), close_window()
 * @return: ( void )
 ****************************************************************************/
void Applet::open_window()
{    
    containerWindow()->setVisible(true);
    mp_action->setChecked(true);
}

/****************************************************************************
 * @function name: Applet::close_window()
  * @description: closes window make it invisible(remains active)
* @related: isOpened(), open_window()
 * @return: ( void )
 ****************************************************************************/
void Applet::close_window()
{
    containerWindow()->setVisible(false);
    mp_action->setChecked(false);
}

/***************************************************************************
 * @function name: Applet::setTitle()
 *  const QString &text -set title
 * @description:
 * @return: ( void )
 ****************************************************************************/
void Applet::setTitle(const QString &text)
{
    this->setWindowTitle(text);
}

/****************************************************************************
 * @function name: Applet::title()
 * @description: returns window title
 * @return: ( QString )
 ****************************************************************************/
const QString Applet::title()
{
    return windowTitle();
}

/****************************************************************************
 * @function name: Applet::isOpened()
 * @description: returns true if window is visible(opened)
 * @related: open_window(), close_window()
 * @return: ( bool )
 ****************************************************************************/
bool Applet::isOpened()
{
    return isVisible();
}


/****************************************************************************
 * @function name: Applet::windowType()
 * @description:
 * @return: ( Applet::Type )
 ****************************************************************************/
Applet::Type Applet::windowType() const
{
    return m_window_type;
}

/****************************************************************************
 * @function name: slot_adjustSize()
 * @description:
 * @return: ( Applet::Type )
 ****************************************************************************/
void Applet::slot_adjustSize()
{
    adjustSize();
    containerWindow()->adjustSize();
}
