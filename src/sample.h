#ifndef SAMPLE_H
#define SAMPLE_H

#include <QImage>
#include <QColor>
#include <QFileInfo>
/**
 * @brief Klasse zum Speichern der Informationen es Bild-Samples
 */
class Sample
{
public:
    /**
     * @brief Default Konstruktor
     */
    Sample();
    /**
     * @brief Konstruktor
     * @param Pfad zum Bild
     */
    Sample(QString path);

    /**
     * @brief Berechnet die einzelnen private Members der Klasse
     */
    void calculateParameters();

    /**
     * @brief Gibt die Durchschnitts Rot-Wert des Bildes zurück
     * @return durchschnittliche Rot-Wert des Bildes
     */
    double getAvgRed();

    /**
     * @brief Gibt die Durchschnitts Grün-Wert des Bildes zurück
     * @return durchschnittliche Grün-Wert des Bildes
     */
    double getAvgGreen();

    /**
     * @brief Gibt die Durchschnitts Blau-Wert des Bildes zurück
     * @return durchschnittliche Blau-Wert des Bildes
     */
    double getAvgBlue();

    /**
     * @brief Gibt den Dateinamen zurück, mit Suffix
     * @return Der Dateiname
     */
    QString getFileName();
    /**
     * @brief Gibt die tatsächliche Gruppe zu der das Bild gehört zurück
     * @return Die tatsächliche Gruppe
     */
    QString getTrueGroupName();

    /**
     * @brief Gibt den Dateipfad zurück
     * @return Der Dateipfad
     */
    QString getFilePath();

    /**
     * @brief Debug-FUnktion zum Anzeigen von den AVG-RGB-Werten
     * @return
     */
    QString displayAvgValues();

private:
    /**
     * @brief Dateipfad
     */
    QString path;
    /**
     * @brief Dateiname
     */
    QString fileName;
    /**
     * @brief Tatsächliche Gruppe
     */
    QString trueGroupname;

    /**
     * @brief width
     */
    int width;
    /**
     * @brief height
     */
    int height;

    /**
     * @brief avgRed
     */
    double avgRed;
    /**
     * @brief avgGreen
     */
    double avgGreen;
    /**
     * @brief avgBlue
     */
    double avgBlue;
};

#endif // SAMPLE_H
