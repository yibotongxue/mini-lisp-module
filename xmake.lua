add_rules("mode.debug", "mode.release")

set_languages("c++latest")

target("mini_lisp")
  add_files("src/*.cpp", "src/*.cppm")
  set_policy("build.c++.modules", true)
