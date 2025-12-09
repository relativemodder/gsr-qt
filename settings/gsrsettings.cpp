#include "gsrsettings.h"
#include <QDir>
#include <QFileInfo>
#include <QDebug>

GSRSettings::GSRSettings(QObject* parent)
    : QObject(parent),
      m_settings(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) +
                 QLatin1String("/gsr-qt/settings.ini"),
                 QSettings::IniFormat)
{
    qDebug() << "Settings file path:" << m_settings.fileName();
}

QString GSRSettings::calculateDefaultOutputDir() const {
    QString videosDir = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);

    if (videosDir.isEmpty()) {
        videosDir = QDir::homePath() + "/Videos";
    }

    return QDir(videosDir).filePath("Screencasts");
}

QString GSRSettings::getDefaultOutputDirPath() const {
    return calculateDefaultOutputDir();
}

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
    
    QString savedValue = m_settings.value(OUTPUT_DIR_KEY).toString();

    if (savedValue != path) {
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