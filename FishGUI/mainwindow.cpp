#include <mainwindow.h>

MainWindow::MainWindow(QWidget *parent)
    :QWidget(parent)
	{
        ui.reset(new Ui::MainWidget);
        m_fIndex = 0;
        myPlayer.reset(new Player());
//		fList = new fishSerialize::FishList();
        myFishList.clear();
        QObject::connect(myPlayer.get(), SIGNAL(processedImage(QImage)),
			this, SLOT(updatePlayerUI(QImage)));
		ui->setupUi(this);
		ui->Play->setEnabled(false);
		ui->videoSlider->setEnabled(false);
		QStringList typeList;
		typeList.append("Round");
		typeList.append("Flat");
		typeList.append("Skate");
		typeList.append("Other");
		ui->typeMenu->addItems(typeList);
		QObject::connect(ui->typeMenu, SIGNAL(currentIndexChanged(int)),
			this, SLOT(updateSubTypeMenu(int)));
		ui->typeMenu->setCurrentIndex(3);
        QObject::connect(ui->goToFishVal,SIGNAL(returnPressed()),
                         this, SLOT(goToFish()));
        QObject::connect(ui->towStatus, SIGNAL(toggled(bool)),
                         this, SLOT(setTowType(bool)));
//        setTowType(true);
	}
/*
MainWindow::~MainWindow()
{
	delete ui;
}
*/
void MainWindow::updatePlayerUI(QImage img)
{
	if (!img.isNull())
	{
		imgPointer->setPixmap(QPixmap::fromImage(img));
		scene->setSceneRect(img.rect());
		ui->videoWindow->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
		ui->videoSlider->setValue(myPlayer->getCurrentFrame());
		ui->currentTime->setText(getFormattedTime((int)myPlayer->
			getCurrentFrame() / (int)myPlayer->getFrameRate()));
	}
}

void MainWindow::updateSubTypeMenu(int typeIndex)
{
	QStringList sTypeList;
    ui->subTypeMenu->blockSignals(true);
	ui->subTypeMenu->clear();
	if (typeIndex == 0) {
		sTypeList.append("Round");
		sTypeList.append("Haddock");
		sTypeList.append("Cod");
		sTypeList.append("Whiting");
		sTypeList.append("Red Hake");
		sTypeList.append("Pollock");
		sTypeList.append("Herring");
		sTypeList.append("Unknown");
	}
	else if (typeIndex == 1) {
		sTypeList.append("Flat");
		sTypeList.append("Yellowtail");
		sTypeList.append("Windowpane");
		sTypeList.append("Summer");
		sTypeList.append("Winter");
		sTypeList.append("Fourspot");
		sTypeList.append("Grey Sole");
		sTypeList.append("Halibut");
		sTypeList.append("Unknown");
	}
	else if (typeIndex == 2) {
		sTypeList.append("Skate");
		sTypeList.append("Barndoor");
		sTypeList.append("Unknown");
	}
	else if (typeIndex == 3) {
		sTypeList.append("Other");
		sTypeList.append("Dogfish");
		sTypeList.append("Monkfish");
		sTypeList.append("Lobster");
		sTypeList.append("Scallop");
	}
	ui->subTypeMenu->addItems(sTypeList);
    ui->subTypeMenu->setCurrentIndex(0);
    ui->subTypeMenu->blockSignals(false);
}

void MainWindow::goToFish()
{
    int fNumber = ui->goToFishVal->text().toInt();
    if (fNumber > 0)
    {
        listPos = myFishList.begin()+fNumber-1;
        ui->typeMenu->setCurrentIndex((int) listPos->getFishType());
        ui->subTypeMenu->setCurrentIndex((int) listPos->getFishSubType());
        updateVecIndex();
    }
}

QString MainWindow::getFormattedTime(int timeInSeconds)
{
	int seconds = (int) (timeInSeconds) % 60;
	int minutes = (int)((timeInSeconds / 60) % 60);
	int hours = (int)((timeInSeconds / (60 * 60)) % 24);

	QTime t(hours, minutes, seconds);
	if (hours == 0)
		return t.toString("mm:ss");
	else
		return t.toString("h:mm:ss");
}

