#include "ui/ResourceDirPtr.h"
#include "obj/Data.h"
#include "obj/Utl.h"
#include "os/File.h"
#include "os/System.h"
#include "utl/FilePath.h"
#include "utl/MakeString.h"

const char *ResourceDirBase::GetResourcesPath(Symbol s1, Symbol s2) {
    DataArray *pathArr = nullptr;
    std::vector<Symbol> superClasses;
    superClasses.push_back(s1);
    ListSuperClasses(s1, superClasses);
    static Symbol objects("objects");
    static Symbol resources_path("resources_path");
    static DataArray *cfg = SystemConfig(objects);
    const char *path = nullptr;
    int numSuperClasses = superClasses.size();
    for (int i = 0; i < numSuperClasses; i++) {
        DataArray *curClassCfg = cfg->FindArray(superClasses[i], true);
        if (curClassCfg) {
            pathArr = curClassCfg->FindArray(resources_path, false);
            if (pathArr) {
                for (int j = 1; j < pathArr->Size(); j++) {
                    const DataNode &n = pathArr->Evaluate(j);
                    if (n.Type() == kDataArray) {
                        if (n.Array()->Sym(0) == s2) {
                            path = n.Array()->Str(1);
                            if (path && *path)
                                return FileMakePath(FileGetPath(pathArr->File()), path);
                        }
                    } else {
                        path = n.Str();
                    }
                }
            }
        }
    }
    return nullptr;
}

bool ResourceDirBase::MakeResourcePath(
    FilePath &fp, Symbol s1, Symbol s2, const char *cc
) {
    if (*cc) {
        fp.Set(FileRoot(), MakeString("%s/%s.milo", GetResourcesPath(s1, s2), cc));
        return true;
    } else {
        fp.Set(FilePath::Root().c_str(), "");
        return false;
    }
}

DataNode ResourceDirBase::GetFileList(Symbol s1, Symbol s2) {
    const char *resourcePath = GetResourcesPath(s1, s2);
    if (*resourcePath) {
        return MakeFileList(MakeString("%s/*.milo", resourcePath), true, nullptr);
    } else {
        DataArrayPtr ptr(new DataArray(1));
        ptr->Node(0) = "";
        return ptr;
    }
}
