/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtPositioning module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qgeopositioninfosourcefactory_serialnmea.h"
#include <QtPositioning/qnmeapositioninfosource.h>
#include <QtSerialPort/qserialport.h>
#include <QtSerialPort/qserialportinfo.h>
#include <QtCore/qloggingcategory.h>

Q_LOGGING_CATEGORY(lcSerial, "qt.positioning.serialnmea")

class NmeaSource : public QNmeaPositionInfoSource
{
public:
    NmeaSource(QObject *parent);
    bool isValid() const { return !m_port.isNull(); }

private:
    QScopedPointer<QSerialPort> m_port;
};

NmeaSource::NmeaSource(QObject *parent)
    : QNmeaPositionInfoSource(RealTimeMode, parent),
      m_port(new QSerialPort)
{
    QByteArray requestedPort = qgetenv("QT_NMEA_SERIAL_PORT");
    if (requestedPort.isEmpty()) {
        const QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
        qCDebug(lcSerial) << "Found" << ports.count() << "serial ports";
        if (ports.isEmpty()) {
            qWarning("serialnmea: No serial ports found");
            m_port.reset();
            return;
        }

        // Try to find a well-known device.
        QString portName;
        for (int i = 0; i < ports.count(); ++i) {
            const QString candidatePortName = ports[i].portName();
            bool acceptThis = false;

            // GlobalSat (BU-353S4 and probably others)
            acceptThis |= ports[i].hasVendorIdentifier() && ports[i].vendorIdentifier() == 0x67b;

            if (acceptThis) {
                portName = candidatePortName;
                break;
            }
        }

        if (portName.isEmpty()) {
            qWarning("serialnmea: No known GPS device found. Specify the COM port via QT_NMEA_SERIAL_PORT.");
            m_port.reset();
            return;
        }

        m_port->setPortName(portName);
    } else {
        m_port->setPortName(QString::fromUtf8(requestedPort));
    }

    m_port->setBaudRate(4800);

    qCDebug(lcSerial) << "Opening serial port" << m_port->portName();

    if (!m_port->open(QIODevice::ReadOnly)) {
        qWarning("serialnmea: Failed to open %s", qPrintable(m_port->portName()));
        m_port.reset();
        return;
    }

    setDevice(m_port.data());

    qCDebug(lcSerial) << "Opened successfully";
}

QGeoPositionInfoSource *QGeoPositionInfoSourceFactorySerialNmea::positionInfoSource(QObject *parent)
{
    QScopedPointer<NmeaSource> src(new NmeaSource(parent));
    return src->isValid() ? src.take() : Q_NULLPTR;
}

QGeoSatelliteInfoSource *QGeoPositionInfoSourceFactorySerialNmea::satelliteInfoSource(QObject *parent)
{
    Q_UNUSED(parent);
    return Q_NULLPTR;
}

QGeoAreaMonitorSource *QGeoPositionInfoSourceFactorySerialNmea::areaMonitor(QObject *parent)
{
    Q_UNUSED(parent);
    return Q_NULLPTR;
}
