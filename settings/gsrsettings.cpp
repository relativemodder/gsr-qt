#include "gsrsettings.h"
#include <QDebug>

GSRSettings::GSRSettings(QObject* parent)
    : QObject(parent)
    , m_settings(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) +
                 "/gsr-qt/settings.ini", QSettings::IniFormat)
{
    qDebug() << "Settings file path:" << m_settings.fileName();
    initializeDefaults();
}

void GSRSettings::initializeDefaults() {
    auto writeDefault = [this](const QString& key, const QVariant& val) {
        if (!m_settings.contains(key)) {
            m_settings.setValue(key, val);
        }
    };

    // overlay
    writeDefault("overlay/interactivity_warning_shown", false);

    // Record
    writeDefault("record/output_directory", getDefaultOutputDir());
    writeDefault("record/categorize_by_title", true);
    writeDefault("record/capture_option", "screen");

    m_settings.sync();
}

QString GSRSettings::getDefaultOutputDir() const {
    QString videosDir = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
    if (videosDir.isEmpty()) {
        videosDir = QDir::homePath() + "/Videos";
    }
    QString path = QDir(videosDir).filePath("Screencasts");
    QDir().mkpath(path);
    return path;
}

QVariant GSRSettings::getSetting(const QString& key, const QVariant& defaultValue) const {
    m_settings.sync();
    return m_settings.value(key, defaultValue);
}

void GSRSettings::setSetting(const QString& key, const QVariant& value, const char* signalName) {
    if (m_settings.value(key) != value) {
        m_settings.setValue(key, value);
        m_settings.sync();
        
        int idx = metaObject()->indexOfSignal(QMetaObject::normalizedSignature(signalName));
        if (idx != -1) {
            QMetaObject::activate(this, metaObject(), idx, nullptr);
        }
    }
}

void GSRSettings::resetSetting(const QString& key, const char* signalName) {
    if (m_settings.contains(key)) {
        m_settings.remove(key);
        m_settings.sync();
        
        int idx = metaObject()->indexOfSignal(QMetaObject::normalizedSignature(signalName));
        if (idx != -1) {
            QMetaObject::activate(this, metaObject(), idx, nullptr);
        }
    }
}

QVariantMap GSRSettings::getCategorySettings(const QString& category) const {
    QVariantMap result;
    m_settings.beginGroup(category);
    const QStringList keys = m_settings.allKeys();
    for (const QString& key : keys) {
        result[key] = m_settings.value(key);
    }
    m_settings.endGroup();
    return result;
}

void GSRSettings::resetCategory(const QString& category) {
    m_settings.beginGroup(category);
    m_settings.remove("");
    m_settings.endGroup();
    initializeDefaults();
    m_settings.sync();
    emit settingsReset();
}
