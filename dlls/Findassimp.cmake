FIND_PATH( assimp_INCLUDE_DIR postprocess.h 
	/usr/include
	/usr/local/include
	/opt/local/include
	${CMAKE_SOURCE_DIR}/dependencies/Assimp/include/assimp
)
FIND_LIBRARY( assimp_LIBRARY assimp
	/usr/lib64
	/usr/lib
	/usr/local/lib
	/opt/local/lib
	"${CMAKE_SOURCE_DIR}/dependencies/Assimp/lib"
)
MESSAGE(STATUS "Found assimp: ${assimp_INCLUDE_DIR}")
MESSAGE(STATUS "Found assimp: ${assimp_LIBRARY}")

IF(assimp_INCLUDE_DIR AND assimp_LIBRARY)
	SET( assimp_FOUND TRUE )
	SET( assimp_LIBRARIES ${assimp_LIBRARY} )
	set_target_properties(${ASSIMP_LIBRARY} PROPERTIES INCLUDE_DIRECTORIES ASSIMP_INCLUDE_DIRS)

ENDIF(assimp_INCLUDE_DIR AND assimp_LIBRARY)
IF(assimp_FOUND)
	IF(NOT assimp_FIND_QUIETLY)
	MESSAGE(STATUS "Found assimp: ${assimp_LIBRARY}")
	ENDIF(NOT assimp_FIND_QUIETLY)
ELSE(assimp_FOUND)
	IF(assimp_FIND_REQUIRED)
	MESSAGE(FATAL_ERROR "Could not find libassimp")
	ENDIF(assimp_FIND_REQUIRED)
ENDIF(assimp_FOUND)