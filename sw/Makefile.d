build/CDatabase.o: src/database/CDatabase.cpp src/database/CDatabase.hpp \
 src/database/CTable.hpp src/database/CCell.hpp \
 src/database/CRenderSett.hpp src/database/../console/CLog.hpp \
 src/database/CCondition.hpp src/database/../query/CTableQuery.hpp \
 src/database/../query/CQueryOperand.hpp
build/CRenderSett.o: src/database/CRenderSett.cpp src/database/CRenderSett.hpp
build/CTable.o: src/database/CTable.cpp src/database/CTable.hpp \
 src/database/CCell.hpp src/database/CRenderSett.hpp \
 src/database/../console/CLog.hpp src/database/CCondition.hpp
build/CCell.o: src/database/CCell.cpp src/database/CCell.hpp
build/CConsole.o: src/console/CConsole.cpp src/console/CConsole.hpp \
 src/console/CLog.hpp src/console/CQueryParser.hpp \
 src/console/../tool/CDataParser.hpp \
 src/console/../tool/../database/CDatabase.hpp \
 src/console/../tool/../database/CTable.hpp \
 src/console/../tool/../database/CCell.hpp \
 src/console/../tool/../database/CRenderSett.hpp \
 src/console/../tool/../database/CCondition.hpp \
 src/console/../tool/../database/../query/CTableQuery.hpp \
 src/console/../tool/../database/../query/CQueryOperand.hpp \
 src/console/../query/CSelection.hpp src/console/../query/CProjection.hpp \
 src/console/../query/CNaturalJoin.hpp src/console/../query/CJoin.hpp \
 src/console/../query/CUnion.hpp src/query/CBinaryQuery.hpp \
 src/console/../query/CIntersect.hpp src/console/../query/CMinus.hpp
build/CApplication.o: src/console/CApplication.cpp src/console/CApplication.hpp \
 src/console/../database/CDatabase.hpp src/console/../database/CTable.hpp \
 src/console/../database/CCell.hpp \
 src/console/../database/CRenderSett.hpp \
 src/console/../database/../console/CLog.hpp \
 src/console/../database/CCondition.hpp \
 src/console/../database/../query/CTableQuery.hpp \
 src/console/../database/../query/CQueryOperand.hpp \
 src/console/../tool/CFileManager.hpp src/console/../tool/CDataParser.hpp \
 src/console/CConsole.hpp src/console/CQueryParser.hpp \
 src/console/../query/CSelection.hpp src/console/../query/CProjection.hpp \
 src/console/../query/CNaturalJoin.hpp src/console/../query/CJoin.hpp \
 src/console/../query/CUnion.hpp src/query/CBinaryQuery.hpp \
 src/console/../query/CIntersect.hpp src/console/../query/CMinus.hpp
build/CQueryParser.o: src/console/CQueryParser.cpp src/console/CQueryParser.hpp \
 src/console/CLog.hpp src/console/CConsole.hpp \
 src/console/../tool/CDataParser.hpp \
 src/console/../tool/../database/CDatabase.hpp \
 src/console/../tool/../database/CTable.hpp \
 src/console/../tool/../database/CCell.hpp \
 src/console/../tool/../database/CRenderSett.hpp \
 src/console/../tool/../database/CCondition.hpp \
 src/console/../tool/../database/../query/CTableQuery.hpp \
 src/console/../tool/../database/../query/CQueryOperand.hpp \
 src/console/../query/CSelection.hpp src/console/../query/CProjection.hpp \
 src/console/../query/CNaturalJoin.hpp src/console/../query/CJoin.hpp \
 src/console/../query/CUnion.hpp src/query/CBinaryQuery.hpp \
 src/console/../query/CIntersect.hpp src/console/../query/CMinus.hpp
build/CUnion.o: src/query/CUnion.cpp src/query/CUnion.hpp \
 src/query/CBinaryQuery.hpp src/query/../database/CDatabase.hpp \
 src/query/../database/CTable.hpp src/query/../database/CCell.hpp \
 src/query/../database/CRenderSett.hpp \
 src/query/../database/../console/CLog.hpp \
 src/query/../database/CCondition.hpp \
 src/query/../database/../query/CTableQuery.hpp \
 src/query/../database/../query/CQueryOperand.hpp
build/CJoin.o: src/query/CJoin.cpp src/query/CJoin.hpp \
 src/query/CTableQuery.hpp src/query/../console/CLog.hpp \
 src/query/../database/CTable.hpp src/query/../database/CCell.hpp \
 src/query/../database/CRenderSett.hpp \
 src/query/../database/CCondition.hpp src/query/../database/CDatabase.hpp \
 src/query/../database/../query/CQueryOperand.hpp
