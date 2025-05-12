
#include "scan_chain.h"
#include "ui_scan_chain.h"
#include <QInputDialog>

Scanchain *gp_Scanchain;


/****************************************************************************
 *                              REGISTERS
 ****************************************************************************/
#include "register.h"
#include "mainapp.h"


static Register g_ScanChain(0,"SCANCHAIN");

/* register map - configurations from XlS*/
static RegisterMap g_ScanChainConfig;

/* register for remember prev state*/
static Register REM;

static Variable CONFIG("@variable=CONFIG @descr=current configuration");

static QString m_last_xls_file;

static int m_scanchains =0;

/****************************************************************************
 *                              APPLICATION
 ****************************************************************************/
Scanchain::Scanchain(QWidget *parent) :
    Applet(parent)
{
    ui =  new Ui::Scanchain;
    ui->setupUi(this);
    setWindowTitle("Scanchain");
}

void Scanchain::setupCustomUi()
{

    switch(m_scanchains)
    {
    case 5:
        ui->suScanchain4->setLayoutType(CustomControls::SuperBox::SHORT);
    case 4:
        ui->suScanchain3->setLayoutType(CustomControls::SuperBox::SHORT);
    case 3:
        ui->suScanchain2->setLayoutType(CustomControls::SuperBox::SHORT);
    case 2:
        ui->suScanchain1->setLayoutType(CustomControls::SuperBox::SHORT);
    case 1:
        ui->suScanchain0->setLayoutType(CustomControls::SuperBox::SHORT);
    default:
        break;
    }

    connect(this->ui->rdLayoutModeFull,SIGNAL(clicked()),this,SLOT(slot_LayoutMode_toggled()));
    connect(this->ui->rdLayoutModeBrief,SIGNAL(clicked()),this,SLOT(slot_LayoutMode_toggled()));
    connect(this->ui->rdLayoutModeCompact,SIGNAL(clicked()),this,SLOT(slot_LayoutMode_toggled()));
    connect(ui->leHighlightText,SIGNAL(textChanged(QString)),this,SLOT(slot_highlight(QString)));
}

void Scanchain::updateStatistics()
{    
    for(int i=0;i<m_scanchains;i++)
    {
        ui->twScanchains->setTabText(i,QString("%1:%2").arg(i).arg(g_ScanChain.sub("num",QString::number(i))->size()));
    }
}

void Scanchain::inflateWithBits(const int num)
{
    Bit *pbit;
    CustomControls::SuperBox *psuperbox = box(num);
    for(int i=0;i<g_ScanChain.size();i++)
    {
        pbit = g_ScanChain.bitAt(i);
        if(pbit->extra("num").toInt() == num)
        {
            if(ui->cmFilter->currentText()== "ALL"){
                psuperbox->addBit(pbit->extra("sub").toInt()
                                  ,QString("SCANCHAIN[%1]").arg(i)
                                  ,pbit->name()
                                  ,pbit->extra("color")
                                  ,"<br>"+pbit->description()
                                  +"<br>func:"+pbit->name()
                                  );
            }
            else if(pbit->extra("type") == ui->cmFilter->currentText())
            {
                if(pbit->extra("type") == "ANALOG"){
                    psuperbox->addBit(pbit->extra("sub").toInt()
                                      ,QString("SCANCHAIN[%1]").arg(i)
                                      , pbit->name()
                                      , pbit->extra("color")
                                      ,"<br>"+pbit->description()
                                      +"<br>func:"+pbit->name()
                                      );
                }
                else if(pbit->extra("type") == "DIGITAL")
                {
                    psuperbox->addBit(pbit->extra("sub").toInt()
                                      ,QString("SCANCHAIN[%1]").arg(i)
                                      ,pbit->description()
                                      ,pbit->extra("color")
                                      ,"<br>"+pbit->description()
                                      +"<br>func:"+pbit->name()
                                      );
                }
            }
        }
    }
}

