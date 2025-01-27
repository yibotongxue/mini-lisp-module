add_rules("mode.debug", "mode.release")

set_languages("c++20")
-- add_cxxflags("-fmodules-ts")
add_cxxflags("-fmodules", "-fcxx-modules", "-stdlib=libc++")
add_ldflags("-stdlib=libc++")
-- add_requires("libstdc++")

target("mini_lisp")
  -- set_kind("static")
  add_files("src/*.cpp", "src/*.cppm")
  set_languages("c++20")

-- target("mini_lisp")
--   add_deps("token_lib")
--   add_files("src/main.cpp")
--   set_languages("c++20")
