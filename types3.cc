#include "types.h"
#include "symbol_table.h"

// immediate values

std::ostream& operator<<( std::ostream& os, const immediateValue v ) {
	switch( v.type ) {
		case ERROR_TYPE:
			os << "ERROR";
			break;
		case VOID_TYPE:
			os << "VOID";
			break;
		case INT_TYPE:
			os << v.value.integer;
			break;
		case FLT_TYPE:
			os << v.value.floating;
			break;
		case STR_TYPE:
			os << std::string( v.value.string );
			break;
		default: {
			os << "(";
			auto itr = v.sub_values.cbegin();
			if( itr != v.sub_values.cend() )
				os << (*itr);
			for( ++itr; itr != v.sub_values.cend(); ++itr )
				os << "," << (*itr);
			os << ")";
		}
	}
	return os;
}

// typedef table

typedef_t typedefTable::addStructure( symbol_t s, std::vector<definition::parameter> par, std::vector<definition::struct_member> mem ) {
	typedef_t t = addStructure( par, mem );
	definitions.at( t ).symbol = s;
	return t;
}

typedef_t typedefTable::addStructure( std::vector<definition::parameter> par, std::vector<definition::struct_member> mem ) {
	definition d;
	d.symbol = ERROR_SYMBOL;
	d.base = VOID_TYPEDEF;
	d.parameters = par;
	d.struct_members = mem;
	return addIfNotExists( d );
}

typedef_t typedefTable::addIfNotExists( definition d ) {
	auto itr = inverse_definitions.find( d );
	if( itr != inverse_definitions.cend() )
		return itr->id;
	d.id = definitions.size();
	definitions.push_back( d );
	inverse_definitions[d] = d.id;
	return d.id;
}

typedef_t typedefTable::addSpecialization( typedef_t t, std::vector<definition::parameter> par, std::vector<definition::struct_member> sub ) {
	definition d;
	d.symbol = ERROR_SYMBOL;
	d.base = t;
	d.parameters = par;
	const definition& o = definitions.at( t );
	for( size_t i = 0; i < o.struct_members.size(); ++i ) {
		definition::struct_member m;
		const auto& om = o.struct_members.at( i );
		m.symbol = om.symbol;
		if( om.mode == free_type ) {
			m.mode = sub.at( om.type.free ).mode;
			m.type = sub.at( om.type.free ).type;
			d.struct_members.push_back( m );
		} else { // om.mode == fixed_type
			m.mode = fixed_type;
			std::vector<definition::parameter> np;
			for( size_t j = 0; j < om.parameters.size(); ++j ) {
				definition::parameter p = om.;

				np.push_back( p );
			}
			m.type.fixed = addSpecialization( om.type.fixed, np, ns );
			m.parameters = np;
			d.struct_members.push_back( m );
		}
	}
	return addIfNotExists( d );
}

typedef_t typedefTable::addTypeDefinition( symbol_t s ) {
	definition d;
	d.symbol = s;
	d.id = definitions.size();
	d.base = VOID_TYPEDEF;
	return addIfNotExists( d );
}

typedef_t typedefTable::addTypeDefinition( symbol_t s, std::vector<std::pair<typedef_t,symbol_t>> members ) {
	definition d;
	d.symbol = s;
	d.id = definitions.size();
	for( auto m : members ) {
		definition::struct_member mem;
		mem.symbol = m.second;
		mem.type = m.first;
		d.struct_members.push_back( mem );
	}
	d.base = VOID_TYPEDEF;
	return addIfNotExists( d );
}

typedef_t typedefTable::addTypeDefinition( symbol_t s, std::vector<typedefTable::definition::parameter> p, std::vector<typedefTable::definition::struct_member> m ) {
	definition d;
	d.symbol = s;
	d.id = definitions.size();
	d.parameters = p;
	d.struct_members = m;
	d.base = VOID_TYPEDEF;
	return addIfNotExists( d );
}

