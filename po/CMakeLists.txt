# Basically the same as the Wesnoth repository
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 
# 1. Redistributions of source code must retain the copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. The name of the author may not be used to endorse or promote products 
#    derived from this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

############ Settings. ###########
set_directory_properties(PROPERTIES CLEAN_NO_CUSTOM true)


############ pot update. ###########

if(ENABLE_POT_UPDATE_TARGET)


		# Update the source file dependencies.
#		include(update_pot_source_dependencies)

		# Generate pot file for c++ data.
		add_custom_command(
			# misses bug address
			OUTPUT ${PROJECT_SOURCE_DIR}/po/tibastro.pot
			# create the pot file make sure we always get output.
			COMMAND ${GETTEXT_XGETTEXT_EXECUTABLE} ${GETTEXT_XGETTEXT_OPTIONS}
					--files-from=${PROJECT_SOURCE_DIR}/po/POTFILES.in 
					--output=${PROJECT_SOURCE_DIR}/po/tibastro.pot
			# replace the chartype
			COMMAND sed -i 
				s/charset=CHARSET/charset=UTF-8/ 
				${PROJECT_SOURCE_DIR}/po/tibastro.pot
			# Remove some header info - Need to test whether needed.
			DEPENDS ${PROJECT_SOURCE_DIR}/po/POTFILES.in
			WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
			COMMENT "pot-update: Generated source pot file."
		)

		# Update / generate the po files for all languages
		foreach(LINGUA ${LINGUAS})

			### Generate new po file.

			# For some reason CMake is rather happy to delete the po file in
			# some cases. Too avoid that problem only generate the init rule
			# if the po file doesn't exist. The case where a po file used to
			# exist and no longer exists should never occur

			if(NOT EXISTS ${PROJECT_SOURCE_DIR}/po/${LINGUA}.po)
				add_custom_command(
					OUTPUT ${PROJECT_SOURCE_DIR}/po/${LINGUA}.po
					COMMAND ${GETTEXT_MSGINIT_EXECUTABLE}
							--no-translator 
							--input=tibastro.pot 
							--output-file=${LINGUA}.po
							--locale=${LINGUA}
					WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/po
					DEPENDS
						${PROJECT_SOURCE_DIR}/po/tibastro.pot
					COMMENT 
						"pot-update [${LINGUA}]: Initialized po file."
				)
			endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/po/${LINGUA}.po)

			### Update existing po file.
			add_custom_command(
				# The pot-update depends on a dummy file which we promise to
				# make. We don't make this dummy file but the pot-update
				# still works.
				OUTPUT ${PROJECT_SOURCE_DIR}/po/${LINGUA}.po.dummy
				COMMAND ${GETTEXT_MSGMERGE_EXECUTABLE} 
						--backup=none
						--previous
						-U ${LINGUA}.po 
						tibastro.pot
				DEPENDS
					${PROJECT_SOURCE_DIR}/po/tibastro.pot 
					${PROJECT_SOURCE_DIR}/po/${LINGUA}.po
				WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/po
				COMMENT "pot-update [${LINGUA}]: Updated po file."
			)

			SET(pot-update-SRC ${pot-update-SRC} ${LINGUA}.po.dummy)

		endforeach(LINGUA ${LINGUAS})

		# Add to target list
		SET(pot-update-SRC 
			${pot-update-SRC} 
			${PROJECT_SOURCE_DIR}/po/tibastro.pot # should depend on languages
		)


	# The target to call
	add_custom_target(pot-update
		COMMENT "pot-update: Done."
		DEPENDS ${pot-update-SRC}
	)


	##### po update per language #####
	foreach(LINGUA ${LINGUAS})
			add_custom_command(
				# The pot-update depends on a dummy file which we promise to
				# make. We don't make this dummy file but the pot-update
				# still works.
				OUTPUT ${PROJECT_SOURCE_DIR}/po/${LINGUA}.po.lang.dummy
				# After the po file is updated it might look not entirely as
				# wanted, for example poedit reorders the file. Use msgattrib
				# to reformat the file, use a helper file to do so.
				COMMAND ${GETTEXT_MSGATTRIB_EXECUTABLE}
						--output ${LINGUA}.po.tmp
						${LINGUA}.po
				COMMAND ${CMAKE_COMMAND} -E copy
						${LINGUA}.po.tmp
						${LINGUA}.po
				COMMAND ${CMAKE_COMMAND} -E remove
						${LINGUA}.po.tmp
				# Now merge with the pot file.
				COMMAND ${GETTEXT_MSGMERGE_EXECUTABLE}
						--backup=none
						--previous
						-U ${LINGUA}.po
						tibastro.pot
				WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/po
				COMMENT "po-update [${LINGUA}]: Updated po file."
			)

			set(po-update-LINGUA-SRC ${po-update-LINGUA-SRC} 
				${PROJECT_SOURCE_DIR}/po/${LINGUA}.po.lang.dummy
			)

		add_custom_target(po-update-${LINGUA}
			COMMENT "po-update ${LINGUA}: Done."
			DEPENDS ${po-update-LINGUA-SRC}
		)
		set(po-update-LINGUA-SRC "")
	endforeach(LINGUA ${LINGUAS})

