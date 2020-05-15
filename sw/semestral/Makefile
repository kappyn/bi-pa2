# CONFIGURATION VARIABLES
TARGET = kroupkev

CXX = g++
CXX_FLAGS = -Wall -pedantic -Wextra -std=c++14
MKDIR = mkdir -p

BUILD_DIR = build
SOURCE = src

$(TARGET): $(BUILD_DIR)/main.o $(BUILD_DIR)/CLog.o $(BUILD_DIR)/CQueryParser.o $(BUILD_DIR)/CDatabase.o $(BUILD_DIR)/CRenderSett.o $(BUILD_DIR)/CTable.o $(BUILD_DIR)/CDataParser.o $(BUILD_DIR)/CDatabase.o $(BUILD_DIR)/CFileManager.o
	$(MKDIR) $(BUILD_DIR)
	$(CXX) $(CXX_FLAGS) $^ -o $@

$(BUILD_DIR)/main.o: $(SOURCE)/main.cpp
	$(MKDIR) $(BUILD_DIR)
	$(CXX) $(CXX_FLAGS) src/main.cpp -c -o $@

$(BUILD_DIR)/%.o: $(SOURCE)/*/%.cpp
	$(MKDIR) $(BUILD_DIR)
	$(CXX) $(CXX_FLAGS) $< -c -o $@

.PHONY: clean
clean:
	rm -rf $(TARGET) $(BUILD_DIR)/ 2>/dev/null

# DEPENDENCIES
build/main.o: src/main.cpp src/console/CApplication.hpp \
 src/console/../database/CDatabase.hpp src/console/../database/CTable.hpp \
 src/console/../database/CCell.hpp \
 src/console/../database/CRenderSett.hpp \
 src/console/../database/../console/CLog.hpp \
 src/console/../tool/CFileManager.hpp src/console/../tool/CDataParser.hpp \
 src/console/CConsole.hpp src/console/CQueryParser.hpp
build/CLog.o: src/console/CLog.cpp src/console/CLog.hpp
build/CQueryParser.o: src/console/CQueryParser.cpp src/console/CQueryParser.hpp
build/CDatabase.o: src/database/CDatabase.cpp src/database/CDatabase.hpp \
 src/database/CTable.hpp src/database/CCell.hpp \
 src/database/CRenderSett.hpp src/database/../console/CLog.hpp
build/CRenderSett.o: src/database/CRenderSett.cpp src/database/CRenderSett.hpp
build/CTable.o: src/database/CTable.cpp src/database/CTable.hpp \
 src/database/CCell.hpp src/database/CRenderSett.hpp \
 src/database/../console/CLog.hpp
build/CDataParser.o: src/tool/CDataParser.cpp src/tool/CDataParser.hpp \
 src/tool/../database/CDatabase.hpp src/tool/../database/CTable.hpp \
 src/tool/../database/CCell.hpp src/tool/../database/CRenderSett.hpp \
 src/tool/../database/../console/CLog.hpp
build/CFileManager.o: src/tool/CFileManager.cpp src/tool/CFileManager.hpp \
 src/tool/../database/CDatabase.hpp src/tool/../database/CTable.hpp \
 src/tool/../database/CCell.hpp src/tool/../database/CRenderSett.hpp \
 src/tool/../database/../console/CLog.hpp src/tool/CDataParser.hpp






