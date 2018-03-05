#    File:    FindArcGIS_SDK.cmake
#    Author:  Marvin Smith
#    Date:    3/2/2018
#

set( ArcGIS_SDK_ROOT
        $ENV{HOME}/ArcGIS_SDKs/Qt100.2.1/sdk
        $ENV{ArcGIS_SDK_ROOT})

#--------------------------------------#
#-      Find the Include Path         -#
#--------------------------------------#
#  Find the Comprimato Header File
find_path( ArcGIS_SDK_INCLUDE_DIR
            NAMES
                Raster.h
            PATHS
                ${ArcGIS_SDK_ROOT}
            PATH_SUFFIXES
                include
        )


#----------------------------------------#
#-          Find the Libraries          -#
#----------------------------------------#
if( APPLE )

    find_library( ArcGIS_SDK_ESRI_COMMON_QT
                    EsriCommonQt
                  PATHS
                      ${ArcGIS_SDK_ROOT}
                  PATH_SUFFIXES
                      macOS/x64/lib)

    find_library( ArcGIS_SDK_RT_CPP_API
                    ArcGISRuntimeToolkitCppApi
                  PATHS
                    ${ArcGIS_SDK_ROOT}
                  PATH_SUFFIXES
                    macOS/x64/lib)

    find_library( ArcGIS_SDK_RT_QT
                    EsriRuntimeQt
                  PATHS
                    ${ArcGIS_SDK_ROOT}
                  PATH_SUFFIXES
                    macOS/x64/lib)

    find_library( ArcGIS_SDK_RT_CORE
                    runtimecore
                  PATHS
                    ${ArcGIS_SDK_ROOT}
                  PATH_SUFFIXES
                    macOS/x64/lib )

    if( ArcGIS_SDK_ESRI_COMMON_QT )
        set( ArcGIS_SDK_LIBRARIES ${ArcGIS_SDK_ESRI_COMMON_QT} )
    endif()

    if( ArcGIS_SDK_RT_CPP_API )
        set( ArcGIS_SDK_LIBRARIES
                ${ArcGIS_SDK_LIBRARIES}
                ${ArcGIS_SDK_RT_CPP_API})
    endif()

    if( ArcGIS_SDK_RT_QT )
        set( ArcGIS_SDK_LIBRARIES
                ${ArcGIS_SDK_LIBRARIES}
                ${ArcGIS_SDK_RT_QT})
    endif()

    if( ArcGIS_SDK_RT_CORE )
        set( ArcGIS_SDK_LIBRARIES
                ${ArcGIS_SDK_LIBRARIES}
                ${ArcGIS_SDK_RT_CORE})
    endif()

else()
    find_library( ArcGIS_Runtime_QT_LIB
                    EsriRuntimeQt
                    PATHS
                        ${ArcGIS_SDK_ROOT} )

    find_library( ArcGIS_Runtime_RT_CORE_LIB
            runtimecore
            PATHS
            /opt/software/arcgis/runtime_sdk/qt100.2/sdk/linux/x64/lib
            $ENV:{HOME}/ArcGIS_SDKs/Qt100.2.1/sdk/lib
            )
    find_library( ArcGIS_Runtime_QT_COMMON_LIB
            libEsriCommonQt.so
            PATHS
            /opt/software/arcgis/runtime_sdk/qt100.2/sdk/linux/x64/lib
            $ENV:{HOME}/ArcGIS_SDKs/Qt100.2.1/sdk/lib
            )

    set( ArcGIS_LIBS
            ${ArcGIS_Runtime_QT_LIB}
            ${ArcGIS_Runtime_RT_CORE_LIB}
            ${ArcGIS_Runtime_QT_COMMON_LIB}
            )
endif()


#--------------------------------#
#-          Wrap it Up          -#
#--------------------------------#
if( ArcGIS_SDK_LIBRARIES AND ArcGIS_SDK_INCLUDE_DIR )
    set( ArcGIS_SDK_FOUND TRUE)
endif()
