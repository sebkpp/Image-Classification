#include "groupcontroller.h"

GroupController::GroupController()
{
    //groups = new QList<Group *>();
}

bool GroupController::isGroupExisting(QString name)
{

    QString groupName = splittingString(name);

    for(int i=0;i < this->groups.size();++i)
    {
        if(this->groups.at(i)->getName() == groupName)
        {
            //qDebug() << "Group Exists";
            return true;
        }
    }
    return false;
}

bool GroupController::isGroupExistingIn(Group *group, QList<Group*> listOfGroups)
{
    for(int i=0;i < listOfGroups.size();++i)
    {
        if(listOfGroups.at(i)->getName() == group->getName())
        {
            return true;
        }
    }
    return false;
}

bool GroupController::isGroupExistingIn(QString groupName, QList<Group*> listOfGroups)
{
    for(int i=0;i < listOfGroups.size();++i)
    {
        if(listOfGroups.at(i)->getName() == groupName)
        {
            return true;
        }
    }
    return false;
}

Group* GroupController::findGroupByName(QString groupName)
{
    for(int i=0;i < this->groups.size();++i)
    {
        if(this->groups.at(i)->getName() == groupName)
        {
            return this->groups.at(i);
        }
    }
    //qDebug() << "Group not found";
    return NULL;
}

void GroupController::createGroup(QString name)
{
    //qDebug() << "create Group";
    QString groupName = splittingString(name);
    Group *group = new  Group(groupName);
    this->groups.append(group);
    //qDebug() << "added Group";

}

void GroupController::addValuesToGroup(Sample *sample)
{
    QString groupName = splittingString(sample->getFileName());
    for(int i=0;i < this->groups.size();++i)
    {
        if(this->groups.at(i)->getName() == groupName)
        {
            //qDebug() << "GRed: " << sample->getAvgRed() << "GGreen: " << sample->getAvgGreen() << "GBlue: " << sample->getAvgBlue();
            this->groups.at(i)->addSampleValues(sample);
            return;
        }
    }
}

void GroupController::addToGroup(Sample *sample)
{
    QString groupName = splittingString(sample->getFileName());
    for(int i=0;i < this->groups.size();++i)
    {
        if(this->groups.at(i)->getName() == groupName)
        {
            this->groups.at(i)->addSamples(sample);
            return;
        }
    }
}

QList<Group *> GroupController::getGroups()
{
    return this->groups;
}

void GroupController::clearSamplesFromGroups()
{
    for(int i= 0; i < this->groups.size();++i)
    {
        this->groups.at(i)->clearSamples();
    }
}

void GroupController::displayGroupNames()
{
    for(int i=0;i < this->groups.size();++i)
    {
        qDebug() << this->groups.at(i)->getName() << "AvgRed: " <<  this->groups.at(i)->getAvgRed() << "AvgGreen: " <<  this->groups.at(i)->getAvgGreen() << "AvgBlue: " <<  this->groups.at(i)->getAvgBlue();
    }
}

QString GroupController::splittingString(QString unsplittedString)
{
    QStringList sList = unsplittedString.split("_");
    return sList.at(0);
}

void GroupController::deletingAllGroups()
{
    for(int i = 0; i < this->groups.size();++i)
    {
        this->groups.at(i)->~Group();
    }
    this->groups.clear();
}
