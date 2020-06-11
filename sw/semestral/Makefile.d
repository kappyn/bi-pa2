build/CDatabase.o: src/database/CDatabase.cpp src/database/CDatabase.hpp \
 src/database/CTable.hpp src/database/CCell.hpp \
 src/database/CRenderSett.hpp src/database/../console/CLog.hpp
build/CRenderSett.o: src/database/CRenderSett.cpp src/database/CRenderSett.hpp
build/CTable.o: src/database/CTable.cpp src/database/CTable.hpp \
 src/database/CCell.hpp src/database/CRenderSett.hpp \
 src/database/../console/CLog.hpp
build/CCell.o: src/database/CCell.cpp src/database/CCell.hpp
build/CLog.o: src/console/CLog.cpp src/console/CLog.hpp
build/CConsole.o: src/console/CConsole.cpp src/console/CConsole.hpp \
 src/console/CLog.hpp src/console/CQueryParser.hpp \
 src/console/../tool/CDataParser.hpp \
 src/console/../tool/../database/CDatabase.hpp \
 src/console/../tool/../database/CTable.hpp \
 src/console/../tool/../database/CCell.hpp \
 src/console/../tool/../database/CRenderSett.hpp \
 src/console/../query/CSelection.hpp src/console/../query/CTableQuery.hpp
build/CApplication.o: src/console/CApplication.cpp src/console/CApplication.hpp \
 src/console/../database/CDatabase.hpp src/console/../database/CTable.hpp \
 src/console/../database/CCell.hpp \
 src/console/../database/CRenderSett.hpp \
 src/console/../database/../console/CLog.hpp \
 src/console/../tool/CFileManager.hpp src/console/../tool/CDataParser.hpp \
 src/console/CConsole.hpp src/console/CQueryParser.hpp \
 src/console/../query/CSelection.hpp src/console/../query/CTableQuery.hpp
build/CQueryParser.o: src/console/CQueryParser.cpp src/console/CQueryParser.hpp \
 src/console/CLog.hpp src/console/../tool/CDataParser.hpp \
 src/console/../tool/../database/CDatabase.hpp \
 src/console/../tool/../database/CTable.hpp \
 src/console/../tool/../database/CCell.hpp \
 src/console/../tool/../database/CRenderSett.hpp \
 src/console/../query/CSelection.hpp src/console/../query/CTableQuery.hpp
build/CSelection.o: src/query/CSelection.cpp src/query/CSelection.hpp \
 src/query/CTableQuery.hpp src/query/../console/CLog.hpp \
 src/query/../database/CTable.hpp src/query/../database/CCell.hpp \
 src/query/../database/CRenderSett.hpp \
 src/query/../database/CDatabase.hpp
build/main.o: src/main.cpp src/console/CApplication.hpp \
 src/console/../database/CDatabase.hpp src/console/../database/CTable.hpp \
 src/console/../database/CCell.hpp \
 src/console/../database/CRenderSett.hpp \
 src/console/../database/../console/CLog.hpp \
 src/console/../tool/CFileManager.hpp src/console/../tool/CDataParser.hpp \
 src/console/CConsole.hpp src/console/CQueryParser.hpp \
 src/console/../query/CSelection.hpp src/console/../query/CTableQuery.hpp
build/CDataParser.o: src/tool/CDataParser.cpp src/tool/CDataParser.hpp \
 src/tool/../database/CDatabase.hpp src/tool/../database/CTable.hpp \
 src/tool/../database/CCell.hpp src/tool/../database/CRenderSett.hpp \
 src/tool/../database/../console/CLog.hpp
build/CFileManager.o: src/tool/CFileManager.cpp src/tool/CFileManager.hpp \
 src/tool/../database/CDatabase.hpp src/tool/../database/CTable.hpp \
 src/tool/../database/CCell.hpp src/tool/../database/CRenderSett.hpp \
 src/tool/../database/../console/CLog.hpp src/tool/CDataParser.hpp
