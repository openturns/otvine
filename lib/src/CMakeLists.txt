

ot_add_current_dir_to_include_dirs ()

ot_add_source_file (RotatedCopula.cxx)
ot_add_source_file (TawnCopula.cxx)
ot_add_source_file (VineCopulaFactory.cxx)
ot_add_source_file (VineCopula.cxx)

ot_install_header_file (RotatedCopula.hxx)
ot_install_header_file (TawnCopula.hxx)
ot_install_header_file (VineCopulaFactory.hxx)
ot_install_header_file (VineCopula.hxx)


include_directories (${INTERNAL_INCLUDE_DIRS})

add_library (otvine ${SOURCEFILES})
set_target_properties (otvine PROPERTIES POSITION_INDEPENDENT_CODE ON)
if (BUILD_SHARED_LIBS)
  set_target_properties (otvine PROPERTIES COMPILE_DEFINITIONS "OTVINE_DLL_EXPORTS")
endif ()

if (NOT DEFINED LIB_VERSION) 
  set (LIB_VERSION 0.0.0)
endif ()
if (NOT DEFINED LIB_SOVERSION) 
  set (LIB_SOVERSION 0 )
endif ()
set_target_properties (otvine PROPERTIES VERSION ${LIB_VERSION})
set_target_properties (otvine PROPERTIES SOVERSION ${LIB_SOVERSION})
target_link_libraries (otvine PUBLIC ${OPENTURNS_LIBRARY})
target_link_libraries (otvine PUBLIC vinecopulib)

# Add targets to the build-tree export set
export (TARGETS otvine FILE ${PROJECT_BINARY_DIR}/otvine-Targets.cmake)

# Install the export set for use with the install-tree
install(EXPORT otvine-Targets
        DESTINATION "${OTVINE_CONFIG_CMAKE_PATH}"
        COMPONENT Development)

install(TARGETS otvine
        EXPORT  otvine-Targets 
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${OTVINE_LIBRARY_PATH}
        ARCHIVE DESTINATION ${OTVINE_LIBRARY_PATH}
)

