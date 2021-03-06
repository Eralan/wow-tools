
#ifndef UpdateFieldDumper_h__
#define UpdateFieldDumper_h__

#include <cstdint>
#include <string>
#include <unordered_map>
#include <fstream>
#include <iomanip>
#include <Windows.h>
#include "ProcessTools/ProcessTools.h"

#pragma pack(push,4)

enum UpdateFieldSizes
{
    OBJECT_COUNT                = 12,
    OBJECT_DYNAMIC_COUNT        = 0,
    ITEM_COUNT                  = 70,
    ITEM_DYNAMIC_COUNT          = 2,
    CONTAINER_COUNT             = 145,
    CONTAINER_DYNAMIC_COUNT     = 0,
    UNIT_COUNT                  = 191,
    UNIT_DYNAMIC_COUNT          = 2,
    PLAYER_COUNT                = 2473,
    PLAYER_DYNAMIC_COUNT        = 6,
    GAMEOBJECT_COUNT            = 21,
    GAMEOBJECT_DYNAMIC_COUNT    = 0,
    DYNAMICOBJECT_COUNT         = 8,
    DYNAMICOBJECT_DYNAMIC_COUNT = 0,
    CORPSE_COUNT                = 33,
    CORPSE_DYNAMIC_COUNT        = 0,
    AREATRIGGER_COUNT           = 8,
    AREATRIGGER_DYNAMIC_COUNT   = 0,
    SCENEOBJECT_COUNT           = 7,
    SCENEOBJECT_DYNAMIC_COUNT   = 0,
    CONVERSATION_COUNT          = 1,
    CONVERSATION_DYNAMIC_COUNT  = 2
};

struct UpdateField
{
    std::uint32_t NameAddress;
    std::uint32_t Size;
    std::uint16_t Flags;
    std::uint16_t __PADDING__;
};

struct DynamicUpdateField
{
    std::uint32_t NameAddress;
    std::uint16_t Flags;
    std::uint16_t __PADDING__;
};

class Data
{
public:
    Data(HANDLE wow);
    std::string ReadProcessMemoryCString(DWORD_PTR address);

    UpdateField ObjectFields[OBJECT_COUNT];
    UpdateField ItemFields[ITEM_COUNT];
    DynamicUpdateField ItemDynamicFields[ITEM_DYNAMIC_COUNT];
    UpdateField ContainerFields[CONTAINER_COUNT];
    UpdateField UnitFields[UNIT_COUNT];
    DynamicUpdateField UnitDynamicFields[UNIT_DYNAMIC_COUNT];
    UpdateField PlayerFields[PLAYER_COUNT];
    DynamicUpdateField PlayerDynamicFields[PLAYER_DYNAMIC_COUNT];
    UpdateField GameObjectFields[GAMEOBJECT_COUNT];
    UpdateField DynamicObjectFields[DYNAMICOBJECT_COUNT];
    UpdateField CorpseFields[CORPSE_COUNT];
    UpdateField AreaTriggerFields[AREATRIGGER_COUNT];
    UpdateField SceneObjectFields[SCENEOBJECT_COUNT];
    UpdateField ConversationFields[CONVERSATION_COUNT];
    DynamicUpdateField ConversationDynamicFields[CONVERSATION_DYNAMIC_COUNT];

private:
    std::unordered_map<DWORD_PTR, std::string> _stringCache;
    HANDLE _process;
};

namespace Offsets
{
    extern DWORD_PTR BaseAddress;
}

#pragma pack(pop)

class UpdateFieldDumper
{
public:
    UpdateFieldDumper(HANDLE source, Data* input, FileVersionInfo const& version) : _source(source), _input(input), _versionInfo(version) { }
    virtual ~UpdateFieldDumper() { }

    virtual void Dump() = 0;

    static std::string const Tab;

protected:
    virtual void DumpUpdateFields(std::ofstream& file, std::string const& name, UpdateField* data, UpdateFieldSizes count, std::string const& end, std::string const& fieldBase) = 0;
    virtual void DumpDynamicFields(std::ofstream& file, std::string const& name, DynamicUpdateField* data, UpdateFieldSizes count, std::string const& end, std::string const& fieldBase) = 0;
    std::string FormatVersion(std::string const& partSeparator) const;

    Data* GetInputData() { return _input; }
    FileVersionInfo const& GetVersionInfo() const { return _versionInfo; }
    static std::string GetUpdateFieldFlagName(std::uint16_t flag);
    static std::string GetUpdateFieldFlagFullName(std::uint16_t flag);

private:
    static void AppendIf(std::uint16_t flag, std::uint16_t flagToCheck, std::string& str, std::string const& flagName, std::string const& separator);

    HANDLE _source;
    Data* _input;
    FileVersionInfo _versionInfo;
};

struct hex_number
{
    explicit hex_number(std::uint32_t v) : Value(v) { }
    std::uint32_t Value;
};

std::ostream& operator<<(std::ostream& stream, hex_number const& hex);

#endif // UpdateFieldDumper_h__
