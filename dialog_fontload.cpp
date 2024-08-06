#include "dialog_fontload.h"
#include "ui_dialog_fontload.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <QFontMetrics>
#include <QPaintDevice>
#include <QRgb>
#include <QPoint>
#include <QString>
#include <QTextCodec>

QString current_family ;
QString current_style ;
QFont current_font ;

char cBuffer[256] ;


unsigned char iDecodeRusTable[256] = {
    0x0,  0x1,  0x2,  0x3,  0x4,  0x5,  0x6,  0x7,
    0x8,  0x9,  0xA,  0xB,  0xC,  0xD,  0xE,  0xF,
    0x10,  0x11,  0x12,  0x13,  0x14,  0x15,  0x16,  0x17,
    0x18,  0x19,  0x1A,  0x1B,  0x1C,  0x1D,  0x1E,  0x1F,
    0x20,  0x21,  0xDD,  0x23,  0x24,  0x25,  0x3F,  0xFD,
    0x28,  0x29,  0x2A,  0x2B,  0xE1,  0x2D,  0xFE,  0x2E,
    0x30,  0x31,  0x32,  0x33,  0x34,  0x35,  0x36,  0x37,
    0x38,  0x39,  0xC6,  0xE6,  0xC1,  0x3D,  0xDE,  0x2C,
    0x22,  0xD4,  0xC8,  0xD1,  0xC2,  0xD3,  0xC0,  0xCF,
    0xD0,  0xD8,  0xCE,  0xCB,  0xC4,  0xDC,  0xD2,  0xD9,
    0xC7,  0xC9,  0xCA,  0xDB,  0xC5,  0xC3,  0xCC,  0xD6,
    0xD7,  0xCD,  0xDF,  0xF5,  0x5C,  0xFA,  0x3A,  0x5F,
    0xB8,  0xF4,  0xE8,  0xF1,  0xE2,  0xF3,  0xE0,  0xEF,
    0xF0,  0xF8,  0xEE,  0xEB,  0xE4,  0xFC,  0xF2,  0xF9,
    0xE7,  0xE9,  0xEA,  0xFB,  0xE5,  0xE3,  0xEC,  0xF6,
    0xF7,  0xED,  0xFF,  0xD5,  0x7C,  0xDA,  0xA8,  0x0,
  } ;




void Dialog_FontLoad::SaveState()
{
    FILE *fp ;

    fp = fopen("fontload.cfg","wb") ;

    if(fp)
    {
        ProgramCfg.iFasmily = ui->comboBox_FontFamily->currentIndex() ;
        ProgramCfg.iStyle = ui->comboBox_Style->currentIndex() ;
        ProgramCfg.iFontOrientation =
                ui->comboBox_StringDataOrientation->currentIndex() ;
        ProgramCfg.iDigitLenght = ui->comboBox_DigitLenght->currentIndex() ;
        ProgramCfg.iSeparateSymbols = ui->checkBox_Separate->isChecked() ;

        ProgramCfg.iTextSize = ui->spinBox_Size->value() ;

        fwrite(&ProgramCfg,sizeof(ProgramCfg),1,fp) ;
        fclose(fp) ;
    }
}

void Dialog_FontLoad::LoadState()
{
    FILE *fp ;

    fp = fopen("fontload.cfg","rb") ;

    if(fp)
    {
        fread(&ProgramCfg,sizeof(ProgramCfg),1,fp) ;
        fclose(fp) ;

        ui->comboBox_FontFamily->setCurrentIndex( ProgramCfg.iFasmily ) ;
        ui->comboBox_Style->setCurrentIndex( ProgramCfg.iStyle ) ;
        ui->comboBox_StringDataOrientation->setCurrentIndex(ProgramCfg.iFontOrientation) ;
        ui->comboBox_DigitLenght->setCurrentIndex(ProgramCfg.iDigitLenght) ;
        ui->checkBox_Separate->setCheckState((Qt::CheckState)(ProgramCfg.iSeparateSymbols)) ;

        ui->spinBox_Size->setValue(ProgramCfg.iTextSize) ;

    }
}


