#ifndef CUSTOMCONTROLS_HTMLLISTBOX_H
#define CUSTOMCONTROLS_HTMLLISTBOX_H
#include <QStyledItemDelegate>
#include <QPainter>
#include <QTextDocument>
#include <QStandardItemModel>
#include <QListView>




namespace CustomControls
{

class HtmlTextItemDelegate : public QStyledItemDelegate
{
public:
    HtmlTextItemDelegate(QWidget *p=0): QStyledItemDelegate(p)    {}
    inline QWidget *createEditor(QWidget*,
                                 const QStyleOptionViewItem&,
                                 const QModelIndex&) const;
    void setSize(const QSize &size);
    const QSize size() const;
protected:
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
private:
    QSize m_size;
};


class HtmlListBox: public QListView
{
public:
    HtmlListBox(QWidget *p);
    HtmlListBox(qint32 itemsCount, QWidget *p=0);
    void setRowSize(quint32 width,quint32 height);
    void setDataList(const QStringList &list);
    void setRowText(qint32 item_number, const QString &text);
    void setCurrentRow(qint32 item_number);
    qint32 currentRow();
    QString rowText(qint32 row);
    void addRow(const QString &item_text);
    qint32 count();
    void clear();

private:
    HtmlTextItemDelegate* pitemdelegate;
    QStandardItemModel *pmodel;
    void resizeEvent(QResizeEvent *);

};

} // namespace CustomControls

#endif // CUSTOMCONTROLS_HTMLLISTBOX_H
