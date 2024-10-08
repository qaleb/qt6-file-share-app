#ifndef THEME_H
#define THEME_H

#include <QObject>

class Theme : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString color1 READ color1 NOTIFY color1Changed)
    Q_PROPERTY(QString color2 READ color2 NOTIFY color2Changed)
    Q_PROPERTY(QString color3 READ color3 NOTIFY color3Changed)
    Q_PROPERTY(QString color4 READ color4 NOTIFY color4Changed)
    Q_PROPERTY(QString color5 READ color5 NOTIFY color5Changed)
    Q_PROPERTY(QString color6 READ color6 NOTIFY color6Changed)
    Q_PROPERTY(QString color7 READ color7 NOTIFY color7Changed)
    Q_PROPERTY(QString color8 READ color8 NOTIFY color8Changed)
    Q_PROPERTY(QString color9 READ color9 NOTIFY color9Changed)

public:
    explicit Theme(QObject *parent = 0);
    inline QString color1() { return mColor1; }
    inline QString color2() { return mColor2; }
    inline QString color3() { return mColor3; }
    inline QString color4() { return mColor4; }
    inline QString color5() { return mColor5; }
    inline QString color6() { return mColor6; }
    inline QString color7() { return mColor7; }
    inline QString color8() { return mColor8; }
    inline QString color9() { return mColor9; }
    void setThemeColor(QString color);

    static const QString DEFAULT_THEME_COLOR;

signals:
    void color1Changed();
    void color2Changed();
    void color3Changed();
    void color4Changed();
    void color5Changed();
    void color6Changed();
    void color7Changed();
    void color8Changed();
    void color9Changed();

public slots:
    float getHue(QString color);
    float getSaturation(QString color);
    float getLightness(QString color);

private:
    QString mColor1;
    QString mColor2;
    QString mColor3;
    QString mColor4;
    QString mColor5;
    QString mColor6;
    QString mColor7;
    QString mColor8;
    QString mColor9;

};

#endif // THEME_H
