#include "os/Debug.h"
#include "os/System.h"
#include "xdk/XAPILIB.h"
#include "xdk/XBDM.h"

namespace {
    DiscErrorCallbackFunc *gCallback;
}

unsigned long ULSystemLocale() { return XGetLocale(); }
unsigned long ULSystemLanguage() { return XTLGetLanguage(); }

DiscErrorCallbackFunc *SetDiskErrorCallback(DiscErrorCallbackFunc *func) {
    DiscErrorCallbackFunc *old = gCallback;
    gCallback = func;
    return old;
}

DiscErrorCallbackFunc *GetDiskErrorCallback() { return gCallback; }

#define SUCCEEDED(hr) hr >= 0

namespace {
    void XDKCheck() {
        DM_SYSTEM_INFO info;
        info.SizeOfStruct = 0x20;
        int hr = DmGetSystemInfo(&info);
        MILO_ASSERT(SUCCEEDED(hr), 0x27);
        if (info.XDKVersion.Build < 21173) {
            MILO_NOTIFY(
                "Console firmware is out of date.  Console: %d  Binary: %d",
                info.XDKVersion.Build,
                21173
            );
        }
    }
}

Symbol GetSystemLanguage(Symbol s) {
    static Symbol eng("eng");
    static Symbol fre("fre");
    static Symbol ita("ita");
    static Symbol deu("deu");
    static Symbol esl("esl");
    static Symbol mex("mex");
    static Symbol swe("swe");
    static Symbol pol("pol");
    static Symbol nor("nor");
    static Symbol fin("fin");
    static Symbol dut("dut");
    static Symbol dan("dan");
    static Symbol ptb("ptb");
    static Symbol rus("rus");
    static Symbol cht("cht");
    static Symbol kor("kor");
    static Symbol jpn("jpn");
    unsigned long lang = ULSystemLanguage();
    unsigned long locale = ULSystemLocale();
    switch (locale) {
    case XC_LOCALE_DENMARK:
        if (IsSupportedLanguage(dan, false))
            return dan;
    case XC_LOCALE_FINLAND:
        if (IsSupportedLanguage(fin, false))
            return fin;
    case XC_LOCALE_NETHERLANDS:
        if (IsSupportedLanguage(dut, false))
            return dut;
    case XC_LOCALE_NORWAY:
        if (IsSupportedLanguage(nor, false))
            return nor;
    case XC_LOCALE_SWEDEN:
        if (IsSupportedLanguage(swe, false))
            return swe;
    default:
        break;
    }

    switch (lang) {
    case XC_LANGUAGE_ENGLISH:
        if (locale == XC_LOCALE_BELGIUM && IsSupportedLanguage(dut, false))
            return dut;
    case XC_LANGUAGE_SCHINESE:
        return eng;
    case XC_LANGUAGE_JAPANESE:
        return jpn;
    case XC_LANGUAGE_GERMAN:
        return deu;
    case XC_LANGUAGE_FRENCH:
        return fre;
    case XC_LANGUAGE_SPANISH:
        if (locale == XC_LOCALE_CHILE || locale == XC_LOCALE_COLOMBIA
            || locale == XC_LOCALE_MEXICO) {
            return IsSupportedLanguage(mex, false) ? mex : esl;
        }
        break;
    case XC_LANGUAGE_ITALIAN:
        return ita;
    case XC_LANGUAGE_KOREAN:
        return kor;
    case XC_LANGUAGE_TCHINESE:
        return cht;
    case XC_LANGUAGE_PORTUGUESE:
        return ptb;
    case XC_LANGUAGE_POLISH:
        return pol;
    case XC_LANGUAGE_RUSSIAN:
        return rus;
    default:
        break;
    }

    return s;
}

Symbol GetSystemLocale(Symbol s) {
    static Symbol aus("aus");
    static Symbol aut("aut");
    static Symbol bel("bel");
    static Symbol bra("bra");
    static Symbol can("can");
    static Symbol chi("chi");
    static Symbol chn("chn");
    static Symbol col("col");
    static Symbol cze("cze");
    static Symbol den("den");
    static Symbol esp("esp");
    static Symbol fin("fin");
    static Symbol fra("fra");
    static Symbol gbr("gbr");
    static Symbol ger("ger");
    static Symbol gre("gre");
    static Symbol hkg("hkg");
    static Symbol hun("hun");
    static Symbol ind("ind");
    static Symbol irl("irl");
    static Symbol ita("ita");
    static Symbol jpn("jpn");
    static Symbol kor("kor");
    static Symbol mex("mex");
    static Symbol ned("ned");
    static Symbol nor("nor");
    static Symbol nzl("nzl");
    static Symbol pol("pol");
    static Symbol por("por");
    static Symbol rsa("rsa");
    static Symbol rus("rus");
    static Symbol sin("sin");
    static Symbol svk("svk");
    static Symbol swe("swe");
    static Symbol sui("sui");
    static Symbol tpe("tpe");
    static Symbol usa("usa");
    switch (ULSystemLocale()) {
    case XC_LOCALE_AUSTRALIA:
        return aus;
    case XC_LOCALE_AUSTRIA:
        return aut;
    case XC_LOCALE_BELGIUM:
        return bel;
    case XC_LOCALE_BRAZIL:
        return bra;
    case XC_LOCALE_CANADA:
        return can;
    case XC_LOCALE_CHILE:
        return chi;
    case XC_LOCALE_CHINA:
        return chn;
    case XC_LOCALE_COLOMBIA:
        return col;
    case XC_LOCALE_CZECH_REPUBLIC:
        return cze;
    case XC_LOCALE_DENMARK:
        return den;
    case XC_LOCALE_FINLAND:
        return fin;
    case XC_LOCALE_FRANCE:
        return fra;
    case XC_LOCALE_GERMANY:
        return ger;
    case XC_LOCALE_GREECE:
        return gre;
    case XC_LOCALE_HONG_KONG:
        return hkg;
    case XC_LOCALE_HUNGARY:
        return hun;
    case XC_LOCALE_INDIA:
        return ind;
    case XC_LOCALE_IRELAND:
        return irl;
    case XC_LOCALE_ITALY:
        return ita;
    case XC_LOCALE_JAPAN:
        return jpn;
    case XC_LOCALE_KOREA:
        return kor;
    case XC_LOCALE_MEXICO:
        return mex;
    case XC_LOCALE_NETHERLANDS:
        return ned;
    case XC_LOCALE_NEW_ZEALAND:
        return nzl;
    case XC_LOCALE_NORWAY:
        return nor;
    case XC_LOCALE_POLAND:
        return pol;
    case XC_LOCALE_PORTUGAL:
        return por;
    case XC_LOCALE_SINGAPORE:
        return sin;
    case XC_LOCALE_SLOVAK_REPUBLIC:
        return svk;
    case XC_LOCALE_SOUTH_AFRICA:
        return rsa;
    case XC_LOCALE_SPAIN:
        return esp;
    case XC_LOCALE_SWEDEN:
        return swe;
    case XC_LOCALE_SWITZERLAND:
        return sui;
    case XC_LOCALE_TAIWAN:
        return tpe;
    case XC_LOCALE_GREAT_BRITAIN:
        return gbr;
    case XC_LOCALE_UNITED_STATES:
        return usa;
    case XC_LOCALE_RUSSIAN_FEDERATION:
        return rus;
    default:
        return s;
    }
}

bool HongKongExceptionMet() {
    if (ULSystemLanguage() == 8 && ULSystemLocale() == XC_LOCALE_HONG_KONG) {
        return true;
    } else
        return false;
}