SuperBox *Scanchain::box(int num)
{
    CustomControls::SuperBox *psuperbox;
    switch(num)
    {
    case 4:
        psuperbox = ui->suScanchain4;
    break;

    case 3:
        psuperbox = ui->suScanchain3;
    break;

    case 2:
        psuperbox = ui->suScanchain2;
    break;

    case 1:
        psuperbox = ui->suScanchain1;
    break;

    case 0:
        psuperbox = ui->suScanchain0;
    break;

    }
    return psuperbox;
}

bool Scanchain::initialize()
{
    setupCustomUi();
    addDataNode(&g_ScanChain);
    addDataNode(&CONFIG);
    return true;
}

void Scanchain::rebuild()
{    
    unbindNode("SCANCHAIN");

    switch(m_scanchains)
    {
    case 5:
        ui->suScanchain4->clear();
        inflateWithBits(4);
        ui->suScanchain4->reconstruct();
    case 4:
        ui->suScanchain3->clear();
        inflateWithBits(3);
        ui->suScanchain3->reconstruct();
    case 3:
        ui->suScanchain2->clear();
        inflateWithBits(2);
        ui->suScanchain2->reconstruct();
    case 2:
        ui->suScanchain1->clear();
        inflateWithBits(1);
        ui->suScanchain1->reconstruct();
    case 1:
        ui->suScanchain0->clear();
        inflateWithBits(0);
        ui->suScanchain0->reconstruct();
    default:
        break;
    }


    updateStatistics();
    autoBind();
    slot_updateAllControls();
}

bool Scanchain::setConfig(const QString &name)
{
    if( g_ScanChainConfig.contains(name) )
    {
        g_ScanChain = g_ScanChainConfig[name];
        slot_updateAllControls();
        return true;
    }
    return true;
}

bool Scanchain::addConfig(const QString &name, Register *preg)
{
    bool result =false;
    if(!name.isEmpty())
    {        
        g_ScanChainConfig[name] = *preg;        
        ui->cmCONFIG->addItem(name);
        result=true;
    }    
    return result;
}

Register *Scanchain::scanchain(int num) const
{
    if(num == -1)
    {
        return &g_ScanChain;
    }
    else {
        return g_ScanChain.sub("num",QString::number(num));
    }
}

qint32 Scanchain::size()
{
    return g_ScanChain.size();
}

void Scanchain::setModesList(const QStringList &modes_list_before,const QStringList &modes_list_after)
{
    ui->cmTestModeBefore->clear();
    ui->cmTestModeBefore->insertItems(0,modes_list_before);
    ui->cmTestModeAfter->clear();
    ui->cmTestModeAfter->insertItems(0,modes_list_after);
}

void Scanchain::setScanchain(Register *preg)
{
    ui->cmCONFIG->clear();
    g_ScanChain.clear();    
    g_ScanChain = *preg;
}

const QByteArray Scanchain::saveAppData()
{
    QByteArray result;
    result += "BITS:\n";
    for(qint32 i =0;i<g_ScanChain.size();i++)
    {
        Bit *pbit = g_ScanChain.bitAt(i);
        result +=
                /*0*/ pbit->name()+';'
                /*1*/+pbit->description()+';'
                /*2*/+pbit->extra("type")+';'
                /*3*/+pbit->extra("num")+';'
                /*4*/+pbit->extra("sub")+';'
                /*5*/+QByteArray::number(pbit->value())+';'
                /*6*/+pbit->extra("color")+'\n';
    }
    result += "CFGS:\n";
    QStringList config_list = g_ScanChainConfig.keys();
    for(int i=0;i<config_list.count();i++)
    {
        Register *preg = const_cast<Register*>(&g_ScanChainConfig[config_list.at(i)]);
        result += config_list.at(i)+';'+preg->toBitString()+'\n';
    }
    result+="MODES1:\n";
    for(int i=0;i<ui->cmTestModeBefore->count();i++)
    {
        result+=ui->cmTestModeBefore->itemText(i)+"\n";
    }
    result+="MODES2:\n";
    for(int i=0;i<ui->cmTestModeAfter->count();i++)
    {
        result+=ui->cmTestModeAfter->itemText(i)+"\n";
    }
    return result;
}

