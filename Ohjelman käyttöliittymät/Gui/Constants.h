#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

namespace Default {
    const QString Port = "com3";
    const QString Card = "0A0079C7BF";
    const int NumRecentEvents = 5;
}

namespace Logger {
    const QString DB = "DatabaseDLL";
    const QString Rfid = "RfidDLL";
}

namespace BSError {
    const QString Withdrawal = "Account doesn't have enough funds for the withdrawal.";
    const QString PinTimeout = "Please input your pin code within 10 seconds.";
    const QString Deposit = "Could not add money to the account, please take it from the dispenser";
}

namespace BSMessage {
    const QString Withdrawal = "Succesfully withdrew money from the account. Remember to take the bills from the dispenser.";
    const QString Logout = "Thank you for using BankSimul. See you soon!";
    const QString Deposit = "Money was succesfully added to the account.";
}

#endif // CONSTANTS_H
