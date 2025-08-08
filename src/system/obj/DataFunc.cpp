#include "obj/DataFunc.h"
#include "math/Decibels.h"
#include "math/Rand.h"
#include "math/Utl.h"
#include "obj/Data.h"
#include "obj/DataFile.h"
#include "obj/DataUtl.h"
#include "obj/Object.h"
#include "obj/Utl.h"
#include "os/Debug.h"
#include "os/File.h"
#include "os/System.h"
#include "utl/Locale.h"
#include "utl/Str.h"
#include "utl/TextFileStream.h"

bool SwitchMatch(const DataNode &n1, const DataNode &n2) {
    if (n1.Type() == kDataArray) {
        DataArray *arr = n1.UncheckedArray();
        for (int i = 0; i < arr->Size(); i++) {
            DataNode &cur = arr->Node(i);
            if (cur.Equal(n2, nullptr, true)) {
                return true;
            }
        }
        return false;
    } else
        return n1.Equal(n2, nullptr, true);
}

DataNode DataFuncObj::New(DataArray *arr) {
    Hmx::Object *o = ObjectDir::Main()->Find<Hmx::Object>(arr->Str(1), false);
    if (o)
        delete o;
    return new DataFuncObj(arr);
}

DEF_DATA_FUNC(DataSprintf) {
    FormatString fs(array->Str(1));
    for (int i = 2; i < array->Size(); i++) {
        fs << array->Evaluate(i);
    }
    return fs.Str();
}

DEF_DATA_FUNC(DataPrintf) {
    FormatString fs(array->Str(1));
    for (int i = 2; i < array->Size(); i++) {
        fs << array->Evaluate(i);
    }
    TheDebug << fs.Str();
    return 0;
}

DEF_DATA_FUNC(DataPrint) {
    for (int i = 1; i < array->Size(); i++) {
        array->Evaluate(i).Print(TheDebug, true, 0);
    }
    return 0;
}

DEF_DATA_FUNC(DataSprint) {
    class String str;
    for (int i = 1; i < array->Size(); i++) {
        array->Evaluate(i).Print(str, true, 0);
    }
    return str.c_str();
}

DEF_DATA_FUNC(DataSet) {
    const DataArray *a = array;
    DataNode ret(a->Evaluate(2));
    if (a->Type(1) == kDataProperty) {
        MILO_ASSERT(gDataThis, 0xA9);
        gDataThis->SetProperty(a->Node(1).UncheckedArray(), ret);
    } else
        *a->Var(1) = ret;
    return ret;
}

DEF_DATA_FUNC(DataSetVar) {
    DataNode ret = array->Evaluate(2);
    DataVariable(array->ForceSym(1)) = ret;
    return ret;
}

DEF_DATA_FUNC(DataIfElse) {
    MILO_ASSERT_FMT(
        array->Size() == 4,
        "One condition and two alternatives expected (file %s, line %d)",
        array->File(),
        array->Line()
    );
    if (array->Node(1).NotNull())
        return array->Evaluate(2);
    else
        return array->Evaluate(3);
}

DEF_DATA_FUNC(DataIf) {
    if (array->Node(1).NotNull()) {
        for (int i = 2; i < array->Size(); i++) {
            array->Command(i)->Execute(true);
        }
        return 0;
    } else
        return 0;
}

DEF_DATA_FUNC(DataUnless) {
    if (!array->Node(1)) {
        for (int i = 2; i < array->Size(); i++) {
            array->Command(i)->Execute(true);
        }
        return 0;
    }
    return 0;
}

/** Verifies if two DataNodes are equivalent. */
DEF_DATA_FUNC(DataEq) {
    const DataNode &dn1 = array->Evaluate(1);
    const DataNode &dn2 = array->Evaluate(2);
    return dn1.Equal(dn2, array, true);
}

DEF_DATA_FUNC(DataFindElem) {
    DataArray *arr = array->Array(1);
    arr->AddRef();
    const DataNode &dn = array->Evaluate(2);
    for (int i = 0; i < arr->Size(); i++) {
        DataNode &arrNode = arr->Node(i);
        if (arrNode.Equal(dn, nullptr, true)) {
            if (array->Size() > 3) {
                *array->Var(3) = i;
            }
            arr->Release();
            return 1;
        }
    }
    arr->Release();
    return 0;
}

/** Verifies if two DataNodes are NOT equivalent. */
DEF_DATA_FUNC(DataNe) { return DataEq(array).UncheckedInt() == 0; }

DEF_DATA_FUNC(DataLe) { return array->Float(1) <= array->Float(2); }

DEF_DATA_FUNC(DataLt) { return array->Float(1) < array->Float(2); }

DEF_DATA_FUNC(DataGe) { return array->Float(1) >= array->Float(2); }

DEF_DATA_FUNC(DataGt) { return array->Float(1) > array->Float(2); }

DEF_DATA_FUNC(DataDbToRatio) {
    float f1 = array->Float(1);
    float f2 = -96 - f1 >= 0 ? -96 : f1;
    f1 = f2 >= 0 ? 0 : f2;
    return DbToRatio(f1);
}

DEF_DATA_FUNC(DataRatioToDb) {
    float f1 = array->Float(1);
    float f2;
    f2 = -f1 >= 0 ? 0 : f1;
    f1 = f2 - 1 >= 0 ? 1 : f2;
    return RatioToDb(f1);
}

