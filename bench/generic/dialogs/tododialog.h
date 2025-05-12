#ifndef TODODIALOG_H
#define TODODIALOG_H
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QPushButton>


class TodoDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit TodoDialog(QWidget *parent = 0);
    ~TodoDialog();
    void showDialog(const QString &alltext);
    const QString text() const;

private slots:
    void slot_pbAdd_clicked();
    void slot_pbClose_clicked();

private:
    void setupUi();
    void deleteUi();
    QGridLayout *gridLayout;
    QLabel *label;
    QComboBox *cmType;
    QTextEdit *teAll;
    QLineEdit *leText;
    QPushButton *pbAdd;
    QPushButton *pbClose;
    QLineEdit *leAuthor;
    QLabel *label_2;
};

#endif // TODODIALOG_H
