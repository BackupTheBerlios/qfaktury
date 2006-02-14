TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

INSTALLS += templates \
	icons \
	applications \
	pixmaps \
	target

applications.files += ./share/applications/*.desktop
applications.path = /usr/share/applications
templates.files += ./share/qfaktury/templates/*.css
templates.path = /usr/share/qfaktury/templates 
icons.files += ./share/qfaktury/icons/*
icons.path = /usr/share/qfaktury/icons
pixmaps.files = ./share/qfaktury/icons/qfaktury_48.png
pixmaps.path = /usr/share/pixmaps
target.path = /usr/bin 

DEFAULTCODEC = ISO8859-2

SOURCES	+= main.cpp \
	slownie.cpp \
	version.cpp \
	zaokr.cpp

FORMS	= kontr.ui \
	kreator.ui \
	mainform.ui \
	preview.ui \
	fraform.ui \
        sett.ui \
	user.ui \ 
	kontlist.ui \
	towlist.ui \
	chamount.ui \
	korform.ui \
	towary.ui 
	

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

TARGET = ./bin/qfaktury
