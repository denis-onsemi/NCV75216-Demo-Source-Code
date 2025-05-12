#ifndef CUSTOMCOMBOBOX_H
#define CUSTOMCOMBOBOX_H

#include "universalcontrol.h"

namespace CustomControls
{


/****************************************************************************
   * @class :
   * ComboBox
   ****************************************************************************/
class ComboBox : public QComboBox, public UniversalControl
{
    Q_OBJECT

    enum{
        GIGA = (0),
        MEGA =(1),
        KILO =(2),
        NONE =(3),
        MILI =(4),
        MICRO =(5),
        NANO =(6),
        PICO= (7),
        PREFIX_COUNT =(8)
    };

    static const char m_metricPrefixSymbol[PREFIX_COUNT];
    static const double m_metricPrefixMultiplier[PREFIX_COUNT];

public:
    ComboBox(QWidget *p);

    ~ComboBox();
    bool editEnabled()const;
    void setEditEnabled(bool on);
    void setEditValue(double v);
    double editValue();
    const QString name(DragName dragname=NODENAME);
    QByteArray array();
    void setArray(const QByteArray &array);

    bool bind(const char *slot, QObject *pobj);
    void unbind();
    void setDescription(const QString &description);
    const QString  description();
    void setCaption(const QString & ){}
    const QString  caption(){return QString();}
    bool setCurrentItem(const QString &itemtext);
    bool setCurrentItem(const QByteArray &itemdata);
    bool setCurrentItem(const double &v);
    void setDictionary(const QString &dict_script);
    qint32 currentItem();
    bool useDictionary() const;
    QVariant currentData();
    bool engFormat() const;
    void setEngFormat(bool on);

signals:
    void signal_activated();

protected:
    int m_prefix_i ;
    bool m_engFormat;
    bool m_read_only;
    bool m_dictionary_set;
    bool m_changed;
    void leaveEvent(QEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *pevent);
    void keyPressEvent(QKeyEvent *pev);

    static int detectPrefix(const QString &text);
    double toNormalFormat(const QString &text);
    QString toEngFormat(double v);

    /* paint event used for painting rectangle on CTRL press*/
    void paintEvent(QPaintEvent *pev);
    void markChanged();
    bool isChanged() const;
    void unmark();

private slots:
    void slot_activated();
    void slot_TextChanged();
};
}
#endif
