add_rules("mode.debug", "mode.release")
add_requires("fmt", "magic_enum", "nlohmann_json", "sfml")

target("sakura")
    set_kind("binary")
    set_languages("c++20")
    add_files("sakura/*.cpp", "sakura/elaina/*.cpp")
    add_packages("fmt", "magic_enum", "nlohmann-json", "sfml")
    add_cxxflags("-Wall", "-Wextra")

    if is_plat("mingw") then
        add_ldflags("-static")
        if is_mode("release") then
            add_ldflags("-mwindows")
        end
    end
