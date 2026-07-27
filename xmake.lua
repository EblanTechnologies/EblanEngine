add_rules("mode.debug", "mode.release")
set_languages("cxx20")

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