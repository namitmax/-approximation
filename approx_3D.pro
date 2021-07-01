QT += widgets
CONFIG += c++1z

QMAKE_CXXFLAGS_RELEASE += -O3 -ffast-math

HEADERS       = read_file.h \
		env.h    \
		window.h \
		utils.h \
		draw_tools.h \
		executor.h \
		integrate.h \
		init_iter_alg.h \
		solver.h \
		params.h 

SOURCES       = main.cpp \
		env.cpp \
		read_file.cpp \
		window.cpp \
		utils.cpp \
		integrate.cpp \
		draw_tools.cpp \
		init_iter_alg.cpp \
		solver.cpp \
		executor.cpp

