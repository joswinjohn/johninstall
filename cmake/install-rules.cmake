install(
    TARGETS johninstall_exe
    RUNTIME COMPONENT johninstall_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
