#include "obj/Data.h"
#include "obj/Object.h"
#include "os/Debug.h"
#include "utl/BinStream.h"
#include "utl/MemMgr.h"
#include "utl/Str.h"

DataNode *NodesAlloc(int size) {
    return (DataNode *)MemOrPoolAlloc(size, __FILE__, 0xFE, "Nodes");
}

void NodesFree(int size, DataNode *mem) { MemOrPoolFree(size, mem); }

bool DataArray::PrintUnused(TextStream &ts, DataType ty, bool b) const {
    bool ret = false;
    for (int i = 0; i < mSize; i++) {
        ret |= mNodes[i].PrintUnused(ts, b);
    }
    return ret;
}

DataArray::~DataArray() {
    int i2;
    if (mSize < 0) {
        i2 = -mSize;
    } else {
        for (int i = 0; i < mSize; i++) {
            mNodes[i].~DataNode();
        }
        i2 = mSize * 8;
    }
    NodesFree(i2, mNodes);
}

void DataArray::SetFileLine(Symbol file, int line) {
    mFile = file;
    mLine = line;
}

void DataArray::SetFile(Symbol file) { gFile = file; }

bool strncat_tofit(FixedString &str, const char *cc, int i) {
    if ((int)(strlen(str.c_str()) + strlen(cc)) < i) {
        str += cc;
        return true;
    } else
        return false;
}

DataNode &DataArray::Node(int i) const {
    MILO_ASSERT_FMT(
        i >= 0 && i < mSize,
        "Array doesn't have node %d, only has %d (file %s, line %d)",
        i,
        mSize,
        File(),
        Line()
    );
    return mNodes[i];
}

DataNode &DataArray::Node(int i) {
    MILO_ASSERT_FMT(
        i >= 0 && i < mSize,
        "Array doesn't have node %d, only has %d (file %s, line %d)",
        i,
        mSize,
        File(),
        Line()
    );
    return mNodes[i];
}

void DataArray::Print(TextStream &ts, DataType type, bool b, int i3) const {
    DataNode *end = &mNodes[mSize];
    MILO_ASSERT(type & kDataArray, 0xA3);
    char open = '\0';
    char close = '\0';
    if (type == kDataArray) {
        open = '(';
        close = ')';
    } else if (type == kDataCommand) {
        open = '{';
        close = '}';
    } else if (type == kDataProperty) {
        open = '[';
        close = ']';
    } else {
        MILO_FAIL("Unrecognized array type %d", type);
    }

    DataNode *n;
    for (n = mNodes; n < end && !(n->Type() & kDataArray); n++)
        ;

    if (n == end || b) {
        ts << open;
        for (n = mNodes; n < end; n++) {
            if (n != mNodes) {
                ts << " ";
            }
            n->Print(ts, b, i3);
        }
    } else {
        ts << open;
        n = mNodes;
        if (n->Type() == kDataSymbol) {
            n->Print(ts, b, i3);
            n++;
        }
        ts << "\n";
        i3 += 3;
        for (; n < end; n++) {
            ts.Space(i3);
            n->Print(ts, b, i3);
            ts << "\n";
        }
        i3 -= 3;
        ts.Space(i3);
    }
    ts << close;
}

void DataArray::Insert(int count, const DataNode &dn) {
    int i = 0;
    int newNodeCount = mSize + 1;
    DataNode *oldNodes = mNodes; // Save all nodes pointer
    // allocate new nodes
    mNodes = NodesAlloc(newNodeCount * sizeof(DataNode));

    for (i = 0; i < count; i++) {
        new (&mNodes[i]) DataNode(oldNodes[i]);
    }
    for (; i < count + 1; i++) {
        new (&mNodes[i]) DataNode(dn);
    }
    for (; i < newNodeCount; i++) {
        new (&mNodes[i]) DataNode(oldNodes[i - 1]);
    }
    for (i = 0; i < mSize; i++) {
        oldNodes[i].~DataNode();
    }

    // free old nodes
    NodesFree(mSize * sizeof(DataNode), oldNodes);
    mSize = newNodeCount;
}

void DataArray::InsertNodes(int count, const DataArray *da) {
    if ((da == 0) || (da->Size() == 0))
        return;
    int i = 0;
    int dacnt = da->Size();
    int newNodeCount = mSize + dacnt;
    DataNode *oldNodes = mNodes; // Save all nodes pointer
    // allocate new nodes
    mNodes = (DataNode *)NodesAlloc(newNodeCount * sizeof(DataNode));

    for (i = 0; i < count; i++) {
        new (&mNodes[i]) DataNode(oldNodes[i]);
    }

    for (; i < count + dacnt; i++) {
        new (&mNodes[i]) DataNode(da->Node(i - count));
    }

    for (; i < newNodeCount; i++) {
        new (&mNodes[i]) DataNode(oldNodes[i - dacnt]);
    }
    for (i = 0; i < mSize; i++) {
        oldNodes[i].~DataNode();
    }
    NodesFree(mSize * sizeof(DataNode), oldNodes);
    mSize = newNodeCount;
}

