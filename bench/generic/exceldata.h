/**
 * @file:exceldata.h   -
 * @description: This is Excel communication class.
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-51-10
 *
 */


#ifndef EXCELDATA_H
#define EXCELDATA_H

#include <QObject>
#include <QAxObject>


class ExcelData : public QObject
{
    Q_OBJECT
public:
    explicit ExcelData(const QString filename);
    ~ExcelData();
    /* opens excel document*/
    bool open(char mode);
    /* returns if document is opened*/
    bool isOpen() const;
    /* add sheet with name to opened workbook*/
    bool addSheet(const QString sheetname);
    /* sets current sheet name*/
    bool setCurrentSheet(const QString sheetname);
    int sheetsCount();
    QStringList sheets();
    /* removes sheet sheetname*/
    bool removeSheet(const QString sheetname);
    /* write data to cell*/
    bool write(qint32 i, qint32 j, const QVariant data);
    /* reads data from cell*/
    bool read(qint32 row, qint32 col, QVariant &data);
    /* */
    QVariantList readRange(const QString &c1,const QString &c2);
    /* sets range */
    void setRange(const QString &c1,const QString &c2, const QVariantList data);
    /* sets color of cell*/
    bool setColor(qint32 i, qint32 j, const QColor background, const QColor foreground);
    /* gets color of cell */
    bool color(qint32 i, qint32 j, QColor &background, QColor &foreground);
    /* sets visible workbook*/
    void setVisible(bool visible);
    /* closes workbook*/
    void close(void);
    /* saves workbook to file*/
    bool save();

private:
    QAxObject *mp_excel;
    QAxObject *mp_workbook;
    QAxObject *mp_sheet;
    QString m_filename;
    QString m_sheetname;
    bool m_opened;

signals:

public slots:

};



#endif // EXCELDATA_H