Dialog_FontLoad::Dialog_FontLoad(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_FontLoad)
{
    ui->setupUi(this);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));


    // Generate DecodeTable
    /*strcpy((char*)(&iDecodeRusTable[32]),QString(" !Э#$%?э()*+б-ю.0123456789ЖжБ=Ю,@ФИСВУАПРШОЛДЬТЩЗЙКЫЕГМЦЧНЯх\\ъ:_ёфисвуапршолдьтщзйкыегмцчняХ|ЪЁ").toLocal8Bit()) ;

    int i ;
    for(i=0;i<32;i++) iDecodeRusTable[i] = i ;
    char *cPtr = (char*)iDecodeRusTable ;

    i = 50 ;
    while((i<120)&&(*cPtr!='@'))
    {
        cPtr ++ ;
        i++ ;
    }

    *cPtr = '"' ;*/


    ui->comboBox_FontFamily->addItems(base.families()) ;
    //ui->comboBox_FontFamily->setCurrentText(/*"Fixed [Misc]"*/"DejaVu Serif");

    ui->comboBox_FontFamily->setCurrentText("Nimbus Mono L");

    ui->comboBox_Style->setCurrentText("Bold Oblique");

    ui->spinBox_Size->setValue(15);


    current_family = ui->comboBox_FontFamily->currentText() ;
    current_style = ui->comboBox_Style->currentText() ;

    current_font = base.font(current_family, current_style, ui->spinBox_Size->value());



    ui->label_Metrix->setText(QString().sprintf("Height %2d,   MaxWidth %2d", iHeight, iMaxWidth));

    ui->comboBox_StringDataOrientation->addItem("X-direction");
    ui->comboBox_StringDataOrientation->addItem("Y-direction");

    ui->comboBox_DigitLenght->addItem("8-bit");
    ui->comboBox_DigitLenght->addItem("16-bit");
    ui->comboBox_DigitLenght->addItem("32-bit");
    ui->comboBox_DigitLenght->setCurrentText("16-bit");

    LoadState() ;

    image = QImage(32,32,QImage::Format_Mono) ;
    image_test = QImage(640,128,QImage::Format_Mono) ;

    ui->label_Display->setPixmap(QPixmap::fromImage(image_test));
    ui->label_Test->setPixmap(QPixmap::fromImage(image));

    FontView();
}


Dialog_FontLoad::~Dialog_FontLoad()
{
    SaveState() ;

    delete ui;
}



void Dialog_FontLoad::FontView()
{
    ui->textBrowser->clear();
    ui->textBrowser->setFont(current_font);

    ui->textBrowser->append("Здесь напечатан пример текста.") ;
    ui->textBrowser->append("Специально для преобразование в с-заголовок, для программ микроконтроллеров.");
    ui->textBrowser->append("Hello World!");
    ui->textBrowser->append("There is an text example for current font.");

    iHeight = QFontMetrics(current_font).height() ;
    iMaxWidth = QFontMetrics(current_font).maxWidth() ;
    ui->label_Metrix->setText(QString().sprintf("Height %2d,   MaxWidth %2d", iHeight, iMaxWidth));

    ui->spinBox_Height->setValue(iHeight) ;
    ui->spinBox_Width->setValue(iMaxWidth);
    //DetectSymbolRect() ;

    SetFontName();
}

void Dialog_FontLoad::SetFontName()
{
    char cOrient[2] = { 'x','y' } ;
    int iOrient = ui->comboBox_StringDataOrientation->currentIndex() ;

    if( iOrient == -1 ) iOrient = 0;

    char cBuffer2[256] ;
    strcpy(cBuffer2,ui->comboBox_Style->currentText().toLocal8Bit()) ;

    strncpy(cBuffer, current_family.toLocal8Bit(),4) ;
    sprintf(FontName,"font_%s_%s_%d_%d_h%c",cBuffer, cBuffer2,
            ui->spinBox_Width->value(),
            ui->spinBox_Height->value(),
            cOrient[iOrient] ) ;

    unsigned int i ;
    for(i=0;i<strlen(FontName);i++)
        if(FontName[i] == ' ')
                    FontName[i] = '_' ;

    strcpy(FileName, FontName) ;
    strcat(FileName,".h") ;
    ui->label_FileName->setText(FileName) ;
}


void Dialog_FontLoad::on_comboBox_FontFamily_currentIndexChanged(const QString &arg1)
{
    current_family = arg1 ;
    //setFont(base.font(arg1);
    ui->comboBox_Style->clear();
    ui->comboBox_Style->addItems(base.styles(arg1)) ;
    ui->comboBox_Style->setCurrentText("Regular");
    //ui->comboBox_Style->setCurrentIndex(0);
    QString current_style = ui->comboBox_Style->currentText() ;
    //ui->comboBox_TextSize->addItems( base.pointSizes(arg1))) ;
    current_font = base.font(current_family, current_style, ui->spinBox_Size->value());

    FontView();
}


