add_executable(${THIS_PROJECT_NAME}
	${THIS_SRC_DIR}/main.cpp)

target_link_libraries(${THIS_PROJECT_NAME} GL GLU glut)