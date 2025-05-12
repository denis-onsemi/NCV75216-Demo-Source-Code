#include "applethelper.h"
#include "applet.h"
#include <QTextCursor>
#include <QScrollBar>
#include <customdialogs.h>
#include <QInputDialog>
#include "mainapp.h"

AppletHelper::AppletHelper(Applet *papplet, const QString &title) :
    QWidget(0)
{
    setupUi();
    mp_applet = papplet;
    setWindowTitle(title);
    teNodes->setReadOnly(true);
    setStyleSheet("QTextEdit{background: rgb(255,255,240)}");
    teFunctions->setReadOnly(true);
    leReturnReal->setVisible(0);
    leReturnInt->setVisible(0);
    leReturnText->setVisible(1);
    teNodes->scroll(0,0);
    teFunctions->scroll(0,0);
    updateConfigsList();
}

AppletHelper::~AppletHelper()
{

}


/****************************************************************************
 * @function name: Applet::slot_exportRegistersTo101()
 * @description:
 * @return: ( void )
 ****************************************************************************/
void AppletHelper::slot_exportRegistersTo101()
{
    saveConfig();
    updateConfigsList();
}


void AppletHelper::saveConfig()
{
    QStringList reg_names = dialogGetRegisters();
    QString filename = QFileDialog::getSaveFileName(0, "Save file",
                                                    "",
                                                    "Files (*.101)");
    if (!filename.isEmpty() && !reg_names.isEmpty())
    {
        QByteArray data;
        foreach(const QString &name, reg_names)
        {
            data+= name+"::"+mp_applet->dataNode(name)->pRegister->toBitString()+"\n";
        }        
        QFile file(filename+"."+mp_applet->name());
        if (file.open(QFile::WriteOnly))
        {
            file.write(data);
            file.close();
        }
    }
}

bool AppletHelper::loadConfig(const QString &filename)
{
    if (!filename.isEmpty())
    {
        QByteArray data;
        QFile file(filename);
        if (file.open(QFile::ReadOnly))
        {
            data = file.readAll();
            file.close();
        QStringList lines = QString(data).split('\n');
        foreach(const QString &line, lines)
        {
            QStringList items = line.split("::");
            if(items.count() == 2){
                if(mp_applet->m_applet_data.contains(items[0]) )
                {
                    Register *preg= mp_applet->m_applet_data[items[0]]->pRegister;
                    if(preg ) preg->fromBitString(items[1].toLatin1());
                }
            }
        }
        makeHelp("nodes");
        mp_applet->slot_updateAllControls();
        mp_applet->uploadHardware("*");
        return true;
        }
    }
    return false;
}



/****************************************************************************
 * @function name: Applet::slot_importRegistersFrom101()
 * @description:
 * @return: ( void )
 ****************************************************************************/
void AppletHelper::slot_importRegistersFrom101()
{
    QString filename = QFileDialog::getOpenFileName(0, "Load file",
                                                    "",
                                                    "Files (*.101)");
    loadConfig(filename);
}

void AppletHelper::slot_ApplyFile()
{
    loadConfig(cmConfigs->currentText());
}


/****************************************************************************
 * @function name: Applet::dialogGetRegister()
 * @description: makes dialog for selecting register name
 * @return: ( const QString  )
 ****************************************************************************/
const QStringList AppletHelper::dialogGetRegisters()
{
    DialogCheckBoxes dialog;
    QStringList list = mp_applet->registers_list();

    if (list.size())
    {
        dialog.setWindowTitle("REGISTER");
        dialog.inflate(list);// populate listbox
        dialog.exec();

        list.clear();
        dialog.readStringList(list);//reading back

        if (dialog.result() == QDialog::Accepted && list.size()>0)
        {
            return list;
        }
    }
    return QStringList();
}


/****************************************************************************
 * @function name: AppletHelper::makeHelp()
 *
 * @param:
 *
 * @description: generates help html from registers using data list(m_applet_data)
 * @return: ( void )
 ****************************************************************************/
