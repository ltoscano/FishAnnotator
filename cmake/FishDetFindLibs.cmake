# --- Qt5 ---
find_package( Qt5Widgets )
if( NOT Qt5Widgets_FOUND )
  message( FATAL_ERROR "Could not find Qt5Widgets.  Build cannot continue." )
endif()
find_package( Qt5Core )
if( NOT Qt5Core_FOUND )
  message( FATAL_ERROR "Could not find Qt5Core.  Build cannot continue." )
endif()
find_package( Qt5Gui )
if( NOT Qt5Gui_FOUND )
  message( FATAL_ERROR "Could not find Qt5Gui.  Build cannot continue." )
endif()
find_package( Qt5PrintSupport )
if( NOT Qt5PrintSupport_FOUND )
  message( FATAL_ERROR "Could not find Qt5PrintSupport.  Build cannot continue." )
endif()
find_package( Qt5DBus )
if( NOT Qt5DBus_FOUND )
  message( FATAL_ERROR "Could not find Qt5DBus.  Build cannot continue." )
endif()
find_package( Qt5Test )
if( NOT Qt5Test_FOUND )
  message( FATAL_ERROR "Could not find Qt5Test.  Build cannot continue." )
endif()
get_target_property(QT5_WIDGETS_LOCATION Qt5::Widgets LOCATION )
message( STATUS "Found Qt5 at ${QT5_WIDGETS_LOCATION}" )
if( WIN32 )
  set( QT_THIRD_PARTY_LIBS
    "${_qt5Widgets_install_prefix}/lib/qtpcre.lib"
    "${_qt5Widgets_install_prefix}/lib/qtfreetype.lib"
    "${_qt5Widgets_install_prefix}/lib/qtpng.lib"
    "${_qt5Widgets_install_prefix}/lib/qtharfbuzzng.lib"
    "${_qt5Widgets_install_prefix}/lib/Qt5PlatformSupport.lib"
    "${_qt5Widgets_install_prefix}/plugins/platforms/qwindows.lib"
    "${_qt5Widgets_install_prefix}/plugins/imageformats/qdds.lib"
    "${_qt5Widgets_install_prefix}/plugins/imageformats/qgif.lib"
    "${_qt5Widgets_install_prefix}/plugins/imageformats/qicns.lib"
    "${_qt5Widgets_install_prefix}/plugins/imageformats/qico.lib"
    "${_qt5Widgets_install_prefix}/plugins/imageformats/qjpeg.lib"
    "${_qt5Widgets_install_prefix}/plugins/imageformats/qsvg.lib"
    "${_qt5Widgets_install_prefix}/plugins/imageformats/qtga.lib"
    "${_qt5Widgets_install_prefix}/plugins/imageformats/qtiff.lib"
    "${_qt5Widgets_install_prefix}/plugins/imageformats/qwbmp.lib"
    "${_qt5Widgets_install_prefix}/plugins/imageformats/qwebp.lib"
    "${_qt5Widgets_install_prefix}/lib/Qt5Svg.lib"
    )
elseif( APPLE )
  set( QT_THIRD_PARTY_LIBS
    "${_qt5Widgets_install_prefix}/lib/libqtpcre.a"
    "${_qt5Widgets_install_prefix}/lib/libqtharfbuzzng.a"
    "${_qt5Widgets_install_prefix}/lib/libqtfreetype.a"
    "${_qt5Widgets_install_prefix}/plugins/platforms/libqcocoa.a"
    "${_qt5Widgets_install_prefix}/plugins/platforms/libqminimal.a"
    "${_qt5Widgets_install_prefix}/plugins/platforms/libqoffscreen.a"
    "${_qt5Widgets_install_prefix}/plugins/printsupport/libcocoaprintersupport.a"
    "${_qt5Widgets_install_prefix}/plugins/imageformats/libqico.a"
    )
