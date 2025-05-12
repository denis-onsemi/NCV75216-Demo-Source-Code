/********************************************************************************
** Form generated from reading UI file 'measurement.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEASUREMENT_H
#define UI_MEASUREMENT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <customcontrols.h>
#include "applet.h"
#include "customcontrols.h"
#include "customcontrols_combobox.h"
#include "customcontrols_foldingbox.h"

QT_BEGIN_NAMESPACE

class Ui_Measurement
{
public:
    QGridLayout *gridLayout_4;
    CustomControls::FoldingBox *gb_advsettings;
    QGridLayout *gridLayout;
    QGridLayout *gridLayout_2;
    QLabel *label_5;
    QSpinBox *spMemTol;
    QCheckBox *cb_AdAxRg;
    CustomControls::LineEdit *le_S3Y;
    CustomControls::LineEdit *le_S3X;
    QLabel *label_6;
    QSpinBox *spMMThr;
    QLabel *label_9;
    CustomControls::PushButton *pbMeasurement_advsettsave;
    QLabel *label_10;
    QSpinBox *spAlgTol;
    QSpinBox *spIntTol;
    QLabel *label_8;
    CustomControls::LineEdit *le_S2X;
    CustomControls::ComboBox *cb_projection;
    CustomControls::LineEdit *le_S2Y;
    CustomControls::LineEdit *le_SOS;
    QLabel *label_4;
    QLabel *label_7;
    QLabel *label_11;
    QGridLayout *gridLayout_3;
    CustomControls::PushButton *pbMeasurement_uCConfig;
    CustomControls::ComboBox *cbIndirectThr;
    CustomControls::PushButton *pbMeasurement_getdata;
    QFrame *line;
    QFrame *line_2;
    QSpinBox *spInterval;
    CustomControls::PushButton *pbMeasurement_DefaultCfg;
    QLabel *label_3;
    QLabel *label_2;
    QLabel *label;
    CustomControls::PushButton *pbMeasurement_Once;
    CustomControls::PushButton *pbMeasurement_Start;
    QSpacerItem *verticalSpacer;

    void setupUi(Applet *Measurement)
    {
        if (Measurement->objectName().isEmpty())
            Measurement->setObjectName(QString::fromUtf8("Measurement"));
        Measurement->resize(442, 515);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/board/board.png"), QSize(), QIcon::Normal, QIcon::Off);
        Measurement->setWindowIcon(icon);
        gridLayout_4 = new QGridLayout(Measurement);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gb_advsettings = new CustomControls::FoldingBox(Measurement);
        gb_advsettings->setObjectName(QString::fromUtf8("gb_advsettings"));
        gb_advsettings->setMinimumSize(QSize(420, 220));
        gb_advsettings->setCheckable(true);
        gb_advsettings->setChecked(true);
        gridLayout = new QGridLayout(gb_advsettings);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_5 = new QLabel(gb_advsettings);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 4, 0, 1, 1);

        spMemTol = new QSpinBox(gb_advsettings);
        spMemTol->setObjectName(QString::fromUtf8("spMemTol"));
        spMemTol->setValue(10);

        gridLayout_2->addWidget(spMemTol, 4, 1, 1, 1);

        cb_AdAxRg = new QCheckBox(gb_advsettings);
        cb_AdAxRg->setObjectName(QString::fromUtf8("cb_AdAxRg"));
        cb_AdAxRg->setChecked(false);

        gridLayout_2->addWidget(cb_AdAxRg, 8, 0, 1, 1);

        le_S3Y = new CustomControls::LineEdit(gb_advsettings);
        le_S3Y->setObjectName(QString::fromUtf8("le_S3Y"));
        le_S3Y->setInputMethodHints(Qt::ImhDigitsOnly);
        le_S3Y->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(le_S3Y, 1, 2, 1, 1);

        le_S3X = new CustomControls::LineEdit(gb_advsettings);
        le_S3X->setObjectName(QString::fromUtf8("le_S3X"));
        le_S3X->setInputMethodHints(Qt::ImhDigitsOnly);
        le_S3X->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(le_S3X, 1, 1, 1, 1);

        label_6 = new QLabel(gb_advsettings);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 1, 0, 1, 1);

        spMMThr = new QSpinBox(gb_advsettings);
        spMMThr->setObjectName(QString::fromUtf8("spMMThr"));
        spMMThr->setMaximum(8);
        spMMThr->setValue(2);

        gridLayout_2->addWidget(spMMThr, 5, 1, 1, 1);

        label_9 = new QLabel(gb_advsettings);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_2->addWidget(label_9, 5, 0, 1, 1);

        pbMeasurement_advsettsave = new CustomControls::PushButton(gb_advsettings);
        pbMeasurement_advsettsave->setObjectName(QString::fromUtf8("pbMeasurement_advsettsave"));

        gridLayout_2->addWidget(pbMeasurement_advsettsave, 8, 2, 1, 1);

        label_10 = new QLabel(gb_advsettings);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_2->addWidget(label_10, 6, 0, 1, 1);

        spAlgTol = new QSpinBox(gb_advsettings);
        spAlgTol->setObjectName(QString::fromUtf8("spAlgTol"));
        spAlgTol->setValue(5);

        gridLayout_2->addWidget(spAlgTol, 6, 1, 1, 1);

        spIntTol = new QSpinBox(gb_advsettings);
        spIntTol->setObjectName(QString::fromUtf8("spIntTol"));
        spIntTol->setValue(5);

        gridLayout_2->addWidget(spIntTol, 7, 1, 1, 1);

        label_8 = new QLabel(gb_advsettings);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_2->addWidget(label_8, 2, 0, 1, 1);

        le_S2X = new CustomControls::LineEdit(gb_advsettings);
        le_S2X->setObjectName(QString::fromUtf8("le_S2X"));
        le_S2X->setInputMethodHints(Qt::ImhDigitsOnly);
        le_S2X->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(le_S2X, 0, 1, 1, 1);

        cb_projection = new CustomControls::ComboBox(gb_advsettings);
        cb_projection->setObjectName(QString::fromUtf8("cb_projection"));

        gridLayout_2->addWidget(cb_projection, 3, 1, 1, 1);

        le_S2Y = new CustomControls::LineEdit(gb_advsettings);
        le_S2Y->setObjectName(QString::fromUtf8("le_S2Y"));
        le_S2Y->setInputMethodHints(Qt::ImhDigitsOnly);
        le_S2Y->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(le_S2Y, 0, 2, 1, 1);

        le_SOS = new CustomControls::LineEdit(gb_advsettings);
        le_SOS->setObjectName(QString::fromUtf8("le_SOS"));
        le_SOS->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(le_SOS, 2, 1, 1, 1);

        label_4 = new QLabel(gb_advsettings);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 0, 0, 1, 1);

        label_7 = new QLabel(gb_advsettings);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_2->addWidget(label_7, 3, 0, 1, 1);

        label_11 = new QLabel(gb_advsettings);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_2->addWidget(label_11, 7, 0, 1, 1);


        gridLayout->addLayout(gridLayout_2, 1, 1, 1, 1);


        gridLayout_4->addWidget(gb_advsettings, 2, 0, 1, 1);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        pbMeasurement_uCConfig = new CustomControls::PushButton(Measurement);
        pbMeasurement_uCConfig->setObjectName(QString::fromUtf8("pbMeasurement_uCConfig"));

        gridLayout_3->addWidget(pbMeasurement_uCConfig, 1, 0, 1, 1);

        cbIndirectThr = new CustomControls::ComboBox(Measurement);
        cbIndirectThr->setObjectName(QString::fromUtf8("cbIndirectThr"));

        gridLayout_3->addWidget(cbIndirectThr, 5, 1, 1, 1);

        pbMeasurement_getdata = new CustomControls::PushButton(Measurement);
        pbMeasurement_getdata->setObjectName(QString::fromUtf8("pbMeasurement_getdata"));

        gridLayout_3->addWidget(pbMeasurement_getdata, 2, 0, 1, 1);

        line = new QFrame(Measurement);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(line, 6, 0, 1, 4);

        line_2 = new QFrame(Measurement);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(line_2, 4, 0, 1, 4);

        spInterval = new QSpinBox(Measurement);
        spInterval->setObjectName(QString::fromUtf8("spInterval"));
        spInterval->setInputMethodHints(Qt::ImhDigitsOnly);
        spInterval->setMinimum(6);
        spInterval->setMaximum(65);
        spInterval->setSingleStep(1);
        spInterval->setValue(30);

        gridLayout_3->addWidget(spInterval, 3, 1, 1, 1);

        pbMeasurement_DefaultCfg = new CustomControls::PushButton(Measurement);
        pbMeasurement_DefaultCfg->setObjectName(QString::fromUtf8("pbMeasurement_DefaultCfg"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pbMeasurement_DefaultCfg->sizePolicy().hasHeightForWidth());
        pbMeasurement_DefaultCfg->setSizePolicy(sizePolicy);
        pbMeasurement_DefaultCfg->setMinimumSize(QSize(85, 23));

        gridLayout_3->addWidget(pbMeasurement_DefaultCfg, 0, 0, 1, 1);

        label_3 = new QLabel(Measurement);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_3->addWidget(label_3, 3, 2, 1, 2);

        label_2 = new QLabel(Measurement);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_3->addWidget(label_2, 5, 0, 1, 1);

        label = new QLabel(Measurement);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_3->addWidget(label, 3, 0, 1, 1);

        pbMeasurement_Once = new CustomControls::PushButton(Measurement);
        pbMeasurement_Once->setObjectName(QString::fromUtf8("pbMeasurement_Once"));

        gridLayout_3->addWidget(pbMeasurement_Once, 1, 3, 1, 1);

        pbMeasurement_Start = new CustomControls::PushButton(Measurement);
        pbMeasurement_Start->setObjectName(QString::fromUtf8("pbMeasurement_Start"));
        pbMeasurement_Start->setCheckable(true);
        pbMeasurement_Start->setChecked(false);

        gridLayout_3->addWidget(pbMeasurement_Start, 0, 3, 1, 1);


        gridLayout_4->addLayout(gridLayout_3, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer, 3, 0, 1, 1);


        retranslateUi(Measurement);

        cb_projection->setCurrentIndex(1);

    } // setupUi

    void retranslateUi(Applet *Measurement)
    {
        Measurement->setWindowTitle(QApplication::translate("Measurement", "Measurement", 0, QApplication::UnicodeUTF8));
        gb_advsettings->setTitle(QApplication::translate("Measurement", "Advanced setup settings", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("Measurement", "Memory tolerance (cm)", 0, QApplication::UnicodeUTF8));
        cb_AdAxRg->setText(QApplication::translate("Measurement", "Adaptive axis range", 0, QApplication::UnicodeUTF8));
        le_S3Y->setText(QApplication::translate("Measurement", "0", 0, QApplication::UnicodeUTF8));
        le_S3Y->setPlaceholderText(QString());
        le_S3X->setText(QApplication::translate("Measurement", "60", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("Measurement", "Sensor 3 coordinates [X, Y] (cm)", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("Measurement", "Memory threshold (count)", 0, QApplication::UnicodeUTF8));
        pbMeasurement_advsettsave->setText(QApplication::translate("Measurement", "Save settings", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("Measurement", "Algorithm tolerance", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("Measurement", "Speed of sound (m/s)", 0, QApplication::UnicodeUTF8));
        le_S2X->setText(QApplication::translate("Measurement", "30", 0, QApplication::UnicodeUTF8));
        le_S2X->setPlaceholderText(QApplication::translate("Measurement", "X ", 0, QApplication::UnicodeUTF8));
        cb_projection->clear();
        cb_projection->insertItems(0, QStringList()
         << QApplication::translate("Measurement", "View from behind", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Measurement", "View from front", 0, QApplication::UnicodeUTF8)
        );
        le_S2Y->setInputMask(QString());
        le_S2Y->setText(QApplication::translate("Measurement", "2.25", 0, QApplication::UnicodeUTF8));
        le_SOS->setText(QApplication::translate("Measurement", "343", 0, QApplication::UnicodeUTF8));
        le_SOS->setPlaceholderText(QString());
        label_4->setText(QApplication::translate("Measurement", "Sensor 2 coordinates [X, Y] (cm)", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("Measurement", "Type of projection", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("Measurement", "Intersecion tolerance", 0, QApplication::UnicodeUTF8));
        pbMeasurement_uCConfig->setText(QApplication::translate("Measurement", "Configure microcontroller", 0, QApplication::UnicodeUTF8));
        cbIndirectThr->clear();
        cbIndirectThr->insertItems(0, QStringList()
         << QApplication::translate("Measurement", "Thr. 1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Measurement", "Thr. 2", 0, QApplication::UnicodeUTF8)
        );
        pbMeasurement_getdata->setText(QApplication::translate("Measurement", "Get Data", 0, QApplication::UnicodeUTF8));
        pbMeasurement_DefaultCfg->setText(QApplication::translate("Measurement", "Default P215 configuration", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Measurement", "<5 ms; 65 ms>; 1 ms step", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Measurement", "Indirect mode THR. table", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Measurement", "Measurement period [ms]", 0, QApplication::UnicodeUTF8));
        pbMeasurement_Once->setText(QApplication::translate("Measurement", "Meas. Once", 0, QApplication::UnicodeUTF8));
        pbMeasurement_Start->setText(QApplication::translate("Measurement", "Start/Stop", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Measurement: public Ui_Measurement {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEASUREMENT_H