void AppletHelper::makeHelp(const QString &command)
{
    static const char html_style[]=            "<style type=\"text/css\">"
            "p {font-family: Verdana;font-size:12px;margin-top:1px;color:#585858;}"
            "body {font-family: Calibri;}"
            "strong {font-weight: bold; background-color:#DAE3DC;}"
            "em {font-size:12px;font-style: italic;color:#BDBDBD;}"
            "h1 {font-weight: bold;background-color:#A8E39F;text-align:left;}"
            "div {font-size:12px;}"
            "ul {list-style-type: disc;}"
            "fu {font-style: italic;font-weight: bold;}"
            "</style>";


    if(command.contains("nodes"))
    {
        int nodesLine = teNodes->verticalScrollBar()->value();
        QString item_before= cmNodeItem->currentText();
        //generate items for top panel
        cmNodeItem->clear();
        QStringList l_nodes = mp_applet->registers_list();
        qSort(l_nodes);
        QStringList l_node_items;
        foreach(const QString &node_name, l_nodes){
            Applet::DataNode *pnode = mp_applet->dataNode(node_name);
            if(pnode->type == Applet::NODE_REGISTER){
                QStringList l_items = pnode->pRegister->toString("@purename;",1,1).split(';');
                foreach(const QString &item, l_items){
                    l_node_items.append(QString("%1[%2]").arg(node_name).arg(item));
                }
            }
            else {
                l_node_items.append(pnode->pVariable->name());
            }
        }
        cmNodeItem->addItems(l_node_items);


        //generate items help body

        m_help_text.clear();
        m_help_text +=QString(html_style);
        m_help_text += "<body>";
        foreach(const QString &node_name, l_nodes)
        {
            Applet::DataNode *pnode = mp_applet->dataNode(node_name);
            if(pnode->type == Applet::NODE_REGISTER)
            {
                QStringList l = QString(pnode->pRegister->toString("@bitn;@name;@value;@descr;",1,1)).split(';');
                m_help_text += QString("<p><strong><em>REG &nbsp;</em>%1[%2..0]</strong></p>")
                        .arg(node_name)
                        .arg(pnode->pRegister->size()-1);
                m_help_text += "<ul>";
                for(qint32 j=4;j<l.size();j+=4)
                {
                    m_help_text += QString("<p><li><b>{%1}&nbsp; %2</b> = %3</li>%4</p>")
                            .arg(l.at(j-4))
                            .arg(l.at(j-3))
                            .arg(l.at(j-2))
                            .arg(l.at(j-1));
                }
                m_help_text += "</ul><br>";
            }else
            {
                m_help_text += QString("<p><strong><em>VARIABLE  &nbsp;</em>%1 </strong><br>=%3<br>%2<p>")
                        .arg(node_name)
                        .arg(pnode->pVariable->description())
                        .arg(pnode->pVariable->value());
            }
        }
        m_help_text += "</body>";
        teNodes->clear();
        teNodes->setHtml(m_help_text);
        teNodes->verticalScrollBar()->setValue(nodesLine);
        for(int i=0;i<cmNodeItem->count();i++){
            if(cmNodeItem->itemText(i) ==  item_before)
            {
                cmNodeItem->setCurrentIndex(i);
                break;
            }
        }

    }
    if(command.contains("functions"))
    {
        int functionsLine = teFunctions->verticalScrollBar()->value();
        // generate functions
        m_help_text.clear();
        cmFunctionName->clear();
        m_help_text +=QString(html_style);
        m_help_text += "<body>";
        mp_applet->runAppCommand("",0); // run for making help
        QStringList l_funcs = mp_applet->functions_list();
        foreach(const QString func_name, l_funcs)
        {
            QString descr = mp_applet->m_functions[func_name];
            m_help_text += QString("<p><strong><em>FUNCTION</em> %1</strong></p>").arg(func_name);
            m_help_text += QString("<p>%1</p>").arg(descr);
        }
        m_help_text += "</body>";
        cmFunctionName->addItems(l_funcs);
        teFunctions->clear();
        teFunctions->setHtml(m_help_text);
        teFunctions->verticalScrollBar()->setValue(functionsLine);
    }
}

void AppletHelper::updateConfigsList()
{
    QDir currentDir = QDir::currentPath();
    QStringList bench_config_list = currentDir.entryList(QStringList()<<("*.101."+mp_applet->name()));
    QString current_text_before = cmConfigs->currentText();
    cmConfigs->clear();
    cmConfigs->addItem("");
    foreach(const QString &item, bench_config_list)
    {
        cmConfigs->addItem(item);
    }
    for(int i=0;i<cmConfigs->count();i++)
    {
        if(current_text_before == cmConfigs->itemText(i))
        {
                cmConfigs->setCurrentIndex(i);
                break;
        }
    }
}

void AppletHelper::slot_pbUpdateAll_clicked()
{
    emit signal_update();
    updateConfigsList();
}

void AppletHelper::slot_pbSet_clicked()
{
    mp_applet->SET_REG(cmNodeItem->currentText(),leValue->editValue());
    makeHelp("nodes");
}

