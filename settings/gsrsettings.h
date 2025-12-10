#pragma once

#include <QObject>
#include <QSettings>
#include <QVariantMap>
#include <QStandardPaths>
#include <QDir>

// Macro to generate property with getter, setter, default, and reset (holy shit C++)
#define SETTING_PROPERTY(Type, name, key, defaultValue) \
    Q_PROPERTY(Type name READ get_##name WRITE set_##name NOTIFY name##Changed) \
public: \
    Type get_##name() const { return getSetting(key, defaultValue).value<Type>(); } \
    void set_##name(const Type& val) { setSetting(key, val, #name "Changed"); } \
    Q_INVOKABLE Type getDefault_##name() const { return defaultValue; } \
    Q_INVOKABLE void reset_##name() { resetSetting(key, #name "Changed"); } \
private: \
    Q_SIGNAL void name##Changed();


class GSRSettings : public QObject {
    Q_OBJECT

    // Record settings
    SETTING_PROPERTY(QString, recordOutputDirectory, "record/output_directory", getDefaultOutputDir())
    SETTING_PROPERTY(bool, recordCategorizeByTitle, "record/categorize_by_title", true)
    SETTING_PROPERTY(QString, recordCaptureOption, "record/capture_option", "screen")
    
public:
    static GSRSettings& instance() {
        static GSRSettings inst;
        return inst;
    }

    // Get all settings for a category
    Q_INVOKABLE QVariantMap getReplaySettings() const { return getCategorySettings("replay"); }
    Q_INVOKABLE QVariantMap getRecordSettings() const { return getCategorySettings("record"); }
    Q_INVOKABLE QVariantMap getStreamSettings() const { return getCategorySettings("stream"); }

    // Reset entire category
    Q_INVOKABLE void resetReplaySettings() { resetCategory("replay"); }
    Q_INVOKABLE void resetRecordSettings() { resetCategory("record"); }
    Q_INVOKABLE void resetStreamSettings() { resetCategory("stream"); }
    Q_INVOKABLE void resetAllSettings() {
        m_settings.clear();
        initializeDefaults();
        m_settings.sync();
        emit settingsReset();
    }

signals:
    void settingsReset();

private:
    GSRSettings(QObject* parent = nullptr);
    ~GSRSettings() override = default;
    GSRSettings(const GSRSettings&) = delete;
    GSRSettings& operator=(const GSRSettings&) = delete;

    void initializeDefaults();
    QString getDefaultOutputDir() const;
    
    QVariant getSetting(const QString& key, const QVariant& defaultValue) const;
    void setSetting(const QString& key, const QVariant& value, const char* signalName);
    void resetSetting(const QString& key, const char* signalName);
    QVariantMap getCategorySettings(const QString& category) const;
    void resetCategory(const QString& category);

    mutable QSettings m_settings;
};