/**
 * @file:customdialogs.h   -
 * @description:
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-52-19
 *
 */

#ifndef CUSTOMDIALOGS_H
#define CUSTOMDIALOGS_H

#include <QCheckBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QLabel>

class DialogCheckBoxes : public QDialog
{
    Q_OBJECT
public:
    /* type of items representation */
    typedef enum
    {
        CHECKBOX, // as checkboxes
        RADIOBUTTON, // as radio buttons
        BUTTON //as button
    } Type;

    explicit DialogCheckBoxes(Type type = CHECKBOX, QWidget *parent = 0);
    ~DialogCheckBoxes();
    /* sets data from stringlist as items*/
    void inflate(const QStringList &list);
    void setChecked(const QList<int> &list);
    /*adds widget to bottom */
    void addWidget(QWidget *pwidget);
    /* reads string list of checked */
    qint32 readStringList(QStringList &list);
    /* reads index list as int values*/
    qint32 readIndexList(QList<qint32> &list);

private:
    Type mp_type;
    QListWidget *mp_list_widget;
    QVBoxLayout *mp_layout;
    QList<QListWidgetItem *> *mp_widgetitems;
    QList<QAbstractButton *> *mp_checkboxes;
    QDialogButtonBox *mp_buttonbox;

signals:

private slots:
    void slot_SetAllOn();
    void slot_SetAllOff();

};

class DialogProgressBar : public QDialog
{
    Q_OBJECT
public:
    explicit DialogProgressBar(QWidget *parent = 0, const QPixmap &pixmap = QPixmap());
    ~DialogProgressBar();
    /* adds custom widget to bottom */
    void addWidget(QWidget *pwidget);
    /* sets progress bar value */
    void setProgress(qint32 value,qint32 maximum);
    /* returns progress bar value */
    qint32 progress();
    /* sets text */
    void setText(const QString &text);
    /* sleep static function */
    static void sleep(qint32 sleep);

private:
    /* progreeebar control */
    QProgressBar m_progress_bar;
    /* layout*/
    QVBoxLayout m_layout;
    /* label with  message */
    QLabel m_label;
    /* label for image*/
    QLabel m_image;

};
#endif // CUSTOMDIALOGS_H
