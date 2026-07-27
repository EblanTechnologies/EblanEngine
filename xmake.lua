add_rules("mode.debug", "mode.release")
set_languages("cxx20")

add_requires("glfw", "glad", "glm")
add_requires("opengl", {system = true, optional = true})

add_rules("plugin.compile_commands.autoupdate", {lsp = "clangd"})

target("EblanEngine")
    set_kind("binary")
    add_files("src/*.cpp")
    add_files("src/**.cppm")
    add_files("src/**.hpp")

    add_packages("glfw", "glad", "opengl", "glm")
