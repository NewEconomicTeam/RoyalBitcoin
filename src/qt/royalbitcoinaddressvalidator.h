// Copyright (c) 2011-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ROYALBITCOIN_QT_ROYALBITCOINADDRESSVALIDATOR_H
#define ROYALBITCOIN_QT_ROYALBITCOINADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class RoyalbitcoinAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit RoyalbitcoinAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Royalbitcoin address widget validator, checks for a valid royalbitcoin address.
 */
class RoyalbitcoinAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit RoyalbitcoinAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // ROYALBITCOIN_QT_ROYALBITCOINADDRESSVALIDATOR_H