DEF_DATA_FUNC(DataNot) { return !array->Node(1); }

DEF_DATA_FUNC(DataAnd) {
    for (int i = 1; i < array->Size(); i++) {
        if (!array->Node(i)) {
            return 0;
        }
    }
    return 1;
}

DEF_DATA_FUNC(DataOr) {
    for (int i = 1; i < array->Size(); i++) {
        if (array->Node(i).NotNull())
            return 1;
    }
    return 0;
}

DEF_DATA_FUNC(DataXor) { return array->Node(1).NotNull() != array->Node(2).NotNull(); }

DEF_DATA_FUNC(DataBitAnd) {
    MILO_ASSERT(array->Size() >= 3, 0x146);
    int res = array->Int(1);
    for (int i = 2; i < array->Size(); i++) {
        res &= array->Int(i);
    }
    return res;
}

DEF_DATA_FUNC(DataAndEqual) {
    MILO_ASSERT(array->Size() == 3, 0x152);
    if (array->Type(1) == kDataProperty) {
        MILO_ASSERT(gDataThis, 0x157);
        DataArray *arr = array->UncheckedArray(1);
        int res = gDataThis->Property(arr, true)->Int() & array->Int(2);
        gDataThis->SetProperty(arr, res);
        return res;
    } else {
        DataNode *var = array->Var(1);
        int res = var->Int() & array->Int(2);
        return (*var = res);
    }
}

DEF_DATA_FUNC(DataMaskEqual) {
    MILO_ASSERT(array->Size() == 3, 0x168);
    if (array->Type(1) == kDataProperty) {
        MILO_ASSERT(gDataThis, 0x16D);
        DataArray *arr = array->UncheckedArray(1);
        int res = gDataThis->Property(arr, true)->Int() & ~array->Int(2);
        gDataThis->SetProperty(arr, res);
        return res;
    } else {
        DataNode *var = array->Var(1);
        int res = var->Int() & ~array->Int(2);
        return (*var = res);
    }
}

DEF_DATA_FUNC(DataOrEqual) {
    MILO_ASSERT(array->Size() == 3, 0x17F);
    if (array->Type(1) == kDataProperty) {
        MILO_ASSERT(gDataThis, 0x184);
        DataArray *arr = array->UncheckedArray(1);
        int res = gDataThis->Property(arr, true)->Int() | array->Int(2);
        gDataThis->SetProperty(arr, res);
        return res;
    } else {
        DataNode *var = array->Var(1);
        int res = var->Int() | array->Int(2);
        return (*var = res);
    }
}

DEF_DATA_FUNC(DataBitOr) {
    MILO_ASSERT(array->Size() >= 3, 0x196);
    int res = array->Int(1);
    for (int i = 2; i < array->Size(); i++)
        res |= array->Int(i);
    return res;
}

DEF_DATA_FUNC(DataBitXor) { return array->Int(1) ^ array->Int(2); }

DEF_DATA_FUNC(DataBitNot) { return ~array->Int(1); }

DEF_DATA_FUNC(DataLowestBit) { return LowestBit(array->Int(1)); }

DEF_DATA_FUNC(DataCountBits) { return CountBits(array->Int(1)); }

DEF_DATA_FUNC(DataWhile) {
    while (array->Node(1).NotNull()) {
        for (int i = 2; i < array->Size(); i++) {
            array->Command(i)->Execute(true);
        }
    }
    return 0;
}

DEF_DATA_FUNC(DataVar) { return &DataVariable(array->ForceSym(1)); }

DEF_DATA_FUNC(DataPackColor) {
    float b = array->Float(3);
    float g = array->Float(2);
    float r = array->Float(1);

    int r_packed = ((int)(r * 255.0f) & 0xFF);
    int g_packed = ((int)(g * 255.0f) & 0xFF) << 8;
    int b_packed = ((int)(b * 255.0f) & 0xFF) << 16;

    return r_packed | g_packed | b_packed;
}

DEF_DATA_FUNC(DataUnpackColor) {
    int packed = array->Int(1);
    *array->Var(2) = (float)(packed & 0xFF) / 255.0f;
    *array->Var(3) = (float)(packed >> 8 & 0xFF) / 255.0f;
    *array->Var(4) = (float)(packed >> 0x10 & 0xFF) / 255.0f;
    return 0;
}

DEF_DATA_FUNC(DataDo) {
    int size = array->Size(); // this needs to be up here to match
    int i;
    for (i = 1; array->Type(i) == kDataArray; i++) {
        DataArray *binding = array->UncheckedArray(i);
        DataNode *n = binding->Var(0);
        DataPushVar(n);
        if (binding->Size() == 2) {
            *n = binding->Evaluate(1);
        } else {
            MILO_ASSERT_FMT(binding->Size() == 1, "do var has more than one initializer");
        }
    }
    int numVars = i - 1;

    for (; i < size - 1; i++) {
        array->Command(i)->Execute(true);
    }
    DataNode ret(array->Evaluate(i));

    while (numVars-- != 0) {
        DataPopVar();
    }

    return ret;
}