void DataArray::Resize(int i) {
    DataNode *oldNodes = mNodes;
    mNodes = (DataNode *)NodesAlloc(i * sizeof(DataNode));
    int min = Min<int>(mSize, i);
    int cnt = 0;
    for (cnt = 0; cnt < min; cnt++) {
        new (&mNodes[cnt]) DataNode(oldNodes[cnt]);
    }
    for (; cnt < i; cnt++) {
        new (&mNodes[cnt]) DataNode();
    }
    for (cnt = 0; cnt < mSize; cnt++) {
        oldNodes[cnt].~DataNode();
    }
    NodesFree(mSize * sizeof(DataNode), oldNodes);
    mSize = i;
    mDeprecated = 0;
}

void DataArray::Remove(int index) {
    MILO_ASSERT(index < mSize, 0x16E);
    DataNode *oldNodes = mNodes;
    int newCnt = mSize - 1;
    mNodes = NodesAlloc(newCnt * sizeof(DataNode));
    int cnt = 0;
    for (cnt = 0; cnt < index; cnt++) {
        new (&mNodes[cnt]) DataNode(oldNodes[cnt]);
    }
    for (; index < newCnt; index++) {
        new (&mNodes[index]) DataNode(oldNodes[index + 1]);
    }
    for (int j = 0; j < mSize; j++) {
        oldNodes[j].~DataNode();
    }
    NodesFree(mSize * sizeof(DataNode), oldNodes);
    mSize = newCnt;
}

void DataArray::Remove(const DataNode &dn) {
    int searchType = dn.UncheckedInt();
    for (int lol = mSize - 1; lol >= 0; lol--) {
        if (mNodes[lol].UncheckedInt() == searchType) {
            Remove(lol);
            return;
        }
    }
}

bool DataArray::Contains(const DataNode &dn) const {
    int searchType = dn.UncheckedInt();
    for (int lol = mSize - 1; lol >= 0; lol--) {
        if (mNodes[lol].UncheckedInt() == searchType) {
            return true;
        }
    }
    return false;
}

DataArray::DataArray(int size)
    : mFile(), mSize(size), mRefs(1), mLine(0), mDeprecated(0) {
    mNodes = NodesAlloc(size * 8);
    for (int n = 0; n < size; n++) {
        new (&mNodes[n]) DataNode();
    }
}

DataArray::DataArray(const void *glob, int size)
    : mFile(), mSize(-size), mRefs(1), mLine(0), mDeprecated(0) {
    mNodes = NodesAlloc(size);
    memcpy(mNodes, glob, size);
}

int NodeCmp(const void *a, const void *b) {
    const DataNode *anode = (const DataNode *)a;
    const DataNode *bnode = (const DataNode *)b;
    switch (anode->Type()) {
    case kDataFloat:
    case kDataInt: {
        float a = anode->LiteralFloat();
        float b = bnode->LiteralFloat();
        if (a < b)
            return -1;
        return a != b;
    }
    case kDataString:
    case kDataSymbol:
        return stricmp(anode->Str(), bnode->Str());
    case kDataArray:
        return NodeCmp(&anode->Array()->Node(0), &bnode->Array()->Node(0));
    case kDataObject: {
        const char *a = anode->GetObj() ? anode->GetObj()->Name() : "";
        const char *b = bnode->GetObj() ? bnode->GetObj()->Name() : "";
        return stricmp(a, b);
    }
    default:
        MILO_WARN("could not sort array, bad type");
        return 0;
    }
}

void DataArray::SortNodes(int idx) {
    if (mSize <= 0)
        return;
    if (idx >= mSize)
        return;
    qsort(&mNodes[idx], mSize - idx, 8, NodeCmp);
}

void DataArrayGlitchCB(float f, void *v) {
    DataArray *arr = (DataArray *)v;
    arr->Node(0).Print(TheDebug, true, 0);
    MILO_LOG(" took %.2f ms (File: %s Line: %d)\n", f, arr->File(), arr->Line());
}

void DataArray::Save(BinStream &bs) const {
    bs << mSize << mLine << mDeprecated;
    for (int i = 0; i < mSize; i++) {
        mNodes[i].Save(bs);
    }
}

void DataArray::SaveGlob(BinStream &bs, bool b) const {
    if (b) {
        int i = -1 - mSize;
        bs << i;
        bs.Write(mNodes, i);
    } else {
        bs << mSize;
        bs.Write(mNodes, -mSize);
    }
}

