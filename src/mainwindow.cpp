#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    groupController = new GroupController();
    trainer = new Trainer(groupController);
    connect(trainer,SIGNAL(updateStatus(QString)),this,SLOT(updatingStatusMessage(QString)));

    classificator = new Classificator(groupController);
    fileName = "";
    directory = "";

    // Initalisiert die View
    initView();
}

MainWindow::~MainWindow()
{

}

void MainWindow::initView()
{
    mainWidget = new QWidget(this);
    mainWidget->setMinimumSize(500,500);

    vLayout = new QVBoxLayout(mainWidget);
    buttonLayout = new QHBoxLayout();
    mainViewLayout = new QHBoxLayout();
    innerLayout = new QHBoxLayout();

    vocabularySize = new QSpinBox();
    vocabularySize->setValue(50);
    vocabularySize->setRange(50,10000);
    vocabularySize->setVisible(false);

    vocabularySizeLabel = new QLabel(tr("VocabularySize"));
    vocabularySizeLabel->setBuddy(vocabularySize);
    vocabularySizeLabel->setVisible(false);

    additionalMenu = new QHBoxLayout();

    // Pushbuttons
    QPushButton *openTrainData = new QPushButton(tr("Set Training Files"));
    connect(openTrainData,SIGNAL(clicked(bool)),this,SLOT(openingTrainingFiles()));
    QPushButton *openSamples = new QPushButton(tr("Open Sample Pictures"));
    connect(openSamples,SIGNAL(clicked(bool)),this,SLOT(openingSampleFiles()));
    QPushButton *classifier = new QPushButton(tr("Classifiering"));
    connect(classifier,SIGNAL(clicked(bool)),this,SLOT(classification()));

    QComboBox *methodBox = new QComboBox();
    connect(methodBox,SIGNAL(currentIndexChanged(int)),this,SLOT(setMethod(int)));
    methodBox->addItem(tr("Avg-Color"));
    methodBox->addItem(tr("OpenCV SIFT"));



    // MainView
    correctionRate = new QLabel(tr("Correction rate: %1 %").arg(QString::number(0)));
    meanRate = new QLabel(tr("Mean rate: %1 %").arg(QString::number(0)));

    innerLayout->addWidget(correctionRate);
    innerLayout->addWidget(meanRate);

    additionalMenu->addWidget(vocabularySizeLabel);
    additionalMenu->addWidget(vocabularySize);

    buttonLayout->addWidget(openTrainData);
    buttonLayout->addWidget(openSamples);
    buttonLayout->addWidget(classifier);

    vLayout->addWidget(methodBox);
    vLayout->addLayout(additionalMenu);
    vLayout->addLayout(buttonLayout);
    vLayout->addLayout(innerLayout);
    vLayout->addLayout(mainViewLayout);

    QMenuBar *menubar = new QMenuBar();
    QMenu * fileMenu = new QMenu(tr("file"));
    QAction *closeAction = new QAction(tr("&Close"),this);
    connect(closeAction,SIGNAL(triggered()),this,SLOT(close()));
    fileMenu->addAction(closeAction);
    menubar->addMenu(fileMenu);

    statusBar = new QStatusBar();
    statusBar->showMessage("App started");

    this->setStatusBar(statusBar);

    this->setMenuBar(menubar);
    this->setCentralWidget(mainWidget);
}

void MainWindow::openingTrainingFiles()
{
    directory = QFileDialog::getExistingDirectory(this,tr("Open Training-Files"),"");
    trainer->setVocabularySize(this->vocabularySize->value());
    trainer->training(directory,this->method);
}

void MainWindow::openingSampleFiles()
{
    files.clear();
    files = QFileDialog::getOpenFileNames(this,tr("Open Samples"),"",tr("Image Files (*.png *.jpg *.JPG *.PNG)"));
}

void MainWindow::classification()
{
    // Wenn OpenCV Sift ausgewählt wurde
    if(this->method == 1)
    {
        // Übergib dem Classificator das Vokabular von Features aus der Trainingsphase
        classificator->setDictonary(trainer->getDictonary());
    }
    // Klassifiziert ein oder mehrere Files mit der angegeben Methode
    classificator->classificationMultipleFiles(files,this->method);

    // Rechte Seite der View
    QTableWidget* listItems = classificator->getListItems();
    listItems->setMaximumWidth(300);
    mainViewLayout->insertWidget(0,listItems);

    // Linke Seite der View Confusion-Matrix
    QTableWidget *newtable = classificator->getConfusionMatrix();
    mainViewLayout->insertWidget(1,newtable);

    // Setzt CorrectionRate und MeanRate
    correctionRate->setText(tr("Correction rate: %1 %").arg(QString::number(classificator->getCorrectionRate())));
    meanRate->setText(tr("meanRate rate: %1 %").arg(QString::number(classificator->getMeanRate())));
}

void MainWindow::setMethod(int method)
{
    if(method == 1)
    {
        this->vocabularySize->setVisible(true);
        this->vocabularySizeLabel->setVisible(true);
    }
    else
    {
        this->vocabularySize->setVisible(false);
        this->vocabularySizeLabel->setVisible(false);

    }
    this->method = method;
}

void MainWindow::updatingStatusMessage(QString message)
{

    this->statusBar->showMessage(message);
}