DEF_DATA_FUNC(DataMin) {
    int min = -1;
    float minf; // this matches but wow this is not good
    for (int i = 1; i < array->Size(); i++) {
        float f = array->Float(i);
        if (min < 0 || f < minf) {
            minf = f;
            min = i;
        }
    }
    if (min > 0) {
        return array->Evaluate(min);
    }
    MILO_FAIL("DataMin: No valid data to compare");
    return 0;
}

DEF_DATA_FUNC(DataMax) {
    int max = -1;
    float maxf;
    for (int i = 1; i < array->Size(); i++) {
        float f = array->Float(i);
        if (max < 0 || f > maxf) {
            maxf = f;
            max = i;
        }
    }
    if (max > 0) {
        return array->Evaluate(max);
    }
    MILO_FAIL("DataMax: No valid data to compare");
    return 0;
}

DEF_DATA_FUNC(DataAbs) {
    const DataNode &n = array->Evaluate(1);
    float f = std::fabs(n.LiteralFloat(array));
    if (n.Type() == kDataInt)
        return (int)f;
    else
        return f;
}

DEF_DATA_FUNC(DataAdd) {
    float sum_f = 0.0f;
    int sum_int = 0;
    int cnt = array->Size();
    int i;
    for (i = 1; i < cnt; i++) {
        const DataNode &n = array->Evaluate(i);
        if (n.Type() != kDataInt) {
            sum_f = sum_int + n.LiteralFloat(array);
            break;
        }
        sum_int += n.UncheckedInt();
    }
    if (i == cnt)
        return sum_int;
    for (i++; i < cnt; i++) {
        sum_f += array->Float(i);
    }
    return sum_f;
}

DEF_DATA_FUNC(DataAddEq) {
    DataNode ret = DataAdd(array);
    if (array->Type(1) == kDataProperty) {
        MILO_ASSERT(gDataThis, 0x24F);
        gDataThis->SetProperty(array->UncheckedArray(1), ret);
    } else
        *array->Var(1) = ret;
    return ret;
}

DEF_DATA_FUNC(DataSub) {
    const DataNode &dn = array->Evaluate(1);
    if (array->Size() == 2) {
        if (dn.Type() == kDataFloat) {
            return -dn.LiteralFloat(array);
        } else
            return -dn.LiteralInt(array);
    } else {
        const DataNode &dn2 = array->Evaluate(2);
        if (dn.Type() == kDataFloat || dn2.Type() == kDataFloat) {
            return dn.LiteralFloat(array) - dn2.LiteralFloat(array);
        } else {
            return dn.LiteralInt(array) - dn2.LiteralInt(array);
        }
    }
}

DEF_DATA_FUNC(DataMean) {
    float sum = 0.0;
    int cnt = array->Size();
    for (int i = 1; i < cnt; i++) {
        sum += array->Float(i);
    }
    return sum / (cnt - 1);
}

DEF_DATA_FUNC(DataClamp) {
    const DataNode &n1 = array->Evaluate(1);
    const DataNode &n2 = array->Evaluate(2);
    const DataNode &n3 = array->Evaluate(3);
    if (n1.Type() == kDataFloat || n2.Type() == kDataFloat || n3.Type() == kDataFloat) {
        return Clamp<float>(
            n2.LiteralFloat(array), n3.LiteralFloat(array), n1.LiteralFloat(array)
        );
    } else {
        return Clamp<int>(
            n2.LiteralInt(array), n3.LiteralInt(array), n1.LiteralInt(array)
        );
    }
}

DEF_DATA_FUNC(DataSubEq) {
    DataNode ret = DataSub(array);
    if (array->Type(1) == kDataProperty) {
        MILO_ASSERT(gDataThis, 0x285);
        gDataThis->SetProperty(array->UncheckedArray(1), ret);
    } else
        *array->Var(1) = ret;
    return ret;
}

DEF_DATA_FUNC(DataClampEq) {
    DataNode ret = DataClamp(array);
    if (array->Type(1) == kDataProperty) {
        MILO_ASSERT(gDataThis, 0x28C);
        gDataThis->SetProperty(array->UncheckedArray(1), ret);
    } else
        *array->Var(1) = ret;
    return ret;
}

DEF_DATA_FUNC(DataMultiply) {
    const DataNode &n1 = array->Evaluate(1);
    const DataNode &n2 = array->Evaluate(2);
    bool isFloat = false;
    int multi = 1;
    float multf = 1;
    int size = array->Size();
    for (int i = 1; i < size; i++) {
        const DataNode &cur = array->Evaluate(i);
        if (cur.Type() != kDataFloat) {
            multi *= cur.LiteralInt();
        } else {
            multf *= cur.LiteralFloat();
            isFloat = true;
        }
    }
    if (isFloat) {
        return multf * (float)multi;
    } else {
        return multi;
    }
    return 0;
}

DEF_DATA_FUNC(DataMultiplyEq) {
    DataNode ret = DataMultiply(array);
    if (array->Type(1) == kDataProperty) {
        MILO_ASSERT(gDataThis, 0x2C3);
        gDataThis->SetProperty(array->UncheckedArray(1), ret);
    } else
        *array->Var(1) = ret;
    return ret;
}

DEF_DATA_FUNC(DataDivide) { return array->Float(1) / array->Float(2); }

