add_rules("mode.debug", "mode.release")
set_languages("cxx20")

-- The engine no longer uses `import std;`; keep the std-module lookup quiet.
set_policy("build.c++.modules.std", false)

if is_plat("linux") then
    set_toolchains("clang", {cc = "clang-18", cxx = "clang++-18"})
end

includes("src/engine")
includes("src/tests")
includes("src/app")

-- glad v2: headers are `glad/gl.h`, loader is `gladLoadGL()`.
-- Generate a GL 4.6 core profile to match the GLFW context hints.
add_requires("glfw")
add_requires("glad", {configs = {api = "gl:core=4.6", profile = "core"}})
add_requires("opengl", {system = true, optional = true})

add_rules("plugin.compile_commands.autoupdate", {lsp = "clangd"})
