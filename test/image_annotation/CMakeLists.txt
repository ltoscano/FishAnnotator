set( TEST_IMAGE_ANNOTATION_SOURCES test_image_annotation.cc )
set( IMAGE_ANNOTATION_SOURCES
  "../../src/image_annotator/image_annotation.cc"
  )
include_directories( 
  "../../include/fish_annotator/common"
  "../../include/fish_annotator/image_annotator"
  )
if( WIN32 )
  add_executable( test_image_annotation WIN32 
    ${TEST_IMAGE_ANNOTATION_SOURCES} 
    ${IMAGE_ANNOTATION_SOURCES}
    )
  target_link_libraries( 
    test_image_annotation 
    common
    ${WINDOWS_LIBRARIES}
    Qt5::Widgets
    Qt5::Test
    ${QT_THIRD_PARTY_LIBS}
    ${Boost_LIBRARIES}
    )
  set_target_properties(
    test_image_annotation
    PROPERTIES
    LINK_FLAGS "${LINK_FLAGS} /SUBSYSTEM:CONSOLE"
    )
  add_test( 
    NAME test_image_annotation 
    COMMAND test_image_annotation 
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Release"
    )
  set_tests_properties( 
    test_image_annotation PROPERTIES
    FAIL_REGULAR_EXPRESSION "FAIL"
    )
  file( COPY "ceres" DESTINATION "Release" )
elseif( APPLE )
  add_executable( test_image_annotation
    ${TEST_IMAGE_ANNOTATION_SOURCES} 
    ${IMAGE_ANNOTATION_SOURCES}
    )
  target_link_libraries( 
    test_image_annotation 
    common
    ${APPLE_LIBRARIES}
    Qt5::Widgets
    Qt5::PrintSupport
    Qt5::Test
    ${QT_THIRD_PARTY_LIBS}
    ${Boost_LIBRARIES}
    ${OPENGL_LIBRARIES}
    )
  add_test( 
    NAME test_image_annotation 
    COMMAND test_image_annotation 
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    )
  set_tests_properties( 
    test_image_annotation PROPERTIES
    FAIL_REGULAR_EXPRESSION "FAIL"
    )
  file( COPY "ceres" DESTINATION "." )
elseif( UNIX )
  add_executable( test_image_annotation
    ${TEST_IMAGE_ANNOTATION_SOURCES} 
    ${IMAGE_ANNOTATION_SOURCES}
    )
  target_link_libraries( 
    test_image_annotation 
    dl
    common
    ${UNIX_LIBRARIES}
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
    Qt5::PrintSupport
    Qt5::QXcbIntegrationPlugin
    Qt5::Test
    ${QT_THIRD_PARTY_LIBS}
    Qt5::DBus
    ${Boost_LIBRARIES}
    ${OPENGL_LIBRARIES}
    )
  add_test( 
    NAME test_image_annotation 
    COMMAND test_image_annotation 
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    )
  set_tests_properties( 
    test_image_annotation PROPERTIES
    FAIL_REGULAR_EXPRESSION "FAIL"
    )
  file( COPY "ceres" DESTINATION "." )
endif()