DEF_DATA_FUNC(DataDivideEq) {
    DataNode ret = DataDivide(array);
    if (array->Type(1) == kDataProperty) {
        MILO_ASSERT(gDataThis, 0x2CF);
        gDataThis->SetProperty(array->UncheckedArray(1), ret);
    } else
        *array->Var(1) = ret;
    return ret;
}

DEF_DATA_FUNC(DataSqrt) { return std::sqrt(array->Float(1)); }

DEF_DATA_FUNC(DataPow) { return std::pow(array->Float(1), array->Float(2)); }

DEF_DATA_FUNC(DataMod) {
    const DataNode &n1 = array->Evaluate(1);
    const DataNode &n2 = array->Evaluate(2);
    if (n1.Type() == kDataFloat || n2.Type() == kDataFloat) {
        return Mod(n1.LiteralFloat(array), n2.LiteralFloat(array));
    } else {
        return Mod(n1.LiteralInt(array), n2.LiteralInt(array));
    }
}

DEF_DATA_FUNC(DataDist) {
    float z1 = array->Float(3);
    float z2 = array->Float(6);
    float y1 = array->Float(2);
    float y2 = array->Float(5);
    float x1 = array->Float(1);
    float x2 = array->Float(4);

    return std::sqrt(
        (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2)
    );
}

DEF_DATA_FUNC(DataSymbol) { return array->ForceSym(1); }

DEF_DATA_FUNC(DataChar) {
    static char newChar[2];
    const DataNode &n = array->Evaluate(1);
    newChar[0] = n.Int();
    newChar[1] = '\0';
    return newChar;
}

DEF_DATA_FUNC(DataAsc) {
    const char *str = array->Str(1);
    MILO_ASSERT(strlen(str) == 1, 0x2FF);
    return *str;
}

DEF_DATA_FUNC(DataInt) {
    const DataNode &n = array->Evaluate(1);
    DataType t = n.Type();
    if (t == kDataSymbol || t == kDataString) {
        return atoi(n.Str());
    } else if (t == kDataObject || t == kDataInt) {
        return n.UncheckedInt();
    } else {
        return (int)n.LiteralFloat(array);
    }
}

DEF_DATA_FUNC(DataRound) { return Round(array->Evaluate(1).LiteralFloat()); }

DEF_DATA_FUNC(DataFloor) { return std::floor(array->Float(1)); }

DEF_DATA_FUNC(DataCeil) { return std::ceil(array->Float(1)); }

DEF_DATA_FUNC(DataDelete) {
    delete array->Obj<Hmx::Object>(1);
    return 0;
}

DEF_DATA_FUNC(DataNew) {
    Hmx::Object *obj = Hmx::Object::NewObject(array->Sym(1));
    if (array->Size() > 2) {
        if (array->Type(2) == kDataArray) {
            obj->SetTypeDef(array);
        } else {
            obj->SetName(array->Str(2), gDataDir);
            if (array->Size() > 3) {
                if (array->Type(3) == kDataArray) {
                    obj->SetTypeDef(array);
                } else {
                    obj->SetType(array->Sym(3));
                }
            }
        }
    }
    return obj;
}

DEF_DATA_FUNC(DataForEach) {
    DataArray *arr = array->Array(2);
    arr->AddRef();
    DataNode *var = array->Var(1);

    DataNode save(*var);
    for (int i = 0; i < arr->Size(); i++) {
        *var = arr->Evaluate(i);
        for (int j = 3; j < array->Size(); j++) {
            array->Command(j)->Execute(true);
        }
    }

    *var = save;
    arr->Release();
    return 0;
}

DEF_DATA_FUNC(DataForEachInt) {
    DataNode *var = array->Var(1);
    int begin = array->Int(2);
    int end = array->Int(3);
    int inc = end > begin ? 1 : -1;

    DataNode save(*var);
    for (int cur = begin; cur != end; cur += inc) {
        *var = cur;
        for (int cnt = 4; cnt < array->Size(); cnt++) {
            array->Command(cnt)->Execute(true);
        }
        cur = var->UncheckedInt();
    }

    *var = save;
    return 0;
}

DEF_DATA_FUNC(DataGetElem) {
    int i = array->Int(2);
    DataArray *a = array->Array(1);
    return a->Node(i);
}

DEF_DATA_FUNC(DataGetLastElem) {
    DataArray *arr = array->Array(1);
    MILO_ASSERT(arr->Size() > 0, 0x403);
    return arr->Node(arr->Size() - 1);
}

DEF_DATA_FUNC(DataObject) {
    const DataNode &n = array->Evaluate(1);
    if (n.Type() == kDataObject)
        return n;
    else {
        const char *s = n.LiteralStr(array);
        Hmx::Object *o = gDataDir->FindObject(s, true, true);
        if (!o && s[0]) {
            MILO_FAIL(
                kNotObjectMsg, s, PathName(gDataDir) ? PathName(gDataDir) : "**no file**"
            );
        }
        return o;
    }
}

DEF_DATA_FUNC(DataLocalize) {
    const char *localized = Localize(array->ForceSym(1), nullptr, TheLocale);
    return localized ? localized : gNullStr;
}

DEF_DATA_FUNC(DataLocalizeSeparatedInt) {
    return LocalizeSeparatedInt(array->Int(1), TheLocale);
}

