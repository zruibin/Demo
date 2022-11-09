#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

#include <QMap>

class QTranslator;

enum Language
{
    Chinese,
    English
};

class GlobalConfig
{
public:
    static GlobalConfig &Instance();
    void LoadLanguage();
    void ChangeLanguage(Language lau);

private:
    GlobalConfig();
    ~GlobalConfig();
private:
    QMap<Language, QTranslator*> m_LanguageMap;
};

#endif // GLOBALCONFIG_H