bool Scanchain::loadAppData(const QByteArray &data)
{
    QList<QByteArray> l_lines = data.split('\n');
    qint32 part=0;
    g_ScanChain.clear();
    ui->cmTestModeBefore->clear();
    ui->cmTestModeAfter->clear();
    foreach(const QByteArray &line, l_lines)
    {
        if(line == "CFGS:") part =1;
        else if(line == "BITS:") part =0;
        else if(line == "MODES1:") part =2;
        else if(line == "MODES2:") part =3;
        else
        {
            switch(part)
            {
            case 0:
            {
                QList<QByteArray> l_entries = line.split(';');                
                if(l_entries.size() == 7)
                {
                    Bit *pbit = new Bit;
                    /*0*/pbit->setName(l_entries.at(0));
                    /*1*/pbit->setDescription(l_entries.at(1));
                    /*2*/pbit->setExtra("type",l_entries.at(2));
                    int num = l_entries.at(3).toInt();
                    if((num+1)>m_scanchains) m_scanchains = num+1;
                    /*3*/pbit->setExtra("num",l_entries.at(3));
                    /*4*/pbit->setExtra("sub",l_entries.at(4));
                    /*5*/pbit->setValue(l_entries.at(5).toInt()!=0);
                    /*6*/pbit->setExtra("color",l_entries.at(6));
                    g_ScanChain.addBit(pbit);
                }
            }
                break;

            case 1:
            {
                Register config = g_ScanChain;
                QList<QByteArray> l_entries = line.split(';');
                if(l_entries.size() == 2)
                {
                    config.fromBitString(l_entries.at(1));
                    addConfig(l_entries.at(0),&config);
                }
            }
                break;
            case 2:
                if(line.trimmed() != "")
                    ui->cmTestModeBefore->addItem(line);
                break;

            case 3:
                if(line.trimmed() != "")
                    ui->cmTestModeAfter->addItem(line);
                break;
            }
        }
    }

    rebuild();
    resume();    
    return true;
}


QVariant Scanchain::FUNCTION_LIST()
{
    FUNCTION("SetConfig","applies scanchain with name-parameter"
             "<i><br> RETURN true/false")
    {
        bool result= false;
        if(!arg_data.isNull())
            result= setConfig(arg_data.toString());        
        return result;
    }    

    FUNCTION("SetModeAfter","As parameter use mode name as text")
    {
        return SetModeAfter(arg_data.toString());
    }

    FUNCTION("SetModeBefore","As parameter use mode name as text")
    {
        return SetModeBefore(arg_data.toString());
    }

    FUNCTION("Write","Writes scanchain in current(usual DIGITAL) mode"
             "<i><br> RETURN true/false")
    {
        bool result;
        if(ui->cbAutoconnectPins->isChecked()){
            BOARD->SaveState();
        }
        result = BOARD->SetModeByName(ui->cmTestModeBefore->currentText());
        result &= BOARD->WriteScanchain(ui->twScanchains->currentIndex());
        if(ui->cbAutoconnectPins->isChecked()){
            LOG(0,"restore pins");
            BOARD->RestoreState();
        }
        return  result;
    }

    FUNCTION("Read","Reads scanchain in current(usual DIGITAL)"
             "<i><br> RETURN true/false")
    {
        bool result;
        if(ui->cbAutoconnectPins->isChecked()){
            BOARD->SaveState();
        }
        result = BOARD->SetModeByName(ui->cmTestModeBefore->currentText());
        result &= BOARD->ReadScanchain(ui->twScanchains->currentIndex());
        slot_updateAllControls();
        if(ui->cbAutoconnectPins->isChecked()){
            LOG(0,"restore pins");
            BOARD->RestoreState();
        }
        return result;
    }

    FUNCTION("AnalogWrite","Writes scanchain in current(usual DIGITAL) and then goes to ANALOG mode."
             "<i><br> RETURN true/false")
    {
        bool result;
        if(ui->cbAutoconnectPins->isChecked()){
            BOARD->SaveState();
        }
        result = BOARD->SetModeByName(ui->cmTestModeBefore->currentText());
        result &= BOARD->WriteScanchain(ui->twScanchains->currentIndex());
        result &= BOARD->SetModeByName(ui->cmTestModeAfter->currentText());        
        if(ui->cbAutoconnectPins->isChecked()){
            LOG(0,"restore pins");
            BOARD->RestoreState();
        }
        return result;
    }
    FUNCTION("AnalogRead"," Reads scanchain in current(usual DIGITAL) and then goes to ANALOG mode"
             "<i><br> RETURN true/false")
    {
        bool result;
        if(ui->cbAutoconnectPins->isChecked()){
            BOARD->SaveState();
        }
        result = BOARD->SetModeByName(ui->cmTestModeBefore->currentText());
        result &= BOARD->ReadScanchain(ui->twScanchains->currentIndex());
        result &= BOARD->SetModeByName(ui->cmTestModeAfter->currentText());
        slot_updateAllControls();
        if(ui->cbAutoconnectPins->isChecked()){
            LOG(0,"restore pins");
            BOARD->RestoreState();
        }
        return result;
    }

    return false;
}