void Dialog_FontLoad::on_comboBox_Style_currentIndexChanged(const QString &arg1)
{
    current_style = ui->comboBox_Style->currentText() ;
    current_font = base.font(current_family, current_style, ui->spinBox_Size->value());

    FontView();
}


void Dialog_FontLoad::on_spinBox_Size_valueChanged(int arg1)
{
    ui->textBrowser->setFont(base.font(current_family, current_style, ui->spinBox_Size->value()));
    current_font = base.font(current_family, current_style, ui->spinBox_Size->value());

    FontView();
}




void Dialog_FontLoad::DetectSymbolRect()
{
    unsigned int iSymb ;
    int ix,iy ;
    char string[16] ;

    int iminx = 32, iminy = 32, imaxx = 0, imaxy = 0 ;

    QPainter painter ;

    painter.begin(&image) ;


    painter.setFont(current_font);

    for(iSymb = 33; iSymb<255; iSymb++)
    {
        string[0] = iSymb ;
        string[1] = 0x0 ;

        painter.setPen(QColor(0,0,0));

        for(iy=0;iy<32;iy++)
            for(ix=0;ix<32;ix++)
        {
            painter.drawPoint(ix,iy);
        }

        painter.setPen(QColor(0,0,255));

        painter.drawLine(0,ui->spinBox_Height->value(),ui->spinBox_Width->value(),ui->spinBox_Height->value()) ;
        painter.drawLine(ui->spinBox_Width->value(),ui->spinBox_Height->value(),ui->spinBox_Width->value(),0) ;

        painter.setPen(QColor(255,255,255));

        QString str_sym;

        str_sym = QString().fromLocal8Bit(string) ;

         //QTextCodec::codecForName("Windows-1251")
        painter.drawText(0,ui->spinBox_Height->value()-1,str_sym) ;


        for(iy=0;iy<32;iy++)
           for(ix=0;ix<32;ix++)
        {
            QColor color ;
            QColor col2 = painter.pen().color();

            color = image.pixel(QPoint(ix,iy));

            int iVal = (color.red()+color.blue()+color.green())/3 ;
            if(/*color!=col2*/iVal>100)
            {
                //DATA[i][iy] |= 1<<(31-ix) ;
                if(ix<iminx) iminx = ix ;
                if(iy<iminy) iminy = iy ;
                if(ix>imaxx) imaxx = ix ;
                if(iy>imaxy) imaxy = iy ;
            }
        }
    }

    iSymbolRect_x1 = iminx ;
    iSymbolRect_x2 = imaxx;
    iSymbolRect_y1 = iminy ;
    iSymbolRect_y2 = imaxy;

    painter.end() ;

    ui->spinBox_Height->setValue(iSymbolRect_y2-iSymbolRect_y1+1);
    ui->spinBox_Width->setValue(iSymbolRect_x2-iSymbolRect_x1+1);
}


