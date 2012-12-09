# - Find the tools needed for updating the potfiles and translations
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

set(TRANSLATION_TOOLS_FOUND true)

find_program(GETTEXT_MSGINIT_EXECUTABLE msginit)
if(NOT GETTEXT_MSGINIT_EXECUTABLE)
	message("msginit not found")
	set(TRANSLATION_TOOLS_FOUND false)
endif(NOT GETTEXT_MSGINIT_EXECUTABLE)

find_program(GETTEXT_XGETTEXT_EXECUTABLE xgettext)
if(NOT GETTEXT_XGETTEXT_EXECUTABLE)
	message("xgettext not found")
	set(TRANSLATION_TOOLS_FOUND false)
endif(NOT GETTEXT_XGETTEXT_EXECUTABLE)
set(GETTEXT_XGETTEXT_OPTIONS
	--force-po
	--copyright-holder=\"Tibastro development team\"
	--msgid-bugs-address=\"https://github.com/eroux/tibastro/issues/\"
	--from-code=UTF-8
	--sort-by-file
	--keyword=_
	--keyword=N_
	--keyword=sgettext
	--keyword=vgettext
	--keyword=VGETTEXT
	--keyword=_n:1,2
	--keyword=sngettext:1,2
	--keyword=vngettext:1,2
)

find_program(GETTEXT_MSGCAT_EXECUTABLE msgcat)
if(NOT GETTEXT_MSGCAT_EXECUTABLE )
	message("msgcat not found")
	set(TRANSLATION_TOOLS_FOUND false)
endif(NOT GETTEXT_MSGCAT_EXECUTABLE )

find_program(GETTEXT_MSGATTRIB_EXECUTABLE msgattrib)
if(NOT GETTEXT_MSGATTRIB_EXECUTABLE)
	message("msgattrib not found")
	set(TRANSLATION_TOOLS_FOUND false)
endif(NOT GETTEXT_MSGATTRIB_EXECUTABLE)

find_program(DOS2UNIX_EXECUTABLE dos2unix)
if(NOT DOS2UNIX_EXECUTABLE)
	message("dos2unix not found")
	set(TRANSLATION_TOOLS_FOUND false)
endif(NOT DOS2UNIX_EXECUTABLE)

find_program(PO4A-TRANSLATE_EXECUTABLE po4a-translate)
set(PO4A-TRANSLATE_OPTIONS
	-f docbook
	-k 80
	-M utf-8
	-L utf-8
)
if(NOT PO4A-TRANSLATE_EXECUTABLE)
	message("po4a-translate not found")
	set(TRANSLATION_TOOLS_FOUND false)
endif(NOT PO4A-TRANSLATE_EXECUTABLE)

find_program(PO4A-UPDATEPO_EXECUTABLE po4a-updatepo)
set(PO4A-UPDATEPO_OPTIONS
	-M utf-8
)
if(NOT PO4A-UPDATEPO_EXECUTABLE)
	message("po4a-updatepo not found")
	set(TRANSLATION_TOOLS_FOUND false)
endif(NOT PO4A-UPDATEPO_EXECUTABLE)

find_program(PO4A-GETTEXTIZE_EXECUTABLE po4a-gettextize)
set(PO4A-GETTEXTIZE_OPTIONS
	--copyright-holder "Wesnoth Development Team"
	-f docbook
	-M utf-8
	-L utf-8
)
if(NOT PO4A-GETTEXTIZE_EXECUTABLE)
	message("po4a-gettextize not found")
	set(TRANSLATION_TOOLS_FOUND false)
endif(NOT PO4A-GETTEXTIZE_EXECUTABLE)

find_program(XSLTPROC_EXECUTABLE xsltproc)
set(XSLTPROC_OPTIONS
	--nonet
	--stringparam callout.graphics 0
	--stringparam navig.graphics 0
	--stringparam admon.textlabel 1
	--stringparam admon.graphics 0
	--stringparam html.stylesheet "./styles/manual.css"
)
if(NOT XSLTPROC_EXECUTABLE)
	message("xsltproc not found")
	set(TRANSLATION_TOOLS_FOUND false)
endif(NOT XSLTPROC_EXECUTABLE)

if(NOT TRANSLATION_TOOLS_FOUND)
	if(TranslationTools_FIND_REQUIRED)
		message(FATAL_ERROR "Not all translation tools are found")
	endif(TranslationTools_FIND_REQUIRED)
endif(NOT TRANSLATION_TOOLS_FOUND)

