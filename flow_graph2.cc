

// ***********************************************
// * I/O
// ***********************************************

void flowGraph::print( std::ostream& os, bool info ) const {
	for( const auto& b : basic_blocks ) {
		if( info ) {
			os << "BLOCK " << b.label << " [to " << b.children[0] << " " << b.children[1] << "; from";
			for( basic_block_t x : b.parents )
				os << " " << x;
			os << "]:\n";
			os << "USE: ";
			for( variable_t x : b.use )
				os << symtab->getName( scptab->getVariableSymbol( x ) ) << " ";
			os << "\nDEF: ";
			for( variable_t x : b.def )
				os << symtab->getName( scptab->getVariableSymbol( x ) ) << " ";
			os << "\nLIVE IN : ";
			for( variable_t x : b.in )
				os << symtab->getName( scptab->getVariableSymbol( x ) ) << " ";
			os << "\nLIVE OUT: ";
			for( variable_t x : b.out )
				os << symtab->getName( scptab->getVariableSymbol( x ) ) << " ";
			os << "\nCONST IN: ";
			for( const auto& p : b.const_in )
				os << symtab->getName( scptab->getVariableSymbol( p.first ) ) << " ";
			os << "\nCONST OUT: ";
			for( const auto& p : b.const_out )
				os << symtab->getName( scptab->getVariableSymbol( p.first ) ) << " ";
			os << "\n";
		}
		size_t j = b.instruction_offset;
		if( ( not info ) and b.label != ERROR_LABEL )
			os << b.label << ":\n"; 
		for( iop_t op : b.operations )
			os << " (" << std::setw(2) << (j++) << ") " << op << std::endl;
		/*{// temp
			for( auto i : b.computeLiveness() )
				os << i << std::endl;
		}*/
	}
}


std::ostream& operator<<( std::ostream& os, const live_interval_t& i ) {
	return os << symtab->getName( scptab->getVariableSymbol( i.variable ) ) << " [" << i.begin << "," << i.end << "]"; 
}

std::ostream& operator<<( std::ostream& os, const flowGraph& fg ) {
	fg.print( os, true );
	return os;
}