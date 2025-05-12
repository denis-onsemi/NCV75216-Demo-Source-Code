/**
 * @file:exceldata.cpp   -
 * @description: This is Excel communication class.
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-51-10
 *
 */


#include "exceldata.h"
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QRgb>



/****************************************************************************
 * @function name: Constructor
 *
 * @param:
 *
 *          const QString filename  - file name *.xlsx
 * @description:
 ****************************************************************************/
ExcelData::ExcelData(const QString filename) :
    QObject(0)
{
    m_opened = false;
    mp_excel =  0;
    mp_workbook = 0;
    mp_sheet = 0;
    m_filename = filename;
    mp_excel = new QAxObject( "Excel.Application", 0 );
    if (mp_excel)
    {
        mp_excel->setProperty("DisplayAlerts", false);
    }
    else
    {

    }
}

ExcelData::~ExcelData()
{

}


/****************************************************************************
 * @function name: ExcelData::open()
 *
 * @param:
 *
 *       char mode  - file mode 'r' -read 'w'- write
 * @description: opens excel file for read or write
 * @return: ( bool )  success = true
 ****************************************************************************/
bool ExcelData::open(char mode)
{
    bool result = false;
    do
    {
        /* check if opened and file name is empty*/
        if (mp_excel == NULL || m_opened || m_filename.isEmpty()) break;

        /* try open workbooks*/
        QAxObject *workbooks = mp_excel->querySubObject( "Workbooks" );
        if (workbooks == NULL) break;

        bool file_exists = QFile::exists(m_filename);
        if (file_exists)
        {
            mode = 'r';
        }
        if (mode == 'r' )
        {
            mp_workbook = workbooks->querySubObject( "Open(const QString&)", m_filename );
        }
        else if (mode == 'w' )
        {
            mp_workbook = workbooks->querySubObject("Add()");
            if (mp_workbook == NULL) break;

            mp_workbook->dynamicCall("SaveAs(const QString &)", m_filename);
            mp_workbook->dynamicCall("Close()");

            /* and try to open newly created file */
            mp_workbook = workbooks->querySubObject( "Open(const QString&)", m_filename );
        }
        result = mp_workbook != 0;
        m_opened = result;
    }
    while (0);
    return result;
}


/****************************************************************************
 * @function name: ExcelData::isOpen()
 *
 * @param:
 *             void
 * @description: Check if file is opened
 * @return: ( bool ) is opened = true
 ****************************************************************************/
bool ExcelData::isOpen() const
{
    return m_opened;
}


/****************************************************************************
 * @function name: ExcelData::addSheet()
 *
 * @param:
 *
 *       const QString sheetname  - sheet name
 * @description: adds sheet to workbook
 * @return: ( bool ) - success = true
 ****************************************************************************/
bool ExcelData::addSheet(const QString sheetname)
{
    if (m_opened)
    {
        QAxObject *sheets = mp_workbook->querySubObject( "Worksheets" );
        if ( sheets )
        {
            sheets->dynamicCall("Add()");
            mp_sheet = sheets->querySubObject( "Item( int )", 1 );
            if (mp_sheet)
            {
                mp_sheet->setProperty("Name", sheetname);
                return true;
            }
        }
    }
    return false;
}


/****************************************************************************
 * @function name: ExcelData::setCurrentSheet - ---
 *
 * @param:
 *
    const QString sheetname  - sheetn name
 * @description: Sets active sheet
 * @return: ( bool ) - success = true
 ****************************************************************************/
bool ExcelData::setCurrentSheet(const QString sheetname)
{
    if (m_opened)
    {
        QAxObject *sheets = mp_workbook->querySubObject("Sheets (const QString&)", sheetname);
        if ( sheets )
        {
            sheets->dynamicCall("Activate");
            QAxObject *sheet  = mp_workbook->querySubObject("ActiveSheet");
            if (sheet )
            {
                mp_sheet = sheet;
                return true;
            }
        }
    }
    return false;
}




/****************************************************************************
 * @function name: ExcelData::removeSheet()
 *
 * @param:
 *
    const QString sheetname
 * @description: removes sheet
 * @return: ( bool ) success = true
 ****************************************************************************/
bool ExcelData::removeSheet(const QString sheetname)
{
    if (m_opened)
    {
        QAxObject *sheets = mp_workbook->querySubObject("Sheets (const QString&)", sheetname);
        if ( sheets )
        {
            sheets->dynamicCall("Delete()");
            return true;
        }
    }
    return false;;
}


/****************************************************************************
 * @function name: ExcelData::save - ---
 *
 * @param:
 *
 *          void
 * @description:
 * @return: ( bool ) success = true
 ****************************************************************************/
bool ExcelData::save(void)
{
    if ( mp_workbook )
    {
        mp_workbook->dynamicCall( "Save()");
        return true;
    }
    return false;
}


/****************************************************************************
 * @function name: ExcelData::write - ---
 *
 * @param:
 *
 *    qint32 i
 *    qint32 j
 *    const QVariant data
 * @description: writes data to cell i,j
 * @return: ( bool ) success = true
 ****************************************************************************/
bool ExcelData::write(qint32 i, qint32 j, const QVariant data)
{
    if(!data.isNull()) {
    if (m_opened && i > 0 && j > 0 && mp_sheet)
    {
        QAxObject *cell = mp_sheet->querySubObject("Cells( int, int)", i, j);
        if ( cell )
        {
            cell->setProperty("Value", data);
            return true;
        }
    }
    }
    return false;
}


