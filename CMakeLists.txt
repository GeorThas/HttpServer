# Установка переменной со списком исходников
set( SOURCE_LIB 
    extclib/main.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extclib)

add_executable( math_parese ${SOURCE_LIB} extclib/hashtab.c extclib/bintree.c )

#target_link_libraries(math_parese PRIVATE MathParseLib)