#ifndef LIBCONSTANTS_H
#define LIBCONSTANTS_H

#include <QString>

namespace Config {
    const QString Host = "mysli.oamk.fi";
    const QString User = "c8ikja00";
    const QString Password = "8uL4khDtm2H6H7ag";
    const QString Database = "opisk_c8ikja00";
}

namespace DBError {
    const QString Deposit = "Could not deposit funds to account.";
    const QString Withdraw = "Could not withdraw funds from account.";
    const QString Connection = "Could not connect to database.";
    const QString Auth = "Login failed. Please give correct login credentials.";
    const QString Account = "Login required to access account's data.";
}

namespace DBSuccess {
    const QString Connection = "Succesfully connected to database.";
    const QString Auth = "Login successful for account with id %1.";
}


#endif // LIBCONSTANTS_H
