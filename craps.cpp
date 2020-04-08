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
#include <sstream>

void displayResults(float currentBank, float currentBet);

CrapsMainWindow :: CrapsMainWindow(QMainWindow *parent):
// Build a GUI  main window for two dice.

        firstRoll{ true },
        winsCount { 0 },
        lossesCount { 0 },
        currentBankValue { 10000 },
        currentBet { 10 },
        previousRoll { 0 },
        statusMessage { "" },
        payouts {0.0, 0.0, 1.0, 1.0, 2.0, 1.5, 1.2, 1.0, 1.2, 1.5, 2.0, 1.0, 1.0}
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
    if (firstRoll) {
        rollingForUI->setText(QString::fromStdString("")); //figure what is wrong with this
    }
    else {
        snprintf(outputString, sizeof(outputString), "%i", previousRoll);
        rollingForUI->setText(QString::fromStdString(outputString));
    }
    snprintf(outputString, sizeof(outputString), "%i", winsCount);
    winsCountUI->setText(QString::fromStdString(outputString ));

    snprintf(outputString, sizeof(outputString), "%i", lossesCount);
    lossesCountUI->setText(QString::fromStdString(outputString ));

    snprintf(outputString, sizeof(outputString), "%.2f", currentBankValue);
    currentBankValueUI->setText(QString::fromStdString(outputString ));
    statusUI->setText(QString::fromStdString(statusMessage));
}

int CrapsMainWindow::processBet(float currentBank) {
    int attemptedBet;

    attemptedBet = currentBetUI->value();
    if (float(attemptedBet) <= currentBank) {
        return attemptedBet;
    } else {
        statusMessage = "You don't have that much money!";
    }
}

std::tuple<bool, float>  CrapsMainWindow::playFirstRoll(int rollValue, float currentBank, float currentBet){
    std::cout << rollValue << "\n";
    switch (rollValue) {
        case 7:
        case 11: {
            currentBank = processWin(rollValue, 1, currentBank, currentBet);
            return std::make_tuple(true, currentBank);
        }
        case 2:
        case 3:
        case 12: {
            currentBank = processLoss(rollValue, 1, currentBank, currentBet);
            return std::make_tuple(true, currentBank);
        }
        default: {
            return std::make_tuple(false, currentBank);
        }
    }
};

std::tuple<bool, float>  CrapsMainWindow::playSecondRoll(int rollValue, int previousRoll,  float currentBank, float currentBet){
    if (rollValue == previousRoll) {
        return std::make_tuple(true, processWin(rollValue, 2, currentBank, currentBet));
    } else {
        return std::make_tuple(true, processLoss(rollValue, 2, currentBank, currentBet));
    }
};

float CrapsMainWindow::processWin(int rollValue, int rollNumber, float currentBank, float currentBet) {
    std::cout << "You won!" << "\n";
    winsCount++;
    statusMessage = "You won!";
    return calculateCurrentBank(rollValue, rollNumber, currentBank, currentBet, true);
}

float CrapsMainWindow::processLoss(int rollValue, int rollNumber, float currentBank, float currentBet) {
    std::cout << "You lost." << "\n";
    lossesCount++;
    statusMessage = "You lost.";
    return calculateCurrentBank(rollValue, rollNumber, currentBank, currentBet, false);
};

float CrapsMainWindow::calculateCurrentBank(int rollValue, int rollNumber, float currentBank, float currentBet, bool wonBet) {
    if (rollNumber == 1) {
        if(wonBet)
            return currentBank + currentBet;
        else
            return currentBank - currentBet;
    }
    else {
        if(wonBet)
            return currentBank + currentBet * payouts[rollValue];
        else
            return currentBank - currentBet * payouts[rollValue];
    }
};



// Player asked for another roll of the dice.
void CrapsMainWindow::rollButtonClickedHandler() {
    bool rollCompleted = false;
    float localBank = currentBankValue;
    int rollValue;

//    printf("Roll button clicked\n");
    rollValue = die1.roll() + die2.roll();
    statusMessage = "";
    if(firstRoll) {
        currentBet = processBet(currentBankValue);
        std::tie(rollCompleted, localBank) = playFirstRoll(rollValue, currentBankValue, currentBet);
        currentBankValue = localBank;
        if (rollCompleted) {
            firstRoll = true;
        } else {
            statusMessage = "Roll again! Good Luck!";
            previousRoll = rollValue;
            firstRoll = false;
        }
    } else {
        std::tie(rollCompleted, localBank) = playSecondRoll(rollValue, previousRoll, currentBankValue, currentBet);
        currentBankValue = localBank;
        firstRoll = true;
    }
    printStringRep();
    updateUI();
}
