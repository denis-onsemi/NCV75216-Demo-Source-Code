#ifndef CUSTOMCONTROLS_SUPERBOX_H
#define CUSTOMCONTROLS_SUPERBOX_H


#include "customcontrols.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <register.h>
#include <QScrollArea>
#include <QLabel>

namespace CustomControls
{



/****************************************************************************
 * @class :
 * SuperBox
 ****************************************************************************/
class SuperBox : public QScrollArea
{
    /* this is Checkbox with special function 
     * as right click highlighting */
    class CheckBoxForSuperBox:public CheckBox
    {
    public:
        CheckBoxForSuperBox(QWidget *p);
        void highlight(bool on);
        void setBackground(const QString &color);

    protected:
        void mousePressEvent(QMouseEvent *pevent);
        void contextMenuEvent(QContextMenuEvent *){}

    private:
        void updateStyleSheet();
        QString m_background_color;
        bool m_highlighted;
    };

    /* this is  list Item*/
    struct Item
    {
        Item(QWidget *p);
        ~Item();
        CheckBoxForSuperBox *mp_cb;
        QLabel *mp_lb_number;
    };


public:
    typedef enum
    {
        BRIEF = 0,
        LARGE,
        SHORT
    } LayoutType;


    SuperBox(QWidget *parent);
    ~SuperBox();
    /* sets layout type BRIEF, LARGE,SHORT */
    void setLayoutType(LayoutType type);
    /* reconstruct - recalculates checkboxes positions */
    void reconstruct();
    /* adds register regname pointer preg*/
    void addRegister(const QString &regname, Register *preg);
    /* adds single bit with name */
    void addBit(int group,const QString &bitname, const QString &caption
                , const QString &cb_color= QString(),const QString &descr=QString());
    typedef enum {BY_TEXT,BY_ACCESSIBLE_NAME} HighlightMethod;
    void highlight(HighlightMethod method, const QString by_text);
    void clearHighlighiting();
    void clear();
    qint32 itemCount() const;
    CheckBox* controlAt(qint32 i) const;

private:
    LayoutType m_type;
    qint32 m_columns;
    qint32 m_rows;
    qint32 m_width_lb;
    qint32 m_width_cb;
    /* labels intended for short mode rows*/
    QList<QLabel *> m_short_mode_labels;
    QList<SuperBox::Item *> m_list;
    /* clear all labels for short mode*/
    void clearAllLabels();
    /* draws item at exact position with w1-label width ,w2-checkbox*/
    void drawItem(SuperBox::Item *pitem, qint32 x, qint32 y, qint32 w1, qint32 w2);
    /* adds item widget with name text-is checkbox caption*/
    void addItemWidget(int group,const QString &name, const QString &caption
                       , const QString &cb_color= QString(),const QString &descr=QString());
};



}

#endif // CUSTOMCONTROLS_SUPERBOX_H
