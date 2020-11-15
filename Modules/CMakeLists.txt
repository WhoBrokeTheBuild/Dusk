
INCLUDE(DefineModule)

###
### Include Modules
###

FOREACH(_module ${REQUIRED_MODULES})
    ADD_SUBDIRECTORY(${_module})
ENDFOREACH()

# Update the global RUNTIME_SEARCH_PATH to include paths to the
# module's .dll's
SET(RUNTIME_SEARCH_PATH ${RUNTIME_SEARCH_PATH} PARENT_SCOPE)

# Update the global MODULE_TARGETS for use in the install section
SET(MODULE_TARGETS ${MODULE_TARGETS} PARENT_SCOPE)