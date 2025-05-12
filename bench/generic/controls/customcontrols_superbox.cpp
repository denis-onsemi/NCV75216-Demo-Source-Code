#include "customcontrols_superbox.h"

/* misc */
const qint32 CHECKBOX_WIDTH = (17);
const qint32 ROW_HEIGHT = (17);
const qint32 SPACE = (5);

/* short mode*/
const qint32 SHORT_MODE_DELIMIT_STEP = (5);
const qint32 SHORT_MODE_COLUMNS = (10);
const qint32 SHORT_MODE_DELIMETER_LENGTH = (10);
const qint32 SHORT_MODE_GROUPLABEL_WIDTH = (55);

/* brief mode */
const qint32 BRIEF_MODE_COLUMN_DIST = (8);
const qint32 BRIEF_MODE_ROWS = (40);
const char CHECKBOX_HIGHLIGHT[] = "background-color: rgb(255, 170, 0);";
const char LABEL_STYLESHEET[] = "background-color: rgb(255, 226, 178);";





namespace CustomControls
{

  SuperBox::SuperBox(QWidget *parent): QScrollArea(parent)
  {      
    setWidget(new QWidget(this));
    setStyleSheet("background-color: rgb(255, 252, 235);");
    setLayoutType( BRIEF);
  }

  SuperBox::~SuperBox()
  {
      clear();
  }

  void SuperBox::setLayoutType(SuperBox::LayoutType type)
  {
    switch (type)
      {
      case BRIEF:
        m_columns = (m_list.size() / 40) + 1;
        m_rows = BRIEF_MODE_ROWS;
        m_width_lb = 40;
        m_width_cb = 120;
        break;
      case LARGE:
        m_columns = 1;
        m_rows = m_list.size();
        m_width_lb = 50;
        m_width_cb = 500;
        break;
      case SHORT:
        m_columns = SHORT_MODE_COLUMNS;
        m_rows = (m_list.size() / SHORT_MODE_COLUMNS) + 1;
        m_width_lb = 0;
        m_width_cb = CHECKBOX_WIDTH;
        break;
      default:
        break;
      }
    m_type = type;
  }

  void SuperBox::reconstruct()
  {
    setLayoutType(m_type);
    qint32 i, j, cur_item;
    cur_item = 0;
    qint32 widget_height = m_rows * (ROW_HEIGHT + 1);
    qint32 widget_width = m_columns * (m_width_lb + m_width_cb+BRIEF_MODE_COLUMN_DIST);
    widget_width += SPACE;

    /* delete labels list*/
    clearAllLabels();
    /* construct field*/
    if (m_type == SHORT)
      {
        widget_width += (m_columns / 5 + 1) * SHORT_MODE_DELIMETER_LENGTH
            + SHORT_MODE_GROUPLABEL_WIDTH + SPACE;
        for (j = 0; j < m_rows; j++)
          {
            char tmp[12];
            SPRINTF(tmp, "%4u:%4u", cur_item, cur_item + m_columns-1);
            QLabel *plb = new QLabel(QString(tmp), widget() );
            plb->setGeometry(SPACE, j * (ROW_HEIGHT + 1), SHORT_MODE_GROUPLABEL_WIDTH, ROW_HEIGHT );
            plb->setVisible(true);
            m_short_mode_labels.append(plb);

            qint32 x = SHORT_MODE_GROUPLABEL_WIDTH;
            qint32 y = j * (ROW_HEIGHT + 1);
            for (i = 0; i < m_columns; i++)
              {
                if ( cur_item >= m_list.size() )
                  break;
                x += m_width_lb + m_width_cb;
                if ((cur_item % SHORT_MODE_DELIMIT_STEP) == 0)
                  {
                    x += SHORT_MODE_DELIMETER_LENGTH;
                  }
                drawItem(m_list[cur_item], x , y , m_width_lb, m_width_cb);
                cur_item++;
              }
          }
      }
    else
      {
        for (i = 0; i < m_columns; i++)
          {
            qint32 x = i * (m_width_lb + m_width_cb + BRIEF_MODE_COLUMN_DIST) + SPACE;
            qint32 y;
            for (j = 0; j < m_rows; j++)
              {
                if ( cur_item >= m_list.size() )
                  break;
                y = j * (ROW_HEIGHT + 1);
                drawItem(m_list[cur_item], x, y, m_width_lb, m_width_cb);
                cur_item++;
              }
          }
      }
    widget()->setGeometry(0, 0, widget_width, widget_height);
  }

