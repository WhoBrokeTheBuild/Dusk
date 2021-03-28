
INCLUDE(DefineModule)

###
### Include Modules
###

ADD_SUBDIRECTORY(Core)

FOREACH(_module ${DUSK_REQUIRED_MODULES})
    ADD_SUBDIRECTORY(${_module})
ENDFOREACH()

# Update the global DUSK_ASSET_PATH
SET(DUSK_ASSET_PATH ${DUSK_ASSET_PATH} PARENT_SCOPE)

# Update the global DUSK_MODULE_PATH to include paths to the module's libraries
SET(DUSK_MODULE_PATH ${DUSK_MODULE_PATH} PARENT_SCOPE)

# Update the global DUSK_MODULE_TARGETS for use in the install section
SET(DUSK_MODULE_TARGETS ${DUSK_MODULE_TARGETS} PARENT_SCOPE)