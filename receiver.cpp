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

#include <QtWidgets>
#include <QtNetwork>

#include "receiver.h"
#include "receiver.h"

Receiver::Receiver(QWidget *parent)
    : QWidget(parent)
{
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    file = new QFile(date.toString("yyyyMMdd") + "-" + time.toString("HHmmss") + "_datafile.raw");
    file->open(QIODevice::Append);

    e31Label = new QLabel(tr("E31"));
    e31Label->setWordWrap(true);
    l31Label = new QLabel(tr("L31"));
    l31Label->setWordWrap(true);
    x30Label = new QLabel(tr("X30"));
    x30Label->setWordWrap(true);

    auto quitButton = new QPushButton(tr("&Quit"));

//! [0]
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(18246, QUdpSocket::ShareAddress);
//! [0]

//! [1]
    connect(udpSocket, SIGNAL(readyRead()),
            this, SLOT(processPendingDatagrams()));
//! [1]
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    auto buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(quitButton);
    buttonLayout->addStretch(1);

    auto mainLayout = new QVBoxLayout;
    auto e31FlagLayout = new QHBoxLayout;
    auto l31FlagLayout = new QHBoxLayout;
    auto x30FlagLayout = new QHBoxLayout;
    auto g31e31FlagLayout = new QHBoxLayout;
    auto g31l31FlagLayout = new QHBoxLayout;
    auto s30FlagLayout = new QHBoxLayout;

    mainLayout->addWidget(l31Label);
    mainLayout->addLayout(l31FlagLayout);
    mainLayout->addLayout(g31l31FlagLayout);
    mainLayout->addWidget(e31Label);
    mainLayout->addLayout(e31FlagLayout);
    mainLayout->addLayout(g31e31FlagLayout);
    mainLayout->addWidget(x30Label);
    mainLayout->addLayout(x30FlagLayout);
    mainLayout->addLayout(s30FlagLayout);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    for (int i = 0; i < 32; i++) {
        if ((i % 8) == 0) {
            QLabel *tmpl = new QLabel("  " + QString::number(i / 8) + ":");
            l31FlagLayout->addWidget(tmpl);
        }
        l31FlagLabel.push_back(new QLabel());
        l31FlagLayout->addWidget(l31FlagLabel[i]);
    }
    l31FlagLayout->addStretch();

    for (int i = 0; i < 48; i++) {
        if ((i % 8) == 0) {
            QLabel *tmpl;
            if (i < 16)
                tmpl = new QLabel("  " + QString::number(i / 8) + ":");
            else
                tmpl = new QLabel("  " + QString::number((i+6*8) / 8) + ":");
            e31FlagLayout->addWidget(tmpl);
        }
        e31FlagLabel.push_back(new QLabel());
        e31FlagLayout->addWidget(e31FlagLabel[i]);
    }
    e31FlagLayout->addStretch();

    for (int i = 0; i < 50; i++) {
        if ((i % 8) == 0) {
            QLabel *tmpl;
            if (i < 16)
                tmpl = new QLabel("  " + QString::number(i / 8) + ":");
            else if (i < 48)
                tmpl = new QLabel("  " + QString::number((i+6*8) / 8) + ":");
            else
                tmpl = new QLabel("  " + QString::number((i+66*8) / 8) + ":");
            x30FlagLayout->addWidget(tmpl);
        }
        x30FlagLabel.push_back(new QLabel());
        x30FlagLayout->addWidget(x30FlagLabel[i]);
    }
    x30FlagLayout->addStretch();

    for (int i = 0; i < 15; i++) {
        if (i == 12 || i == 14) {
            g31e31FlagLayout->addWidget(new QLabel(" | "));
        }
        g31e31FlagLabel.push_back(new QLabel("_"));
        g31e31FlagLayout->addWidget(g31e31FlagLabel[i]);
    }
    g31e31FlagLayout->addStretch();

    for (int i = 0; i < 6; i++) {
        if (i == 5) {
            g31l31FlagLayout->addWidget(new QLabel(" | "));
        }
        g31l31FlagLabel.push_back(new QLabel("_"));
        g31l31FlagLayout->addWidget(g31l31FlagLabel[i]);
    }
    g31l31FlagLayout->addStretch();

    for (int i = 0; i < 14; i++) {
        if (i == 8 || i == 13) {
            s30FlagLayout->addWidget(new QLabel(" | "));
        }
        s30FlagLabel.push_back(new QLabel("_"));
        s30FlagLayout->addWidget(s30FlagLabel[i]);
    }
    s30FlagLayout->addStretch();

    g31Cmd.e31.ex2.resize(12);
    g31Cmd.e31.ex5.resize(2);
    g31Cmd.l31.ex2.resize(5);
    s30Cmd.x30.ex2.resize(8);
    s30Cmd.x30.ex7.resize(5);

    setWindowTitle(tr("GE Viewer v1.2"));
    setFixedWidth(1240);
}

