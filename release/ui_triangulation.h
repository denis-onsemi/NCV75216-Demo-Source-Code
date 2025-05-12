/********************************************************************************
** Form generated from reading UI file 'triangulation.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRIANGULATION_H
#define UI_TRIANGULATION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>
#include "customcontrols.h"
#include "customcontrols_combobox.h"
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_Triangulation
{
public:
    QGridLayout *gridLayout;
    QTabWidget *tw_results;
    QWidget *tab_5;
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout_6;
    QLabel *lb_obj2title;
    QSpacerItem *verticalSpacer_7;
    QSpacerItem *verticalSpacer_8;
    QFrame *line_3;
    QLabel *lb_obj4;
    QLabel *lb_obj1CF;
    QLabel *lb_obj6;
    QLabel *lb_obj5;
    QLabel *lb_obj2;
    QLabel *lb_obj3;
    QLabel *lb_obj1title;
    QLabel *lb_obj1;
    QLabel *lb_obj2CF;
    QLabel *label_7;
    QLabel *label_10;
    QSpacerItem *verticalSpacer_3;
    QWidget *tab_6;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout_7;
    QLabel *label_11;
    QLabel *label_8;
    QSpacerItem *verticalSpacer_11;
    QLabel *lb_obj1CF_2;
    QFrame *line_4;
    QLabel *lb_obj3title_2;
    QSpacerItem *verticalSpacer_10;
    QLabel *lb_obj1title_2;
    QLabel *lb_obj4CF_2;
    QLabel *lb_obj2CF_2;
    QLabel *lb_obj5_2;
    QLabel *lb_obj6_2;
    QLabel *lb_obj3_2;
    QLabel *lb_obj3CF_2;
    QLabel *lb_obj1_2;
    QLabel *lb_obj4_2;
    QLabel *lb_obj2title_2;
    QLabel *lb_obj4title_2;
    QLabel *lb_obj2_2;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer;
    QSpacerItem *verticalSpacer_4;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_2;
    CustomControls::ComboBox *cbAlgChoice;
    QCustomPlot *ptTriang;
    QFrame *line_2;
    CustomControls::PushButton *pbTriangulation_ReadTriData;
    QLabel *label_6;
    QFrame *line;

    void setupUi(QWidget *Triangulation)
    {
        if (Triangulation->objectName().isEmpty())
            Triangulation->setObjectName(QString::fromUtf8("Triangulation"));
        Triangulation->resize(881, 723);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/board/board.png"), QSize(), QIcon::Normal, QIcon::Off);
        Triangulation->setWindowIcon(icon);
        gridLayout = new QGridLayout(Triangulation);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tw_results = new QTabWidget(Triangulation);
        tw_results->setObjectName(QString::fromUtf8("tw_results"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tw_results->sizePolicy().hasHeightForWidth());
        tw_results->setSizePolicy(sizePolicy);
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        gridLayout_3 = new QGridLayout(tab_5);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_6 = new QGridLayout();
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        lb_obj2title = new QLabel(tab_5);
        lb_obj2title->setObjectName(QString::fromUtf8("lb_obj2title"));
        QFont font;
        font.setPointSize(13);
        lb_obj2title->setFont(font);
        lb_obj2title->setStyleSheet(QString::fromUtf8("color: rgb(0, 170, 0);"));

        gridLayout_6->addWidget(lb_obj2title, 5, 0, 1, 1);

        verticalSpacer_7 = new QSpacerItem(5, 100, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_6->addItem(verticalSpacer_7, 4, 1, 1, 1);

        verticalSpacer_8 = new QSpacerItem(5, 100, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_6->addItem(verticalSpacer_8, 6, 1, 1, 1);

        line_3 = new QFrame(tab_5);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        gridLayout_6->addWidget(line_3, 8, 0, 1, 1);

        lb_obj4 = new QLabel(tab_5);
        lb_obj4->setObjectName(QString::fromUtf8("lb_obj4"));
        lb_obj4->setFont(font);

        gridLayout_6->addWidget(lb_obj4, 16, 0, 1, 1);

        lb_obj1CF = new QLabel(tab_5);
        lb_obj1CF->setObjectName(QString::fromUtf8("lb_obj1CF"));
        QFont font1;
        font1.setPointSize(15);
        lb_obj1CF->setFont(font1);
        lb_obj1CF->setWordWrap(true);

        gridLayout_6->addWidget(lb_obj1CF, 4, 0, 1, 1);

        lb_obj6 = new QLabel(tab_5);
        lb_obj6->setObjectName(QString::fromUtf8("lb_obj6"));
        lb_obj6->setFont(font);

        gridLayout_6->addWidget(lb_obj6, 18, 0, 1, 1);

        lb_obj5 = new QLabel(tab_5);
        lb_obj5->setObjectName(QString::fromUtf8("lb_obj5"));
        lb_obj5->setFont(font);

        gridLayout_6->addWidget(lb_obj5, 17, 0, 1, 1);

        lb_obj2 = new QLabel(tab_5);
        lb_obj2->setObjectName(QString::fromUtf8("lb_obj2"));
        lb_obj2->setFont(font);

        gridLayout_6->addWidget(lb_obj2, 14, 0, 1, 1);

        lb_obj3 = new QLabel(tab_5);
        lb_obj3->setObjectName(QString::fromUtf8("lb_obj3"));
        lb_obj3->setFont(font);

        gridLayout_6->addWidget(lb_obj3, 15, 0, 1, 1);

        lb_obj1title = new QLabel(tab_5);
        lb_obj1title->setObjectName(QString::fromUtf8("lb_obj1title"));
        lb_obj1title->setFont(font);
        lb_obj1title->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));

        gridLayout_6->addWidget(lb_obj1title, 3, 0, 1, 1);

        lb_obj1 = new QLabel(tab_5);
        lb_obj1->setObjectName(QString::fromUtf8("lb_obj1"));
        lb_obj1->setFont(font);

        gridLayout_6->addWidget(lb_obj1, 13, 0, 1, 1);

        lb_obj2CF = new QLabel(tab_5);
        lb_obj2CF->setObjectName(QString::fromUtf8("lb_obj2CF"));
        lb_obj2CF->setFont(font1);

        gridLayout_6->addWidget(lb_obj2CF, 6, 0, 1, 1);

        label_7 = new QLabel(tab_5);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        QFont font2;
        font2.setPointSize(15);
        font2.setBold(true);
        font2.setWeight(75);
        label_7->setFont(font2);

        gridLayout_6->addWidget(label_7, 2, 0, 1, 1);

        label_10 = new QLabel(tab_5);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        QFont font3;
        font3.setPointSize(12);
        font3.setBold(true);
        font3.setWeight(75);
        label_10->setFont(font3);

        gridLayout_6->addWidget(label_10, 10, 0, 1, 1);


        gridLayout_3->addLayout(gridLayout_6, 0, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_3, 1, 0, 1, 1);

        tw_results->addTab(tab_5, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName(QString::fromUtf8("tab_6"));
        gridLayout_2 = new QGridLayout(tab_6);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_7 = new QGridLayout();
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        label_11 = new QLabel(tab_6);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setFont(font3);

        gridLayout_7->addWidget(label_11, 14, 0, 1, 1);

        label_8 = new QLabel(tab_6);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setFont(font2);

        gridLayout_7->addWidget(label_8, 2, 0, 1, 1);

        verticalSpacer_11 = new QSpacerItem(5, 55, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_7->addItem(verticalSpacer_11, 6, 1, 1, 1);

        lb_obj1CF_2 = new QLabel(tab_6);
        lb_obj1CF_2->setObjectName(QString::fromUtf8("lb_obj1CF_2"));
        QFont font4;
        font4.setPointSize(12);
        lb_obj1CF_2->setFont(font4);
        lb_obj1CF_2->setWordWrap(true);

        gridLayout_7->addWidget(lb_obj1CF_2, 4, 0, 1, 1);

        line_4 = new QFrame(tab_6);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        gridLayout_7->addWidget(line_4, 12, 0, 1, 1);

        lb_obj3title_2 = new QLabel(tab_6);
        lb_obj3title_2->setObjectName(QString::fromUtf8("lb_obj3title_2"));
        QFont font5;
        font5.setPointSize(10);
        font5.setBold(true);
        font5.setWeight(75);
        lb_obj3title_2->setFont(font5);
        lb_obj3title_2->setStyleSheet(QString::fromUtf8("color: rgb(170, 85, 0);"));

        gridLayout_7->addWidget(lb_obj3title_2, 7, 0, 1, 1);

        verticalSpacer_10 = new QSpacerItem(5, 55, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_7->addItem(verticalSpacer_10, 4, 1, 1, 1);

        lb_obj1title_2 = new QLabel(tab_6);
        lb_obj1title_2->setObjectName(QString::fromUtf8("lb_obj1title_2"));
        lb_obj1title_2->setFont(font5);
        lb_obj1title_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));

        gridLayout_7->addWidget(lb_obj1title_2, 3, 0, 1, 1);

        lb_obj4CF_2 = new QLabel(tab_6);
        lb_obj4CF_2->setObjectName(QString::fromUtf8("lb_obj4CF_2"));
        lb_obj4CF_2->setFont(font4);

        gridLayout_7->addWidget(lb_obj4CF_2, 10, 0, 1, 1);

        lb_obj2CF_2 = new QLabel(tab_6);
        lb_obj2CF_2->setObjectName(QString::fromUtf8("lb_obj2CF_2"));
        lb_obj2CF_2->setFont(font4);

        gridLayout_7->addWidget(lb_obj2CF_2, 6, 0, 1, 1);

        lb_obj5_2 = new QLabel(tab_6);
        lb_obj5_2->setObjectName(QString::fromUtf8("lb_obj5_2"));
        lb_obj5_2->setFont(font);

        gridLayout_7->addWidget(lb_obj5_2, 21, 0, 1, 1);

        lb_obj6_2 = new QLabel(tab_6);
        lb_obj6_2->setObjectName(QString::fromUtf8("lb_obj6_2"));
        lb_obj6_2->setFont(font);

        gridLayout_7->addWidget(lb_obj6_2, 22, 0, 1, 1);

        lb_obj3_2 = new QLabel(tab_6);
        lb_obj3_2->setObjectName(QString::fromUtf8("lb_obj3_2"));
        lb_obj3_2->setFont(font);

        gridLayout_7->addWidget(lb_obj3_2, 19, 0, 1, 1);

        lb_obj3CF_2 = new QLabel(tab_6);
        lb_obj3CF_2->setObjectName(QString::fromUtf8("lb_obj3CF_2"));
        lb_obj3CF_2->setFont(font4);

        gridLayout_7->addWidget(lb_obj3CF_2, 8, 0, 1, 1);

        lb_obj1_2 = new QLabel(tab_6);
        lb_obj1_2->setObjectName(QString::fromUtf8("lb_obj1_2"));
        lb_obj1_2->setFont(font);

        gridLayout_7->addWidget(lb_obj1_2, 17, 0, 1, 1);

        lb_obj4_2 = new QLabel(tab_6);
        lb_obj4_2->setObjectName(QString::fromUtf8("lb_obj4_2"));
        lb_obj4_2->setFont(font);

        gridLayout_7->addWidget(lb_obj4_2, 20, 0, 1, 1);

        lb_obj2title_2 = new QLabel(tab_6);
        lb_obj2title_2->setObjectName(QString::fromUtf8("lb_obj2title_2"));
        lb_obj2title_2->setFont(font5);
        lb_obj2title_2->setStyleSheet(QString::fromUtf8("color: rgb(0, 170, 0);"));

        gridLayout_7->addWidget(lb_obj2title_2, 5, 0, 1, 1);

        lb_obj4title_2 = new QLabel(tab_6);
        lb_obj4title_2->setObjectName(QString::fromUtf8("lb_obj4title_2"));
        lb_obj4title_2->setFont(font5);
        lb_obj4title_2->setStyleSheet(QString::fromUtf8("color: rgb(170, 85, 255);"));

        gridLayout_7->addWidget(lb_obj4title_2, 9, 0, 1, 1);

        lb_obj2_2 = new QLabel(tab_6);
        lb_obj2_2->setObjectName(QString::fromUtf8("lb_obj2_2"));
        lb_obj2_2->setFont(font);

        gridLayout_7->addWidget(lb_obj2_2, 18, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(5, 55, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_7->addItem(verticalSpacer_2, 10, 1, 1, 1);

        verticalSpacer = new QSpacerItem(5, 55, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_7->addItem(verticalSpacer, 8, 1, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_7->addItem(verticalSpacer_4, 23, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout_7, 0, 0, 1, 1);

        tw_results->addTab(tab_6, QString());

        gridLayout->addWidget(tw_results, 6, 7, 1, 1);

        label_3 = new QLabel(Triangulation);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 2);

        horizontalSpacer_2 = new QSpacerItem(200, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 7, 1, 1);

        cbAlgChoice = new CustomControls::ComboBox(Triangulation);
        cbAlgChoice->setObjectName(QString::fromUtf8("cbAlgChoice"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(cbAlgChoice->sizePolicy().hasHeightForWidth());
        cbAlgChoice->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(cbAlgChoice, 1, 4, 1, 1);

        ptTriang = new QCustomPlot(Triangulation);
        ptTriang->setObjectName(QString::fromUtf8("ptTriang"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(ptTriang->sizePolicy().hasHeightForWidth());
        ptTriang->setSizePolicy(sizePolicy2);
        ptTriang->setMinimumSize(QSize(0, 200));

        gridLayout->addWidget(ptTriang, 6, 0, 1, 7);

        line_2 = new QFrame(Triangulation);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_2, 1, 2, 1, 1);

        pbTriangulation_ReadTriData = new CustomControls::PushButton(Triangulation);
        pbTriangulation_ReadTriData->setObjectName(QString::fromUtf8("pbTriangulation_ReadTriData"));
        sizePolicy.setHeightForWidth(pbTriangulation_ReadTriData->sizePolicy().hasHeightForWidth());
        pbTriangulation_ReadTriData->setSizePolicy(sizePolicy);

        gridLayout->addWidget(pbTriangulation_ReadTriData, 1, 6, 1, 1);

        label_6 = new QLabel(Triangulation);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 1, 3, 1, 1);

        line = new QFrame(Triangulation);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 1, 5, 1, 1);


        retranslateUi(Triangulation);

        cbAlgChoice->setCurrentIndex(1);

    } // setupUi

    void retranslateUi(QWidget *Triangulation)
    {
        Triangulation->setWindowTitle(QApplication::translate("Triangulation", "Form", 0, QApplication::UnicodeUTF8));
        lb_obj2title->setText(QApplication::translate("Triangulation", "Object 2:", 0, QApplication::UnicodeUTF8));
        lb_obj4->setText(QApplication::translate("Triangulation", "(Obj4 distance)", 0, QApplication::UnicodeUTF8));
        lb_obj1CF->setText(QApplication::translate("Triangulation", "(Obj1 coordinates)", 0, QApplication::UnicodeUTF8));
        lb_obj6->setText(QApplication::translate("Triangulation", "(Obj6 distance)", 0, QApplication::UnicodeUTF8));
        lb_obj5->setText(QApplication::translate("Triangulation", "(Obj5 distance)", 0, QApplication::UnicodeUTF8));
        lb_obj2->setText(QApplication::translate("Triangulation", "(Obj2 distance)", 0, QApplication::UnicodeUTF8));
        lb_obj3->setText(QApplication::translate("Triangulation", "(Obj3 distance)", 0, QApplication::UnicodeUTF8));
        lb_obj1title->setText(QApplication::translate("Triangulation", "Object 1:", 0, QApplication::UnicodeUTF8));
        lb_obj1->setText(QApplication::translate("Triangulation", "(Obj1 distance)", 0, QApplication::UnicodeUTF8));
        lb_obj2CF->setText(QApplication::translate("Triangulation", "(Obj2 coordinates)", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("Triangulation", "Detected positions", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("Triangulation", "Dist. to unlocalized obj.", 0, QApplication::UnicodeUTF8));
        tw_results->setTabText(tw_results->indexOf(tab_5), QApplication::translate("Triangulation", "Tripe conf.", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("Triangulation", "Dist. to unlocalized obj.", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("Triangulation", "Detected positions", 0, QApplication::UnicodeUTF8));
        lb_obj1CF_2->setText(QApplication::translate("Triangulation", "(Obj1 coordinates)", 0, QApplication::UnicodeUTF8));
        lb_obj3title_2->setText(QApplication::translate("Triangulation", "Object 3:", 0, QApplication::UnicodeUTF8));
        lb_obj1title_2->setText(QApplication::translate("Triangulation", "Object 1:", 0, QApplication::UnicodeUTF8));
        lb_obj4CF_2->setText(QApplication::translate("Triangulation", "(Obj4 coordinates)", 0, QApplication::UnicodeUTF8));
        lb_obj2CF_2->setText(QApplication::translate("Triangulation", "(Obj2 coordinates)", 0, QApplication::UnicodeUTF8));
        lb_obj5_2->setText(QApplication::translate("Triangulation", "(Obj5 distance)", 0, QApplication::UnicodeUTF8));
        lb_obj6_2->setText(QApplication::translate("Triangulation", "(Obj6 distance)", 0, QApplication::UnicodeUTF8));
        lb_obj3_2->setText(QApplication::translate("Triangulation", "(Obj3 distance)", 0, QApplication::UnicodeUTF8));
        lb_obj3CF_2->setText(QApplication::translate("Triangulation", "(Obj3 coordinates)", 0, QApplication::UnicodeUTF8));
        lb_obj1_2->setText(QApplication::translate("Triangulation", "(Obj1 distance)", 0, QApplication::UnicodeUTF8));
        lb_obj4_2->setText(QApplication::translate("Triangulation", "(Obj4 distance)", 0, QApplication::UnicodeUTF8));
        lb_obj2title_2->setText(QApplication::translate("Triangulation", "Object 2:", 0, QApplication::UnicodeUTF8));
        lb_obj4title_2->setText(QApplication::translate("Triangulation", "Object 4:", 0, QApplication::UnicodeUTF8));
        lb_obj2_2->setText(QApplication::translate("Triangulation", "(Obj2 distance)", 0, QApplication::UnicodeUTF8));
        tw_results->setTabText(tw_results->indexOf(tab_6), QApplication::translate("Triangulation", "Double conf.", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Triangulation", "Measurement setup and control is located in the 'Measurement' window", 0, QApplication::UnicodeUTF8));
        cbAlgChoice->clear();
        cbAlgChoice->insertItems(0, QStringList()
         << QApplication::translate("Triangulation", "Triple", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Triangulation", "Double", 0, QApplication::UnicodeUTF8)
        );
        pbTriangulation_ReadTriData->setText(QApplication::translate("Triangulation", "Refresh Results", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("Triangulation", "Confirmation alg. type", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Triangulation: public Ui_Triangulation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRIANGULATION_H