void Scanchain::resume()
{
    setEnabled(SCANCHAIN->size()>0);
}








// SLOTS

void Scanchain::on_pbMs_clicked()
{
    REM = g_ScanChain;
}

void Scanchain::on_pbMr_clicked()
{
    if( REM.size() == g_ScanChain.size())
    {
        g_ScanChain = REM;
    }
    slot_updateAllControls();
}

void Scanchain::slot_LayoutMode_toggled()
{
    for(int i=0;i<m_scanchains;i++)
    {
        if(ui->rdLayoutModeFull->isChecked())
        {
            box(i)->setLayoutType(CustomControls::SuperBox::LARGE);
        }
        else if(ui->rdLayoutModeCompact->isChecked())
        {
            box(i)->setLayoutType(CustomControls::SuperBox::SHORT);
        }
        else if(ui->rdLayoutModeBrief->isChecked())
        {
            box(i)->setLayoutType(CustomControls::SuperBox::BRIEF);
        }
        box(i)->reconstruct();
    }
}

void Scanchain::on_cmFilter_currentIndexChanged(int )
{
    rebuild();
}

const int MAX_ROWS =    (10000);
/* columns define */
const int COL_NUM =     (1);
const int COL_BITN =    (2);
const int COL_NAME =    (3);
const int COL_ANAME =   (4);
const int COL_CFGS =    (5);
const int MAX_CFGS =    (100);
const int START_ROW =   (6);
const int HEADER_ROW =   (5);