void MainWindow::on_LoadVideo_clicked()
{
	QString filename = QFileDialog::getOpenFileName(this,
		tr("Open Video"), ".",
		tr("Video Files (*.avi *.mpg *.mp4)"));

    QFileInfo name = filename;
	if (!filename.isEmpty())
	{
        if (!myPlayer->loadVideo(filename.toLatin1().data()))
		{
			QMessageBox msgBox;
			msgBox.setText("The selected video could not be opened!");
			msgBox.exec();
		}
		else
        {
            this->setWindowTitle(name.fileName());
			ui->Play->setEnabled(true);
			ui->videoSlider->setEnabled(true);
			ui->videoSlider->setMaximum(myPlayer->getNumberOfFrames());
			ui->totalTime->setText(getFormattedTime((int)myPlayer-> 
				getNumberOfFrames() / (int)myPlayer->getFrameRate()));
			QImage firstImage = myPlayer->getOneFrame();
            scene.reset(new QGraphicsScene(this));
//            scene = new QGraphicsScene(this);
			imgPointer = scene->addPixmap(QPixmap::fromImage(firstImage));
			scene->setSceneRect(firstImage.rect());
            ui->videoWindow->setScene(scene.get());
            ui->videoWindow->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
            ui->videoWindow->show();
		}
	}

}

void MainWindow::on_loadAnnotate_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Open Annotation File"), ".",
        tr("CSV Files (*.csv)"));

    //QFileInfo name = filename;
    ifstream inFile(filename.toLatin1().data());
    string line;
    string tripID, reviewer, towType, fishType, species;
    int frame, towNum;
    double timeInVid;
    getline(inFile,line);
    line.clear();
    while(getline(inFile,line))
    {
        stringstream linestream(line);
        //line.clear();

        //Assign to the members of the struct
        string tempToken;
        std::getline(linestream,tripID,',');
        std::getline(linestream,tempToken,',');
        stringstream tempConvert(tempToken);
        tempConvert >> towNum;
        tempToken.clear();
        tempConvert.str("");
        tempConvert.clear();
        std::getline(linestream,reviewer,',');
        std::getline(linestream,towType,',');
        std::getline(linestream,fishType,',');
        std::getline(linestream,species,',');
        std::getline(linestream,tempToken,',');
        tempConvert << tempToken;
        tempConvert >> frame;
        tempToken.clear();
        tempConvert.str("");
        tempConvert.clear();
        std::getline(linestream,tempToken,',');
        tempConvert << tempToken;
        tempConvert >> timeInVid;
        tempToken.clear();
        tempConvert.str("");
        tempConvert.clear();


        //linestream >> tripID >> towNum >> reviewer >> towType >> fishType >> species >> frame >> timeInVid;
        //std::cout << "Fish Stats: " << std::endl;
        //std::cout << towNum << std::endl << reviewer << std::endl << towType << std::endl << fishType << std::endl;
        //std::cout << species << std::endl << frame << std::endl << timeInVid << std::endl << std::endl;
        /*
        tripID << linestream;
        towNum << linestream;
        reviewer << linestream;
        towType << linestream;
        fishType << linestream;
        species << linestream;
        frame << linestream;
        timeInVid << linestream;
        */
        FishTypeEnum fType = getFishType(fishType);
        unique_ptr<Fish> tempFish(new Fish(fType,frame));
        tempFish->setFishSubType(getFishSpecies(fType,species));
        myFishList.push_back(*tempFish);

        linestream.str("");
        linestream.clear();
    }
    inFile.close();
    ui->totalFishVal->setText(QString::number(myFishList.size()));
    listPos = myFishList.begin();
    ui->typeMenu->setCurrentIndex((int) listPos->getFishType());
    ui->subTypeMenu->setCurrentIndex((int) listPos->getFishSubType());
    updateVecIndex();
}

