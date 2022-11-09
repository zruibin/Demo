#include "globalconfig.h"
#include <QApplication>
#include <QTranslator>
#include <utility>

GlobalConfig &GlobalConfig::Instance()
{
    static GlobalConfig impl;
    return impl;
}

void GlobalConfig::LoadLanguage()
{
    {
        QTranslator *translator = new QTranslator();
        translator->load(":/qmfile/qm/TestApp_zh_CN.qm");
        m_LanguageMap.insert(Language::Chinese, translator);
    }

    {
        QTranslator *translator = new QTranslator();
        translator->load(":/qmfile/qm/TestApp_en.qm");
        m_LanguageMap.insert(Language::English, translator);
    }
}

void GlobalConfig::ChangeLanguage(Language lau)
{
    for (const auto &it : qAsConst(m_LanguageMap))
    {
        qApp->removeTranslator(it);
    }
    auto ret = m_LanguageMap.find(lau);
    if (ret != m_LanguageMap.end())
    {
        qApp->installTranslator(ret.value());
    }

}

GlobalConfig::GlobalConfig()
{
    //    LoadLanguage();
}

GlobalConfig::~GlobalConfig()
{
    qDeleteAll(m_LanguageMap);
}

