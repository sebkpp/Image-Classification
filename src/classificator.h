#ifndef CLASSIFICATION_H
#define CLASSIFICATION_H

#include <QTableWidget>
#include <QStringList>
#include <QtMath>
#include <opencv2/nonfree/features2d.hpp> // Feature detection/extration
#include <opencv2/core/core.hpp> // Mat usw.
#include <opencv2/highgui/highgui.hpp> // cv::imread etc.
#include "opencv2/nonfree/nonfree.hpp" // SIFT

#include "sample.h"
#include "group.h"
#include "groupcontroller.h"

/**
 * @brief Diese Klasse wird zur Klassifizierung der einzelnen Sample-Bilder verwendet und den Gruppen zugeordnet
 */
class Classificator
{
public:
    /**
     * @brief Konstruktor
     * @param groupController Der GroupController
     */
    Classificator(GroupController *groupController);

    /**
     * @brief Klassifiziert mehrere Dateien anhand der übergebenen Method
     * @param filesToClassifier Liste von Bild-Dateien die Klassifiziert werden sollen
     * @param method Methode mit der Klassifiziert werden soll
     */
    void classificationMultipleFiles(QStringList filesToClassifier, int method);

    /**
     * @brief Klassifiziert eine einzelne Datei nach der übergebene Methode
     * @param fileToClassifier Die Datei die Klassifiziert werden soll
     * @param method Methode mit der KLassifiziert werden soll
     * @return
     */
    Group* classificationSingleFile(QString fileToClassifier, int method);

    /**
     * @brief Erstellt die Confusion-Matrix und gibt diese zurück
     * @return Die ConfusionMatrix
     */
    QTableWidget* getConfusionMatrix();

    /**
     * @brief Erstellt eine Liste aller eingegeben Dateien und in welche Gruppen diese Klassifiziert wurden
     * @return Liste mit den Bild-Dateien und der klassifizierten Gruppe
     */
    QTableWidget* getListItems();

    /**
     * @brief Gibt die Mean-Rate zurück
     * @return Die MeanRate
     */
    double getMeanRate();

    /**
     * @brief Gibt die CorrectionRate zurück
     * @return Die CorrectionRate
     */
    double getCorrectionRate();

    /**
     * @brief Setzt das Vokabular von Features aus dem Training für die Klassifikation
     * @param dictionary Das Vokabular von Features
     */
    void setDictonary(cv::Mat dictionary);

private:
    /**
     * @brief Klassifiziert nach OpenCV Sift
     * @param classSample Das Bild-Sample was klassifiziert werden soll
     * @return Die Gruppe in der das Sample klassifiziert wurde
     */
    Group* classificationSift(Sample* classSample);

    /**
     * @brief Klassifiziert anhand der Durchschnittsfarbe
     * @param classSample Das Bild-Sample was klassifiziert werden soll
     * @return Die Gruppe in der das Sample klassifiziert wurde
     */
    Group* classificationAvgColor(Sample *classSample);

    /**
     * @brief correctionRate
     */
    double correctionRate;

    /**
     * @brief meanRate
     */
    double meanRate;

    /**
     * @brief groupController
     */
    GroupController *groupController;

    /**
     * @brief Container für alle gefunden Klasse eines Bild-Samples
     */
    std::map<std::string,std::pair<int,float> > found_classes;


    /**
     * @brief confusionMatrix
     */
    QTableWidget *confusionMatrix;

    /**
     * @brief itemList
     */
    QTableWidget *itemList;

    /**
     * @brief Container der Classifier der einzelnen Gruppen
     */
    QMap<QString,CvSVM*> classes_classifiers;


    /**
     * @brief Vokabular aus Features
     */
    cv::Mat dictionary;

    /**
     * @brief Liste mit vorhergesagten Gruppen
     */
    QList<Group*> predicted;

    /**
     * @brief Liste mit tatsächlichen Gruppen
     */
    QList<Group*> actual;

};

#endif // CLASSIFICATION_H
