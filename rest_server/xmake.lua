add_requires("openssl")


target("rest_server")
    set_kind("static")
    add_packages("openssl", {public=true})
    add_includedirs("include", {public=true})