DEF_DATA_FUNC(DataLocalizeFloat) { return LocalizeFloat(array->Str(1), array->Float(2)); }

DEF_DATA_FUNC(DataSyncReloadLocale) {
    SyncReloadLocale();
    return 1;
}

DEF_DATA_FUNC(DataPrintUnused) {
    return SystemConfig(array->Sym(1))->PrintUnused(TheDebug, kDataArray, true);
}

DEF_DATA_FUNC(DataPrintStack) {
    StackString<2048> str;
    DataAppendStackTrace(str);
    MILO_LOG("%s\n", str);
    return 0;
}

DEF_DATA_FUNC(DataStartsWith) {
    return strneq(
        array->Str(1),
        array->Str(2),
        (array->Size() > 3) ? array->Int(3) : strlen(array->Str(2))
    );
}

DEF_DATA_FUNC(DataTime) {
    int i;
    for (i = 1; i < array->Size(); i++) {
        DataNode &eval = array->Node(i);
        if (eval.Type() == kDataCommand)
            break;
        eval.Print(TheDebug, true, 0);
    }

    if (i == 1) {
        MILO_LOG("Timing %s, line %d:", array->File(), array->Line());
    }

    Timer time;
    time.Start();
    while (i < array->Size()) {
        array->Command(i++)->Execute(true);
    }
    time.Stop();

    MILO_LOG(" %f ms\n", time.Ms());
    return time.Ms();
};

DEF_DATA_FUNC(DataRandomInt) { return RandomInt(array->Int(1), array->Int(2)); }

DEF_DATA_FUNC(DataRandomFloat) {
    if (array->Size() > 1) {
        return RandomFloat(array->Float(1), array->Float(2));
    } else {
        return RandomFloat();
    }
}

DEF_DATA_FUNC(DataRandomElem) {
    DataArray *a = array->Array(1);
    MILO_ASSERT_FMT(
        a->Size() != 0, "Empty array (file %s, line %d)", array->File(), array->Line()
    );
    return a->Node(RandomInt(0, a->Size()));
}

DEF_DATA_FUNC(DataRandom) { return array->Evaluate(RandomInt(1, array->Size())); }

DEF_DATA_FUNC(DataRandomSeed) {
    SeedRand(array->Int(1));
    return 0;
}

DEF_DATA_FUNC(DataRandomSort) {
    array->Array(1)->RandomSortNodes();
    return 0;
}

DEF_DATA_FUNC(DataWarn) {
    String str;
    for (int i = 1; i < array->Size(); i++) {
        array->Evaluate(i).Print(str, true, 0);
    }
    TheDebug.Warn(str.c_str());
    return 0;
}

DEF_DATA_FUNC(DataNotify) {
    String str;
    for (int i = 1; i < array->Size(); i++) {
        array->Evaluate(i).Print(str, true, 0);
    }
    TheDebug.Notify(str.c_str());
    return 0;
}

DEF_DATA_FUNC(DataFail) {
    String str;
    for (int i = 1; i < array->Size(); i++) {
        array->Evaluate(i).Print(str, true, 0);
    }
    TheDebug << MakeString("%d\n", array->Line());
    TheDebug.Fail(str.c_str(), nullptr);
    return 0;
}

DEF_DATA_FUNC(DataAssert) {
    bool pass = array->Evaluate(1).Int();
    if (!pass) {
        String str("");
        for (int i = 2; i < array->Size(); i++) {
            array->Evaluate(i).Print(str, true, 0);
        }
        MILO_FAIL("DataAssert %s (file %s, line %d)", str, array->File(), array->Line());
    }
    return 0;
}

DEF_DATA_FUNC(DataCond) {
    for (int i = 1; i < array->Size(); i++) {
        DataNode &n = array->Node(i);
        if (n.Type() == kDataArray) {
            DataArray *arr = n.UncheckedArray();
            if (arr->Node(0).NotNull()) {
                return arr->ExecuteScript(1, gDataThis, nullptr, 1);
            }
        } else {
            return array->ExecuteScript(i, gDataThis, nullptr, 1);
        }
    }
    return 0;
}

DEF_DATA_FUNC(DataSwitch) {
    const DataNode &match = array->Evaluate(1);
    for (int i = 2; i < array->Size(); i++) {
        DataNode &n = array->Node(i);
        if (n.Type() == kDataArray) {
            DataArray *arr = n.UncheckedArray();
            if (SwitchMatch(arr->Node(0), match)) {
                return arr->ExecuteScript(1, gDataThis, nullptr, 1);
            }
        } else {
            return array->ExecuteScript(i, gDataThis, nullptr, 1);
        }
    }
    return 0;
}

DEF_DATA_FUNC(DataInsertElems) {
    array->Array(1)->InsertNodes(array->Int(2), array->Array(3));
    return 0;
}

DEF_DATA_FUNC(DataInsertElem) {
    DataArray *arr = array->Array(1);
    int idx = array->Int(2);
    arr->Insert(idx, array->Evaluate(3));
    return 0;
}

DEF_DATA_FUNC(DataPrintArray) {
    array->Array(1)->Print(TheDebug, kDataArray, false, 0);
    return 0;
}