Receiver::~Receiver()
{
    file->close();
}

int Receiver::writeDatagramToFile(QByteArray datagram, QHostAddress ip, quint16 port)
{
    QByteArray header;
    qint64 time = QDateTime::currentMSecsSinceEpoch();
    quint32 addr = ip.toIPv4Address();
    int size = datagram.size();

    header.append(QByteArray::fromRawData((char *)&time, sizeof(time)));
    header.append(QByteArray::fromRawData((char *)&addr, sizeof(addr)));
    header.append(QByteArray::fromRawData((char *)&port, sizeof(port)));
    header.append(QByteArray::fromRawData((char *)&size, sizeof(size)));

    file->write(header);
    file->write(datagram);

    return 0;
}

void Receiver::processPendingDatagrams()
{
    QByteArray datagram;
    QHostAddress ip;
    quint16 port;
    QHostAddress ipG31 = QHostAddress("192.168.101.111");
    QHostAddress ipL31 = QHostAddress("192.168.101.110");
    QHostAddress ipE31M1 = QHostAddress("192.168.101.116");
    QHostAddress ipE31M2 = QHostAddress("192.168.101.117");
    QHostAddress ipS30 = QHostAddress("192.168.101.211");
    QHostAddress ipX30M1 = QHostAddress("192.168.101.216");
    QHostAddress ipX30M2 = QHostAddress("192.168.101.217");
//! [2]
    while (udpSocket->hasPendingDatagrams()) {
            datagram.resize(int(udpSocket->pendingDatagramSize()));
            udpSocket->readDatagram(datagram.data(), datagram.size(), &ip, &port);

            const char * tmp = datagram.constData();
            QByteArray datarecv = QByteArray(tmp+32, datagram.size() - 32);

            if (ip.toIPv4Address() == ipL31.toIPv4Address()) {
                writeDatagramToFile(datagram, ip, port);

                if (datagram.at(8) == 0x01) { // ExtchangeId == 1
                    reciveL31(datarecv);
                }
            }

            if (ip.toIPv4Address() == ipE31M1.toIPv4Address() ||
                ip.toIPv4Address() == ipE31M2.toIPv4Address() ) {

                writeDatagramToFile(datagram, ip, port);

                if (datagram.at(8) == 0x01) { // ExtchangeId == 1
                    reciveE31(datarecv);
                }
            }

            if (ip.toIPv4Address() == ipX30M1.toIPv4Address() ||
                ip.toIPv4Address() == ipX30M2.toIPv4Address() ) {

                writeDatagramToFile(datagram, ip, port);

                if (datagram.at(8) == 0x01) { // ExtchangeId == 1
                    reciveX30(datarecv);
                }
            }

            if (ip.toIPv4Address() == ipG31.toIPv4Address()) {
                writeDatagramToFile(datagram, ip, port);
                int exid = datagram.at(8);
                if (exid == 0x02 || exid == 0x03 || exid == 0x05)
                    reciveG31(datarecv, exid);
            }

            if (ip.toIPv4Address() == ipS30.toIPv4Address()) {
                writeDatagramToFile(datagram, ip, port);
                int exid = datagram.at(8);
                if (exid == 0x02)
                    reciveS30(datarecv, exid);
            }
    }

//! [2]
}

