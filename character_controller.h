#ifndef CHARACTER_CONTROLLER_H
#define CHARACTER_CONTROLLER_H

#include <QStringList>
#include <QByteArray>
#include <unicode/ucnv.h>

#include <unicode/ucnv.h>

// UTF-16 → Shift-JIS（ICU)
QByteArray toShiftJIS_ICU(const QString& src)
{
    QByteArray result;

    UErrorCode status = U_ZERO_ERROR;
    UConverter* conv = ucnv_open("Shift_JIS", &status);
    if (U_FAILURE(status)) return result;

    const UChar* uSource = reinterpret_cast<const UChar*>(src.utf16());
    int32_t uLen = src.size();

    int32_t neededSize = ucnv_fromUChars(conv, nullptr, 0, uSource, uLen, &status);
    status = U_ZERO_ERROR;
    result.resize(neededSize);
    ucnv_fromUChars(conv, result.data(), neededSize, uSource, uLen, &status);

    ucnv_close(conv);
    return result;
}

// Shift-JIS → UTF-16（ICU）
QString fromShiftJIS_ICU(const QByteArray& src)
{
    UErrorCode status = U_ZERO_ERROR;
    UConverter* conv = ucnv_open("Shift_JIS", &status);
    if (U_FAILURE(status)) return QString();

    int32_t needed = ucnv_toUChars(conv, nullptr, 0, src.data(), src.size(), &status);

    status = U_ZERO_ERROR;
    QString out;
    out.resize(needed);
    ucnv_toUChars(conv,
                  reinterpret_cast<UChar*>(out.data()),
                  needed,
                  src.data(),
                  src.size(),
                  &status);

    ucnv_close(conv);
    return out;
}

QStringList getAvailableEncodings()
{
    QStringList list;

    int32_t count = ucnv_countAvailable();
    for (int32_t i = 0; i < count; i++) {
        const char *name = ucnv_getAvailableName(i);
        list << QString::fromUtf8(name);
    }

    list.sort();
    return list;
}
QByteArray encodeWithICU(const QString &text, const QString &encodingName)
{
    UErrorCode status = U_ZERO_ERROR;

    // コンバータを開く
    UConverter *conv = ucnv_open(encodingName.toUtf8().constData(), &status);
    if (U_FAILURE(status)) {
        qWarning("Failed to open encoding: %s", encodingName.toUtf8().constData());
        return text.toUtf8();  // フォールバック
    }

    QByteArray buffer;
    buffer.resize(text.size() * 4);

    int32_t len = ucnv_fromUChars(
        conv,
        buffer.data(),
        buffer.size(),
        reinterpret_cast<const UChar *>(text.utf16()),
        text.length(),
        &status
        );

    ucnv_close(conv);

    if (U_FAILURE(status)) {
        qWarning("Encoding failed: %s", encodingName.toUtf8().constData());
        return text.toUtf8();  // フォールバック
    }

    buffer.resize(len);
    return buffer;
}
static QByteArray toICUEncoded(const QString& text, const char* charset)
{
    UErrorCode status = U_ZERO_ERROR;
    UConverter* conv = ucnv_open(charset, &status);
    if (U_FAILURE(status)) {
        return {};
    }

    const UChar* src =
        reinterpret_cast<const UChar*>(text.utf16());
    int32_t srcLen = text.length();

    // 必要サイズ取得
    int32_t required = ucnv_fromUChars(
        conv, nullptr, 0,
        src, srcLen,
        &status
        );
    status = U_ZERO_ERROR;

    QByteArray result;
    result.resize(required);

    ucnv_fromUChars(
        conv,
        result.data(),
        result.size(),
        src, srcLen,
        &status
        );

    ucnv_close(conv);
    return result;
}



#endif // CHARACTER_CONTROLLER_H