DEF_DATA_FUNC(DataArrayToString) {
    bool compact = true;
    if (array->Size() > 2)
        compact = array->Int(2);
    String str;
    array->Array(1)->Print(str, kDataArray, compact, 0);
    return MakeString("%s", str.c_str());
}

DEF_DATA_FUNC(DataSize) {
    if (array->Type(1) == kDataProperty) {
        MILO_ASSERT(gDataThis, 0x54D);
        return gDataThis->PropertySize(array->UncheckedArray(1));
    }
    return array->Array(1)->Size();
}

DEF_DATA_FUNC(DataRemoveElem) {
    DataArray *a = array->Array(1);
    a->Remove(array->Evaluate(2));
    return 0;
}

DEF_DATA_FUNC(DataResize) {
    array->Array(1)->Resize(array->Int(2));
    return 0;
}

DEF_DATA_FUNC(DataNewArray) {
    const DataNode &n = array->Evaluate(1);
    DataArrayPtr ptr;
    if (n.Type() == kDataInt) {
        ptr->Resize(n.LiteralInt());
    } else if (n.Type() == kDataArray) {
        ptr = n.LiteralArray()->Clone(true, true, 0);
    } else
        MILO_FAIL("DataNewArray wrong argument for %s %d", array->File(), array->Line());
    return ptr;
}

DEF_DATA_FUNC(DataSetElem) {
    DataArray *aaaa = array->Array(1);
    int i = array->Int(2);
    const DataNode &n = array->Evaluate(3);
    return aaaa->Node(i) = n;
}

DEF_DATA_FUNC(DataQuote) { return array->Node(1); }

DEF_DATA_FUNC(DataEval) { return array->Evaluate(1).Evaluate(); }

DEF_DATA_FUNC(DataReverseInterp) {
    float value = InverseLerp(array->Float(1), array->Float(2), array->Float(3));
    return Clamp(0.0f, 1.0f, value);
}

DEF_DATA_FUNC(DataInterp) {
    float st, end, pct;
    pct = array->Float(3);
    end = array->Float(2);
    st = array->Float(1);
    return Interp(st, end, pct);
}

DEF_DATA_FUNC(DataInc) {
    if (array->Type(1) == kDataProperty) {
        MILO_ASSERT(gDataThis, 0x596);
        DataArray *a = array->UncheckedArray(1);
        int x = gDataThis->Property(a, true)->Int() + 1;
        gDataThis->SetProperty(a, x);
        return x;

    } else {
        DataNode *Pn = array->Var(1);
        int i = Pn->Int();
        return *Pn = i + 1;
    }
}

DEF_DATA_FUNC(DataDec) {
    if (array->Type(1) == kDataProperty) {
        MILO_ASSERT(gDataThis, 0x5A7);
        DataArray *a = array->UncheckedArray(1);
        int x = gDataThis->Property(a, true)->Int() - 1;
        gDataThis->SetProperty(a, x);
        return x;

    } else {
        DataNode *Pn = array->Var(1);
        int i = Pn->Int();
        return *Pn = i - 1;
    }
}

DEF_DATA_FUNC(DataHandleType) {
    for (int i = 1; i < array->Size(); i++) {
        DataArray *arr = array->Array(i);
        const DataNode &n = arr->Evaluate(0);
        Hmx::Object *obj;
        if (n.Type() == kDataObject) {
            obj = n.UncheckedObj();
        } else {
            obj = gDataDir->FindObject(n.LiteralStr(array), true, true);
        }
        if (obj)
            obj->HandleType(arr);
    }
    return 0;
}

DEF_DATA_FUNC(DataHandleTypeRet) {
    DataArray *arr = array->Array(1);
    const DataNode &n = arr->Evaluate(0);
    Hmx::Object *obj;
    if (n.Type() == kDataObject) {
        obj = n.UncheckedObj();
    } else {
        obj = gDataDir->FindObject(n.LiteralStr(array), true, true);
    }
    if (!obj) {
        String str;
        n.Print(str, true, 0);
        MILO_FAIL(
            "Object %s not found (file %s, line %d)",
            str.c_str(),
            array->File(),
            array->Line()
        );
    }
    return obj->HandleType(arr);
}

DEF_DATA_FUNC(DataExport) {
    DataArray *a = array->Array(1);
    bool i = array->Int(2);
    const DataNode &n = a->Evaluate(0);
    Hmx::Object *obj;
    if (n.Type() == kDataObject)
        obj = n.UncheckedObj();
    else
        obj = gDataDir->FindObject(n.LiteralStr(array), true, true);
    if (obj)
        obj->Export(a, i);
    return 0;
}

DEF_DATA_FUNC(DataHandle) {
    for (int i = 1; i < array->Size(); i++) {
        DataArray *handlo = array->Array(i);
        const DataNode &n = handlo->Evaluate(0);
        Hmx::Object *obj;
        if (n.Type() == kDataObject)
            obj = n.UncheckedObj();
        else if (n.Type() == kDataInt)
            obj = nullptr;
        else
            obj = gDataDir->FindObject(n.LiteralStr(array), true, true);
        if (obj)
            obj->Handle(handlo, false);
    }
    return 0;
}