definition typedefTable::computeSpecialization( typedef_t original, std::vector<definition::parameter> new_parameters, std::vector<definition::parameter> old_parameters ) const {
	const definition& o = definitions.at( original );
	definition d;
	d.parameters = new_parameters;
	for( size_t i = 0; i < o.struct_members.size(); ++i ) {
		definition::struct_member sm;
		sm.symbol = o.struct_members.at( i ).symbol;
		typedef_t general_type = o.struct_members.at( i ).type;
		const definition& m = definitions.at( general_type );
		for( size_t j = 0; j < m.parameters.size(); ++j ) {
			const definition::parameter& p = m.parameters.at(j);
			if( p.mode  )
		}
		sm.type = computeSpecialization( general_type, np, op );

		
		typedef_t mt = m.type;
		sm.
	}
	d.symbol = NONE_SYMBOL;
	d.id = ERROR_TYPEDEF;
	d.parameters = new_parameters;

}

typedef_t typedefTable::getTypeSpecialization( symbol_t s, typedef_t base_def, std::vector<definition::parameter> new_parameters, std::vector<definition::parameter> old_parameters ) {
	definition d;
	d._symbol = s;
	d.id = definitions.size();
	d.parameters = new_parameters;
	for(  )
	definition::struct_member m;
	m.symbol = SYMBOL_NONE;
	m.type = base_def;
	m.parameters = old_parameters;
	d.struct_members = {m};
	d.base = base_def;
	return addIfNotExists( d );
}

typedef_t typedefTable::addIfNotExists( definition d ) {
	typedef_t r = findTypeDefinition( d );
	if( r == ERROR_TYPEDEF ) {
		r = d.id;
		definitions.push_back( d );
	}
	return r;
}

typedef_t typedefTable::findTypeDefinition( const definition& d ) const {
	if( d.base == VOID_TYPEDEF )
		return ERROR_TYPEDEF;
	for( typedef_t spc : definitions.at( d.base ).specializations ) {
		if( definitions.at( spc ) == d )
			return spc;
	}
	return ERROR_TYPE;
}

bool typedefTable::operator==( const definition& other ) const {
	const definition& d[2] = {*this,other};
	// check if both have same positive number of member variables
	/*if( d[0].struct_members.size() != d[1].struct_members.size() )
		return false;
	if( d[0].struct_members.size() == 0 and d[1].struct_members.size() == 0 )
		return true;*/
	// check if any is a specialization
	if( d[0].base != VOID_TYPEDEF and d[1].base != VOID_TYPEDEF )
		return false;
	for( int i = 0; i < 2; ++i ) 
		if( d[i].base != VOID_TYPEDEF and d[i].base != d[!i].id and d[i].base != d[!i].base )
			return false;
	// check if both take the same parameters
	if( d[0].parameters.size() != d[1].parameters.size() )
		return false;
	size_t pars = d[0].parameters.size();
	for( int j = 0; j < pars; ++j )
		if( d[0].parameters.at(j).mode != d[1].parameters.at(j).mode )
			return false;
	for( int j = 0; j < pars; ++j )
		if( ( d[0].parameters.at(j).mode == fixed_immediate and 
				d[0].parameters.at(j).type.fixed_immediate->type != d[1].parameters.at(j).fixed_immediate->type ) )
			or ( d[0].parameters.at(j).mode != fixed_immediate and d[0].parameters.at(j).type.free_type != d[1].parameters.at(j).type.free_type )
			return false;
	// fuck it
	return true;
}

bool typedefTable::isFinal( typedef_t d ) const {
	return definitions.at( d ).parameters.size() == 0;
}

typedefTable::typedefTable() {
	addTypeDefinition( ERROR_SYMBOL );
	addTypeDefinition( NONE_SYMBOL );
	addTypeDefinition( INT_SYMBOL );
	addTypeDefinition( FLT_SYMBOL );
	addTypeDefinition( STR_SYMBOL );
}

// type table

typedef_t typeTable::getTypedef( type_t t ) const {
	return types.at(t).def;
}

size_t typeTable::getSize( type_t t ) const {
	return types.at(t).size;
}

type_t typeTable::addType( typedef_t d ) {
	if( definitions->isFinal( d ) and inverse_def.find( d ) == inverse_def.end() ) {
		type_t id = types.size();
		types.push_back({ id, d, 0 });
		inverse_def[d] = id;
		return id;
	} else
		return ERROR_TYPE;
}

typeTable::typeTable( const typedefTable* ds ) {
	definitions = ds;
	addType( ERROR_TYPEDEF );
	addType( VOID_TYPEDEF );
	addType( INT_TYPEDEF );
	addType( FLT_TYPEDEF );
	addType( STR_TYPEDEF );
}