void Dialog_FontLoad::MakeSymbol(unsigned int i)
{
    int ix,iy ;
    char string[16] ;
    unsigned int ii = i ;

    int iminx = 32, iminy = 32, imaxx = 0, imaxy = 0 ;

    //if(ii>=128) ii -= 128 ;

    //if(i>=128)
    //    ii = iDecodeRusTable[i-128] ;


    string[0] = ii ;
    string[1] = 0x0 ;

    QPainter painter ;

    painter.begin(&image) ;


    painter.setFont(current_font);

    painter.setPen(QColor(0,0,0));

    for(iy=0;iy<32;iy++)
        for(ix=0;ix<32;ix++)
    {
        painter.drawPoint(ix,iy);
    }

    painter.setPen(QColor(0,0,255));

    //painter.drawLine(0,ui->spinBox_Height->value(),ui->spinBox_Width->value(),ui->spinBox_Height->value()) ;
    //painter.drawLine(ui->spinBox_Width->value(),ui->spinBox_Height->value(),ui->spinBox_Width->value(),0) ;

    painter.setPen(QColor(255,255,255));

    QString str_sym;

    str_sym = QString().fromLocal8Bit(string) ;
     //QTextCodec::codecForName("Windows-1251")
    painter.drawText(0,ui->spinBox_Height->value()-1,str_sym) ;


    for(iy=0;iy<32;iy++) DATA[i][iy] = 0x0 ;


    painter.setPen(QColor(0,0,0));

    int ih,iw, ixIndex, iyIndex ;


    ih = ui->spinBox_Height->value() ;
    iw = ui->spinBox_Width->value() ;

    int iWidth = iSymbolRect_x2 - iSymbolRect_x1 ;
    int iHeight = iSymbolRect_y2 - iSymbolRect_y1 ;

    double dYstep = (1.0*iHeight)/ih ;
    double dyindex = iSymbolRect_y1 ;
    double dXstep = (1.0*iWidth)/iw ;
    double dXindex = iSymbolRect_x1 ;

    for(iy=0;iy<ih;iy++)
    {
      iyIndex = iy + iSymbolRect_y1 ;//dyindex + 0.1 ;

      dXindex = 0.0 ;

      for(ix=0;ix<iw;ix++)
      {
        QColor color ;
        QColor col2 = painter.pen().color();


        ixIndex = iSymbolRect_x1+ix ; //dXindex + 0.1 ;

        if(ui->comboBox_StringDataOrientation->currentIndex()==0)
        {
            color = image.pixel(QPoint(/*ix*/ixIndex,iyIndex));

            int iVal = (color.red()+color.blue()+color.green())/3 ;
            if(/*color!=col2*/iVal>20)
            {
                DATA[i][iy] |= 1<<(31-ix) ;
                if(ix<iminx) iminx = ix ;
                if(iy<iminy) iminy = iy ;
                if(ix>imaxx) imaxx = ix ;
                if(iy>imaxy) imaxy = iy ;
            }
        }
        else
        {
            color = image.pixel(QPoint(/*iw-1-ix*/iSymbolRect_x2-ixIndex,/*ih-iy*/iSymbolRect_y2 - iy));

            int iVal = (color.red()+color.blue()+color.green())/3 ;
            if(/*color!=col2*/iVal>100)
            {
                DATA[i][iw-1-ix] |= 1<<(31-iy) ;
                if(ix<iminx) iminx = ix ;
                if(iy<iminy) iminy = iy ;
                if(ix>imaxx) imaxx = ix ;
                if(iy>imaxy) imaxy = iy ;
            }
        }
        dXindex += dXstep ;
      }  //for( x )

      dyindex += dYstep ;

    }  //for( y )

    painter.end() ;

    DATA_Width[i] = imaxx - iSymbolRect_x1 /*- iminx*/ + 1 ;

    string_append.clear() ;

    if(ui->comboBox_StringDataOrientation->currentIndex()!=0)
    {
        iw = ui->spinBox_Height->value() ;
        ih = ui->spinBox_Width->value() ;
    }

    if(ui->checkBox_Separate->checkState()!=false)
    {
        string_append += "{" ;
    }

    for(iy=0;iy<ih;iy++)
    {
            int iBitsWidth = ui->comboBox_DigitLenght->currentIndex() ;
            int iWordsNumber ;
            int iBitsNumber = ui->spinBox_Width->value() ;
            char string2[64] ;
            int j;


            switch(iBitsWidth)
            {
            case 0: iWordsNumber =  iBitsNumber/8 ;
                if( (iBitsNumber - (iWordsNumber*8)) >0 ) iWordsNumber ++ ;
                sprintf(string2, " 0x%2X,",DATA[i][iy]>>24);
                for(j=2;j<strlen(string2);j++) if(string2[j]==' ') string2[j] = '0';
                string_append += string2 ;

                break ;
            case 1: iWordsNumber =  iBitsNumber/8 ;
                if( (iBitsNumber - (iWordsNumber*8)) >0 ) iWordsNumber ++ ;
                sprintf(string2, " 0x%4X,",DATA[i][iy]>>16);
                for(j=2;j<strlen(string2);j++) if(string2[j]==' ') string2[j] = '0';
                string_append += string2 ;

                break ;
            default: iWordsNumber = 1 ;

                sprintf(string2, " 0x%8X,",DATA[i][iy]);
                for(j=2;j<strlen(string2);j++) if(string2[j]==' ') string2[j] = '0';


                string_append += string2 ;
            }

    }

    if(string[0] == '\\' ) strcpy(string,"slash");

    if(ui->checkBox_Separate->checkState()==false)
        string_append += QString().sprintf("   //  %s",string);
    else
        string_append += QString().sprintf("},   //  %s",string);



}


