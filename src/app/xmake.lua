target("EblanEngineApp")
    set_kind("binary")

    add_deps("EblanEngine")

    add_packages("glfw", "glad", "glm", "opengl")
    add_files("../engine/**.cppm", {rule = "c++.build_modules"})

    set_languages("cxx20")

    add_files("main.cpp")