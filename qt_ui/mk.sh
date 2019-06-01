g++ -c -m64 -pipe -O2 -Wall -W -D_REENTRANT -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED -I~/Qt5.7.0/5.7/gcc_64/mkspecs/linux-g++-64/linux-g++-64 -I. -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/   -l/home/abhi/Qt5.7.0/5.7/gcc_64/include/QQmlContext -l/usr/include/x86_64-linux-gnu/qt5/QtQml/QQmlContext -l/usr/include/x86_64-linux-gnu/qt5/QtWidgets/QApplication -I. -o main.o main.cpp




'''
g++ -c -m64 -pipe -O2 -Wall -W -D_REENTRANT -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED -I/usr/share/qt4/mkspecs/linux-g++-64 -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4   -l/home/abhi/Qt5.7.0/5.7/gcc_64/include/QQmlContext -l/usr/include/x86_64-linux-gnu/qt5/QtQml/QQmlContext -I. -o main.o main.cpp


''' 


# g++ -c -pipe -O2 -Wall -W -D_REENTRANT -DQT_NO_DEBUG -DQT_DECLARATIVE_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED -I/opt/qt_47x/mkspecs/linux-g++ -I. -I/opt/qt_47x/include/QtCore -I/opt/qt_47x/include/QtGui -I/opt/qt_47x/include/QtDeclarative -I/opt/qt_47x/include -I/usr/X11R6/include -I. -o main.o main.cpp

# g++ -Wl,-O1 -Wl,-rpath,/opt/qt_47x/lib -o main main.o -L/opt/qt_47x/lib -L/usr/X11R6/lib -lQtDeclarative -L/opt/qt_47x/lib -lQtScript -lQtSvg -L/usr/X11R6/lib -lQtSql -lQtXmlPatterns -lQtNetwork -lQtGui -lQtCore -lpthread

