#ifndef TRAINER_H
#define TRAINER_H


#include <QStringList>
#include <QDir>
#include <QObject>

// OpenCV
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp> // imread usw.
#include "opencv2/nonfree/nonfree.hpp" // SIFT
#include <opencv2/ml/ml.hpp>  // Maschine Learning (SVM)

#include "sample.h"
#include "groupcontroller.h"

/**
 * @brief Die Klasse zum Trainieren auf die Bilder, für die Klassifizierung nach SIFT wird die OpenCV-bibilothek genutzt.
 */
class Trainer : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Construktor
     * @param groupController der Controller für die Gruppen
     */
    Trainer(GroupController *groupController);
    /**
     * @brief Trainiert die Bilddaten
     * @param directoryWithFiles Verzeichnis mit den Trainings-Daten
     * @param method  Die Methode mit der trainiert werden soll
     */
    void training(QString directoryWithFiles, int method);

    /**
     * @brief Gibt das aus dem Training erstellte Vokabular zurück
     * @return Das erstellt Vokabular
     */
    cv::Mat getDictonary();

    /**
     * @brief Setzt die größe des Feature Vokabulars fest
     * @param size die größe des Vokabulars
     */
    void setVocabularySize(int size);
private:
    /**
     * @brief groupController
     */
    GroupController *groupController;

    /**
     * @brief Baut das Vokabular für Sift
     */
    void buildSiftVocabulary();

    /**
     * @brief Trainiert das BoF und die SVM mit den Daten
     * @param directoryWithFiles Verzeichnis mit den Trainings-Daten
     */
    void trainSift(QString directoryWithFiles);

    /**
     * @brief Das Vokabular mit den Features
     */
    cv::Mat dictionary;

    /**
     * @brief Container zum speichern des Input-Trainingsbilds
     */
    cv::Mat input;
    /**
     * @brief Container zum speichern der Keypoints
     */
    std::vector<cv::KeyPoint> keypoints;

    /**
     * @brief Container zum speichern eines einzelnen Bilddescriptors
     */
    cv::Mat descriptor;
    /**
     * @brief Container zum speichern aller Bilddescriptoren
     */
    cv::Mat featuresUnclustered;
    /**
     * @brief SiftFeaturedetector
     */
    cv::SiftFeatureDetector detector;

    /**
     * @brief vocabularySize
     */
    int vocabularySize;
signals:
    void updateStatus(QString statusMessage);
};

#endif // TRAINER_H
