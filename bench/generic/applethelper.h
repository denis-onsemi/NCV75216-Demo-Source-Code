#ifndef APPLETHELPER_H
#define APPLETHELPER_H

#include <QWidget>
class AppletHelper;



#include "applet.h"


class AppletHelper : public QWidget
{
    Q_OBJECT
    
public:
    explicit AppletHelper(Applet *papplet,const QString &title);
    ~AppletHelper();
    void setHtml(const QString &html)    ;
    void makeHelp(const QString &command);
    void appendHelpText(const QString &text) {m_help_text+=text;}
    void clearHelpText() {m_help_text.clear();}
    void updateConfigsList();
    void saveConfig();
    bool loadConfig(const QString &filename);
private slots:

    void slot_pbUpdateAll_clicked();

    void slot_pbSet_clicked();
    void slot_pbFill0_clicked();
    void slot_pbFill1_clicked();

    void slot_pbGet_clicked();
    void slot_pbRun_clicked();
    void slot_FindInNodes(const QString &text);
    // [SOME FUNCTION FOR SAVE]
    /* exports registers to 101 format */
    void slot_exportRegistersTo101();
    /* imports register from 101 format */
    void slot_importRegistersFrom101();
    void slot_ApplyFile();

signals:
    void signal_update();

private:    
    /* dialog with checkboxes list of registers to save or load */
    const QStringList dialogGetRegisters();/* legacy from VB*/
    QComboBox *cmConfigs;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tabNodes;
    QGridLayout *gridLayout_2;
    QFrame *frame;
    QLabel *configLabel ;
    QHBoxLayout *horizontalLayout;
    QComboBox *cmNodeItem;
    QLineEdit *mp_highlight_text_in_nodes;
    QLabel *label;
    CustomControls::LineEdit *leValue;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *pbSet;
    QPushButton *pbGet;
    QPushButton *pbFill0;
    QPushButton *pbFill1;
    QPushButton *pbExport101;
    QPushButton *pbImport101;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbUpdateAll;
    QTextEdit *teNodes;
    QWidget *tabFunctions;
    QGridLayout *gridLayout_3;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout_2;
    QComboBox *cmFunctionName;
    QLabel *label_2;
    CustomControls::LineEdit *leParam;
    QLabel *label_5;
    CustomControls::LineEdit *leReturnInt;
    CustomControls::RealEdit *leReturnReal;
    QLineEdit *leReturnText;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pbRun;
    QSpacerItem *horizontalSpacer_2;
    QTextEdit *teFunctions;
    /* help text */
    QString     m_help_text;
    Applet *mp_applet;    
    void setupUi();
};

#endif // APPLETHELPER_H
