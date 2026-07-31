#define LOG_TAG "Fullbright"
#include <android/log_macros.h>
#include <dlfcn.h>
#include <libhat/process.hpp>
#include <libhat/scanner.hpp>

struct MenuEntryABI {
    const char* name;
    void*       user             = nullptr;
    bool (*selected)(void* user) = [](void*) { return false; };
    void (*click)(void* user)    = nullptr;
    size_t        length         = 0;
    MenuEntryABI* subentries     = nullptr;
};

static bool enabled = true;

extern "C" [[gnu::visibility("default")]] void mod_preinit() {
    auto menuLib = dlopen("libmcpelauncher_menu.so", 0);
    auto addMenu = reinterpret_cast<void (*)(size_t, MenuEntryABI*)>(dlsym(menuLib, "mcpelauncher_addmenu"));

    MenuEntryABI menuEntry{
        .name     = "Enable Fullbright",
        .selected = [](void*) { return enabled; },
        .click    = [](void*) { enabled = !enabled; },
    };

    addMenu(1, &menuEntry);
}

[[nodiscard]] static void* find_vtable(std::string_view name, const hat::process::module& mod) {
    auto result = hat::find_pattern(hat::string_to_signature(name).value(), ".rodata", mod);
    if (!result.has_result())
        return nullptr;
    result = hat::find_pattern(hat::object_to_signature(result.get()), ".data.rel.ro", mod);
    if (!result.has_result())
        return nullptr;
    result = hat::find_pattern(hat::object_to_signature(result.get() - sizeof(void*)), ".data.rel.ro", mod);
    return result.has_result() ? result.get() + sizeof(void*) : nullptr;
}

struct BaseLightData {
    float mSunriseColor[4];
    float mGamma;
    float mSkyDarken;
    int   mDimensionType;
    float mDarkenWorldAmount;
    float mPreviousDarkenWorldAmount;
    bool  mNightvisionActive;
    float mNightvisionScale;
    bool  mUnderwaterVision;
    float mUnderwaterScale;
    int   mSkyFlashTime;
    float mDarknessFactor;
    float mDarknessFactorPreviousFrame;
};

template<typename = decltype([] {})>
static void hook(std::string_view name, const hat::process::module& mod) {
    auto vt = find_vtable(name, mod);

    if (!vt) {
        ALOGE("Vtable not found (%.*s)", static_cast<int>(name.size()), name.data());
        return;
    }

    auto fn = static_cast<std::unique_ptr<BaseLightData> (**)(void*, void*, const BaseLightData&)>(vt) + 5;

    static auto original = *fn;

    *fn = [](void* self, void* client, const BaseLightData& currentData) {
        auto ret = original(self, client, currentData);

        if (enabled) {
            ret->mNightvisionActive = true;
            ret->mNightvisionScale  = 1.0f;
        }

        return ret;
    };
}

extern "C" [[gnu::visibility("default")]] void mod_init() {
    auto mcLib = hat::process::get_module("libminecraftpe.so").value();

    hook("33OverworldLightTextureImageBuilder", mcLib);
    hook("30NetherLightTextureImageBuilder", mcLib);
    hook("30TheEndLightTextureImageBuilder", mcLib);
}
