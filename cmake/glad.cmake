add_library(glad
	"${PROJECT_SOURCE_DIR}/glad/src/glad.c"
)

target_include_directories(glad PUBLIC
	"${PROJECT_SOURCE_DIR}/glad/include"
)