
MACRO(COMPILE_SHADERS _shader_includes _shaders_in _shaders_out)
    IF(VulkanSDK_FOUND)
        FOREACH(file ${_shaders_in})
            GET_FILENAME_COMPONENT(_shader_ext ${file} LAST_EXT)
            IF(_shader_ext STREQUAL ".glsl")
                GET_FILENAME_COMPONENT(_shader_name ${file} NAME_WLE)
                GET_FILENAME_COMPONENT(_shader_path ${file} DIRECTORY)
                STRING(REPLACE 
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    _shader_out_path
                    ${_shader_path}
                )

                FILE(MAKE_DIRECTORY ${_shader_out_path})

                SET(_shader_cflags "")
                FOREACH(dir ${DUSK_ASSET_PATH})
                    SET(_shader_cflags ${_shader_cflags} -I"${dir}/Shaders/")
                ENDFOREACH()

                SET(_shader_out "${_shader_out_path}/${_shader_name}.spv")

                GET_FILENAME_COMPONENT(_shader_type ${_shader_name} LAST_EXT)
                STRING(SUBSTRING ${_shader_type} 1 -1 _shader_type)

                IF(_shader_type STREQUAL "inc")
                    CONTINUE()
                ELSE()
                    SET(_shader_cflags ${_shader_cflags} -fshader-stage=${_shader_type})
                ENDIF()

                ADD_CUSTOM_COMMAND(
                    OUTPUT ${_shader_out}
                    COMMAND ${VulkanSDK_glslc_PROGRAM} ${_shader_cflags} -o ${_shader_out} ${file}
                    DEPENDS ${file} ${_shader_includes}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                )
                
                LIST(APPEND ${_shaders_out} ${_shader_out})
            ENDIF()
        ENDFOREACH()
    ENDIF()

    IF(WindowsSDK_FOUND)
        FOREACH(file ${_shaders_in})
            FILE(TO_NATIVE_PATH "${file}" file)
            
            GET_FILENAME_COMPONENT(_shader_ext ${file} LAST_EXT)
            IF(_shader_ext STREQUAL ".hlsl")
                GET_FILENAME_COMPONENT(_shader_name ${file} NAME_WLE)
                GET_FILENAME_COMPONENT(_shader_path ${file} DIRECTORY)
                STRING(REPLACE 
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    _shader_out_path
                    ${_shader_path}
                )

                FILE(MAKE_DIRECTORY ${_shader_out_path})

                SET(_shader_cflags "")
                FOREACH(dir ${DUSK_ASSET_PATH})
                    FILE(TO_NATIVE_PATH "${dir}" dir)
                    SET(_shader_cflags ${_shader_cflags} -I"${dir}/Shaders/")
                ENDFOREACH()

                SET(_shader_out "${_shader_out_path}/${_shader_name}.cso")

                GET_FILENAME_COMPONENT(_shader_type ${_shader_name} LAST_EXT)
                STRING(SUBSTRING ${_shader_type} 1 -1 _shader_type)

                # Use row-major matricies
                SET(_shader_cflags ${_shader_cflags} -Zpc)

                IF(_shader_type STREQUAL "inc")
                    CONTINUE()
                ELSEIF(_shader_type STREQUAL "vert" OR _shader_type STREQUAL "vertex")
                    SET(_shader_cflags ${_shader_cflags} -T vs_6_0 -E VSMain)
                ELSEIF(_shader_type STREQUAL "frag" OR _shader_type STREQUAL "fragment" OR _shader_type STREQUAL "pixel")
                    SET(_shader_cflags ${_shader_cflags} -T ps_6_0 -E PSMain)
                ELSEIF(_shader_type STREQUAL "hull")
                    SET(_shader_cflags ${_shader_cflags} -T hs_6_0 -E HSMain)
                ELSEIF(_shader_type STREQUAL "domain")
                    SET(_shader_cflags ${_shader_cflags} -T ds_6_0 -E DSMain)
                ELSEIF(_shader_type STREQUAL "geom" OR _shader_type STREQUAL "geometry")
                    SET(_shader_cflags ${_shader_cflags} -T gs_6_0 -E GSMain)
                ELSEIF(_shader_type STREQUAL "comp" OR _shader_type STREQUAL "compute")
                    SET(_shader_cflags ${_shader_cflags} -T cs_6_0 -E CSMain)
                ENDIF()

                ADD_CUSTOM_COMMAND(
                    OUTPUT ${_shader_out}
                    COMMAND ${WindowsSDK_dxc_PROGRAM} ${_shader_cflags} -Fo ${_shader_out} ${file}
                    DEPENDS ${file} ${_shader_includes}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                )

                LIST(APPEND ${_shaders_out} ${_shader_out})
            ENDIF()
        ENDFOREACH()
    ENDIF()
ENDMACRO()