void Receiver::reciveG31(QByteArray egd, int exid)
{
    int num;
    if (exid == 0x02) {
        num = 0;
        g31Cmd.e31.ex2[num++] = egd.at(17) & (1 << 1);
        g31Cmd.e31.ex2[num++] = egd.at(27) & (1 << 6);
        g31Cmd.e31.ex2[num++] = egd.at(60) & (1 << 3);
        g31Cmd.e31.ex2[num++] = egd.at(106) & (1 << 5);
        g31Cmd.e31.ex2[num++] = egd.at(118) & (1 << 3);
        g31Cmd.e31.ex2[num++] = egd.at(144) & (1 << 0);
        g31Cmd.e31.ex2[num++] = egd.at(149) & (1 << 2);
        g31Cmd.e31.ex2[num++] = egd.at(172) & (1 << 4);
        g31Cmd.e31.ex2[num++] = egd.at(173) & (1 << 7);
        g31Cmd.e31.ex2[num++] = egd.at(176) & (1 << 3);
        g31Cmd.e31.ex2[num++] = egd.at(218) & (1 << 3);
        g31Cmd.e31.ex2[num++] = egd.at(227) & (1 << 2);

        num = 0;
        g31Cmd.l31.ex2[num++] = egd.at(13) & (1 << 0);
        g31Cmd.l31.ex2[num++] = egd.at(107) & (1 << 3);
        g31Cmd.l31.ex2[num++] = egd.at(140) & (1 << 7);
        g31Cmd.l31.ex2[num++] = egd.at(157) & (1 << 1);
        g31Cmd.l31.ex2[num++] = egd.at(217) & (1 << 3);
    } else if (exid == 0x03) {
        float *tmp = (float *)(egd.constData() + 136);
        g31Cmd.e31.ex3 = *tmp;

        tmp = (float *)(egd.constData() + 1316);
        g31Cmd.l31.ex3 = *tmp;
    } else if (exid == 0x05) {
        num = 0;
        g31Cmd.e31.ex5[num++] = egd.at(0) & (1 << 0);
        g31Cmd.e31.ex5[num++] = egd.at(0) & (1 << 1);
    }
}

void Receiver::reciveS30(QByteArray egd, int exid)
{
    int num;
    if (exid == 0x02) {
        num = 0;
        s30Cmd.x30.ex2[num++] = egd.at(22) & (1 << 4);
        s30Cmd.x30.ex2[num++] = egd.at(26) & (1 << 7);
        s30Cmd.x30.ex2[num++] = egd.at(46) & (1 << 2);
        s30Cmd.x30.ex2[num++] = egd.at(70) & (1 << 2);
        s30Cmd.x30.ex2[num++] = egd.at(78) & (1 << 5);
        s30Cmd.x30.ex2[num++] = egd.at(90) & (1 << 2);
        s30Cmd.x30.ex2[num++] = egd.at(92) & (1 << 5);
        s30Cmd.x30.ex2[num++] = egd.at(97) & (1 << 5);

        float *tmp = (float *)(egd.constData() + 1104);
        s30Cmd.x30.ex2DtggcMax = *tmp;

    } else if (exid == 0x07) {
        num = 0;
        s30Cmd.x30.ex7[num++] = egd.at(179) & (1 << 4);
        s30Cmd.x30.ex7[num++] = egd.at(179) & (1 << 5);
        s30Cmd.x30.ex7[num++] = egd.at(179) & (1 << 6);
        s30Cmd.x30.ex7[num++] = egd.at(179) & (1 << 7);
        s30Cmd.x30.ex7[num++] = egd.at(180) & (1 << 0);
    }
}

static QString getRegulator(QByteArray egd)
{
    QString mode = "";
    if ( egd.at(0) & (1 << 0)) {
        mode += " Auto M";
    } else {
        mode += " Man M";
    }

    if ( egd.at(0) & (1 << 5)) {
        mode += "1";
    } else if ( egd.at(0) & (1 << 6))  {
        mode += "2";
    } else {
        mode += " ";
    }
    return mode;
}

