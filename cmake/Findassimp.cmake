FIND_PATH( assimp_INCLUDE_DIR assimp/postprocess.h 
	"${CMAKE_SOURCE_DIR}/dependencies/Assimp/include"
)
FIND_LIBRARY( assimp_LIBRARY assimp
	"${CMAKE_SOURCE_DIR}/dependencies/Assimp/lib"
)
MESSAGE(STATUS "include: ${assimp_INCLUDE_DIR}")
MESSAGE(STATUS "lib: ${assimp_LIBRARY}")

IF(assimp_INCLUDE_DIR AND assimp_LIBRARY)
	SET( assimp_FOUND TRUE )
	SET( assimp_LIBRARIES ${assimp_LIBRARY} )
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