build/CIntersect.o: src/query/CIntersect.cpp src/query/CIntersect.hpp \
 src/query/CBinaryQuery.hpp src/query/../database/CDatabase.hpp \
 src/query/../database/CTable.hpp src/query/../database/CCell.hpp \
 src/query/../database/CRenderSett.hpp \
 src/query/../database/../console/CLog.hpp \
 src/query/../database/CCondition.hpp \
 src/query/../database/../query/CTableQuery.hpp \
 src/query/../database/../query/CQueryOperand.hpp
build/CSetQuery.o: src/query/CBinaryQuery.cpp src/query/CBinaryQuery.hpp \
 src/query/../database/CDatabase.hpp src/query/../database/CTable.hpp \
 src/query/../database/CCell.hpp src/query/../database/CRenderSett.hpp \
 src/query/../database/../console/CLog.hpp \
 src/query/../database/CCondition.hpp \
 src/query/../database/../query/CTableQuery.hpp \
 src/query/../database/../query/CQueryOperand.hpp
build/CNaturalJoin.o: src/query/CNaturalJoin.cpp src/query/CNaturalJoin.hpp \
 src/query/CTableQuery.hpp src/query/../console/CLog.hpp \
 src/query/../database/CTable.hpp src/query/../database/CCell.hpp \
 src/query/../database/CRenderSett.hpp \
 src/query/../database/CCondition.hpp src/query/../database/CDatabase.hpp \
 src/query/../database/../query/CQueryOperand.hpp
build/CProjection.o: src/query/CProjection.cpp src/query/CProjection.hpp \
 src/query/CTableQuery.hpp src/query/../console/CLog.hpp \
 src/query/../database/CTable.hpp src/query/../database/CCell.hpp \
 src/query/../database/CRenderSett.hpp \
 src/query/../database/CCondition.hpp src/query/../database/CDatabase.hpp \
 src/query/../database/../query/CQueryOperand.hpp
build/CMinus.o: src/query/CMinus.cpp src/query/CMinus.hpp \
 src/query/CBinaryQuery.hpp src/query/../database/CDatabase.hpp \
 src/query/../database/CTable.hpp src/query/../database/CCell.hpp \
 src/query/../database/CRenderSett.hpp \
 src/query/../database/../console/CLog.hpp \
 src/query/../database/CCondition.hpp \
 src/query/../database/../query/CTableQuery.hpp \
 src/query/../database/../query/CQueryOperand.hpp
build/CSelection.o: src/query/CSelection.cpp src/query/CSelection.hpp \
 src/query/CTableQuery.hpp src/query/../console/CLog.hpp \
 src/query/../database/CTable.hpp src/query/../database/CCell.hpp \
 src/query/../database/CRenderSett.hpp \
 src/query/../database/CCondition.hpp src/query/../database/CDatabase.hpp \
 src/query/../database/../query/CQueryOperand.hpp
build/main.o: src/main.cpp src/console/CApplication.hpp \
 src/console/../database/CDatabase.hpp src/console/../database/CTable.hpp \
 src/console/../database/CCell.hpp \
 src/console/../database/CRenderSett.hpp \
 src/console/../database/../console/CLog.hpp \
 src/console/../database/CCondition.hpp \
 src/console/../database/../query/CTableQuery.hpp \
 src/console/../database/../query/CQueryOperand.hpp \
 src/console/../tool/CFileManager.hpp src/console/../tool/CDataParser.hpp \
 src/console/CConsole.hpp src/console/CQueryParser.hpp \
 src/console/../query/CSelection.hpp src/console/../query/CProjection.hpp \
 src/console/../query/CNaturalJoin.hpp src/console/../query/CJoin.hpp \
 src/console/../query/CUnion.hpp src/query/CBinaryQuery.hpp \
 src/console/../query/CIntersect.hpp src/console/../query/CMinus.hpp
build/CDataParser.o: src/tool/CDataParser.cpp src/tool/CDataParser.hpp \
 src/tool/../database/CDatabase.hpp src/tool/../database/CTable.hpp \
 src/tool/../database/CCell.hpp src/tool/../database/CRenderSett.hpp \
 src/tool/../database/../console/CLog.hpp \
 src/tool/../database/CCondition.hpp \
 src/tool/../database/../query/CTableQuery.hpp \
 src/tool/../database/../query/CQueryOperand.hpp
build/CFileManager.o: src/tool/CFileManager.cpp src/tool/CFileManager.hpp \
 src/tool/../database/CDatabase.hpp src/tool/../database/CTable.hpp \
 src/tool/../database/CCell.hpp src/tool/../database/CRenderSett.hpp \
 src/tool/../database/../console/CLog.hpp \
 src/tool/../database/CCondition.hpp \
 src/tool/../database/../query/CTableQuery.hpp \
 src/tool/../database/../query/CQueryOperand.hpp src/tool/CDataParser.hpp
