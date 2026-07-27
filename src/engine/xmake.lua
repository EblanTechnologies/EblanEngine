target("EblanEngine")
    set_kind("static")

    add_files("**.cpp")
    add_files("**.cppm")

    add_includedirs("../thirdparty", {public = true})

    add_packages("glfw", "glad", "glm", "opengl")

    set_languages("cxx20")

    add_rules("c++.build_modules", {interface = true})

    set_configvar("EBLAN_ENGINE_MODULES", "1")