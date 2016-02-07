#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMenuBar>
#include <QtMath>
#include <QFileDialog>
#include <QTableWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QStatusBar>

#include "sample.h"
#include "groupcontroller.h"
#include "trainer.h"
#include "classificator.h"

/**
 * @brief View für die Anwendung
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor
     * @param parent (Qt-Spezifisch)
     */
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:

    /**
     * @brief Initiert die View
     */
    void initView();

    /**
     * @brief groupController
     */
    GroupController *groupController;

    /**
     * @brief trainer
     */
    Trainer *trainer;

    /**
     * @brief classificator
     */
    Classificator *classificator;

    /**
     * @brief vLayout
     */
    QVBoxLayout *vLayout;
    /**
     * @brief buttonLayout
     */
    QHBoxLayout *buttonLayout;

    /**
     * @brief mainViewLayout
     */
    QHBoxLayout *mainViewLayout;

    /**
     * @brief innerLayout
     */
    QHBoxLayout *innerLayout;

    /**
     * @brief additionalMenu
     */
    QHBoxLayout* additionalMenu;

    /**
     * @brief mainWidget
     */
    QWidget *mainWidget;

    /**
     * @brief confusionMatrix
     */
    QTableWidget * table;

    /**
     * @brief correctionRate
     */
    QLabel *correctionRate;

    /**
     * @brief meanRate
     */
    QLabel *meanRate;

    /**
     * @brief Bild-Datei
     */
    QString fileName;

    /**
     * @brief Liste von Bild-Dateien
     */
    QStringList files;

    /**
     * @brief Verzeichnis mit Bild-Dateien
     */
    QString directory;

    /**
     * @brief Methode nach der Klassifiziert werden soll
     */
    int method;

    /**
     * @brief vocabularySizeLabel
     */

    QLabel *vocabularySizeLabel;
    /**
     * @brief vocabularySize
     */
    QSpinBox *vocabularySize;

    /**
     * @brief statusBar
     */
    QStatusBar *statusBar;

private slots:

    /**
     * @brief Slot wird ausgeführt wenn der Button zum Trainieren gedrückt wird
     */
    void openingTrainingFiles();

    /**
     * @brief Slot wird ausgeführt wenn der Button zur Auswahl von Bildern gedrückt wird
     */
    void openingSampleFiles();

    /**
     * @brief Slot setzt die Methodem, nach der Klassifiziert werden soll, wenn die Methode im View geändert wird
     * @param method Die Methode
     */
    void setMethod(int method);

    /**
     * @brief Slot wird ausgeführt wenn der Button zur Klassifikation gedrückt wird
     */
    void classification();

    /**
     * @brief Slot updatet die Status-Message in der Statusbar
     * @param message
     */
    void updatingStatusMessage(QString message);
};

#endif // MAINWINDOW_H
