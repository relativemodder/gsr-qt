#include "gsrargs.h"
#include <QDebug>

GSRArgs::GSRArgs() {}

void GSRArgs::setWindowTarget(const QString& target) {
    setOption("-w", target);
}

void GSRArgs::setContainerFormat(const QString& format) {
    setOption("-c", format);
}

void GSRArgs::setResolution(const QString& resolution) {
    setOption("-s", resolution);
}

void GSRArgs::setCaptureRegion(const QString& region) {
    setOption("-region", region);
}

void GSRArgs::setFrameRate(int fps) {
    setOption("-f", QString::number(fps));
}

void GSRArgs::addAudioSource(const QString& audioSource) {
    addMultiOption("-a", audioSource);
}

void GSRArgs::clearAudioSources() {
    multiOptions.remove("-a");
}

void GSRArgs::setQuality(const QString& quality) {
    setOption("-q", quality);
}

void GSRArgs::setOutputFile(const QString& path) {
    setOption("-o", path);
}

void GSRArgs::setRegularOutputDirectory(const QString& dirPath) {
    setOption("-ro", dirPath);
}

// --- replay options ---

void GSRArgs::setReplayBufferSize(int seconds) {
    if (seconds > 0) {
        setOption("-r", QString::number(seconds));
    } else {
        removeOption("-r");
    }
}

void GSRArgs::setReplayStorage(const QString& storage) {
    setOption("-replay-storage", storage);
}

void GSRArgs::setRestartReplayOnSave(bool restart) {
    setOption("-restart-replay-on-save", restart ? "yes" : "no");
}

// codec and mode options

void GSRArgs::setVideoCodec(const QString& codec) {
    setOption("-k", codec);
}

void GSRArgs::setAudioCodec(const QString& codec) {
    setOption("-ac", codec);
}

void GSRArgs::setAudioBitrate(int bitrate) {
    if (bitrate > 0) {
        setOption("-ab", QString::number(bitrate));
    } else {
        removeOption("-ab");
    }
}

void GSRArgs::setFramerateMode(const QString& mode) {
    setOption("-fm", mode);
}

void GSRArgs::setBitrateMode(const QString& mode) {
    setOption("-bm", mode);
}


void GSRArgs::setColorRange(const QString& range) {
    setOption("-cr", range);
}

void GSRArgs::setTuning(const QString& setting) {
    setOption("-tune", setting);
}

void GSRArgs::setKeyframeInterval(double seconds) {
    setOption("-keyint", QString::number(seconds));
}

void GSRArgs::setRecordCursor(bool record) {
    setOption("-cursor", record ? "yes" : "no");
}

void GSRArgs::setEncoder(const QString& device) {
    setOption("-encoder", device);
}

void GSRArgs::setOrganizeReplaysByDate(bool enable) {
    setFlag("-df", enable);
}

void GSRArgs::setOverclockMemory(bool enable) {
    setOption("-oc", enable ? "yes" : "no");
}

void GSRArgs::setScriptPath(const QString& path) {
    setOption("-sc", path);
}

void GSRArgs::addPlugin(const QString& path) {
    addMultiOption("-p", path);
}

void GSRArgs::setRestorePortalSession(bool enable) {
    setOption("-restore-portal-session", enable ? "yes" : "no");
}

void GSRArgs::setPortalSessionTokenFilepath(const QString& path) {
    setOption("-portal-session-token-filepath", path);
}

void GSRArgs::setFallbackCpuEncoding(bool enable) {
    setOption("-fallback-cpu-encoding", enable ? "yes" : "no");
}

void GSRArgs::setPrintFpsInfo(bool enable) {
    setOption("-v", enable ? "yes" : "no");
}

void GSRArgs::setGlDebug(bool enable) {
    setOption("-gl-debug", enable ? "yes" : "no");
}



void GSRArgs::setOption(const QString& option, const QString& value) {
    if (!value.isEmpty()) {
        singleOptions.insert(option, value);
    } else {
        singleOptions.remove(option);
    }
}

void GSRArgs::addMultiOption(const QString& option, const QString& value) {
    if (!value.isEmpty()) {
        multiOptions[option].append(value);
    }
}

void GSRArgs::setFlag(const QString& option, bool enable) {
    flags.insert(option, enable);
}

void GSRArgs::removeOption(const QString& option) {
    singleOptions.remove(option);
}


QStringList GSRArgs::toQStringList() const {
    QStringList args;

    // add single options (flag and value)
    for (auto it = singleOptions.constBegin(); it != singleOptions.constEnd(); ++it) {
        args << it.key() << it.value();
    }

    // add multi options (flag, value 1, flag, value 2, ...)
    for (auto it = multiOptions.constBegin(); it != multiOptions.constEnd(); ++it) {
        for (const QString& value : it.value()) {
            args << it.key() << value;
        }
    }
    
    // add flags (flag without value, if enabled)
    for (auto it = flags.constBegin(); it != flags.constEnd(); ++it) {
        if (it.value()) {
            args << it.key();
        }
    }

    return args;
}