void Receiver::reciveE31(QByteArray egd)
{
    static unsigned char count;
    if (count != 3) {
        count += 1;
        return;
    } else {
        count = 0;
    }

    QString mode = " E31:" + getRegulator(egd);

    bool k52closed = egd.at(1) & (1 << 7);
    bool k41closed = egd.at(1) & (1 << 0);
    bool exiterStart = egd.at(10) & (1 << 2);
    bool exiterStop = egd.at(10) & (1 << 4);

    float *If = (float *)(egd.constData() + 24);
    float *Uf = (float *)(egd.constData() + 36);
    float *Ug = (float *)(egd.constData() + 40);
    float *Ig = (float *)(egd.constData() + 12);
    float *Hz = (float *)(egd.constData() + 16);
    float *Pg = (float *)(egd.constData() + 44);
    float *Qg = (float *)(egd.constData() + 32);
    float *PF = (float *)(egd.constData() + 28);
    float *XFR = (float *)(egd.constData() + 48);

    mode += " [ If = " + QString::number(*If);
    mode += " ][ Uf = " + QString::number(*Uf);
    mode += " ][ Ug = " + QString::number(*Ug * 1000);
    mode += " ][ Hz = " + QString::number(*Hz);
    if (k52closed) {
        mode += " ][ Ig = " + QString::number(*Ig * 1000);
        mode += " ][ P = " + QString::number(*Pg);
        mode += " ][ Q = " + QString::number(*Qg);
        mode += " ][ PF = " + QString::number(*PF);
    }
    mode += " ][ XFR = " + QString::number(*XFR) + " ]";

    e31Label->setText(mode);
    if (k41closed && exiterStart)
        e31Label->setStyleSheet("QLabel {background-color: #ff8787}");
    if (k41closed && exiterStop)
        e31Label->setStyleSheet("QLabel {background-color: #FFFF00}");
    if (! k41closed && exiterStop)
        e31Label->setStyleSheet("QLabel {background-color: #00FF00}");

    int numberLabel = 0;
    for (int byte = 0; byte < 12; byte++) {
        for (int bit = 0; bit < 8; bit++) {
            if (egd.at(byte) & (1 << bit))
                e31FlagLabel[numberLabel]->setStyleSheet("QLabel {background-color: #ff8787}");
            else
                e31FlagLabel[numberLabel]->setStyleSheet("QLabel {background-color: #00ff00}");
            e31FlagLabel[numberLabel]->setText(" " + QString::number(bit) + " ");
            numberLabel++;
        }

        if (byte == 1)
            byte = 7; // next will be 8
    }

    numberLabel = 0;
    for (bool b: g31Cmd.e31.ex2) {
        if (b == true)
            g31e31FlagLabel[numberLabel]->setStyleSheet("QLabel {background-color: #ff8787}");
        else
            g31e31FlagLabel[numberLabel]->setStyleSheet("QLabel {background-color: #00ff00}");
        numberLabel++;
    }
    for (bool b: g31Cmd.e31.ex5) {
        if (b == true)
            g31e31FlagLabel[numberLabel]->setStyleSheet("QLabel {background-color: #ff8787}");
        else
            g31e31FlagLabel[numberLabel]->setStyleSheet("QLabel {background-color: #00ff00}");
        numberLabel++;
    }
    g31e31FlagLabel[numberLabel]->setText(" DTGGC_MAX: " + QString::number(g31Cmd.e31.ex3));
}

void Receiver::reciveL31(QByteArray egd)
{
    static unsigned char count;
    if (count != 20) {
        count += 1;
        return;
    } else {
        count = 0;
    }

    QString mode = " L31:";

    float *SPDSP = (float *)(egd.constData() + 20);
    float *SPDFB = (float *)(egd.constData() + 24);
    float *ISP = (float *)(egd.constData() + 28);
    float *IFB = (float *)(egd.constData() + 32);
    float *LF = (float *)(egd.constData() + 36);
    float *SACMD = (float *)(egd.constData() + 40);
    float *LACMD = (float *)(egd.constData() + 44);
    float *FACMD = (float *)(egd.constData() + 48);
    float *LV = (float *)(egd.constData() + 56);
    float *SV = (float *)(egd.constData() + 60);
    float *EXREF = (float *)(egd.constData() + 64);

    mode += " [ SPDSP = " + QString::number(*SPDSP);
    mode += " ][ SPDFB = " + QString::number(*SPDFB);
    mode += " ][ ISP = " + QString::number(*ISP);
    mode += " ][ IFB = " + QString::number(*IFB);
    mode += " ][ LF = " + QString::number(*LF);
    mode += " ][ SACMD = " + QString::number(*SACMD);
    mode += " ][ LACMD = " + QString::number(*LACMD);
    mode += " ][ FACMD = " + QString::number(*FACMD);
    mode += " ][ LV = " + QString::number(*LV);
    mode += " ][ SV = " + QString::number(*SV);
    mode += " ][ EXREF = " + QString::number(*EXREF) + " ]";

    bool run = egd.at(0) & (1 << 2);

    l31Label->setText(mode);
    if (run)
        l31Label->setStyleSheet("QLabel {background-color: #ff8787}");
    if (! run)
        l31Label->setStyleSheet("QLabel {background-color: #00FF00}");

    int numberLabel = 0;
    for (int byte = 0; byte <4; byte++) {
        for (int bit = 0; bit < 8; bit++) {
            if (egd.at(byte) & (1 << bit))
                l31FlagLabel[numberLabel]->setStyleSheet("QLabel {background-color: #ff8787}");
            else
                l31FlagLabel[numberLabel]->setStyleSheet("QLabel {background-color: #00ff00}");
            l31FlagLabel[numberLabel]->setText(" " + QString::number(bit) + " ");
            numberLabel++;
        }
    }

    numberLabel = 0;
    for (bool b: g31Cmd.l31.ex2) {
        if (b == true)
            g31l31FlagLabel[numberLabel]->setStyleSheet("QLabel {background-color: #ff8787}");
        else
            g31l31FlagLabel[numberLabel]->setStyleSheet("QLabel {background-color: #00ff00}");
        numberLabel++;
    }
    g31l31FlagLabel[numberLabel]->setText(" SS_REF_OUT: " + QString::number(g31Cmd.l31.ex3));
}