DEF_DATA_FUNC(DataHandleRet) {
    DataArray *a = array->Array(1);
    Hmx ::Object *o;
    const DataNode &n = a->Evaluate(0);
    if (n.Type() == kDataObject)
        o = n.UncheckedObj();
    else
        o = gDataDir->FindObject(n.LiteralStr(array), true, true);
    if (!o) {
        String str;
        n.Print(str, true, 0);
        MILO_FAIL(
            "Object %s not found (file %s, line %d)",
            str.c_str(),
            array->File(),
            array->Line()
        );
    }
    return o->Handle(a, false);
}

DEF_DATA_FUNC(DataRun) {
    const char *e = FileMakePath(FileExecRoot(), array->Str(1));
    DataArray *read = DataReadFile(e, true);
    DataNode ret;
    if (read) {
        ret = read->ExecuteScript(0, gDataThis, nullptr, 1);
        read->Release();
    }
    return ret;
}

DEF_DATA_FUNC(OnReadFile) {
    DataArray *read = DataReadFile(array->Str(1), true);
    if (read == 0) {
        return 0;
    } else {
        DataNode dn(read, kDataArray);
        read->Release();
        return dn;
    }
}

DEF_DATA_FUNC(OnWriteFile) {
    DataArray *write_me = array->Array(2);
    DataWriteFile(array->Str(1), write_me, false);
    return 0;
}

DEF_DATA_FUNC(OnWriteStringToFile) {
    bool i2 = array->Size() > 3 ? array->Int(3) : true;
    TextFileStream *stream = new TextFileStream(array->Str(1), i2);
    *stream << array->Str(2);
    delete stream;
    return 0;
}

DEF_DATA_FUNC(OnFileExists) { return FileExists(array->Str(1), 0, nullptr); }

DEF_DATA_FUNC(OnFileMkDir) { return FileMkDir(array->Str(1)); }

DEF_DATA_FUNC(OnFileReadOnly) { return FileReadOnly(array->Str(1)); }

DEF_DATA_FUNC(DataExit) {
    TheDebug.Exit(0, true);
    return 0;
}

DEF_DATA_FUNC(DataContains) {
    DataArray *w = array->Array(1);
    const DataNode &n = array->Evaluate(2);
    bool b = !w->Contains(n.UncheckedInt());
    if (b)
        return DataNode(kDataUnhandled, 0);
    else
        return 1;
}

DataNode DataFindExists(DataArray *array, bool fail) {
    DataArray *arr = array->Array(1);
    for (int i = 2; i < array->Size(); i++) {
        const DataNode &n = array->Evaluate(i);
        if (n.Type() == kDataInt || n.Type() == kDataSymbol) {
            arr = arr->FindArray(n.UncheckedInt(), false);
            if (!arr) {
                if (fail) {
                    String str;
                    n.Print(str, true, 0);
                    MILO_FAIL(
                        "Failed to find %s (file %s, line %d)",
                        str.c_str(),
                        array->File(),
                        array->Line()
                    );
                }
                return DataNode(kDataUnhandled, 0);
            }
        } else {
            String str;
            n.Print(str, true, 0);
            MILO_FAIL(
                "Bad key %s (file %s, line %d)", str.c_str(), array->File(), array->Line()
            );
        }
    }
    return DataNode(arr, kDataArray);
}

DEF_DATA_FUNC(DataFindExists) { return DataFindExists(array, false); }

DEF_DATA_FUNC(DataFind) { return DataFindExists(array, true); }

DEF_DATA_FUNC(DataFindObj) {
    class ObjectDir *d = ObjectDir::Main();
    int i;
    for (i = 1; i < array->Size() - 1; i++) {
        const DataNode &n = array->Evaluate(i);
        if (n.Type() == kDataObject)
            d = n.Obj<ObjectDir>();
        else {
            const char *name = n.LiteralStr();
            d = d->Find<ObjectDir>(name, false);
        }
        if (!d)
            return d;
    }
    return d->Find<Hmx::Object>(array->Str(i), false);
}

DEF_DATA_FUNC(DataBasename) { return FileGetBase(array->Str(1)); }

DEF_DATA_FUNC(DataDirname) {
    const char *s = FileGetPath(array->Str(1));
    unsigned int x = String(s).find_last_of("/");
    return s + (x == FixedString::npos ? 0 : x + 1);
}

DEF_DATA_FUNC(DataHasSubStr) { return (int)strstr(array->Str(1), array->Str(2)); }

DEF_DATA_FUNC(DataHasAnySubStr) {
    DataArray *a = array->Array(2);
    for (int i = 0; i < a->Size(); i++) {
        const char *haystack = a->Str(i);
        if (strstr(array->Str(1), haystack))
            return 1;
    }
    return 0;
}

DEF_DATA_FUNC(DataFindSubStr) {
    String s(array->Str(1));
    return (int)s.find(array->Str(2));
}

DEF_DATA_FUNC(DataSubStr) {
    String s(array->Str(1));
    int i1 = array->Int(2);
    int i2 = array->Int(3);
    String substr = s.substr(i1, i2);
    return substr;
}

DEF_DATA_FUNC(DataStrCat) {
    DataNode &n = *array->Var(1);
    String s(n.Str());
    for (int i = 2; i < array->Size(); i++) {
        s += array->Str(i);
    }
    n = s.c_str();
    return n.Str();
}

