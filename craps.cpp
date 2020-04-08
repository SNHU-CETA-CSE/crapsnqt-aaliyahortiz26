#include <iostream>
#include <cstdio>
//#include <QApplication>
//#include <QWidget>
//#include <QGridLayout>
//#include <QPushButton>
//#include <QLabel>
//#include <QPixmap>

#include "die.h"
#include "craps.h"
#include "ui_CrapsMainWindow.h"


CrapsMainWindow :: CrapsMainWindow(QMainWindow *parent):
// Build a GUI  main window for two dice.

        firstRoll{ true },
        currentBankValue { 10000 },
        winsCount { 0 },
        lossesCount { 0 },
        rollValue { 0 },
        currentBetValue { 100 }

{
    setupUi(this);

    QObject::connect(rollButton, SIGNAL(clicked()), this, SLOT(rollButtonClickedHandler()));
}
void CrapsMainWindow::printStringRep() {
    // String representation for Craps.
    char buffer[25];
    int length =  sprintf(buffer, "Die1: %i\nDie2: %i\n", die1.getValue(), die2.getValue());
    printf("%s", buffer);
}
void CrapsMainWindow::updateUI() {
//    printf("Inside updateUI()\n");
    char outputString[12];
    std::string die1ImageName = ":/dieImages/" + std::to_string(die1.getValue());
    std::string die2ImageName = ":/dieImages/" + std::to_string(die2.getValue());
    die1UI->setPixmap(QPixmap(QString::fromStdString(die1ImageName)));
    die2UI->setPixmap(QPixmap(QString::fromStdString(die2ImageName)));

    snprintf(outputString, sizeof(outputString), "%.2f", currentBankValue);
    currentBankValueUI->setText(QString::fromStdString(outputString));

    snprintf(outputString, sizeof(outputString), "%.0f", winsCount);
    winsValueUI->setText(QString::fromStdString(outputString));

    snprintf(outputString, sizeof(outputString), "%.0f", lossesCount);
    lossesValueUI->setText(QString::fromStdString(outputString));

    snprintf(outputString, sizeof(outputString), "%.0f", currentBetValue);
    currentBetUI->setText(QString::fromStdString(outputString));

}

void CrapsMainWindow::rollButtonClickedHandler() {
    printf("Roll button clicked\n");
    rollValue = die1.roll() + die2.roll();
    if (firstRoll){
        printf("PLaying first roll\n");
        firstRoll = false;
    } else {
        printf("Playing second roll\n");
        firstRoll = true;
    }
    printStringRep();
    currentBankValue -= 100;
    winsCount += 1;
    lossesCount += 1;
    updateUI();
}
