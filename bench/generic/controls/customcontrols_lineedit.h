#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H

#include "universalcontrol.h"
#include <QLineEdit>

namespace CustomControls
{


/****************************************************************************
 * @class :
 * LineEdit
 ****************************************************************************/
class LineEdit : public QLineEdit, public UniversalControl
{
    Q_OBJECT
public:
    explicit LineEdit(QWidget *p);

    ~LineEdit();

    qint32 inputBase() const;

    bool editEnabled()const;
    void setEditEnabled(bool on);
    void setEditValue(double v);
    double editValue();
    void setInputBase(int);

    const QString name(DragName dragname=NODENAME);

    bool bind(const char *slot, QObject *pobj);
    void unbind();
    void setDescription(const QString &description);
    const QString  description();
    void setCaption(const QString & ){}
    const QString  caption(){return QString();}
    QByteArray array();
    void setArray(const QByteArray &data);

signals:
    void signal_activated();

protected:    
    bool m_changed;
    bool m_array;
    qint32 m_input_base;
    bool m_allowUpdate;
    QActionGroup *mp_action_group;
    QMenu *mp_menu;
    QAction *mp_actionHex;
    QAction *mp_actionBin;
    QAction *mp_actionDec;
    QAction *mp_actionArray;
    void focusInEvent(QFocusEvent *pevent);
    void focusOutEvent(QFocusEvent *pevent);
    void contextMenuEvent(QContextMenuEvent *event);
    void mousePressEvent(QMouseEvent *pevent);
    /* paint event used for painting rectangle on CTRL press*/
    void paintEvent(QPaintEvent *pev);
    void wheelEvent(QWheelEvent *pev);    
    void keyPressEvent(QKeyEvent *pev);
    void markChanged();
    void markError();
    void unmark();
    bool isChanged()const;
    void construct();

private slots:
    void slot_TextChanged();
    void slot_ActionMode(bool);
    void slot_triggered();


};




}
#endif