DEF_DATA_FUNC(DataStringFlags) {
    int mask = array->Int(1);
    DataArray *arr = array->Array(2);
    String s("");
    for (int i = 0; i < arr->Size(); i++) {
        DataArray *macro = DataGetMacro(arr->Str(i));
        MILO_ASSERT(macro && macro->Size() == 1, 0x700);
        if (macro->Int(0) & mask) {
            if (s != "") {
                s += "|";
            }
            s += arr->Str(i);
        }
    }
    return s;
}

DEF_DATA_FUNC(DataStrToLower) {
    String s = array->Str(1);
    s.ToLower();
    return s;
}

DEF_DATA_FUNC(DataStrToUpper) {
    String s = array->Str(1);
    s.ToUpper();
    return s;
}

DEF_DATA_FUNC(DataStrlen) { return (int)strlen(array->Str(1)); }

DEF_DATA_FUNC(DataStrElem) {
    char s[2] = "A";
    s[0] = array->Str(1)[array->Int(2)];
    return (Symbol)s;
}

DEF_DATA_FUNC(DataStrieq) { return strieq(array->Str(1), array->Str(2)); }

DEF_DATA_FUNC(DataSearchReplace) {
    const char *s3 = array->Str(3);
    const char *s2 = array->Str(2);
    const char *s1 = array->Str(1);
    char beeg[0x800];
    bool ret = SearchReplace(s1, s2, s3, beeg);
    *array->Var(4) = beeg;
    return ret;
}

DEF_DATA_FUNC(DataFlatten) {
    int numNodes = 0;
    for (int i = 1; i < array->Size(); i++) {
        const DataNode &n = array->Evaluate(i);
        numNodes += n.Type() == kDataArray ? n.Array()->Size() : 1;
    }
    DataArrayPtr ptr(new DataArray(numNodes));
    int idx = 0;
    for (int i = 1; i < array->Size(); i++) {
        const DataNode &n = array->Evaluate(i);
        if (n.Type() == kDataArray) {
            for (int j = 0; j < n.Array()->Size(); j++) {
                ptr->Node(idx++) = n.Array()->Node(j);
            }
        } else {
            ptr->Node(idx++) = n;
        }
    }
    return ptr;
}

DEF_DATA_FUNC(DataPushBack) {
    DataArray *work = array->Array(1);
    int nu_size = work->Size();
    work->Resize(nu_size + 1);
    work->Node(nu_size) = array->Evaluate(2);
    return 0;
}

DEF_DATA_FUNC(DataSort) {
    array->Array(1)->SortNodes(0);
    return 0;
}

DEF_DATA_FUNC(DataGetType) { return array->Evaluate(1).Type(); }

DEF_DATA_FUNC(DataWith) {
    return array->ExecuteScript(2, array->Obj<Hmx::Object>(1), NULL, 1);
}

DEF_DATA_FUNC(OnSetThis) {
    Hmx::Object *new_this = array->Obj<Hmx::Object>(1);
    gDataThisPtr.SetObjConcrete(new_this);
    DataSetThis(new_this);
    return 0;
}

DEF_DATA_FUNC(DataMacroElem) {
    DataArray *macro = DataGetMacro(array->Str(1));
    int i = array->Size() > 2 ? array->Int(2) : 0;
    MILO_ASSERT(macro && macro->Size() > i, 0x797);
    return macro->Node(i);
}

DEF_DATA_FUNC(DataMacroSize) {
    DataArray *macro = DataGetMacro(array->Str(1));
    MILO_ASSERT(macro, 0x79E);
    return macro->Size();
}

DEF_DATA_FUNC(DataReplaceObject) {
    bool copyDeep = array->Size() > 3 ? array->Int(3) : true;
    bool deleteFrom = array->Size() > 4 ? array->Int(4) : true;
    bool setProxyFile = array->Size() > 5 ? array->Int(5) : true;
    ReplaceObject(array->GetObj(1), array->GetObj(2), copyDeep, deleteFrom, setProxyFile);
    return 0;
}

DEF_DATA_FUNC(DataNextName) {
    class ObjectDir *d = gDataDir;
    if (array->Size() > 2) {
        d = array->Obj<class ObjectDir>(2);
    }
    return NextName(array->Str(1), d);
}

DataNode Quasiquote(const DataNode &node) {
    static Symbol unquote("unquote");
    static Symbol unquoteAbbrev(",");
    DataType nodeType = node.Type();
    if (nodeType == kDataArray || nodeType == kDataCommand) {
        DataArray *nodeArr = node.UncheckedArray();

        if (nodeType == kDataCommand && nodeArr->Type(0) == kDataSymbol) {
            const char *str = nodeArr->UncheckedStr(0);
            Symbol sym = STR_TO_SYM(str);
            if (sym == unquote || sym == unquoteAbbrev) {
                return nodeArr->Evaluate(1);
            }
        }

        DataArrayPtr ptr(new DataArray(nodeArr->Size()));
        for (int i = 0; i < nodeArr->Size(); i++) {
            ptr->Node(i) = Quasiquote(nodeArr->Node(i));
        }

        return DataNode(ptr, nodeType);
    } else {
        return node;
    }
}

DEF_DATA_FUNC(DataQuasiquote) { return Quasiquote(array->Node(1)); }

DEF_DATA_FUNC(DataUnquote) { return array->Evaluate(1); }
