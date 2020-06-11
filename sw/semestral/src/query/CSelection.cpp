#include "CSelection.hpp"

CSelection::CSelection ( CDatabase & ref, vector<string> cols, string tableName )
		: m_Database( ref ), m_SelectedCols( std::move( cols ) ), m_TableName( std::move( tableName ) ) {
	cout << "New selection query initiated!" << endl;
	cout << "Table name: " << m_TableName << "\nColumns: " << endl;
	for ( const string & i : m_SelectedCols )
		cout << i << endl;
}

bool CSelection::Evaluate ( ) const {
	CTable * tableRef;
	if ( ( tableRef = m_Database.GetTable( m_TableName ) ) == nullptr ) {
		CLog::HighlightedMsg( CLog::QP, m_TableName, CLog::QP_NO_SUCH );
		return false;
	}
	cout << * tableRef;
	return true;
}
