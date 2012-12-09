include_directories ("${PROJECT_SOURCE_DIR}/libtastro")

add_executable(
	tibastro
	tibastro.c
	ui.c
	ui.h
	translation.c
	translation.h
	tests/tests-jd.c
	tests/tests-jd.h
	tests/tests-tbday.c
	tests/tests-tbday.h
	)

target_link_libraries (tibastro libtastro)
