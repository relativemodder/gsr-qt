#include "gsrsettings.h"
#include <QDir>
#include <QFileInfo>
#include <QDebug>


QString GSRSettings::getOutputDir() const {
    QString configuredPath = m_settings.value(OUTPUT_DIR_KEY).toString();

    if (!configuredPath.isEmpty() && QFileInfo::exists(configuredPath)) {
        return configuredPath;
    }

    QString defaultPath = calculateDefaultOutputDir();
    
    QDir dir;
    if (!dir.mkpath(defaultPath)) {
        qWarning() << "Could not create default output directory:" << defaultPath;
    }

    return defaultPath;
}

void GSRSettings::setOutputDir(const QString& path) {
    QString currentPath = getOutputDir();
    
    if (currentPath != path) {
        m_settings.setValue(OUTPUT_DIR_KEY, path);
        m_settings.sync();
        qDebug() << "Output directory set to:" << path;
        emit outputDirectoryChanged();
    }
}

void GSRSettings::resetOutputDir() {
    if (m_settings.contains(OUTPUT_DIR_KEY)) {
        m_settings.remove(OUTPUT_DIR_KEY);
        m_settings.sync();
        qDebug() << "Output directory setting removed. Default will be used.";
        emit outputDirectoryChanged();
    }
}