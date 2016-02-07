#ifndef GROUPCONTROLLER_H
#define GROUPCONTROLLER_H

#include <QDebug>
#include <QStringList>
#include <QString>
#include <QtMath>
#include <QTableWidgetItem>

#include "group.h"
#include "sample.h"

/**
 * @brief Controller-Klasse zum verwalten der Gruppen
 */
class GroupController
{
public:

    /**
     * @brief Default Konstruktor
     */
    GroupController();

    /**
     * @brief Überprüft ob Gruppe existiert
     * @param GrupenName
     * @return true falls Gruppe existiert,ansonsten false
     */
    bool isGroupExisting(QString name);

    /**
     * @brief Überprüft ob Gruppen-Objekt in der übergebenen Liste existiert
     * @param group das Gruppen-Objekt
     * @param listOfGroups Liste in der überprüft werden soll
     * @return true falls existiert, ansonsten false
     */
    bool isGroupExistingIn(Group *group, QList<Group*> listOfGroups);

    /**
     * @brief Überprüft ob Gruppen-Name in der übergebenen Liste existiert
     * @param GroupName der Gruppen-Name
     * @param listOfGroups Liste in der überprüft werden soll
     * @return true falls existiert, ansonsten false
     */
    bool isGroupExistingIn(QString GroupName, QList<Group*> listOfGroups);

    /**
     * @brief Sucht Gruppe in der Liste
     * @param groupName der Gruppen-Name
     * @return Pointer zur Gruppe
     */
    Group* findGroupByName(QString groupName);

    /**
     * @brief Erstellt eine neue Gruppe
     * @param Der Gruppen-Name
     */
    void createGroup(QString name);

    /**
     * @brief Fügt ein Bild-Sample zur Gruppe hinzu
     * @param Das Sample
     */
    void addToGroup(Sample *sample);
    /**
     * @brief Fügt die Sample-Werte zur Gruppe hinzu(Durchschnittsfarbe z.B.)
     * @param Das Sample
     */
    void addValuesToGroup(Sample *sample);

    /**
     * @brief Löscht alle Samples aus einer Gruppe
     */
    void clearSamplesFromGroups();

    /**
     * @brief Debug-Funktion zum ANzeigen der Gruppen
     */
    void displayGroupNames();

    /**
     * @brief Gibt eine Liste mit allen Gruppen zurück
     * @return Die Liste mit den Gruppen
     */
    QList<Group *> getGroups();

    /**
     * @brief Trennt den Gruppen-Namen aus dem Dateinamen
     * @param unsplittedString
     * @return Der Gruppen-Name
     */
    QString splittingString(QString unsplittedString);

    void deletingAllGroups();

private:
    /**
     * @brief Liste mit allen Gruppen
     */
    QList<Group *> groups;

};

#endif // GROUPCONTROLLER_H
