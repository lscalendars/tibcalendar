include_directories ("${PROJECT_SOURCE_DIR}/libtastro")

add_executable(
	tibastro
	tibastro.c
	ui.c
	ui.h
	translation.c
	translation.h
	gettext.h
	printcalendar.h
	printcalendar.c
	tests/tests.h
	tests/tests-jd.c
	tests/tests-tbday.c
	tests/tests-astrology.c
	)

target_link_libraries (tibastro libtastro)
