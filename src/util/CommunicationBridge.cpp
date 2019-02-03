//
// Created by veli on 9/29/18.
//

#include "CommunicationBridge.h"
#include "AppUtils.h"

CoolSocket::ActiveConnection *
CommunicationBridge::communicate(NetworkDevice *targetDevice, DeviceConnection *targetConnection)
{
    CoolSocket::ActiveConnection *connection
            = connectWithHandshake(targetConnection->ipAddress, false);

    communicate(connection, targetDevice);

    return connection;
}

CoolSocket::ActiveConnection *CommunicationBridge::communicate(
        CoolSocket::ActiveConnection *connection, NetworkDevice *device)
{
    updateDeviceIfOkay(connection, device);
    return connection;
}

CoolSocket::ActiveConnection *CommunicationBridge::connect(const QString &ipAddress)
{
    return Client::openConnection(this, ipAddress, PORT_COMMUNICATION_DEFAULT,
                                PORT_COMMUNICATION_DEFAULT);
}

CoolSocket::ActiveConnection *CommunicationBridge::connect(DeviceConnection *connection)
{
    return connect(connection->ipAddress);
}

CoolSocket::ActiveConnection *CommunicationBridge::connectWithHandshake(const QString &ipAddress,
                                                                        bool handshakeOnly)
{
    return handshake(connect(ipAddress), handshakeOnly);
}

NetworkDevice *CommunicationBridge::getDevice()
{
    return this->m_device;
}

CoolSocket::ActiveConnection *CommunicationBridge::handshake(
        CoolSocket::ActiveConnection *connection, bool handshakeOnly)
{
    try {
        QJsonObject replyJSON;

        replyJSON.insert(KEYWORD_HANDSHAKE_REQUIRED, true);
        replyJSON.insert(KEYWORD_HANDSHAKE_ONLY, handshakeOnly);
        replyJSON.insert(KEYWORD_DEVICE_INFO_SERIAL, getDeviceId());
        replyJSON.insert(KEYWORD_DEVICE_SECURE_KEY, m_device == nullptr
                                                    ? m_secureKey
                                                    : m_device->tmpSecureKey);

        connection->reply(QJsonDocument(replyJSON).toJson().toStdString().c_str());
    } catch (exception &e) {
        throw exception();
    }

    return connection;
}

NetworkDevice *CommunicationBridge::loadDevice(const QString &ipAddress)
{
    return loadDevice(connectWithHandshake(ipAddress, true));
}

NetworkDevice *CommunicationBridge::loadDevice(CoolSocket::ActiveConnection *connection)
{
    try {
        CoolSocket::Response *response = connection->receive();
        QJsonObject receivedJSON = QJsonDocument
        ::fromJson(QByteArray::fromStdString(response->response->toStdString())).object();

        return NetworkDeviceLoader::loadFrom(receivedJSON);
    } catch (exception &e) {
        throw exception();
    }
}

void CommunicationBridge::setDevice(NetworkDevice *device)
{
    this->m_device = device;
}

void CommunicationBridge::setSecureKey(int key)
{
    this->m_secureKey = key;
}

NetworkDevice *CommunicationBridge::updateDeviceIfOkay(
        CoolSocket::ActiveConnection *activeConnection, NetworkDevice *device)
{
    NetworkDevice *loadedDevice = loadDevice(activeConnection);
    DeviceConnection *connection
            = NetworkDeviceLoader::processConnection(loadedDevice, activeConnection
                    ->getSocket()
                    ->localAddress().toString());

    if (device->deviceId != loadedDevice->deviceId)
        throw exception();
    else {
        loadedDevice->lastUsageTime = clock();

        AppUtils::getDatabase()->publish(loadedDevice);
        setDevice(loadedDevice);
    }

    return loadedDevice;
}