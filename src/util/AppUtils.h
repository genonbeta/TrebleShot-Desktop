//
// Created by veli on 12/7/18.
//

#ifndef TREBLESHOT_APPUTILS_H
#define TREBLESHOT_APPUTILS_H

#include "src/config/Config.h"
#include "src/config/Keyword.h"
#include "src/database/AccessDatabase.h"
#include <QApplication>
#include <QtCore/QSettings>
#include <src/database/object/NetworkDevice.h>

class AppUtils {
public:
    static void applyAdapterName(DeviceConnection *connection);

    static void applyDeviceToJSON(QJsonObject &object);

    static AccessDatabase *getDatabase();

    static QSettings &getDefaultSettings();

    static QString getDeviceId();

    static NetworkDevice *getLocalDevice();

    static void loadInfo();
};


#endif //TREBLESHOT_APPUTILS_H