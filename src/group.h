#ifndef GROUP_H
#define GROUP_H

#include <QDebug>
// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>

#include "sample.h"

/**
 * @brief Klasse repräsentiert eine einzelne Bild-Gruppe
 */
class Group
{
public:
    /**
     * @brief Konstruktor
     * @param name der Gruppen-Name
     */
    Group(QString name);

    /**
    * @brief Destruktor
    */
    ~Group();

    /**
     * @brief Fügt ein Bild-Sample zur Gruppe hinzu
     * @param sample Das Sample
     */
    void addSamples(Sample* sample);

    /**
     * @brief Fügt die Bild-Samples Values(Durchschnittsfarbe z.B.) zur Gruppe hinzu
     * @param Das Sample
     */
    void addSampleValues(Sample* sample);

    /**
     * @brief Berechnet die Durchschnittsfarbe der Gruppe
     */
    void calculateAvgColors();

    /**
     * @brief Gibt eine Liste von Samples zurück
     * @return Liste von Samples
     */
    QList<Sample*> getSamples();

    /**
     * @brief Löscht alle Samples aus der Gruppe
     */
    void clearSamples();

    /**
     * @brief Gibt den Namen der Gruppe zurück
     * @return Der Gruppen-Name
     */
    QString getName();
    /**
     * @brief Gibt den durchschnittlichen Rot-Wert der Gruppe zurück
     * @return Der durchschnittliche Rot-Wert
     */
    double getAvgRed();
    /**
     * @brief Gibt den durchschnittlichen Grün-Wert der Gruppe zurück
     * @return Der durchschnittliche Grün-Wert
     */
    double getAvgGreen();
    /**
     * @brief Gibt den durchschnittlichen Blau-Wert der Gruppe zurück
     * @return Der durchschnittliche Blau-Wert
     */
    double getAvgBlue();

    /**
     * @brief Gibt die CorrectionRate zurück
     * @return Die CorrectionRate
     */
    double getCorrectionRate();

    /**
     * @brief Gibt das Histogramm von Features zurück
     * @return Das FeatureHistogramm
     */
    cv::Mat getFeatureHistogram();

    /**
     * @brief Setzt den Gruppen-Classifier
     * @param Der Classifier
     */
    void setClassifier(CvSVM *classifier);

    /**
     * @brief Gibt den Gruppen-Classifier zurück
     * @return Der Classifier
     */
    CvSVM* getClassifier();
private:

    /**
     * @brief Der Gruppenname
     */
    QString name;
    /**
     * @brief Liste von Bild-Samples
     */
    QList<Sample *> samples;
    /**
     * @brief featureHistogram
     */
    cv::Mat featureHistogram;
    /**
     * @brief classifier
     */
    CvSVM *classifier;

    /**
     * @brief avgRed Durchschnittlicher Rot-wert der Gruppe
     */
    double avgRed;
    /**
     * @brief avgGreen Durchschnittlicher Grün-wert der Gruppe
     */
    double avgGreen;
    /**
     * @brief avgBlue Durchschnittlicher Blau-wert der Gruppe
     */
    double avgBlue;

    /**
     * @brief sumRed  Summe der Rot-werte aller Samples
     */
    int sumRed;
    /**
     * @brief sumGreen Summe der Grün-werte aller Samples
     */
    int sumGreen;
    /**
     * @brief sumBlue Summe der Blau-werte aller Samples
     */
    int sumBlue;

    /**
     * @brief normalizer zum ermitteln der Durschnittsfarbe eg. anzahl der in der Gruppe vorhanden Samples
     */
    int normalizer;

};

#endif // GROUP_H