void AppletHelper::slot_pbFill0_clicked()
{
    QString reg_name = cmNodeItem->currentText().mid(0,cmNodeItem->currentText().indexOf('['));
    if(mp_applet->m_applet_data.contains(reg_name))
    {
        Applet::DataNode *pnode = mp_applet->m_applet_data[reg_name];
        if(pnode->type == Applet::NODE_REGISTER)
        {
            pnode->pRegister->fill(0);
        }
        else if(pnode->type == Applet::NODE_VARIABLE)
        {
            pnode->pVariable->setValue(0);
        }
    }
}

void AppletHelper::slot_pbFill1_clicked()
{
    QString reg_name = cmNodeItem->currentText().mid(0,cmNodeItem->currentText().indexOf('['));
    if(mp_applet->m_applet_data.contains(reg_name))
    {
        Applet::DataNode *pnode = mp_applet->m_applet_data[reg_name];
        if(pnode->type == Applet::NODE_REGISTER)
        {
            pnode->pRegister->fill(1);
        }
        else if(pnode->type == Applet::NODE_VARIABLE)
        {
            pnode->pVariable->setValue((qint32)0xfffffff);
        }
    }
}

void AppletHelper::slot_pbGet_clicked()
{
    double value = mp_applet->GET_REG(cmNodeItem->currentText()).toDouble();
    leValue->setEditValue(value);
}

void AppletHelper::slot_pbRun_clicked()
{
    QVariant v= mp_applet->runAppCommand(cmFunctionName->currentText(),leParam->text());
    if(v.type()== QVariant::ByteArray)
    {
        leReturnReal->setVisible(0);
        leReturnInt->setVisible(0);
        leReturnText->setVisible(1);
        leReturnText->setText(v.toByteArray().toHex());
    }
    else if(v.type()== QVariant::Double )
    {
        leReturnReal->setVisible(1);
        leReturnInt->setVisible(0);
        leReturnText->setVisible(0);
        leReturnReal->setEditValue(v.toDouble());
    }
    else if(v.type()==QVariant::Int || v.type() ==QVariant::Bool || v.type() == QVariant::UInt)
    {
        leReturnReal->setVisible(0);
        leReturnInt->setVisible(1);
        leReturnText->setVisible(0);
        leReturnInt->setEditValue(v.toInt());
    }
    else {
        leReturnReal->setVisible(0);
        leReturnInt->setVisible(0);
        leReturnText->setVisible(1);
        leReturnText->setText(v.toString());
    }
}

void AppletHelper::slot_FindInNodes(const QString &text)
{
    static bool isFirstTime =1;
        QTextDocument *document = teNodes->document();
        if (isFirstTime == false)
            document->undo();
        if (!text.isEmpty())
           {
            QTextCursor highlightCursor(document);
            QTextCursor cursor(document);
            cursor.beginEditBlock();
            QTextCharFormat plainFormat(highlightCursor.charFormat());
            QTextCharFormat colorFormat = plainFormat;
            colorFormat.setBackground(Qt::blue);
            while (!highlightCursor.isNull() && !highlightCursor.atEnd())
            {
                highlightCursor = document->find(text, highlightCursor, 0);

                if (!highlightCursor.isNull())
                {
                    highlightCursor.movePosition(QTextCursor::WordRight,
                                           QTextCursor::KeepAnchor);
                    highlightCursor.mergeCharFormat(colorFormat);
                }
            }
            cursor.endEditBlock();
            teNodes->setTextCursor(teNodes->document()->find(text));
        }
        isFirstTime = false;
}


