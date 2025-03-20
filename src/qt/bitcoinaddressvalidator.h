// Copyright (c) 2011-2020 The Bitcoin 3.0 Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_BITCOINADDRESSVALIDATOR_H
#define BITCOIN_QT_BITCOINADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class Bitcoin 3.0AddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit Bitcoin 3.0AddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

/** Bitcoin 3.0 address widget validator, checks for a valid bitcoin address.
 */
class Bitcoin 3.0AddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit Bitcoin 3.0AddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

#endif // BITCOIN_QT_BITCOINADDRESSVALIDATOR_H
