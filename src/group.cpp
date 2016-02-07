#include "group.h"

Group::Group(QString name)
{
    this->name = name;
    this->avgRed = 0;
    this->avgGreen = 0;
    this->avgBlue = 0;

    this->sumRed = 0;
    this->sumGreen = 0;
    this->sumBlue = 0;

    this->normalizer = 0;
}

Group::~Group()
{
    this->getSamples().clear();
}

QString Group::getName()
{
    return this->name;
}

double Group::getAvgRed()
{
    return this->avgRed;
}

double Group::getAvgGreen()
{
    return this->avgGreen;
}

double Group::getAvgBlue()
{
    return this->avgGreen;
}

QList<Sample*> Group::getSamples()
{
    return this->samples;
}

void Group::addSamples(Sample* sample)
{
    this->samples.append(sample);
}

void Group::addSampleValues(Sample *sample)
{
    //qDebug() << "GRed: " << sample->getAvgRed() << "GGreen: " << sample->getAvgGreen() << "GBlue: " << sample->getAvgBlue();

    this->sumRed = this->sumRed + sample->getAvgRed();
    this->sumGreen =this->sumGreen+ sample->getAvgGreen();
    this->sumBlue =this->sumBlue+ sample->getAvgBlue();

    this->normalizer++;

}
void Group::clearSamples()
{
    this->samples.clear();
}

void Group::calculateAvgColors()
{
    this->avgRed = this->sumRed/(double)normalizer;
    this->avgGreen = this->sumGreen/(double)normalizer;
    this->avgBlue = this->sumBlue/(double)normalizer;

    //qDebug() << "GRed: " << avgRed << "GGreen: " << avgGreen << "GBlue: " << avgBlue;

}



double Group::getCorrectionRate()
{
    int correctSamples = 0;
    for(int i= 0 ; i < this->samples.size();++i)
    {
        if(this->samples.at(i)->getTrueGroupName() == this->name)
        {
            //qDebug() << this->samples.at(i)->getTrueGroupName();
            correctSamples++;
        }
        else
        {
            //qDebug() << "Wrong Sample";
        }
    }
    return correctSamples / ( (double) this->samples.size());
}

cv::Mat Group::getFeatureHistogram()
{
    return this->featureHistogram;
}

void Group::setClassifier(CvSVM *classifier)
{
    this->classifier = classifier;
}

CvSVM* Group::getClassifier()
{
    return this->classifier;
}
