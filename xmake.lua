add_rules("mode.debug", "mode.release")

set_languages("c++latest")
--add_cxxflags("-fmodules", "-fcxx-modules", "-stdlib=libc++")
--add_ldflags("-stdlib=libc++")

target("mini_lisp")
  add_files("src/*.cpp", "src/*.cppm")
  set_policy("build.c++.modules", true)
