
MACRO(COMPILE_SHADERS shader_includes shaders_in shaders_out)
    IF(VulkanSDK_FOUND)
        FOREACH(file ${shaders_in})
            GET_FILENAME_COMPONENT(shader_ext ${file} LAST_EXT)
            IF(shader_ext STREQUAL ".glsl")
                GET_FILENAME_COMPONENT(shader_name ${file} NAME_WLE)
                GET_FILENAME_COMPONENT(shader_path ${file} DIRECTORY)
                STRING(REPLACE 
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    shader_out_path
                    ${shader_path}
                )

                FILE(MAKE_DIRECTORY ${shader_out_path})

                SET(shader_cflags "")
                FOREACH(dir ${ASSET_PATH})
                    SET(shader_cflags ${shader_cflags} -I"${dir}/Shaders/")
                ENDFOREACH()

                SET(shader_out "${shader_out_path}/${shader_name}.spv")

                GET_FILENAME_COMPONENT(shader_type ${shader_name} LAST_EXT)
                STRING(SUBSTRING ${shader_type} 1 -1 shader_type)

                IF(shader_type STREQUAL "inc")
                    CONTINUE()
                ELSE()
                    SET(shader_cflags ${shader_cflags} -fshader-stage=${shader_type})
                ENDIF()

                ADD_CUSTOM_COMMAND(
                    OUTPUT ${shader_out}
                    COMMAND ${VulkanSDK_glslc_PROGRAM} ${shader_cflags} -o ${shader_out} ${file}
                    DEPENDS ${file} ${shader_includes}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                )
                
                LIST(APPEND ${shaders_out} ${shader_out})
            ENDIF()
        ENDFOREACH()
    ENDIF()

    IF(WindowsSDK_FOUND)
        FOREACH(file ${shaders_in})
            FILE(TO_NATIVE_PATH "${file}" file)
            
            GET_FILENAME_COMPONENT(shader_ext ${file} LAST_EXT)
            IF(shader_ext STREQUAL ".hlsl")
                GET_FILENAME_COMPONENT(shader_name ${file} NAME_WLE)
                GET_FILENAME_COMPONENT(shader_path ${file} DIRECTORY)
                STRING(REPLACE 
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    shader_out_path
                    ${shader_path}
                )

                FILE(MAKE_DIRECTORY ${shader_out_path})

                SET(shader_cflags "")
                FOREACH(dir ${ASSET_PATH})
                    FILE(TO_NATIVE_PATH "${dir}" dir)
                    SET(shader_cflags ${shader_cflags} -I"${dir}/Shaders/")
                ENDFOREACH()

                SET(shader_out "${shader_out_path}/${shader_name}.cso")

                GET_FILENAME_COMPONENT(shader_type ${shader_name} LAST_EXT)
                STRING(SUBSTRING ${shader_type} 1 -1 shader_type)

                # Use row-major matricies
                SET(shader_cflags ${shader_cflags} -Zpc)

                IF(shader_type STREQUAL "inc")
                    CONTINUE()
                ELSEIF(shader_type STREQUAL "vert" OR shader_type STREQUAL "vertex")
                    SET(shader_cflags ${shader_cflags} -T vs_6_0 -E VSMain)
                ELSEIF(shader_type STREQUAL "frag" OR shader_type STREQUAL "fragment" OR shader_type STREQUAL "pixel")
                    SET(shader_cflags ${shader_cflags} -T ps_6_0 -E PSMain)
                ELSEIF(shader_type STREQUAL "hull")
                    SET(shader_cflags ${shader_cflags} -T hs_6_0 -E HSMain)
                ELSEIF(shader_type STREQUAL "domain")
                    SET(shader_cflags ${shader_cflags} -T ds_6_0 -E DSMain)
                ELSEIF(shader_type STREQUAL "geom" OR shader_type STREQUAL "geometry")
                    SET(shader_cflags ${shader_cflags} -T gs_6_0 -E GSMain)
                ELSEIF(shader_type STREQUAL "comp" OR shader_type STREQUAL "compute")
                    SET(shader_cflags ${shader_cflags} -T cs_6_0 -E CSMain)
                ENDIF()

                ADD_CUSTOM_COMMAND(
                    OUTPUT ${shader_out}
                    COMMAND ${WindowsSDK_dxc_PROGRAM} ${shader_cflags} -Fo ${shader_out} ${file}
                    DEPENDS ${file} ${shader_includes}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                )

                LIST(APPEND ${shaders_out} ${shader_out})
            ENDIF()
        ENDFOREACH()
    ENDIF()
ENDMACRO()