void DataArray::LoadGlob(BinStream &bs, bool b) {
    MILO_ASSERT(mSize <= 0, 0x4A8);
    NodesFree(-mSize, mNodes);
    if (b) {
        int v;
        bs >> v;
        mSize = -(v + 1);
        mNodes = NodesAlloc(-mSize);
        bs.Read(mNodes, v);
        ((unsigned char *)mNodes)[v] = 0;
    } else {
        bs >> mSize;
        mNodes = NodesAlloc(-mSize);
        bs.Read(mNodes, -mSize);
    }
}

TextStream &operator<<(TextStream &ts, const DataArray *da) {
    if (da)
        da->Print(ts, kDataArray, false, 0);
    else
        ts << "<null>";
    return ts;
}

BinStream &operator<<(BinStream &bs, const DataArray *da) {
    if (da != 0) {
        bs << true;
        da->Save(bs);
    } else
        bs << false;
    return bs;
}

DataArray *DataArray::FindArray(int tag, bool fail) const {
    DataNode *dn;
    DataNode *dn_end = &mNodes[mSize];
    for (dn = mNodes; dn < dn_end; dn++) {
        if (dn->Type() == kDataArray) {
            const DataArray *arr = dn->UncheckedArray();
            if (arr->UncheckedInt(0) == tag) {
                return (DataArray *)arr;
            }
        }
    }
    if (fail)
        MILO_FAIL("Couldn't find %d in array (file %s, line %d)", tag, File(), mLine);
    return nullptr;
}

DataArray *DataArray::FindArray(Symbol tag, bool fail) const {
    DataArray *found = FindArray((int)tag, false);
    if (!found && fail)
        MILO_FAIL(
            "Couldn't find '%s' in array (file %s, line %d)", tag.Str(), File(), mLine
        );
    return found;
}

DataArray *DataArray::FindArray(Symbol s1, Symbol s2) const {
    return FindArray(s1)->FindArray(s2);
}

DataArray *DataArray::FindArray(Symbol s1, Symbol s2, Symbol s3) const {
    return FindArray(s1)->FindArray(s2)->FindArray(s3);
}

DataArray *DataArray::FindArray(Symbol s, const char *c) const {
    return FindArray(s, Symbol(c));
}

bool DataArray::FindData(Symbol s, const char *&ret, bool b) const {
    DataArray *arr = FindArray(s, b);
    if (arr != 0) {
        ret = arr->Str(1);
        return true;
    } else {
        return false;
    }
}

bool DataArray::FindData(Symbol s, Symbol &ret, bool b) const {
    DataArray *arr = FindArray(s, b);
    if (arr != nullptr) {
        ret = (arr->Sym(1));
        return true;
    } else {
        return false;
    }
}

bool DataArray::FindData(Symbol s, String &ret, bool b) const {
    const char *c;
    bool found = FindData(s, c, b);
    if (found) {
        ret = c;
        return true;
    } else {
        return false;
    }
}

bool DataArray::FindData(Symbol s, int &ret, bool b) const {
    DataArray *arr = FindArray(s, b);
    if (arr != nullptr) {
        ret = arr->Int(1);
        return true;
    } else {
        return false;
    }
}

bool DataArray::FindData(Symbol s, float &ret, bool b) const {
    DataArray *arr = FindArray(s, b);
    if (arr != nullptr) {
        ret = arr->Float(1);
        return true;
    } else {
        return false;
    }
}

bool DataArray::FindData(Symbol s, bool &ret, bool b) const {
    DataArray *arr = FindArray(s, b);
    if (arr != nullptr) {
        ret = arr->Int(1);
        return true;
    } else {
        return false;
    }
}

DataArray *DataArray::Clone(bool deep, bool eval, int extra) {
    DataArray *da = new DataArray(mSize + extra);
    for (int i = 0; i < mSize; i++) {
        da->mNodes[i] = (eval) ? mNodes[i].Evaluate() : mNodes[i];
        if (deep) {
            if (da->mNodes[i].Type() == kDataArray) {
                DataArray *cloned = da->mNodes[i].LiteralArray()->Clone(true, eval, 0);
                da->mNodes[i] = DataNode(cloned, kDataArray);
                cloned->Release();
            }
        }
    }
    return da;
}

DataNode DataArray::ExecuteBlock(int len) {
    for (; len < mSize - 1; len++) {
        Command(len)->Execute(true);
    }
    return Evaluate(len);
}

BinStream &operator>>(BinStream &bs, DataArray *&da) {
    bool b;
    bs >> b;
    if (b) {
        da = new DataArray(0);
        da->Load(bs);
    } else
        da = nullptr;
    return bs;
}
