#include "classificator.h"

Classificator::Classificator(GroupController *groupController)
{
    this->groupController = groupController;
    confusionMatrix = new QTableWidget();
    this->itemList = new QTableWidget();
    this->itemList->setMouseTracking(true);
    this->correctionRate = 0;
    this->meanRate = 0;
}


void Classificator::classificationMultipleFiles(QStringList filesToClassifier, int method)
{

    //QString sampleName = "../img-samples/set_1/apple1_1.JPG";
    if(filesToClassifier.isEmpty())
    {
        //message->setText("No Sample loaded yet");
        qDebug() << "No files loaded";
    }
    else
    {
        // Bereinigt eventuell vorher gefüllte Listen
        this->groupNames.clear();
        this->actualGroupNames.clear();
        this->groupController->clearSamplesFromGroups();

        // Initalisiert eine neue ItemListe zum Anzeigen der Bild-Dateiennamen und der vorhergesagten Gruppe
        this->itemList->setRowCount(filesToClassifier.size());
        this->itemList->setColumnCount(2);
        this->itemList->setHorizontalHeaderItem(0,new QTableWidgetItem(QString("FileName")));
        this->itemList->setHorizontalHeaderItem(1,new QTableWidgetItem(QString("Pred. Group")));

        // Bestimmung der Zeilen für die itemList
        int rowCounter = 0;
        // Anzahl der korrekten Klassifikationen
        int correctClassifications = 0;


        // Durchgeführt für jedes Bild in filesToClassifier
        foreach(QString file, filesToClassifier)
        {
            // Klassifiziert die einzelnen Bilder und speichert die Id der geschätzten Gruppe
            Group* foundedGroup = this->classificationSingleFile(file, method);

            if(foundedGroup != NULL)
            {
                // Füllt die Zeile mit den Werten für Bild-Dateinamen und der geschätzten Gruppe
                QTableWidgetItem *fileNameItem = new QTableWidgetItem(QFileInfo(file).fileName());
                QTableWidgetItem *groupNameItem = new QTableWidgetItem(foundedGroup->getName());

                // Bestimmt ob die Schätzung richtig ist und falls setzt die Textfarbe für die geschätzte Gruppe, Rot für Falsch und Grün für richtig
                if(this->groupController->splittingString(QFileInfo(file).fileName()) != foundedGroup->getName())
                {
                    groupNameItem->setForeground(QColor("Red"));
                }
                else
                {
                    groupNameItem->setForeground(QColor("Green"));
                    // Zählt die Anzahl der richtigen Klassifikationen für die Overall-Correction-Rate
                    correctClassifications++;
                }

                // Setzt die Zeile in itemList mit dem Bilddateinamen und der vorhergesagten Gruppe
                this->itemList->setItem(rowCounter,0,fileNameItem);
                this->itemList->setItem(rowCounter,1,groupNameItem);

                // geht zur nächsten Zeile
                rowCounter++;
            }
        }

        // Löschen?
        double correctionSum = 0;

        for(int i =0; i < this->groupNames.size();++i)
        {
            qDebug() << this->groupController->findGroupByName(this->groupNames.at(i))->getCorrectionRate();
            correctionSum = correctionSum + this->groupController->findGroupByName(this->groupNames.at(i))->getCorrectionRate();
        }

        this->meanRate = correctionSum / ((double) this->groupNames.size());

        // Setzt die Overall-CorrectionRate
        this->correctionRate = correctClassifications/ ( (double) filesToClassifier.size() );
    }

}

Group* Classificator::classificationSingleFile(QString fileToClassifier, int method)
{
    Sample *classSample = new Sample(fileToClassifier);

    Group* foundedGroup;
    if(method == 0)
    {
        // Führt eine Klassifikation anhand der Durchschnittsfarbe durch
        foundedGroup = this->classificationAvgColor(classSample);
    }
    else if(method == 1)
    {
        // Führt eine Klassifikation anhand des OpenCV SIft durch
        foundedGroup = this->classificationSift(classSample);
    }

    if(foundedGroup != NULL)
    {
        foundedGroup->addSamples(classSample);

        // Checkt ob die tatsächliche gruppe des Test-bildes bereits in der Liste der Tatsächlichen Gruppen existiert.
        if(!this->actualGroupNames.contains(classSample->getTrueGroupName()))
        {
            this->actualGroupNames.append(classSample->getTrueGroupName());
            if(!this->groupNames.contains(classSample->getTrueGroupName()))
            {
                this->groupNames.append(classSample->getTrueGroupName());
            }
        }

        if(!this->groupNames.contains(foundedGroup->getName()))
        {
            this->groupNames.append(foundedGroup->getName());
        }
    }

    // Gibt die gefunden Gruppe zurück
    return foundedGroup;
}

