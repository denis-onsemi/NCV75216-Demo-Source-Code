#include "tododialog.h"



static const char format[] = "<font color=%1><b>%2::</b> %3</font>";
TodoDialog::TodoDialog(QWidget *parent) :
    QDialog(parent)

{
    setupUi();
    QMetaObject::connectSlotsByName(this);
}

TodoDialog::~TodoDialog()
{
    deleteUi();
}

void TodoDialog::setupUi()
{
    setWindowTitle("TodoDialog");
    resize(604, 300);
    gridLayout = new QGridLayout(this);
    label = new QLabel("Text:",this);
    gridLayout->addWidget(label, 2, 0, 1, 1);
    cmType = new QComboBox(this);
    cmType->insertItems(0,QStringList()<<"Idea"<<"Critical"<<"Other");
    gridLayout->addWidget(cmType, 2, 2, 1, 1);
    teAll = new QTextEdit(this);
    gridLayout->addWidget(teAll, 0, 0, 1, 4);
    leText = new QLineEdit(this);
    gridLayout->addWidget(leText, 2, 1, 1, 1);
    pbAdd = new QPushButton("Append",this);
    gridLayout->addWidget(pbAdd, 2, 3, 1, 1);
    pbClose = new QPushButton("Close",this);

    gridLayout->addWidget(pbClose, 5, 1, 1, 3);
    leAuthor = new QLineEdit(this);
    gridLayout->addWidget(leAuthor, 1, 1, 1, 1);
    label_2 = new QLabel("Author:",this);
    gridLayout->addWidget(label_2, 1, 0, 1, 1);
    QObject::connect(pbClose, SIGNAL(clicked()), this, SLOT(slot_pbClose_clicked()));
    QObject::connect(pbAdd, SIGNAL(clicked()), this, SLOT(slot_pbAdd_clicked()));
}

void TodoDialog::deleteUi()
{
    delete label_2;
    delete gridLayout;
    delete label;
    delete cmType;
    delete teAll;
    delete leText;
    delete pbAdd;
    delete pbClose;
    delete leAuthor;

}

void TodoDialog::showDialog(const QString &alltext)
{
    teAll->setHtml(alltext);
    this->exec();
}

const QString TodoDialog::text() const
{
    return teAll->document()->toHtml();
}

void TodoDialog::slot_pbClose_clicked()
{
    close();
}

void TodoDialog::slot_pbAdd_clicked()
{
    if(cmType->currentIndex()==0)
    {
        teAll->append(QString(format)
                          .arg("blue")
                          .arg(leAuthor->text())
                          .arg(leText->text()));
    }
    else if(cmType->currentIndex()==1)
    {
        teAll->append(QString(format)
                          .arg("red")
                          .arg(leAuthor->text())
                          .arg(leText->text()));
    }
    else if(cmType->currentIndex()==2)
    {
        teAll->append(QString(format)
                          .arg("green")
                          .arg(leAuthor->text())
                          .arg(leText->text()));
    }
}
