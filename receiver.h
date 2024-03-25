/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef RECEIVER_H
#define RECEIVER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
class QUdpSocket;
class QFile;
class QHostAddress;
QT_END_NAMESPACE

struct G31Command
{
    struct {
        QVector<bool> ex2;
        float ex3;
        QVector<bool> ex5;
    } e31;

    struct {
        QVector<bool> ex2;
        float ex3;
    } l31;
};

struct S30Command
{
    struct {
        QVector<bool> ex2;
        float ex2DtggcMax;
        QVector<bool> ex7;
    } x30;
};

class Receiver : public QWidget
{
    Q_OBJECT

public:
    explicit Receiver(QWidget *parent = nullptr);
    ~Receiver();

private slots:
    void processPendingDatagrams();

private:
    QFile *file = nullptr;
    QLabel *l31Label = nullptr;
    QLabel *e31Label = nullptr;
    QLabel *x30Label = nullptr;
    QVector<QLabel *> e31FlagLabel;
    QVector<QLabel *> l31FlagLabel;
    QVector<QLabel *> x30FlagLabel;
    QVector<QLabel *> g31e31FlagLabel;
    QVector<QLabel *> g31l31FlagLabel;
    QVector<QLabel *> s30FlagLabel;
    QUdpSocket *udpSocket = nullptr;
    G31Command g31Cmd;
    S30Command s30Cmd;

    void reciveL31(QByteArray egd);
    void reciveE31(QByteArray egd);
    void reciveX30(QByteArray egd);
    int writeDatagramToFile(QByteArray datagram, QHostAddress ip, quint16 port);
    void reciveG31(QByteArray egd);
    void reciveG31(QByteArray egd, int exid);
    void reciveS30(QByteArray egd, int exid);
};



#endif
