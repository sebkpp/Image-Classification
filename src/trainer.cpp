#include "trainer.h"

Trainer::Trainer(GroupController *groupController) : QObject()
{
    this->groupController = groupController;
    this->vocabularySize = 50;
}

void Trainer::training(QString directoryWithFiles, int method)
{
    // Wenn Verzeichnis leer dann beende
    if(directoryWithFiles.isEmpty())
    {
        return;
    }

    // Lösche eventuell vorher vorhandene Gruppen
    this->groupController->deletingAllGroups();

    emit updateStatus("Begin Training");
    // Gehe durch jedes Fall, mit ausnahme von . und ..
    foreach(QFileInfo entry, QDir(directoryWithFiles).entryInfoList(QDir::NoDotAndDotDot| QDir::AllEntries))
    {
        // Erstellt ein neues Sample
        Sample *sample = new Sample(entry.absoluteFilePath());
        // Wenn eine Bild-Gruppe noch nicht vorhanden ist erstelle eine
        if(!groupController->isGroupExisting(sample->getFileName()))
        {
            groupController->createGroup(sample->getFileName());
        }

        // Führe ein Training nach der Durchschnittsfarbe aus
        if(method == 0)
        {
            // Fügt die Durchschnittsfarbe des Bildes zur Durchschnittsfarbe der Gruppe hinzu und berechnet diese neu
            groupController->addValuesToGroup(sample);
        }
        // Oder führe ein Training nach OpenCV-Sift aus
        else if(method == 1)
        {
            // Liest das Bild ein
            input = cv::imread(entry.filePath().toStdString());
            // Sucht interesante Schlüsselpunkte im Bild
            detector.detect(input,keypoints);
            // Erstellt einen Bild-Descriptor anhand der Keypoints und des Bildes
            detector.compute(input,keypoints,descriptor);
            // Fügt den Descriptor dem Mat-Array hinzu
            featuresUnclustered.push_back(descriptor);
        }
    }

    if(method == 1)
    {
        // Baut ein Vokabular aus Features auf
        buildSiftVocabulary();
        // Trainiert Sift und SVM-Classifier
        trainSift(directoryWithFiles);
    }
    emit updateStatus("Training finished");
}

void Trainer::buildSiftVocabulary()
{
    //qDebug() << "Building Vocabulary";
    emit updateStatus("Building Vocabulary...");

    // Anzahl der Features im Vokabular des Bag of Features
    int dictionarySize= this->vocabularySize;
    //int dictionarySize= this->groupController->getGroups().size();

    //Term Criteria
    cv::TermCriteria tc(CV_TERMCRIT_ITER,1000,0.001);

    //Erstellt einen Bag of Words trainer
    cv::BOWKMeansTrainer bowTrainer(dictionarySize,tc,1,cv::KMEANS_PP_CENTERS);
    //cv::BOWKMeansTrainer bowTrainer(dictionarySize);

    //Clustern der Features
    dictionary=bowTrainer.cluster(featuresUnclustered);
}

void Trainer::trainSift(QString directoryWithFiles)
{
    //qDebug() << "Start Training";
    emit updateStatus("Start Training...");

    // Erstellt einen FlannMatcher zur suche des nächsten Nachbarn
    cv::Ptr<cv::DescriptorMatcher> matcher(new cv::FlannBasedMatcher);
    // Erstellt einen Sift Feature Detector
    cv::Ptr<cv::FeatureDetector> detector(new cv::SiftFeatureDetector());
    // Erstellt einen SIft Descriptor Extraktor
    cv::Ptr<cv::DescriptorExtractor> extractor(new cv::SiftDescriptorExtractor);
    // Erstellt ein Bag of Feature Descriptor Extraktor
    cv::BOWImgDescriptorExtractor bowDE(extractor,matcher);
    // Übergibt den BoF das Vokabular
    bowDE.setVocabulary(dictionary);

    //setup training data for classifiers
    std::map<std::string,cv::Mat> classes_training_data; classes_training_data.clear();

    foreach(QFileInfo entry, QDir(directoryWithFiles).entryInfoList(QDir::NoDotAndDotDot| QDir::AllEntries))
    {
        //Einlesen des Images
        cv::Mat img= cv::imread(entry.filePath().toStdString());

        //Vektor zum einlesen der SIFT-Keypoints
        std::vector<cv::KeyPoint> keypoints;
        //Suche von interesanten Key-Points im Bild
        detector->detect(img,keypoints);
        // Container zum speichern die Descriptoren
        cv::Mat bowDescriptor;
        // Extrahiert die Descriptoren vom Bag of Feature Objekt
        bowDE.compute(img,keypoints,bowDescriptor);

        std::string currentGroupName = this->groupController->splittingString(entry.fileName()).toStdString();

        // Erstelle einen neuen Eintrag in der Map falls gruppe noch nicht vorhanden
        if(classes_training_data.count(currentGroupName) == 0)
        {
           classes_training_data[currentGroupName].create(0,bowDescriptor.cols,bowDescriptor.type());
        }
        // Fügt dem Descriptor der jeweiligen Gruppe zu
        classes_training_data[currentGroupName].push_back(bowDescriptor);
    }

    emit updateStatus("Training SVM...");

    // Trainiere die State Vector Machine für jede Gruppe
    for(int i=0; i < this->groupController->getGroups().size();++i)
    {
        std::string currentGroupName = this->groupController->getGroups().at(i)->getName().toStdString();
        // Container für die Samples
        cv::Mat samples(0,bowDE.descriptorSize(),bowDE.descriptorType());

        // Container für die Labels 1 für Positiv 0 für Negativ
        cv::Mat labels(0,1,CV_32FC1);

        // Setzt die Labels für die richtigen Samples der Gruppe auf positiv
        samples.push_back(classes_training_data[currentGroupName]);
        cv::Mat class_label = cv::Mat::ones(classes_training_data[currentGroupName].rows, 1, CV_32FC1);
        labels.push_back(class_label);

        // Setzt alle anderen Labels auf 0
        for (std::map<std::string,cv::Mat>::iterator it1 = classes_training_data.begin(); it1 != classes_training_data.end(); ++it1) {
            std::string not_class_ = (*it1).first;
            // Wenn Klasse bereits auf positiv markiert wurde dann überspringe den SChleifendurchlauf
            if(not_class_.compare(currentGroupName)==0) continue;
            samples.push_back(classes_training_data[not_class_]);
            class_label = cv::Mat::zeros(classes_training_data[not_class_].rows, 1, CV_32FC1);
            labels.push_back(class_label);
        }
        // Konvertiert die Samples in CV_32F
        cv::Mat samples_32f; samples.convertTo(samples_32f, CV_32FC1);

        // Erstellt einen neuen SVM-Classifier
        CvSVM *classifier = new CvSVM;

        // Trainiert den Classifier mit den Samples und Labels
        classifier->train(samples_32f,labels);
        // Setzt den Classifier für die jeweilige Gruppe
        this->groupController->getGroups().at(i)->setClassifier(classifier);

        //qDebug() << "trained " << QString::fromStdString(groupName);

    }
}

cv::Mat Trainer::getDictonary()
{
    return this->dictionary;
}

void Trainer::setVocabularySize(int size)
{
    this->vocabularySize = size;
}
