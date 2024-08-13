#include "theme.h"

#include <QDebug>
#include <QColor>

const QString Theme::DEFAULT_THEME_COLOR = "#248b00";

Theme::Theme(QObject *parent) :
    QObject(parent),
    mColor1("#000000"), mColor2(DEFAULT_THEME_COLOR), mColor3("#4cb328"),
    mColor4("#555555"), mColor5("#888888"), mColor6("#ffffff"),
    mColor7("#cccccc"), mColor8("#eeeeee"), mColor9("#ccffffff")
{
}

void Theme::setThemeColor(QString color)
{
    QColor c;
    c.setNamedColor(color);
    c.setRed(qMin(c.red() + 40, 255));
    c.setGreen(qMin(c.green() + 40, 255));
    c.setBlue(qMin(c.blue() + 40, 255));

    mColor2 = color;
    mColor3 = c.name();
    emit color2Changed();
    emit color3Changed();
}

float Theme::getHue(QString color) {

    QColor c;
    c.setNamedColor(color);
    QColor converted = c.toHsv();
    return converted.hsvHueF();
}

float Theme::getSaturation(QString color) {

    QColor c;
    c.setNamedColor(color);
    QColor converted = c.toHsv();
    return converted.hsvSaturationF();
}

float Theme::getLightness(QString color) {

    QColor c;
    c.setNamedColor(color);
    QColor converted = c.toHsv();
    return converted.valueF();
}
