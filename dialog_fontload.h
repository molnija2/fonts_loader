#ifndef DIALOG_FONTLOAD_H
#define DIALOG_FONTLOAD_H

#include <QDialog>
#include <QFontDatabase>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QPicture>


QT_BEGIN_NAMESPACE
namespace Ui { class Dialog_FontLoad; }
QT_END_NAMESPACE



typedef struct{
    int iFasmily ;
    int iStyle ;
    int iDigitLenght ;
    int iFontOrientation ;
    int iTextSize ;
    int iSeparateSymbols ;

} FONTLOAD_CFG ;





class Dialog_FontLoad : public QDialog
{
    Q_OBJECT

public:
    Dialog_FontLoad(QWidget *parent = nullptr);
    ~Dialog_FontLoad();

    QFontDatabase base ;
    int iHeight ;
    int iMaxWidth ;

    int iSymbolRect_x1,iSymbolRect_x2, iSymbolRect_y1, iSymbolRect_y2 ;

    unsigned int DATA[256][64] ; /*  32x32 bits  */
    unsigned int DATA_Width[256] ;
    QImage image ;
    QImage image_test ;
    QPicture picture ;
    QPixmap pixmap ;
    char FileName[256] ;
    char FontName[256] ;
    QPainter painter ;

    char cTestString[256] ;
    FONTLOAD_CFG ProgramCfg ;

    QString string_append ;

    void SaveState() ;
    void LoadState() ;
    void FontView() ;
    void MakeSymbol(unsigned int i) ;
    void SetFontName() ;
    void DisplaySymbol(int x,int y, int ch) ;
    void DisplayAllSymbols() ;
    void DetectSymbolRect() ;
    void PutString(int x, int y, char *str) ;
    void PutRusString(int x, int y, char *str) ;

private slots:
    void on_comboBox_FontFamily_currentIndexChanged(const QString &arg1);

    void on_comboBox_Style_currentIndexChanged(const QString &arg1);

    void on_spinBox_Size_valueChanged(int arg1);

    void on_pushButton_Test_clicked();

    void on_toolButton_triggered(QAction *arg1);

    void on_toolButton_clicked();

private:
    Ui::Dialog_FontLoad *ui;
};
#endif // DIALOG_FONTLOAD_H
