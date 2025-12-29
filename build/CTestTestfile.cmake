# CMake generated Testfile for 
# Source directory: C:/Users/driuk/Game
# Build directory: C:/Users/driuk/Game/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(PrisonersDilemmaTests "C:/Users/driuk/Game/build/Debug/run_tests.exe")
  set_tests_properties(PrisonersDilemmaTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/driuk/Game/CMakeLists.txt;84;add_test;C:/Users/driuk/Game/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(PrisonersDilemmaTests "C:/Users/driuk/Game/build/Release/run_tests.exe")
  set_tests_properties(PrisonersDilemmaTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/driuk/Game/CMakeLists.txt;84;add_test;C:/Users/driuk/Game/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(PrisonersDilemmaTests "C:/Users/driuk/Game/build/MinSizeRel/run_tests.exe")
  set_tests_properties(PrisonersDilemmaTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/driuk/Game/CMakeLists.txt;84;add_test;C:/Users/driuk/Game/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(PrisonersDilemmaTests "C:/Users/driuk/Game/build/RelWithDebInfo/run_tests.exe")
  set_tests_properties(PrisonersDilemmaTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/driuk/Game/CMakeLists.txt;84;add_test;C:/Users/driuk/Game/CMakeLists.txt;0;")
else()
  add_test(PrisonersDilemmaTests NOT_AVAILABLE)
endif()
