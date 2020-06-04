set_project("dexbox")
set_xmakever("2.3.3")

add_rules("mode.debug", "mode.release", "mode.releasedbg")
add_requires("tbox 1.6.5", {debug = is_mode("debug")})

if is_plat("windows") then 
    if is_mode("release") then
        add_cxflags("-MT") 
    elseif is_mode("debug") then
        add_cxflags("-MTd") 
    end
    add_ldflags("-nodefaultlib:msvcrt.lib")
end

option("dump")
    set_default(true)
    set_showmenu(true)
    add_defines("DX_DUMP_ENABLE")
    set_description("Enable or disable dump interfaces.")
option_end()

target("dexbox")
    set_kind("static")
    add_files("src/dexbox/**.c")
    add_defines("__tb_prefix__=\"dexbox\"")
    add_includedirs("src", {interface = true})
    add_options("dump")
    add_packages("tbox")

target("dexdump")
    if not has_config("dump") then
        set_default(false)
    end
    set_kind("binary")
    add_deps("dexbox")
    add_files("src/demo/dexdump.c")
    set_rundir("$(projectdir)")
    add_packages("tbox")

