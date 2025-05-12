#include "settings.h"
#include "ui_settings.h"
#include "mainapp.h"
#include <QSettings>
Settings * gp_Settings;



Register EXAMPLE("bit[0:1]","EXAMPLE");


Settings::Settings(QWidget *parent) :
    Applet(parent)
{
    ui =  new Ui::Settings;
    ui->setupUi(this);
    setWindowTitle("Settings");  
}


bool Settings::updateHardware(Direction /*direction*/, const QString &/*item*/)
{
    return false;
}

QString Settings::setting(const QString &name)
{
    if(m_all_settings.contains(name)){
        return m_all_settings[name]->text();
    }
    return QString();
}

void Settings::slot_le_editingFinished()
{
    saveSettingsToFile();
}


bool Settings::initialize()
{
    loadSettingsFromfile();
    return true;
}







/****************************************************************************
 *                              [3] FUNCTIONALITY
*****************************************************************************/
QVariant Settings::FUNCTION_LIST()
{
    //UNREFERENCED_PARAMETER(arg_data);       // to avoid warning, parameter is not used
    //UNREFERENCED_PARAMETER(arg_name);       // to avoid warning, parameter is not used
    return false;
}





void Settings::loadSettingsFromfile()
{
    if(ui->scrollArea->widget()) {
        delete ui->scrollArea->widget();
        m_all_settings.clear();
    }
    ui->scrollArea->setWidget(new QWidget());
    ui->scrollArea->widget()->setLayout(new QVBoxLayout());

    QSettings settings_file(qApp->applicationName()+".ini",QSettings::IniFormat);
    QStringList l = settings_file.allKeys();
    foreach(const QString &item,l)
    {
        QWidget *pwi =  new QWidget(ui->scrollArea->widget());
        QHBoxLayout *playout = new QHBoxLayout(pwi);
        pwi->setLayout(playout);
        ui->scrollArea->widget()->layout()->addWidget(pwi);
        QLineEdit *ple = new QLineEdit(pwi);
        connect(ple,SIGNAL(editingFinished()),this,SLOT(slot_le_editingFinished()));
        QLabel *plb = new QLabel(pwi);
        playout->addWidget(plb);
        playout->addWidget(ple);
        ple->setText(settings_file.value(item).toString());
        plb->setText(item);
        m_all_settings[item] = ple;
    }

}

void Settings::saveSettingsToFile()
{
    QSettings settings_file(qApp->applicationName()+".ini",QSettings::IniFormat);
    foreach (const QString &item, m_all_settings.keys()) {
        settings_file.setValue(item,m_all_settings[item]->text());
    }
}