/****************************************************************************
 * @function name: ExcelData::read - ---
 *
 * @param:
 *
 *      qint32 i
 *      qint32 j
 *      QVariant &data
 * @description: rads data from cell i,j
 * @return: ( bool ) success = true
 ****************************************************************************/
bool ExcelData::read(qint32 row, qint32 col, QVariant &data)
{
    bool result = false;
    if (m_opened && row > 0 && col > 0 && mp_sheet)
    {
        QAxObject *cell = mp_sheet->querySubObject("Cells( int, int)", row, col);
        if ( cell )
        {
            data = cell->property("Value");
            result = true;
        }
    }
    return result;
}

QVariantList ExcelData::readRange(const QString &c1, const QString &c2)
{
    if (m_opened &&  mp_sheet)
    {
        QAxObject *pobj = mp_sheet->querySubObject("Range( QString ,QString )", c1,c2);
        if ( pobj )
        {
            return pobj->dynamicCall("Value()").toList();
        }
    }
    return QVariantList();
}

void ExcelData::setRange(const QString &c1, const QString &c2, const QVariantList data)
{
    if (m_opened &&  mp_sheet)
    {
        QAxObject *prange = mp_sheet->querySubObject("Range( QString ,QString )", c1,c2);
        if ( prange )
        {
            prange->dynamicCall("SetValue(const QVariant&)",data);
        }
    }
}


/****************************************************************************
 * @function name: ExcelData::setColor()
 * @param:
 *    qint32 i
 *    qint32 j
 *    const QColor background
 *    const QColor foreground
 * @description: Sets color of background ,foreground cell
 * @return: ( bool ) - success = true
 ****************************************************************************/
bool ExcelData::setColor(qint32 i, qint32 j, const QColor background, const QColor foreground)
{
    bool result = false;
    if (m_opened && i > 0 && j > 0 && mp_sheet)
    {
        QAxObject *cell = mp_sheet->querySubObject("Cells( int, int)", i, j);
        if ( cell )
        {
            QAxObject *interior = cell->querySubObject("Interior");
            QAxObject *font = cell->querySubObject("Font");
            if ( interior && font)
            {
                quint32 color;
                color = (background.red() << 0) | (background.green() << 8) | (background.blue() << 16);
                result = interior->setProperty("Color", QVariant(color));
                color = (foreground.red() << 0) | (foreground.green() << 8) | (foreground.blue() << 16);
                result &= font->setProperty("Color", QVariant(color));
                result = true;
            }
        }
    }
    return result;
}


/****************************************************************************
 * @function name: ExcelData::color()
 *
 * @param:
 *      qint32 i
 *      qint32 j
 *      QColor &background
 *      QColor &foreground
 * @description: gets color of background and foregorund
 * @return: ( bool ) success = true
 ****************************************************************************/
bool ExcelData::color(qint32 i, qint32 j, QColor &background, QColor &foreground)
{
    bool result = false;
    if (m_opened && i > 0 && j > 0 && mp_sheet)
    {
        QAxObject *cell = mp_excel->querySubObject("Cells( int, int)", i, j);
        if ( cell )
        {
            QAxObject *interior = cell->querySubObject("Interior");
            QAxObject *font = cell->querySubObject("Font");
            if ( interior && font)
            {
                QVariant color1 = interior->property("Color");
                background = QColor::fromRgb(color1.toInt());
                QVariant color2 = font->property("Color");
                foreground = QColor::fromRgba(color2.toInt());
                result = true;
            }
        }
    }
    return result;
}


/****************************************************************************
 * @function name: ExcelData::setVisible()
 *
 * @param:
 *
 *      bool visible
 * @description: sets visibility of workbook
 * @return: ( void )
 ****************************************************************************/
void ExcelData::setVisible(bool visible)
{
    if ( mp_excel)
    {
        mp_excel->setProperty("Visible", visible);
    }
}


/****************************************************************************
 * @function name: ExcelData::close()
 *
 * @param:
 *             void
 * @description: closes workbook
 * @return: ( void )
 ****************************************************************************/
void ExcelData::close()
{
    try
    {
        this->save();
        mp_workbook->dynamicCall( "Close()");
        delete mp_excel;
    }
    catch (...)
    {
        m_opened = false;
    }
    mp_sheet = NULL;
    mp_workbook = NULL;
    mp_excel =  NULL;
}


//bool ExcelDataLoader::loadFile(QStringList *plist, const QRect &rect)
//{
//    start(QThread::NormalPriority);
//    this->mp_list = plist;
//    this->m_rect = mp_rect;
//}


//ExcelDataLoader::ExcelDataThread(const QString &filename, const QString &sheet)
//        :QThread(0)
//{
//        m_filename = filename;
//        m_sheet = sheet;
//}

int ExcelData::sheetsCount()
{
     QAxObject *sheets = mp_workbook->querySubObject("Sheets");
     if(sheets)
        return sheets->dynamicCall("Count()").toInt();
     else return -1;
}

QStringList ExcelData::sheets()
{
    QStringList result;
    int sheets_count = sheetsCount();
    if(sheets_count>0)
    {
        for(int i=1;i<=sheets_count;i++)
        {
            QAxObject *sheet = mp_workbook->querySubObject("Sheets(int )",i);
            if ( sheet )
            {
                result += sheet->property("Name").toString();
            }
        }
    }
    return result;
}
