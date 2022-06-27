function(set_compile_options target_name)
  if(MSVS)
    target_compile_options(${target_name} PRIVATE /W4 /Wx)
  else()
    target_compile_options(${target_name} PRIVATE -Wall -Wextra  -pedantic -std=c++2a)
  endif()

  set_target_properties(
    ${target_name}
    PROPERTIES
      CXX_STANDARD 20
      CXX_STANDARD_REQUIRED ON
      CXX_EXTENSIONS OFF
  )

if(TIDY)
   if(CLANG_TIDY_EXE)
     set_target_properties(
       ${target_name}
       PROPERTIES
         CXX_CLANG_TIDY ${CLANG_TIDY_EXE}
     )
   endif()
 endif()
endfunction()

function(set_compile_options_interface target_name)
  if(MSVS)
    target_compile_options(${target_name} INTERFACE /W4 /Wx)
  else()
    target_compile_options(${target_name} INTERFACE -Wall -Wextra  -pedantic -std=c++2a)
  endif()

  set_target_properties(
    ${target_name}
    PROPERTIES
      CXX_STANDARD 20
      CXX_STANDARD_REQUIRED ON
      CXX_EXTENSIONS OFF
  )

if(TIDY)
   if(CLANG_TIDY_EXE)
     set_target_properties(
       ${target_name}
       PROPERTIES
         CXX_CLANG_TIDY ${CLANG_TIDY_EXE}
     )
   endif()
 endif()
endfunction()