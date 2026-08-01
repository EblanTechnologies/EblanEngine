add_requires("catch2")

target("EblanEngineTests")
    set_kind("binary")

    add_deps("EblanEngine")

    add_files("**.cpp")
    add_files("../engine/**.cppm", {rule = "c++.build_modules"})

    add_packages("catch2", "glfw", "glad", "glm", "opengl")

    set_languages("cxx20")

    after_build(function(target)
        print("Running tests...")
        os.exec(target:targetfile())
    end)