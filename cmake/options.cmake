function(Jan_set_option library)
  target_compile_features(${library} PUBLIC cxx_std_17)
  target_include_directories(${library} SYSTEM
    INTERFACE
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  )
  target_include_directories(${library}
    PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/src>
  )
endfunction()