bool Scanchain::loadExcelConfig()
{
    bool result = false;

    /* obtaining filename */
    QString filename = QFileDialog::getOpenFileName(0, "Load file",
                       "",
                       "Files (*.xlsx)");
    if ( filename.isEmpty() )
    {
        return false;
    }

    /* make dialog for progress bar*/
    DialogProgressBar dialog;
    dialog.show();
    dialog.setText("Opening Excel");

    /* open excel file */
    ExcelData excel(filename);
    if ( excel.open('r')  )
    {
        excel.setCurrentSheet("ScanChains");
        excel.setVisible(0);
        QVariant name;
        QVariant aname;
        QVariant bitn;
        QVariant chain;
        QVariant modes1;
        QVariant modes2;

        /* configuration read */
        excel.read(1,3,modes1);
        excel.read(2,3,modes2);
        gp_Scanchain->setModesList(modes1.toString().split(';',QString::SkipEmptyParts)
                                   ,modes2.toString().split(';',QString::SkipEmptyParts));



        // read data

        dialog.setText("Evaluating file...");
        QVariantList l ;

        for(int i=0;i<20;i++)
        {
            QVariantList l2 =excel.readRange(QString("A%1").arg(i*500+1),QString("AZ%1").arg(i*500+500));
            l.append(l2);
            if(l2[0].toList()[1].isNull()) break;
        }


        qint32 rows=0;
        /* evaluate size */
        for(int i=0;i<l.count();i++)
        {
            if(l[i+START_ROW-1].toList()[1].isNull()) break;
            rows++;
        }


        /* scan chain populating */
        dialog.setText("Loading...");
        Register scanchain;
        for (qint32 i = 0; i < rows; i++)
        {
            dialog.setProgress(i,rows);
            QColor foreground,background;
            chain = l[i+START_ROW-1].toList()[0];
            // getting color of bitn field
            excel.color(i+START_ROW, COL_BITN, background, foreground);
            // reading bitn contens
            bitn = l[i+START_ROW-1].toList()[1];
            // reading digital name
            name = l[i+START_ROW-1].toList()[2];
            // reading analog name
            aname = l[i+START_ROW-1].toList()[3];
            if(name.toString() == "<<<>>>") {
                int spare_count = bitn.toInt();
                while(spare_count--){
                    Bit *pbit = new Bit;
                    pbit->setName("...");
                    pbit->setExtra("color", "#808080");
                    scanchain.addBit(pbit);
                }
                continue;
            }
            Bit *pbit =  new Bit;//create bit

            if(background != Qt::white)
            {
                char tmp[10];
                SPRINTF(tmp,"#%02x%02x%02x"
                          ,background.toRgb().blue()
                          ,background.toRgb().green()
                          ,background.toRgb().red());
                pbit->setExtra("color",QByteArray(tmp));
            }
            pbit->setDescription(name.toString());
            //analog or digital
            if (aname.toString().isEmpty() || aname.toString() == "unused")
            {
                pbit->setExtra("type", "DIGITAL");
                pbit->setName("_");
            }
            else
            {
                pbit->setExtra("type", "ANALOG");
                pbit->setName(aname.toString());//analog name to name2
            }
            //chain.subchain
            {
                QStringList lchainsub = chain.toString().split('.');
                switch(lchainsub.size())
                {
                    case 2:
                        pbit->setExtra("sub",lchainsub[1]);
                    case 1:
                        {
                        pbit->setExtra("num",lchainsub[0]);
                        int num = lchainsub[0].toInt();
                        if((num+1)>m_scanchains) m_scanchains = num+1;
                        }
                        break;
                    case 0:
                    default:
                        break;
                }
            }

            scanchain.addBit(pbit);
        }

        gp_Scanchain->setScanchain(&scanchain);
        /* configurations load */
        dialog.setText("Loading configurations...");
        QVariant data;
        Register config;
        config = scanchain;
        for(int j=0;j< MAX_CFGS; j++)// max configs 100
        {
            data = l[HEADER_ROW-1].toList()[j+COL_CFGS-1];
            if(data.toByteArray().isEmpty()) break;
            config.setName(data.toString());
            dialog.setText(data.toString());
            qint32 bit =0;
            for (qint32 i = 0 ;i < rows; i++)
            {
                dialog.setProgress(i,rows);
                data = l[i+START_ROW-1].toList()[j+COL_CFGS-1];
                config.setBit(bit++,data.toInt());
            }
            gp_Scanchain->addConfig(config.name(),&config);
        }
        /* finally refresh */
        SCANCHAIN->rebuild();
        /* close xls file */
        excel.close();
        result = true;
        m_last_xls_file = filename;
    }


    return result;
}

bool Scanchain::SetModeBefore(const QString &mode_name)
{
    int index = ui->cmTestModeBefore->findText(mode_name);
    if(index>=0){
        ui->cmTestModeBefore->setCurrentIndex(index);
        return true;
    }
    return false;
}