char cFontHeader[3][64]={ "uint8_t","uint16_t","uint32_t"};

void Dialog_FontLoad::on_pushButton_Test_clicked()
{
    int i ;

    DetectSymbolRect();

    usleep(5000) ;

    DetectSymbolRect() ;

    SetFontName();

    ui->textBrowser_FileView->clear();

    int isize ;
    if(ui->comboBox_StringDataOrientation->currentIndex()==0) isize = ui->spinBox_Height->value() ;
    else isize = ui->spinBox_Width->value() ;

    int ibits = ui->comboBox_DigitLenght->currentIndex() ;

    ui->textBrowser_FileView->append(QString().sprintf("%s %s[256][%d] = {",cFontHeader[ibits], FontName,isize));

    for(i=33;i<34;i++)
    {
        MakeSymbol(i);

        ui->label_Test->setPixmap(QPixmap::fromImage(image));
        ui->textBrowser_FileView->append(string_append);

    }

    ui->textBrowser_FileView->append("}");


    DisplayAllSymbols();

}


void Dialog_FontLoad::on_toolButton_triggered(QAction *arg1) //Save
{
}


int DetectStringFragment(char *string, char * fragment)
{
    int ilen_s = strlen(string) ;
    int ilen_f = strlen(fragment) ;
    int i, iret = 0 ;
    char *string_ptr = string ;

    i = 0 ;
    do{
        iret = strncmp(string_ptr,fragment, ilen_f) ;
        string_ptr ++ ;
        i++ ;
    } while((iret!=0)&&((i+ilen_f)<=ilen_s)) ;

    if(iret == 0) return 1 ;
    else
        return 0 ;
}

void Dialog_FontLoad::on_toolButton_clicked() //Save
{
    int i ;

    FILE *fp ;

    SetFontName();

    fp = fopen(FileName,"wt") ;

    ui->textBrowser_FileView->clear();

    int isize ;
    if(ui->comboBox_StringDataOrientation->currentIndex()==0) isize = ui->spinBox_Height->value() ;
    else isize = ui->spinBox_Width->value() ;

    int ibits = ui->comboBox_DigitLenght->currentIndex() ;

    string_append.clear() ;

    int iStyle = 0 ;
    char cStyle[256] ;
    strcpy(cStyle, ui->comboBox_Style->currentText().toLocal8Bit()) ;

    if(DetectStringFragment(cStyle,"Oblique")) iStyle |= 1 ;
    if(DetectStringFragment(cStyle,"Italic")) iStyle |= 1 ;
    if(DetectStringFragment(cStyle,"Bold")) iStyle |= 2 ;

    fputs("\n/* Register font commands example :\n", fp) ;
    fprintf(fp,"\n#include %cfonts/%s%c\n\n", '"', FileName, '"') ;

    fprintf(fp,"RegisterNewFont(%d, %d, %d, %d, ",
            ui->spinBox_Height->value(),
            ui->spinBox_Width->value(),
            iStyle,  // Italic and Bold mode flags
            ui->comboBox_StringDataOrientation->currentIndex()) ;

    fprintf(fp,"%c%s%c,", '"', current_family.toLocal8Bit().data(), '"');
    fprintf(fp,"(%s*)(%s)) ;", cFontHeader[ibits], FontName ) ;
    fputs("\n*/\n\n", fp) ;

    string_append.clear() ;
    string_append += QString().sprintf("const %s %s[256][%d] = {",
                                       cFontHeader[ibits], FontName,isize);

    ui->textBrowser_FileView->append(string_append) ;
    fputs(string_append.toLocal8Bit(), fp) ;
    fputs("\n", fp) ;

    for(i=0;i<256;i++)
    {

        MakeSymbol(i);
        fputs(string_append.toLocal8Bit(), fp) ;
        fputs("\n", fp) ;
        ui->textBrowser_FileView->append(string_append) ;

    }


    fputs("\n};\n", fp) ;

    int k, l ;

    fputs("\n/*  Russian decoding table  */ \n", fp) ;
    fputs("\n/*unsigned char iDecodeRusTable[256] = {\n", fp ) ;
    l = 0 ;
    for(k=0;k<128;k++)
    {
      fprintf(fp, "  0x%X,", ((unsigned char *)( iDecodeRusTable))[k] ) ;
      l++ ;
      if(l>=8)
      {
          l = 0;
          fputs("\n",fp);
      }
    }
    fputs("};*/\n", fp) ;

    strcpy(cTestString, QString("Охрененность").toLocal8Bit()) ;
    fputs("\n/*unsigned char TestString[256] = {\n", fp) ;
    for(k=0;k<strlen(cTestString);k++)
    {
      fprintf(fp, "0x%X,", ((unsigned char *)( cTestString))[k] ) ;
    }
    fputs("0x0\n};*/\n\n", fp) ;

    string_append.clear() ;
    string_append += QString().sprintf("const uint8_t %s_Width[256] = {",
                                       FontName,isize);

    ui->textBrowser_FileView->append(string_append) ;
    fputs(string_append.toLocal8Bit(), fp) ;
    fputs("\n", fp) ;

    unsigned int iSpaceWidth = DATA_Width[0x0ff&(unsigned int)'X'] ;
    l = 0 ;
    for(i=0;i<256;i++)
    {
        if(DATA_Width[i] == 0 )
            DATA_Width[i] = iSpaceWidth ;

        if(DATA_Width[i] < iSpaceWidth)
            DATA_Width[i] = iSpaceWidth ;

        //if(DATA_Width[i] > iSpaceWidth)
        //    fprintf(fp, " /*%c*/",  (unsigned char)(i) ) ;

        l++ ;
        if(l>=16)
        {
            l = 0;
            fputs("\n",fp);
        }
        fprintf(fp, " %d,",  DATA_Width[i] ) ;
    }

    fputs("\n};\n", fp) ;

    fclose(fp) ;

    //ui->textBrowser_FileView->append("...   ...");
    ui->textBrowser_FileView->append("}");
    ui->textBrowser_FileView->append("SAVED");

}

