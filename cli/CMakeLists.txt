include_directories ("${PROJECT_SOURCE_DIR}/libtastro")

add_executable(
	tibastro
	tibastro.c
	tests/tests-jd.c
	tests/tests-jd.h
	)

target_link_libraries (tibastro libtastro)
