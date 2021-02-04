
INCLUDE(DefineModule)

###
### Include Modules
###

FOREACH(_module ${DUSK_REQUIRED_MODULES})
    ADD_SUBDIRECTORY(${_module})
ENDFOREACH()

# Update the global DUSK_RUNTIME_SEARCH_PATH to include paths to the
# module's .dll's
SET(DUSK_RUNTIME_SEARCH_PATH ${DUSK_RUNTIME_SEARCH_PATH} PARENT_SCOPE)

# Update the global DUSK_MODULE_TARGETS for use in the install section
SET(DUSK_MODULE_TARGETS ${DUSK_MODULE_TARGETS} PARENT_SCOPE)