void Dialog_FontLoad::DisplaySymbol(int x,int y, int i)
{
    int ix, iy;
    int ih,iw ;

    ih = ui->spinBox_Height->value() ;
    iw = ui->spinBox_Width->value() ;

    for(iy=0;iy<ih;iy++)
        for(ix=0;ix<iw;ix++)
    {

        if(ui->comboBox_StringDataOrientation->currentIndex()==0)
        {

            if( (DATA[i][iy] & (1<<(31-ix)))!=0) painter.drawPoint(ix+x,iy+y) ;
        }
        else
        {

            if( (DATA[i][iw-1-ix] & (1<<(31-iy)))!=0) painter.drawPoint(x+iw-1-ix,y+ih-1-iy) ;
        }
    }

}


void Dialog_FontLoad::PutString(int x, int y, char *str)
{

    while(*str!=0)
    {
        unsigned int i = *(unsigned char *)str ;

        DisplaySymbol(x,y, i ) ;

        x += DATA_Width[i] +1 ;

        str++ ;
    }
}

void Dialog_FontLoad::PutRusString(int x, int y, char *str)
{

    while(*str!=0)
    {
        unsigned int i = *(unsigned char *)str ;

        if(i<128) i = iDecodeRusTable[i] & 0xff ;

        DisplaySymbol(x,y, i ) ;

        x += DATA_Width[i] +1 ;

        str++ ;
    }
}


void Dialog_FontLoad::DisplayAllSymbols()
{
    int i, x,y ;
    int ih = ui->spinBox_Height->value() ;
    int iw = ui->spinBox_Width->value() ;

    for(i=0;i<256;i++)
    {
        MakeSymbol(i);
    }

    painter.begin(&image_test) ;
    painter.setFont(current_font);


    painter.setBackground(QBrush(QColor(0,0,0)));

    painter.setBrush(QColor(0,0,0));
    painter.fillRect(0,0,image_test.width(),image_test.height(),QColor(0,0,0)) ;
    //for(x=0;x<painter.window())

    painter.setPen(QColor(255,255,255));

    x=0 ;
    y=0 ;

    for(i=0;i<256;i++)
    {
        DisplaySymbol(x,y,i) ;
        x += /*iw*/ DATA_Width[i] ;

        if(x>450)
        {
            x=0;
            y += ih ;
        }
    }

    y += ih ;
    strcpy(cTestString,"!& #$%&'()*+,-./001234 J[htytyyjcnm") ;

    PutRusString(0, y, cTestString) ;

    painter.end() ;
    ui->label_Display->setPixmap(QPixmap::fromImage(image_test));
}
