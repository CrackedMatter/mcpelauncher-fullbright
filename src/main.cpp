#include <cstddef>
#include <dlfcn.h>
#include <libhat/scanner.hpp>
#include <link.h>
#include <safetyhook.hpp>
#include <span>
#include <string>

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

extern "C" [[gnu::visibility("default")]] void mod_preinit() {}

extern "C" [[gnu::visibility("default")]] void mod_init() {
    using namespace hat::literals::signature_literals;

    auto mcLib = dlopen("libminecraftpe.so", 0);

    std::span<std::byte> range1;

    auto callback = [&](const dl_phdr_info& info) {
        if (auto h = dlopen(info.dlpi_name, RTLD_NOLOAD); dlclose(h), h != mcLib)
            return 0;
        range1 = {reinterpret_cast<std::byte*>(info.dlpi_addr + info.dlpi_phdr[1].p_vaddr), info.dlpi_phdr[1].p_memsz};
        return 1;
    };

    dl_iterate_phdr(
        [](dl_phdr_info* info, size_t, void* data) {
            return (*static_cast<decltype(callback)*>(data))(*info);
        },
        &callback);

    if (auto addr = hat::find_pattern(range1, "55 41 57 41 56 41 55 41 54 53 48 83 EC 68 F3 0F 11 5C 24 08"_sig, hat::scan_alignment::X16).get();
        addr || ((addr = hat::find_pattern(range1, "55 41 57 41 56 41 55 41 54 53 48 83 EC 48 F3 0F 11 5C 24 10"_sig, hat::scan_alignment::X16).get())))
        FloatOption_ctor_hook = safetyhook::create_inline(addr, FloatOption_ctor);
    else if ((addr = hat::find_pattern(range1, "55 41 57 41 56 41 55 41 54 53 48 83 EC 48 0F 29 54 24 30"_sig, hat::scan_alignment::X16).get()))
        FloatOption_ctor_hook = safetyhook::create_inline(addr, FloatOption_ctor_old);
}