elseif( UNIX )
  set( QT_THIRD_PARTY_LIBS
    "${_qt5Widgets_install_prefix}/lib/libQt5XcbQpa.a"
    "${_qt5Widgets_install_prefix}/lib/libqtpcre.a"
    "${_qt5Widgets_install_prefix}/lib/libqtharfbuzzng.a"
    "${_qt5Widgets_install_prefix}/lib/libqtpng.a"
    "${_qt5Widgets_install_prefix}/lib/libQt5PlatformSupport.a"
    "${_qt5Widgets_install_prefix}/plugins/platforms/libqxcb.a"
    "${_qt5Widgets_install_prefix}/plugins/platforms/libqlinuxfb.a"
    "${_qt5Widgets_install_prefix}/plugins/platforms/libqminimal.a"
    "${_qt5Widgets_install_prefix}/plugins/platforms/libqoffscreen.a"
    "${_qt5Widgets_install_prefix}/plugins/imageformats/libqico.a"
    "${_qt5Widgets_install_prefix}/lib/libxcb-static.a"
    "${_qt5Widgets_install_prefix}/lib/libqtfreetype.a"
    )
endif() 

# --- Boost ---
set( Boost_USE_STATIC_LIBS ON )
set( Boost_USE_STATIC_RUNTIME ON )
find_package( Boost COMPONENTS filesystem system )
if( NOT Boost_FOUND )
  message( FATAL_ERROR "Could not find Boost.  Build cannot continue." )
endif()

# --- OpenCV ---
find_package( OpenCV )
if( NOT OpenCV_FOUND )
  message( FATAL_ERROR "Could not find OpenCV.  Build cannot continue." )
endif()
if( WIN32 )
  file( GLOB_RECURSE OpenCV_SHARED_LIBS "${OpenCV_LIB_PATH}/../bin/*.dll" )
endif()

# --- OpenGL ---
find_package( OpenGL )
if( NOT OPENGL_FOUND )
  message( FATAL_ERROR "Could not find OpenGL.  Build cannot continue." )
endif()

# --- Doxygen ---
find_package( Doxygen )
if( NOT DOXYGEN_FOUND )
  message( WARNING "Could not find Doxygen.  Documentation target will be unavailable." )
endif()

# --- LATEX ---
find_package( LATEX )
if( NOT LATEX_FOUND )
  message( WARNING "Could not find LATEX.  Documentation target will be unavailable." )
endif()

# --- Windows ---
if( MSVC )
  set( WINDOWS_LIBRARIES
    winmm
    UxTheme
    ws2_32
    imm32
    Dwmapi
    )

# --- Mac ---
elseif( APPLE )
  find_library( COCOA_LIBRARY Cocoa )
  message( STATUS "Found Cocoa at ${COCOA_LIBRARY}" )
  find_library( CARBON_LIBRARY Carbon )
  message( STATUS "Found Carbon at ${CARBON_LIBRARY}" )
  find_library( IOKIT_LIBRARY IOKit )
  message( STATUS "Found IOKit at ${IOKIT_LIBRARY}" )
  find_package( Cups )
  if( NOT CUPS_FOUND )
    message (FATAL_ERROR "Could not find Cups.  Build cannot continue." )
  endif()
  set( APPLE_LIBRARIES
    ${COCOA_LIBRARY}
    ${CARBON_LIBRARY}
    ${IOKIT_LIBRARY}
    ${CUPS_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${JPEG_LIBRARIES}
    ${PNG_LIBRARIES}
    )

# --- UNIX ---
elseif( UNIX )
  find_package( X11 REQUIRED )
  if( NOT X11_FOUND )
    message( FATAL_ERROR "Could not find X11. Build cannot continue." )
  endif()
  find_package( Threads )
  set( UNIX_LIBRARIES
    ${CMAKE_THREAD_LIBS_INIT}
    xcb
    X11-xcb
    ${X11_LIBRARIES}
    )
endif()
