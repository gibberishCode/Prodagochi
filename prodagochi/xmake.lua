
rule("assets")
    after_build(function (target, sourcefile)
         path = path.join(target:targetdir(), "assets")
        print(path)
        if os.exists(path) then 
            os.rmdir(path)
        end 
        os.cp("prodagochi/assets", path)
    end)



target("prodagochi")
    set_kind("binary")
    add_includedirs("include")
    add_files("src/*.cpp")
    if is_os("windows") then
        add_files("src/windows/*.cpp")
    elseif is_os("linux") then
        add_files("src/linux/*.cpp")
    end
    add_includedirs("include")
    add_deps("engine")
    add_deps("rest_server")
    add_rules("assets")
