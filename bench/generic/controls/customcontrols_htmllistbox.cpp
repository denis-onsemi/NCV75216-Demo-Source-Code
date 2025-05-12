#include "customcontrols_htmllistbox.h"
#include <QTime>

#include <QDebug>
namespace CustomControls {

QWidget *HtmlTextItemDelegate :: createEditor(QWidget*,
                             const QStyleOptionViewItem&,
                             const QModelIndex&) const
{
    return 0;
}

void HtmlTextItemDelegate::setSize(const QSize &size)
{
    m_size = size;
}

const QSize HtmlTextItemDelegate::size() const
{
    return m_size;
}
void HtmlTextItemDelegate ::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    QStyleOptionViewItemV4 options = option;
    initStyleOption(&options, index);
    painter->save();
    QTextDocument doc;
    doc.setHtml(options.text);
    options.text = "";
    options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &options, painter);
    painter->translate(options.rect.left(), options.rect.top());
    QRect clip(0, 0, options.rect.width(), options.rect.height());
    doc.drawContents(painter, clip);
    painter->restore();

}
QSize HtmlTextItemDelegate ::sizeHint ( const QStyleOptionViewItem & , const QModelIndex &  ) const
{
    return m_size;
}

HtmlListBox::HtmlListBox(QWidget *p):QListView(p)
{
    pitemdelegate = new HtmlTextItemDelegate(this);
    pitemdelegate->setSize(QSize(width()/2-10,20));
    pmodel = new QStandardItemModel(1,1,this);
    setModel(pmodel);
    setItemDelegate(pitemdelegate);
}
HtmlListBox::HtmlListBox(qint32 itemsCount, QWidget *p):QListView(p)
{
    pitemdelegate = new HtmlTextItemDelegate(this);
    pitemdelegate->setSize(QSize(width()/2-10,20));
    pmodel = new QStandardItemModel(itemsCount,1,this);
    setModel(pmodel);
    setItemDelegate(pitemdelegate);
}

void HtmlListBox::setRowSize(quint32 width, quint32 height)
{
    pitemdelegate->setSize(QSize(width,height));
}


qint32 HtmlListBox::currentRow()
{
    QModelIndex index = this->currentIndex();
    return pmodel->itemFromIndex(index)->row();
}

QString HtmlListBox::rowText(qint32 row)
{
    QModelIndex index = model()->index(row,0);
    return pmodel->itemFromIndex(index)->text();
}

void HtmlListBox::addRow(const QString &item_text)
{
    qint32 i = pmodel->rowCount();
    pmodel->setRowCount(i+1);
    QModelIndex index = pmodel->index(i,0);
    pmodel->setData(index, item_text);
}

qint32 HtmlListBox::count()
{
    return pmodel->rowCount();
}

void HtmlListBox::clear()
{
   pmodel->clear();
   pmodel->setColumnCount(1);
}

void HtmlListBox::resizeEvent(QResizeEvent * /*e*/)
{
    pitemdelegate->setSize(QSize(width()/2-10
                                 ,pitemdelegate->size().height()));
}

void HtmlListBox::setCurrentRow(qint32 item_number)
{
    QModelIndex index = pmodel->index(item_number,0);
    setCurrentIndex(index);
}

void HtmlListBox::setDataList(const QStringList &list)
{
    pmodel->setRowCount(list.count());
    for(int i =0;i<pmodel->rowCount();i++){
        QModelIndex index = pmodel->index(i,0);
        pmodel->setData(index,list.at(i));
    }
}
void HtmlListBox::setRowText(qint32 item_number, const QString &text)
{
    QModelIndex index = pmodel->index(item_number,0);
    pmodel->setData(index,text);
}
} // namespace CustomControls
