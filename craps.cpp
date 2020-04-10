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

    /*int rollVal;

    previousRoll = rollValue;
    */

    if (firstRoll == true) {
        switch (rollValue) {
            case 7:
            case 11:
                printf("You won!\n");
                currentBankValue += currentBetValue;
                winsCount += 1;
                break;
            case 2:
            case 3:
            case 12:
                printf("You lost.\n");
                currentBankValue -= currentBetValue;
                lossesCount += 1;
                break;
            case 4:
            case 5:
            case 6:
            case 8:
            case 9:
            case 10:
                printf("You get to roll again!\n");
                if (rollValue == 4, 5, 6, 8, 9, 10){
                    printf("You lost!\n");
                    currentBankValue -= currentBetValue;
                    lossesCount += 1;
                };
                break;
            default:
                currentBankValue -= currentBetValue;
                printf("You lose.\n");
                lossesCount += 1;
                break;
        }
    }

/*

    if (firstRoll == true && rollValue == 7 || firstRoll == true && rollValue == 11){
        printf("You won!\n");
        currentBankValue += currentBetValue;
        winsCount += 1;
    } else if (firstRoll == true && rollValue == 2 || firstRoll == true && rollValue == 3 || firstRoll == true && rollValue == 12){
        printf("You lost.\n");
        currentBankValue -= currentBetValue;
        lossesCount += 1;
    } else if(rollValue == 4 || rollValue == 5 || rollValue == 6 || rollValue == 8 || rollValue == 9 || rollValue == 10) {
        printf("You get to roll again!\n");
        previousRoll = rollValue;
    } else if (rollValue = previousRoll){
        currentBankValue += currentBetValue;
        printf("You win!\n");
        winsCount += 1;
    } else {
        currentBankValue -= currentBetValue;
        printf("You lose.\n");
        lossesCount += 1;
    }*/

    printStringRep();
    updateUI();
}
