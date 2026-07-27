add_rules("mode.debug", "mode.release")
set_languages("cxx20")

if is_plat("linux") then
    -- Используем Clang на Linux
    set_toolchains("clang", {cc = "clang-18", cxx = "clang++-18"})
end

includes("src/engine")
includes("src/tests")
includes("src/app")

add_requires("glfw", "glad", "glm")
add_requires("opengl", {system = true, optional = true})

add_rules("plugin.compile_commands.autoupdate", {lsp = "clangd"})

if is_plat("windows") then
    add_cxxflags("/std:c++20", "/experimental:module")
elseif is_plat("linux") then
    add_cxxflags("-std=c++20", "-fmodules")
end