#ifndef CUSTOMCONTROLS_REGISTERBOX_H
#define CUSTOMCONTROLS_REGISTERBOX_H


#include "customcontrols.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <register.h>
#include <QScrollArea>
#include <QLabel>
#include <QWidget>
#include <QGroupBox>

namespace CustomControls
{


//enum {
#define   ShowHeader  1
#define   BankZebra 2
#define   AbsoluteOffset 4
#define   EnableChecked  8

//}RegisterBoxOptions;


/****************************************************************************
 * @class :
 * RegisterBox
 ****************************************************************************/
class RegisterBox : public QScrollArea
{


public:



    class Control:public QWidget
    {
    public:
        enum {Bit,Field};
        Control(QWidget *p,int type);
        virtual ~Control(){}

        void setAccessibleName(const QString &name);
        void setCaption(const QString &caption);
        void setValue(int v);

    protected:        
        void contextMenuEvent(QContextMenuEvent *){}

    private:
        int m_type;
        QString m_background_color;        



        CheckBox    *mp_cb;


        QLabel      *mp_caption;
        LineEdit    *mp_le;
    };




    void setColumnsCount(int n){m_cols = n;}
    RegisterBox(QWidget *parent);
    ~RegisterBox();

    /* adds register regname pointer preg*/
    void addRegister( Register &reg);

    void clear();

    QStringList items() const;
    Control* control(const QString &item) const;
    void setGroupName(const QString &name) {m_group_name = name;}
    QString groupName() const {return m_group_name;}


    void setOptions(unsigned int opt);
    QList<int> checkedBanks();
    int banksCount(){return m_banks.count();}
    void setChecked(int n,bool on);

private:
    QMap<int, QGroupBox*> m_banks;

    unsigned int m_opt;

    QString m_group_name;
    int m_cols;
    quint32 m_options;
   // QList<RegisterBox::Control* > m_controls;

};



}

#endif // CUSTOMCONTROLS_RegisterBox_H
