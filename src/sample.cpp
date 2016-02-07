#include "sample.h"

Sample::Sample()
{

}

Sample::Sample(QString path)
{
    this->path = path;
    this->fileName = QFileInfo(path).fileName();
    this->trueGroupname = fileName.split("_").at(0);
    calculateParameters();
}

void Sample::calculateParameters()
{
    // Erstellt ein neues Image-Objekt
    QImage myImage(path);
    // wenn das Image valide ist
    if(!myImage.isNull())
    {
        this->width = myImage.width();
        this->height = myImage.height();

        // Vektor mit den RGB werten
        QVector<QRgb> v;

        for(int w=0; w < this->width;++w)
            for(int h=0; h < this->height; ++h)
                v.append(myImage.pixel(w,h));

        //qDebug() << QColor(myImage->pixel(10,10)).red();
        long sumR = 0;
        long sumG = 0;
        long sumB = 0;

        // Summiert die RGB-Werte von allen Pixeln
        for (int j=0; j < v.size(); ++j )
        {
            QColor c = QColor::fromRgb(v.at(j));

            sumR = sumR + c.red();
            sumG = sumG + c.green();
            sumB = sumB + c.blue();
        }

        // teilt die Summe der RGB-Kanäle durch anzahl der Pixel
        this->avgRed = sumR /  (double)(this->width*this->height);
        this->avgGreen = sumG/ (double)(this->width*this->height);
        this->avgBlue = sumB/ (double)(this->width*this->height);
    }
}

double Sample::getAvgRed()
{
    return this->avgRed;
}

double Sample::getAvgGreen()
{
    return this->avgGreen;
}

double Sample::getAvgBlue()
{
    return this->avgBlue;
}

QString Sample::getFileName()
{
    return this->fileName;
}

QString Sample::getTrueGroupName()
{
    return this->trueGroupname;
}


QString Sample::displayAvgValues()
{
    QString s = QString("Avg-Red: %1 Avg-Green: %2 Avg-Blue: %3").arg(this->avgRed).arg(this->avgGreen).arg(this->avgBlue);
    return s;
}

QString Sample::getFilePath()
{
    return this->path;
}