void AppletHelper::setupUi()
{
    resize(400, 400);
    gridLayout = new QGridLayout(this);
    tabWidget = new QTabWidget(this);
    tabNodes = new QWidget();
    gridLayout_2 = new QGridLayout(tabNodes);
    frame = new QFrame(tabNodes);
    frame->setStyleSheet("font: 12pt \"Consolas\";");
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);
    horizontalLayout = new QHBoxLayout(frame);
    cmNodeItem = new QComboBox(frame);
    cmNodeItem->setMinimumSize(QSize(200, 0));
    cmNodeItem->setEditable(true);
    horizontalLayout->addWidget(cmNodeItem);
    mp_highlight_text_in_nodes = new QLineEdit(this);
    connect(mp_highlight_text_in_nodes,SIGNAL(textChanged(QString)),this,SLOT(slot_FindInNodes(QString)));
    label = new QLabel("=",frame);
    horizontalLayout->addWidget(label);
    leValue = new CustomControls::LineEdit(frame);
    leValue->setMaximumSize(QSize(100, 16777215));
    horizontalLayout->addWidget(leValue);
    horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout->addItem(horizontalSpacer_4);
    pbSet = new QPushButton("Set",frame);
    pbFill0 = new QPushButton("Fill 0",frame);
    pbFill1 = new QPushButton("Fill 1",frame);
    horizontalLayout->addWidget(pbSet);
    pbGet = new QPushButton("Get",frame);
    horizontalLayout->addWidget(pbGet);
    horizontalLayout->addWidget(pbFill0);
    horizontalLayout->addWidget(pbFill1);
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout->addItem(horizontalSpacer);
    pbUpdateAll = new QPushButton("Update all",frame);
    teNodes = new QTextEdit(tabNodes);
    pbExport101 = new QPushButton("Export",tabNodes);
    pbImport101 = new QPushButton("Import",tabNodes);
    configLabel = new QLabel("Configurations",tabNodes);
    cmConfigs = new  QComboBox(tabNodes);
    gridLayout_2->setVerticalSpacing(1);
    gridLayout_2->addWidget(frame, 0, 0, 1, 10);
    gridLayout_2->addWidget(mp_highlight_text_in_nodes,1,0,1,10);
    gridLayout_2->addWidget(teNodes, 2, 0, 1, 10);
    gridLayout_2->addWidget(pbUpdateAll,3,9,1,1);
    gridLayout_2->addWidget(pbExport101,3,0,1,1);
    gridLayout_2->addWidget(pbImport101,3,1,1,1);
    gridLayout_2->addWidget(configLabel,3,2,1,1);
    gridLayout_2->addWidget(cmConfigs, 3,3,1,6);
    connect(cmConfigs,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_ApplyFile()));
    connect(pbExport101,SIGNAL(clicked()),this,SLOT(slot_exportRegistersTo101()));
    connect(pbImport101,SIGNAL(clicked()),this,SLOT(slot_importRegistersFrom101()));

    tabWidget->addTab(tabNodes, "Registers/Variables");
    tabFunctions = new QWidget();
    gridLayout_3 = new QGridLayout(tabFunctions);
    frame_2 = new QFrame(tabFunctions);
    frame_2->setFrameShape(QFrame::StyledPanel);
    frame_2->setFrameShadow(QFrame::Raised);
    horizontalLayout_2 = new QHBoxLayout(frame_2);
    cmFunctionName = new QComboBox(frame_2);
    cmFunctionName->setMinimumSize(QSize(150, 0));    
    cmFunctionName->setEditable(true);
    horizontalLayout_2->addWidget(cmFunctionName);
    label_2 = new QLabel("(",frame_2);
    horizontalLayout_2->addWidget(label_2);
    leParam = new CustomControls::LineEdit(frame_2);
    horizontalLayout_2->addWidget(leParam);
    label_5 = new QLabel(")=",frame_2);
    horizontalLayout_2->addWidget(label_5);
    leReturnInt = new CustomControls::LineEdit(frame_2);
    horizontalLayout_2->addWidget(leReturnInt);
    leReturnReal = new CustomControls::RealEdit(frame_2);
    horizontalLayout_2->addWidget(leReturnReal);
    leReturnText = new QLineEdit(frame_2);
    horizontalLayout_2->addWidget(leReturnText);
    horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout_2->addItem(horizontalSpacer_3);
    pbRun = new QPushButton("Run",frame_2);
    horizontalLayout_2->addWidget(pbRun);
    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout_2->addItem(horizontalSpacer_2);
    gridLayout_3->addWidget(frame_2, 0, 0, 1, 1);
    teFunctions = new QTextEdit(tabFunctions);
    gridLayout_3->addWidget(teFunctions, 1, 0, 1, 1);
    tabWidget->addTab(tabFunctions, "Functions");
    gridLayout->addWidget(tabWidget, 0, 0, 1, 1);
    tabWidget->setCurrentIndex(0);    
    connect(this->pbUpdateAll,SIGNAL(clicked()),this,SLOT(slot_pbUpdateAll_clicked()));
    connect(this->pbGet,SIGNAL(clicked()),this,SLOT(slot_pbGet_clicked()));
    connect(this->pbSet,SIGNAL(clicked()),this,SLOT(slot_pbSet_clicked()));
    connect(this->pbFill0,SIGNAL(clicked()),this,SLOT(slot_pbFill0_clicked()));
    connect(this->pbFill1,SIGNAL(clicked()),this,SLOT(slot_pbFill1_clicked()));
    connect(this->pbRun,SIGNAL(clicked()),this,SLOT(slot_pbRun_clicked()));
}