void MainWindow::on_Play_clicked()
{
	if (myPlayer->isStopped())
	{
		myPlayer->Play();
		ui->Play->setText(tr("Stop"));
	}
	else
	{
		myPlayer->Stop();
		ui->Play->setText(tr("Play"));
	}
}

void MainWindow::on_videoSlider_sliderPressed()
{
	myPlayer->Stop();
}

void MainWindow::on_videoSlider_sliderReleased()
{
	myPlayer->Play();
    ui->Play->setText(tr("Stop"));
}

void MainWindow::on_videoSlider_sliderMoved(int position)
{
	myPlayer->setCurrentFrame(position);
	ui->currentTime->setText(getFormattedTime(position / (int)myPlayer->getFrameRate()));
}

void MainWindow::on_SpeedUp_clicked()
{
	myPlayer->speedUp();
}

void MainWindow::on_SlowDown_clicked()
{
	myPlayer->slowDown();
}

void MainWindow::on_minusOneFrame_clicked()
{
	if (!myPlayer->isStopped())
	{
		myPlayer->Stop();
		ui->Play->setText(tr("Play"));
	}
	myPlayer->minusOneFrame();
	QImage thisFrame = myPlayer->getOneFrame();
	if (!thisFrame.isNull())
	{
        imgPointer->setPixmap(QPixmap::fromImage(thisFrame));
		scene->setSceneRect(thisFrame.rect());
        	ui->videoWindow->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
		ui->videoSlider->setValue(myPlayer->getCurrentFrame());
		ui->currentTime->setText(getFormattedTime((int)myPlayer->
			getCurrentFrame() / (int)myPlayer->getFrameRate()));
	}
}

void MainWindow::on_plusOneFrame_clicked()
{
	if (!myPlayer->isStopped())
	{
		myPlayer->Stop();
		ui->Play->setText(tr("Play"));
	}
	QImage thisFrame = myPlayer->getOneFrame();
	if (!thisFrame.isNull())
	{
		imgPointer->setPixmap(QPixmap::fromImage(thisFrame));
		scene->setSceneRect(thisFrame.rect());
		ui->videoWindow->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
		ui->videoSlider->setValue(myPlayer->getCurrentFrame());
		ui->currentTime->setText(getFormattedTime((int)myPlayer->
			getCurrentFrame() / (int)myPlayer->getFrameRate()));
	}

}

void MainWindow::on_addRound_clicked()
{
	addFish((FishTypeEnum) ROUND);
}

void MainWindow::on_addFlat_clicked()
{
	addFish((FishTypeEnum) FLAT);
}

void MainWindow::on_addSkate_clicked()
{
	addFish((FishTypeEnum) SKATE);
}

void MainWindow::on_addOther_clicked()
{
	addFish((FishTypeEnum) OTHER);
}

void MainWindow::on_prevFish_clicked()
{
    if (listPos!=myFishList.begin())
    {
        listPos = listPos-1;
        ui->typeMenu->setCurrentIndex((int) listPos->getFishType());
        ui->subTypeMenu->setCurrentIndex((int) listPos->getFishSubType());
        updateVecIndex();
    }
}

void MainWindow::on_nextFish_clicked()
{
    if (listPos!=myFishList.end()-1)
    {
        listPos = listPos+1;
        ui->typeMenu->setCurrentIndex((int) listPos->getFishType());
        ui->subTypeMenu->setCurrentIndex((int) listPos->getFishSubType());
        updateVecIndex();
    }
}

void MainWindow::on_goToFrame_clicked()
{
    if (!myPlayer->isStopped())
    {
        myPlayer->Stop();
        ui->Play->setText(tr("Play"));
    }
    myPlayer->setCurrentFrame(listPos->getFrameCounted()-1);
    QImage thisFrame = myPlayer->getOneFrame();
    if (!thisFrame.isNull())
    {
        imgPointer->setPixmap(QPixmap::fromImage(thisFrame));
        scene->setSceneRect(thisFrame.rect());
        ui->videoWindow->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
        ui->videoSlider->setValue(myPlayer->getCurrentFrame());
        ui->currentTime->setText(getFormattedTime((int)myPlayer->
            getCurrentFrame() / (int)myPlayer->getFrameRate()));
    }
}