  void SuperBox::addRegister(const QString &regname, Register *preg)
  {
    for (qint32 i = 0; i < preg->size(); i++)
      {       
        addItemWidget( preg->bitAt(i)->extra("group").toInt()
                      ,QString("%1[%2]").arg(regname).arg(i)
                      , preg->bitAt(i)->name()
                      , preg->bitAt(i)->extra("color")
                      , preg->bitAt(i)->description());
      }    
    reconstruct();
  }

  void SuperBox::addBit(int group,const QString &bitname, const QString &caption
                        , const QString &cb_color,const QString &descr)
  {    
    addItemWidget(group,bitname, caption,cb_color,descr);
  }

  void SuperBox::highlight(HighlightMethod method,const QString by_text)
  {
    for (qint32 i = 0; i < m_list.size(); i++)
      {
        if(method== BY_TEXT)
        {
        if (m_list[i]->mp_cb->text().contains(by_text))
          {
            m_list[i]->mp_cb->highlight(true);
          }
        }
        else if(method== BY_ACCESSIBLE_NAME)
        {
            if (m_list[i]->mp_cb->accessibleName()==by_text)
              {
                m_list[i]->mp_cb->highlight(true);
              }
        }
      }
  }

  void SuperBox::clearHighlighiting()
  {
    for (qint32 i = 0; i < m_list.size(); i++)
      {
        m_list[i]->mp_cb->highlight(false);
    }
  }

  void SuperBox::clear()
  {
      while ( m_list.size()>0)
        {
          delete m_list.first();
          m_list.removeFirst();
      }
      clearAllLabels();
  }

  qint32 SuperBox::itemCount() const
  {
      return m_list.size();
  }

  CheckBox *SuperBox::controlAt(qint32 i) const
  {
      return m_list.at(i)->mp_cb;
  }

  void SuperBox::clearAllLabels()
  {
    while (m_short_mode_labels.size() > 0)
      {
        delete m_short_mode_labels.first();
        m_short_mode_labels.removeFirst();
      }
  }

  void SuperBox::drawItem(SuperBox::Item *pitem, qint32 x, qint32 y, qint32 w1, qint32 w2)
  {
    pitem->mp_lb_number->setVisible(w1 != 0);
    pitem->mp_cb->setVisible(true);
    pitem->mp_lb_number->setGeometry(x, y, w1, ROW_HEIGHT );
    pitem->mp_cb->setGeometry(x + w1, y, w2, ROW_HEIGHT );
  }

  void SuperBox::addItemWidget(int group,const QString &name, const QString &caption
                               ,const QString &cb_color,const QString &descr)
  {   
    Item *pitem = new Item(widget());
    pitem->mp_cb->setVisible(true);
    pitem->mp_lb_number->setVisible(true);    
    pitem->mp_lb_number->setText(QString("%1.%2").arg(group).arg(m_list.size()));
    pitem->mp_lb_number->setStyleSheet(LABEL_STYLESHEET);    
    pitem->mp_cb->setAccessibleName(name);
    pitem->mp_cb->setText(caption);
    pitem->mp_cb->setDescription(descr);    
    if(!cb_color.isEmpty())
    {
        pitem->mp_cb->setBackground(cb_color);
    }
    m_list.append(pitem);
  }

  SuperBox::CheckBoxForSuperBox::CheckBoxForSuperBox(QWidget *p): CheckBox(p)
  {
    m_highlighted = 0;
  }

  void SuperBox::CheckBoxForSuperBox::highlight(bool on)
  {
    if (on)
      {
        setStyleSheet(CHECKBOX_HIGHLIGHT);
      }
    else {
    updateStyleSheet();
    }
    m_highlighted = on;
  }

  void SuperBox::CheckBoxForSuperBox::setBackground(const QString &color)
  {
      m_background_color = color;
      updateStyleSheet();
  }


  void SuperBox::CheckBoxForSuperBox::mousePressEvent(QMouseEvent *pevent)
  {
    CheckBox::mousePressEvent(pevent);
    if (pevent->button() == Qt::RightButton)
      {
        highlight(!m_highlighted);
      }
  }

  void SuperBox::CheckBoxForSuperBox::updateStyleSheet()
  {
      setStyleSheet( QString("background-color :%1;").arg(m_background_color));
  }

  SuperBox::Item::Item(QWidget *p)
  {
    mp_lb_number =  new QLabel(p);
    QFont font = mp_lb_number->font();

    font.setPointSize(7);
    mp_lb_number->setFont(font);
    mp_cb =  new CheckBoxForSuperBox(p);
  }

  SuperBox::Item::~Item()
  {
    delete mp_lb_number;
    delete mp_cb;
  }



}
