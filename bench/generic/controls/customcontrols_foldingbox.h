#ifndef FOLDINGBOX_H
#define FOLDINGBOX_H

#include <QGroupBox>

namespace CustomControls
{

/****************************************************************************
 * @class :
 * FoldingGroup
 ****************************************************************************/

class FoldingBox : public QGroupBox
{
    Q_OBJECT

public:
    explicit FoldingBox(QWidget *parent = 0);
    void setExpanded(bool on);
    int formerHeight()const ;
    int enrolledHeight();
    void enableDragDrop(bool yes);

protected:
    bool m_drag_drop_run;
    void mouseDoubleClickEvent(QMouseEvent *);

private:
    int m_former_size;
    void mouseReleaseEvent(QMouseEvent *pevent);
    void mousePressEvent(QMouseEvent *pevent);
    void leaveEvent(QEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *pev);

signals:
    void signal_resized();
    void signal_dragdrop();

public slots:
    
};
}

#endif