QTableWidget* Classificator::getConfusionMatrix()
{
    //Sortiert die Listen der GruppenNamen
    qSort(groupNames);
    qSort(actualGroupNames);
    // Bereinigt eine eventuell vorher vorhandene Matrix
    confusionMatrix->clearContents();
    // Setzt die Zeilen und Spalten anhand der Size tatsächlichen und vorhergesagten Gruppen
    confusionMatrix->setRowCount(this->actualGroupNames.size());
    confusionMatrix->setColumnCount(this->groupNames.size());
    for(int i=0;i < actualGroupNames.size();++i)
    {
        // Erstellt die Header-Spalte mit den tatsächlichen Gruppen-Namen
        confusionMatrix->setVerticalHeaderItem(i,new QTableWidgetItem(actualGroupNames.at(i)) );

        // Für die Summe der Samples
        int sum = 0;
        for(int j = 0; j < groupNames.size(); ++j)
        {
            // Erstellt die Header-Zeile mit den vorhergesagten Gruppen-Namen
            confusionMatrix->setHorizontalHeaderItem(j,new QTableWidgetItem(groupNames.at(j)) );

            int count = 0;
            // Zählt durch wieviele Test-Bilder in der vorhergesagten Gruppe der tatsächlichen Gruppe entsprechen
            for(int k= 0; k < this->groupController->findGroupByName(groupNames.at(j))->getSamples().size(); ++k)
            {
                if(this->groupController->findGroupByName(groupNames.at(j))->getSamples().at(k)->getTrueGroupName() == actualGroupNames.at(i))
                {
                    count++;
                }
            }
            // Summiert die Samples
            sum = sum + count;

            QTableWidgetItem *cellItem = new QTableWidgetItem(QString::number(count));
            if(actualGroupNames.at(i) == groupNames.at(j))
            {
                cellItem->setForeground(QColor("Green"));
            }
            else if (actualGroupNames.at(i) != groupNames.at(j) && cellItem->text() != "0")
            {
                cellItem->setForeground(QColor("Red"));
            }
            confusionMatrix->setItem(i,j,cellItem);

        }
        for(int j = 0; j < groupNames.size();++j)
        {
           confusionMatrix->item(i,j)->setText( QString("%1%").arg (QString::number (confusionMatrix->item(i,j)->text().toDouble()*100/sum) ));
        }
    }
    return this->confusionMatrix;
}

QTableWidget* Classificator::getListItems()
{
    return this->itemList;
}

double Classificator::getCorrectionRate()
{
    return (((this->correctionRate*100)/100)*100);
}

double Classificator::getMeanRate()
{
    return (((this->meanRate*100)/100)*100);
}

