$(BUILD_DIR)/CDatabase.o: src/database/CDatabase.cpp src/database/CDatabase.hpp \
 src/database/CTable.hpp src/database/CCell.hpp \
 src/database/CRenderSett.hpp src/database/../console/CLog.hpp
$(BUILD_DIR)/CRenderSett.o: src/database/CRenderSett.cpp src/database/CRenderSett.hpp
$(BUILD_DIR)/CTable.o: src/database/CTable.cpp src/database/CTable.hpp \
 src/database/CCell.hpp src/database/CRenderSett.hpp \
 src/database/../console/CLog.hpp
$(BUILD_DIR)/CLog.o: src/console/CLog.cpp src/console/CLog.hpp
$(BUILD_DIR)/CQueryParser.o: src/console/CQueryParser.cpp src/console/CQueryParser.hpp
$(BUILD_DIR)/main.o: src/main.cpp src/console/CApplication.hpp \
 src/console/../database/CDatabase.hpp src/console/../database/CTable.hpp \
 src/console/../database/CCell.hpp \
 src/console/../database/CRenderSett.hpp \
 src/console/../database/../console/CLog.hpp \
 src/console/../tool/CFileManager.hpp src/console/../tool/CDataParser.hpp \
 src/console/CConsole.hpp src/console/CQueryParser.hpp
$(BUILD_DIR)/CDataParser.o: src/tool/CDataParser.cpp src/tool/CDataParser.hpp \
 src/tool/../database/CDatabase.hpp src/tool/../database/CTable.hpp \
 src/tool/../database/CCell.hpp src/tool/../database/CRenderSett.hpp \
 src/tool/../database/../console/CLog.hpp
$(BUILD_DIR)/CFileManager.o: src/tool/CFileManager.cpp src/tool/CFileManager.hpp \
 src/tool/../database/CDatabase.hpp src/tool/../database/CTable.hpp \
 src/tool/../database/CCell.hpp src/tool/../database/CRenderSett.hpp \
 src/tool/../database/../console/CLog.hpp src/tool/CDataParser.hpp