void Receiver::reciveX30(QByteArray egd)
{
    static unsigned char count;
    if (count != 3) {
        count += 1;
        return;
    } else {
        count = 0;
    }

    QString mode = " X30:" + getRegulator(egd);

    bool k52closed = egd.at(1) & (1 << 7);
    bool k41closed = egd.at(1) & (1 << 0);
    bool exiterStart = egd.at(9) & (1 << 6);
    bool exiterStop = egd.at(10) & (1 << 0);

    float *Ig = (float *)(egd.constData() + 12);
    float *Hz = (float *)(egd.constData() + 16);

    float *PF = (float *)(egd.constData() + 24);
    float *Qg = (float *)(egd.constData() + 28);
    float *Uf = (float *)(egd.constData() + 32);
    float *Ug = (float *)(egd.constData() + 36);
    float *Pg = (float *)(egd.constData() + 40);

    float *XFR = (float *)(egd.constData() + 52);
    float *REGEXCURR = (float *)(egd.constData() + 56);
    float *REGEXVOLT = (float *)(egd.constData() + 60);

    float *GND = (float *)(egd.constData() + 76);
    float *If = (float *)(egd.constData() + 80);

    mode += " [ If = " + QString::number(*If);
    mode += " ][ Uf = " + QString::number(*Uf);
    mode += " ][ Ug = " + QString::number(*Ug * 1000);
    mode += " ][ Hz = " + QString::number(*Hz);
    if (k52closed) {
        mode += " ][ Ig = " + QString::number(*Ig * 1000);
        mode += " ][ P = " + QString::number(*Pg);
        mode += " ][ Q = " + QString::number(*Qg);
        mode += " ][ PF = " + QString::number(*PF);
    }
    mode += " ][ XFR = " + QString::number(*XFR);
    mode += " ][ GND = " + QString::number(*GND);
    mode += " ][ REGEXCURR = " + QString::number(*REGEXCURR);
    mode += " ][ REGEXVOLT = " + QString::number(*REGEXVOLT) + " ]";

    x30Label->setText(mode);

    x30Label->setText(mode);
    if (k41closed && exiterStart)
        x30Label->setStyleSheet("QLabel {background-color: #ff8787}");
    if (k41closed && exiterStop)
        x30Label->setStyleSheet("QLabel {background-color: #FFFF00}");
    if (! k41closed && exiterStop)
        x30Label->setStyleSheet("QLabel {background-color: #00FF00}");

    int numberLabel = 0;
    for (int byte = 0; byte < 73; byte++) {
        for (int bit = 0; bit < 8; bit++) {
            if (egd.at(byte) & (1 << bit))
                x30FlagLabel[numberLabel]->setStyleSheet("QLabel {background-color: #ff8787}");
            else
                x30FlagLabel[numberLabel]->setStyleSheet("QLabel {background-color: #00ff00}");
            x30FlagLabel[numberLabel]->setText(" " + QString::number(bit) + " ");
            numberLabel++;

            if (byte == 72 && bit == 1)
                break;
        }

        if (byte == 1)
            byte = 7; // next will be 8
        if (byte == 11)
            byte = 71; // next will be 72
    }

    numberLabel = 0;
    for (bool b: s30Cmd.x30.ex2) {
        if (b == true)
            s30FlagLabel[numberLabel]->setStyleSheet("QLabel {background-color: #ff8787}");
        else
            s30FlagLabel[numberLabel]->setStyleSheet("QLabel {background-color: #00ff00}");
        numberLabel++;
    }
    for (bool b: s30Cmd.x30.ex7) {
        if (b == true)
            s30FlagLabel[numberLabel]->setStyleSheet("QLabel {background-color: #ff8787}");
        else
            s30FlagLabel[numberLabel]->setStyleSheet("QLabel {background-color: #00ff00}");
        numberLabel++;
    }
    s30FlagLabel[numberLabel]->setText(" DTGGC_MAX: " + QString::number(s30Cmd.x30.ex2DtggcMax));
}