endif(ENABLE_POT_UPDATE_TARGET)

if(ENABLENLS)

	set(GETTEXT_MSGFMT_PARAMETER --check-format --check-domain )

	if(ENABLE_POT_UPDATE_TARGET)
		# Only set the verbose flag for maintainers.
		set(GETTEXT_MSGFMT_PARAMETER -v ${GETTEXT_MSGFMT_PARAMETER} )
	endif(ENABLE_POT_UPDATE_TARGET)

	foreach(LINGUA ${LINGUAS})

			add_custom_command(
				OUTPUT ${CMAKE_SOURCE_DIR}/po/${LOCALEDIR}/${LINGUA}/LC_MESSAGES
				COMMAND ${CMAKE_COMMAND} -E make_directory 
						${CMAKE_SOURCE_DIR}/po/${LOCALEDIR}/${LINGUA}/LC_MESSAGES
				COMMENT "mo-update [${LINGUA}]: Creating locale directory."
			)

			add_custom_command(
				OUTPUT ${CMAKE_SOURCE_DIR}/po/${LOCALEDIR}/${LINGUA}/LC_MESSAGES/tibastro.mo
				COMMAND ${GETTEXT_MSGFMT_EXECUTABLE}
						${GETTEXT_MSGFMT_PARAMETER}
						-o ${CMAKE_SOURCE_DIR}/po/${LOCALEDIR}/${LINGUA}/LC_MESSAGES/tibastro.mo ${LINGUA}.po
				DEPENDS 
					${CMAKE_SOURCE_DIR}/po/${LOCALEDIR}/${LINGUA}/LC_MESSAGES
					${PROJECT_SOURCE_DIR}/po/${LINGUA}.po
				WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/po
				COMMENT "mo-update [${LINGUA}]: Creating mo file."
			)

			install(FILES ${CMAKE_SOURCE_DIR}/po/${LOCALEDIR}/${LINGUA}/LC_MESSAGES/tibastro.mo
					DESTINATION ${LOCALE_INSTALL}/${LINGUA}/LC_MESSAGES)

			set(mo-update-SRC ${mo-update-SRC} 
				${CMAKE_SOURCE_DIR}/po/${LOCALEDIR}/${LINGUA}/LC_MESSAGES/tibastro.mo
			)
			set(mo-update-LINGUA-SRC ${mo-update-LINGUA-SRC} 
				${CMAKE_SOURCE_DIR}/po/${LOCALEDIR}/${LINGUA}/LC_MESSAGES/tibastro.mo
			)

		if(ENABLE_POT_UPDATE_TARGET)
			add_custom_target(mo-update-${LINGUA}
				COMMENT "mo-update ${LINGUA}: Done."
				DEPENDS ${mo-update-LINGUA-SRC}
			)
			set(mo-update-LINGUA-SRC "")
		endif(ENABLE_POT_UPDATE_TARGET)

	endforeach(LINGUA ${LINGUAS})

	# The target to call
	add_custom_target(mo-update ALL
		COMMENT "mo-update: Done."
		DEPENDS ${mo-update-SRC}
	)

	set_directory_properties(properties 
		ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_SOURCE_DIR}/po/${LOCALEDIR}
	)

endif(ENABLENLS)
