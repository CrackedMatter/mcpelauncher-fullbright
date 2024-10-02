#include <dlfcn.h>
#include <libhat/Scanner.hpp>
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

extern "C" __attribute__ ((visibility ("default"))) void mod_preinit() {}

extern "C" __attribute__ ((visibility ("default"))) void mod_init() {
    using namespace hat::literals::signature_literals;

    static std::span<std::byte> r;

    dl_iterate_phdr([](dl_phdr_info* info, size_t, void* mc) {
        auto h = dlopen(info->dlpi_name, RTLD_NOLOAD);
        dlclose(h);
        if (h == mc) {
            for (auto& phdr: std::span{info->dlpi_phdr, info->dlpi_phnum}) {
                if (phdr.p_type == PT_LOAD && phdr.p_flags & PF_X) {
                    r = {reinterpret_cast<std::byte*>(info->dlpi_addr + phdr.p_vaddr), phdr.p_memsz};
                    return 1;
                }
            }
        }
        return 0;
    }, dlopen("libminecraftpe.so", 0));

    if (auto addr = hat::find_pattern<hat::scan_alignment::X16>(r.begin(), r.end(), "55 41 57 41 56 41 55 41 54 53 48 83 EC 48 F3 0F 11 5C 24 10"_sig).get())
        FloatOption_ctor_hook = safetyhook::create_inline(addr, reinterpret_cast<void*>(FloatOption_ctor));
    else if ((addr = hat::find_pattern<hat::scan_alignment::X16>(r.begin(), r.end(), "55 41 57 41 56 41 55 41 54 53 48 83 EC 48 0F 29 54 24 30"_sig).get()))
        FloatOption_ctor_hook = safetyhook::create_inline(addr, reinterpret_cast<void*>(FloatOption_ctor_old));
}
