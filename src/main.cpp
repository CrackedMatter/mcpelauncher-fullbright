#include <dlfcn.h>
#include <safetyhook.hpp>
#include <string>
#include "sig.hpp"

SafetyHookInline FloatOption_ctor_hook;

void FloatOption_ctor(void* self, int a, int b, int c, void* d, std::string& saveTag, float f, float g, float max, float i) {
    if (saveTag == "gfx_gamma")
        max = 10.0f;
    FloatOption_ctor_hook.call(self, a, b, c, d, saveTag, f, g, max, i);
}

void FloatOption_ctor_old(void* self, int a, int b, int c, void* d, std::string& saveTag, float f, float g, float max) {
    if (saveTag == "gfx_gamma")
        max = 10.0f;
    FloatOption_ctor_hook.call(self, a, b, c, d, saveTag, f, g, max);
}

extern "C" __attribute__ ((visibility ("default"))) void mod_preinit() {}

extern "C" __attribute__ ((visibility ("default"))) void mod_init() {
    auto r = getCodeRegion(dlopen("libminecraftpe.so", 0));

    if (auto addr = findSig(r, "55 41 57 41 56 41 55 41 54 53 48 83 EC 48 F3 0F 11 5C 24 10"))
        FloatOption_ctor_hook = safetyhook::create_inline(addr, reinterpret_cast<void*>(FloatOption_ctor));
    else if ((addr = findSig(r, "55 41 57 41 56 41 55 41 54 53 48 83 EC 48 0F 29 54 24 30")))
        FloatOption_ctor_hook = safetyhook::create_inline(addr, reinterpret_cast<void*>(FloatOption_ctor_old));
}
