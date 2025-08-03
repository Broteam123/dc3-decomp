#include "obj/TextFile.h"
#include <cstring>

void TextFile::SetName(const char *c, class ObjectDir *dir) {
    Hmx::Object::SetName(c, dir);
    RELEASE(mFile);
    if (c && *c) {
        const char *s = strstr(c, "_append");
        if (s) {
            char buf[0x100];
            strcpy(buf, c);
            char *ptr = &buf[s - c];
            int tokLen = sizeof("_append") - 1;
            strncpy(ptr, ptr + tokLen, strlen(s) - (tokLen - 1));
            mFile = NewFile(buf, 0x304);
        } else {
            mFile = NewFile(c, 0xA04);
        }
    }
}

void TextFile::Print(const char *str) {
    MILO_ASSERT(mFile, 0x2F);
    char kCRLF[2] = { '\r', '\n' };
    char *p = (char *)str;
    while (*p != '\0') {
        if (*p == '\n' && p[1] != '\r') {
            mFile->Write(kCRLF, 2);
        } else
            mFile->Write(p, 1);
        mFile->Flush();
        p++;
    }
}

DataNode TextFile::OnPrint(DataArray *array) {
    if (mFile) {
        for (int i = 2; i < array->Size(); i++) {
            array->Evaluate(i).Print(*this, true, 0);
        }
    }
    return 0;
}

DataNode TextFile::OnPrintf(DataArray *array) {
    if (mFile) {
        FormatString fs(array->Str(2));
        for (int i = 3; i < array->Size(); i++) {
            fs << array->Evaluate(i);
        }
        Print(fs.Str());
    }
    return 0;
}

DataNode TextFile::OnReflect(DataArray *array) {
    if (mFile) {
        TextStream *idk = TheDebug.SetReflect(this);
        for (int i = 2; i < array->Size(); i++) {
            array->Command(i)->Execute(true);
        }
        TheDebug.SetReflect(idk);
    }
    return 0;
}

BEGIN_HANDLERS(TextFile)
    HANDLE(print, OnPrint)
    HANDLE(printf, OnPrintf)
    HANDLE(reflect, OnReflect)
    HANDLE_SUPERCLASS(Hmx::Object)
END_HANDLERS