bool Scanchain::SetModeAfter(const QString &mode_name)
{
    int index = ui->cmTestModeAfter->findText(mode_name);
    if(index>=0){
        ui->cmTestModeAfter->setCurrentIndex(index);
        return true;
    }
    return false;
}

void Scanchain::setCurrentFile(const QString &current_file_name)
{
    m_last_xls_file = current_file_name;
    setWindowTitle(m_last_xls_file);
}


void Scanchain::slot_highlight(const QString &text)
{
    for(int i=0;i<m_scanchains;i++)
        box(i)->clearHighlighiting();

    if(!text.isEmpty())
    {
        for(int i=0;i<m_scanchains;i++)
        {
                box(i)->highlight(SuperBox::BY_TEXT,text);
        }
    }
}

void Scanchain::on_pbFill0_clicked()
{
    g_ScanChain.fill(0);
}

void Scanchain::on_pbXor_clicked()
{

}

void Scanchain::on_pbXor_toggled(bool checked)
{
    if(checked){
        if(REM.size() == g_ScanChain.size())
        {
            for(int j=0;j<g_ScanChain.size();j++)
            {
                if(g_ScanChain.bitAt(j)->value() != REM.bitAt(j)->value()) {

                    for(int i=0;i<m_scanchains;i++)
                    {
                            box(i)->highlight(SuperBox::BY_ACCESSIBLE_NAME,QString("SCANCHAIN[%1]").arg(j));
                    }
                }
            }
        }
    }
    else {
        for(int i=0;i<m_scanchains;i++)
            box(i)->clearHighlighiting();
    }
}


void Scanchain::on_cmCONFIG_activated(const QString &arg1)
{
    setConfig(arg1);
}

void Scanchain::on_pbSaveBack_clicked()
{
    bool result =false;
    bool add_new_config = false;
    DialogProgressBar dialog;
    dialog.show();
    dialog.setText("Opening Excel");
    QString name = QInputDialog::getText(0,"","New name of configuration.\n"
                                              "If save to existing - don't change anything."
                                              , QLineEdit::Normal,ui->cmCONFIG->currentText());

    if(name.isEmpty()) return;
    add_new_config =  !g_ScanChainConfig.contains(name);
    g_ScanChainConfig[name] = g_ScanChain;

    if(QFile::exists(m_last_xls_file))
    {
        int col=-1;        
        ExcelData excel(m_last_xls_file);        
        if(excel.open('w'))
        {
            excel.setCurrentSheet("ScanChains");
            excel.setVisible(0);            
            QVariant data;
            dialog.setText("Finding config");
            if(add_new_config){
                for(int i=0;i<MAX_CFGS;i++)
                {
                    if(!excel.read(HEADER_ROW, COL_CFGS+i, data)) break;
                    if(data.toString().trimmed() == ""){
                        excel.write(HEADER_ROW, COL_CFGS+i, name);
                        col = COL_CFGS+i;
                        break;
                    }
                }
            }
            else {
                for(int i=0;i<MAX_CFGS;i++)
                {
                    if(!excel.read(HEADER_ROW,COL_CFGS+i,data)) break;
                    if(data.toString() == ui->cmCONFIG->currentText()){
                        col = COL_CFGS+i;
                        break;
                    }
                }
            }
            if(col >= COL_CFGS){
                for(int i=0;i<g_ScanChain.size();i++){
                    dialog.setText(QString("Appling data %1").arg(i));
                    dialog.setProgress(i,g_ScanChain.size());
                    if(!excel.write(HEADER_ROW+1+i,col,QString::number(g_ScanChain.value(i))))
                    {result=false;break;}
                    else result=true;
                }
            }
            else {
            }
            excel.save();
            excel.close();
        }
        else {
            QMessageBox::warning(0,"",QString("Can't open configuration file %1").arg(m_last_xls_file));
        }
    }
    if(result) QMessageBox::information(0,"",QString("Data saved back to excel file %1").arg(m_last_xls_file));
}