Group* Classificator::classificationSift(Sample* classSample)
{

    cv::Mat img,response_hist;
    // Liest das Bild ein
    img = cv::imread(classSample->getFilePath().toStdString());

    // Erstellt ein nearest neighbor matcher
    cv::Ptr<cv::DescriptorMatcher> matcher(new cv::FlannBasedMatcher);
    // Erstellt ein Sift feature point extracter
    cv::Ptr<cv::FeatureDetector> detector(new cv::SiftFeatureDetector());
    // Erstellt ein Sift descriptor extractor
    cv::Ptr<cv::DescriptorExtractor> extractor(new cv::SiftDescriptorExtractor);
    // Erstellt ein BoF (BoW) descriptor extractor
    cv::BOWImgDescriptorExtractor bowide(extractor,matcher);
    // Setzt das Vokabular, erstellt aus dem Training, auf den BOWImgDescriptorExtraktor
    bowide.setVocabulary(dictionary);

    std::vector<cv::KeyPoint> keypoints;
    // Ermittelt die Keypoints des BIldes
    detector->detect(img,keypoints);

    // Erstellt ein Histogramm von Features(Descriptor) anhand der Keypoints und es Test-Bildes
    bowide.compute(img, keypoints, response_hist);
    cv::Mat converted_response_hist;
    response_hist.convertTo(converted_response_hist,CV_32FC1);
    float minf = FLT_MAX; QString minclass;

    //this->found_classes.clear();

    for(int i = 0; i < this->groupController->getGroups().size();++i)
    {
        QString currentGroup =  this->groupController->getGroups().at(i)->getName();

        // Holt den Classifier der Gruppe
        CvSVM* classifier = this->groupController->getGroups().at(i)->getClassifier();

        // Ermittelt die Distanz des Feature-Histogramms zur Gruppe
        float res = classifier->predict(converted_response_hist,true);

        // Wenn Distanz über 1.0 dann überspringe
        if(res > 1.0) continue;
        // Wenn aktuelle Distanz kleiner als Minimal distanz ersetze minClas und minF
        if (res < minf)
        {
            minf = res;
            minclass = currentGroup;

            //found_classes[minclass].first++;
            //found_classes[minclass].second += minf;
        }
    }

    //qDebug() << minclass << classSample->getFileName();

    //float max_class_f = FLT_MIN, max_class_f1 = FLT_MIN; QString max_class, max_class1;
    //std::vector<double> scores;

    // Sucht die Gruppe und gibt den Zeiger zurück
    return this->groupController->findGroupByName(minclass);
}

Group* Classificator::classificationAvgColor(Sample* classSample)
{
    double gAvgRed, gAvgGreen, gAvgBlue;
    double distanceR = 0;
    double distanceG = 0;
    double distanceB = 0;
    double avgDistance = 0;

    // Durchschnittswerte des Test-Bildes
    double sAvgRed = classSample->getAvgRed();
    double sAvgGreen = classSample->getAvgGreen();
    double sAvgBlue = classSample->getAvgBlue();



    QMap<QString, double> distanceTable;

    // Ermittelt für jede Gruppe die Distanz der einzelnen Farben
    for(int i=0; i<this->groupController->getGroups().size();++i)
    {
        // Berechnet die durchschnittlichen RGB-Werte der Gruppe
        this->groupController->getGroups().at(i)->calculateAvgColors();

        // Holt die durchschnittlichen RGB-Werte der Gruppe
        gAvgRed = this->groupController->getGroups().at(i)->getAvgRed();
        gAvgGreen = this->groupController->getGroups().at(i)->getAvgGreen();
        gAvgBlue = this->groupController->getGroups().at(i)->getAvgBlue();

        //qDebug() << "GRed: " << gAvgRed << "GGreen: " << gAvgGreen << "GBlue: " << gAvgBlue;

        // Ermittelt die Distanz der Durchschnittlichen RGB-Werte der Gruppe zwischen denen des Test-Bildes
        distanceR = abs(gAvgRed - sAvgRed);
        distanceG = abs(gAvgGreen - sAvgGreen);
        distanceB = abs(gAvgBlue - sAvgBlue);

        avgDistance = (distanceR+distanceG+distanceB)/(double)3;

        // Fügt die Durchschnitts-Distanz zur Map hinzu mit den GruppenNamen
        distanceTable.insert(this->groupController->getGroups().at(i)->getName(),avgDistance);
    }

    // Erstellt einen Iterator zum durchlaufen der Map
    QMap<QString, double>::const_iterator i = distanceTable.constBegin();

    //int counter = 0;
    //int id = 0;

    double minDistance = DBL_MAX;
    QString minGroup;

    while (i != distanceTable.constEnd())
    {

        // Wenn der Abstand kleiner ist, ersetze minDistance und minGroup
        if(i.value() < minDistance)
        {
            //id = counter;
            minDistance = i.value();
            minGroup = i.key();
        }
        ++i;
        //++counter;
    }
    //qDebug() << "ID: " << id << " Group: " <<minGroup <<" Distance: " << minDistance;

    // Sucht den Gruppennamen und gibt den Pointer zur Gruppe zurück
    return this->groupController->findGroupByName(minGroup);

}

void Classificator::setDictonary(cv::Mat dictionary)
{
    this->dictionary = dictionary;
}