void MainWindow::on_updateFishFrame_clicked()
{
    int currentFrame = (int) myPlayer->getCurrentFrame();
    listPos->frameCounted = currentFrame;
    updateVecIndex();
}

void MainWindow::on_typeMenu_currentIndexChanged(int tIdx)
{
    if (!myFishList.empty())
        listPos->setFishType((FishTypeEnum) tIdx);
}

void MainWindow::on_subTypeMenu_currentIndexChanged(int sIdx)
{
    if (!myFishList.empty())
        listPos->setFishSubType(sIdx);
}

void MainWindow::addFish(FishTypeEnum fType)
{

	myPlayer->Stop();
	ui->Play->setText(tr("Play"));
	double currentFrame = myPlayer->getCurrentFrame();
    unique_ptr<Fish> tempFish(new Fish(fType,currentFrame));
    tempFish->setFishSubType(0);
	myFishList.push_back(*tempFish);
    std::sort(myFishList.begin(),myFishList.end());
    listPos = myFishList.end()-1;
    updateVecIndex();
    ui->totalFishVal->setText(QString::number(myFishList.size()));
    ui->typeMenu->setCurrentIndex((int) fType);
}
/*
void MainWindow::setTowType(bool towOpenStatus)
{
	//towOpenStatus = true corresponds to tow open
	fList->set_towopen(towOpenStatus);
}
*/
void MainWindow::updateVecIndex()
{
    ui->fishNumVal->setText(QString::number(listPos - myFishList.begin()+1));
    ui->frameCountedVal->setText(QString::number(listPos->getFrameCounted()));
}
/*
void MainWindow::convertFishToSerialize()
{
    fishSerialize::FishEntry* newFish;
    for(vector<Fish>::iterator it = myFishList.begin(); it != myFishList.end(); ++it)
    {
        newFish = fList->add_fish();
        newFish->set_ftype((fishSerialize::FishEntry_fTypeEnum)it->getFishType());
        newFish->set_fspecies(it->getFishSubType());
        newFish->set_fframe(it->frameCounted);
    }
}
*/

FishTypeEnum MainWindow::getFishType (string const& inString)
{
    if (inString == "ROUND") return ROUND;
    if (inString == "FLAT") return FLAT;
    if (inString == "SKATE") return SKATE;
    if (inString == "OTHER") return OTHER;
    return OTHER;
}

int MainWindow::getFishSpecies (FishTypeEnum fType, string const& sString)
{
    switch (fType)
    {
    case ROUND:
        if (sString == "Round") return 0;
        if (sString == "Haddock") return 1;
        if (sString == "Cod") return 2;
        if (sString == "Whiting") return 3;
        if (sString == "Red Hake") return 4;
        if (sString == "Pollock") return 5;
        if (sString == "Herring") return 6;
        if (sString == "Unknown") return 7;
        return 0;
        break;
    case FLAT:
        if (sString == "Flat") return 0;
        if (sString == "Yellowtail") return 1;
        if (sString == "Windowpane") return 2;
        if (sString == "Summer") return 3;
        if (sString == "FourSport") return 4;
        if (sString == "Grey Sole") return 5;
        if (sString == "Halibut") return 6;
        if (sString == "Unknown") return 7;
        return 0;
        break;
    case SKATE:
        if (sString == "Skate") return 0;
        if (sString == "Barndoor") return 1;
        if (sString == "Unknown") return 2;
        return 0;
        break;
    case OTHER:
        if (sString == "Other") return 0;
        if (sString == "Dogfish") return 1;
        if (sString == "Monkfish") return 2;
        if (sString == "Lobster") return 3;
        if (sString == "Scallop") return 4;
        return 0;
        break;
    default:
        return 0;
        break;
    }
}


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow* w = new MainWindow();
	w->setAttribute(Qt::WA_DeleteOnClose, true);

	w->show();

	return a.exec();
}
