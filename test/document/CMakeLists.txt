set( TEST_DOCUMENT_SOURCES test_document.cc )
set( DOCUMENT_SOURCES 
  ../../src/video_annotator/document.cc
  ../../src/common/serialization.cc
  ../../src/common/rect.cc
  )
if( WIN32 )
  add_executable( test_document WIN32 
    ${TEST_DOCUMENT_SOURCES} 
    ${DOCUMENT_SOURCES}
    )
  target_link_libraries( 
    test_document 
    ${WINDOWS_LIBRARIES}
    Qt5::Test
    ${QT_THIRD_PARTY_LIBS}
    )
  set_target_properties(
    test_document
    PROPERTIES
    LINK_FLAGS "${LINK_FLAGS} /SUBSYSTEM:CONSOLE"
    )
  add_test( 
    NAME test_document 
    COMMAND test_document 
    )
  set_tests_properties( 
    test_document PROPERTIES
    FAIL_REGULAR_EXPRESSION "FAIL"
    )
endif()