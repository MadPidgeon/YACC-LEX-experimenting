#include "types.h"
#include "symbol_table.h"

int64_t type_t::free_type_counter = 0;

bool type_t::node::operator==( const type_t::node& other ) const {
	if( free_id >= 0 or other.free_id >= 0 )
		return true;
	if( structure != other.structure )
		return false;
	if( parameters.size() != other.parameters.size() )
		return false;
	for( size_t i = 0; i < parameters.size(); ++i )
		if( *parameters.at(i) != *other.parameters.at(i) )
			return false;
	return true;
}

bool type_t::node::operator!=( const type_t::node& other ) const {
	return not operator==( other );
}

type_t::node* type_t::node::clone() const {
	std::vector<node*> v;
	for( node* n : parameters )
		v.push_back( n->clone() );
	return new node{ free_id, structure, v };
}

type_t::node* type_t::node::intersect( const type_t::node* other ) const {
	const type_t::node* n[2] = {this,other};
	for( size_t i = 0; i < 2; ++i )
		if( n[i]->free_id >= 0 )
			return n[!i]->clone();
	if( n[0]->structure != n[1]->structure or n[0]->parameters.size() != n[1]->parameters.size() )
		return nullptr;
	size_t s = n[0]->parameters.size();
	std::vector<node*> p;
	for( size_t i = 0; i < s; ++i ) {
		p.push_back( n[0]->parameters.at(i)->intersect( n[1]->parameters.at(i) ) );
		if( p.back() == nullptr ) {
			for( size_t j = 0; j < i; ++j ) {
				delete p.at( j );
				return nullptr;
			}
		}
	}
	return new node{free_id,structure,p};
}

type_t::node* type_t::node::applySubstitution( size_t f, type_t target ) {
	if( free_id >= 0 and free_id == int64_t( f ) ) {
		delete this;
		return target.peel();
	}
	for( node*& n : parameters )
		n = n->applySubstitution( f, target );
	return this;
}

size_t type_t::node::rawSize() const {
	if( structure == TUP_STRUCTURE ) {
		size_t sum = 0;
		for( node* n : parameters )
			sum += n->rawSize();
		return sum;
	} else
		return 8; // int, lst*, set*, flt
}

void type_t::node::print( std::ostream& os ) const {
	if( free_id >= 0 ) {
		os << "T" << free_id;
	} else {
		os << symtab->getName( strtab->getName( structure ) );
		auto itr = parameters.cbegin();
		if( itr != parameters.cend() ) {
			os << "(";
			(**itr).print(os);
			for( ++itr; itr != parameters.cend(); ++itr ) {
				os << ",";
				(**itr).print(os);
			}
			os << ")";
		}
	}
}

type_t::node::~node() {
	for( node* n : parameters )
		delete n;
}

structure_t type_t::getBase() const {
	if( root )
		return root->structure;
	return ERROR_STRUCTURE;
}

std::vector<type_t::node*>& type_t::getBaseParameters() {
	if( root )
		return root->parameters;
	throw;
}

type_t type_t::getChildType() const {
	if( root and root->parameters.size() == 1 )
		return type_t( root->parameters.at(0)->clone() );
	return ERROR_TYPE;
}

const type_t::node* type_t::getRoot() const {
	return root;
}

size_t type_t::rawSize() const {
	if( root )
		return root->rawSize();
	throw;
}

type_t::node* type_t::peel() {
	node* r = root;
	root = nullptr;
	return r;
}

bool type_t::isFinal() const {
	return root->free_id == -2;
}

bool type_t::isList() const {
	return getBase() == LST_STRUCTURE;
}

bool type_t::isSet() const {
	return getBase() == SET_STRUCTURE;
}

bool type_t::isFunction() const {
	return getBase() == FNC_STRUCTURE;
}

bool type_t::isTuple() const {
	return getBase() == TUP_STRUCTURE;
}

bool type_t::operator==( const type_t& other ) const {
	if( ( not root ) or ( not other.root ) )
		return root == other.root;
	return root->operator==( *other.root );
}

bool type_t::operator!=( const type_t& other ) const {
	return not operator==( other );
}

type_t type_t::intersect( const type_t& t ) const {
	return type_t( root->intersect( t.getRoot() ) );
}

void type_t::applySubstitution( size_t free_id, type_t target ) {
	root = root->applySubstitution( free_id, target );
}

type_t type_t::rightFlattenTypeProduct( type_t left ) const{
	if( getBase() != TUP_STRUCTURE )
		return ERROR_TYPE;
	type_t r( *this );
	r.getBaseParameters().push_back( left.peel() );
	return r;
}

std::vector<type_t> type_t::unpackProduct() const {
	assert( getBase() == TUP_STRUCTURE );
	std::vector<type_t> r;
	for( node* n : getRoot()->parameters )
		r.push_back( type_t( n->clone() ) );
	return r;
}

type_t type_t::getParameter( int i ) const {
	assert( getRoot() and i < getRoot()->parameters.size() );
	return getRoot()->parameters.at( i )->clone();
}

size_t type_t::getParameterCount() const {
	assert( getRoot() );
	return getRoot()->parameters.size();
}

type_t& type_t::operator=( const type_t& t ) {
	if( t.getRoot() )
		root = t.getRoot()->clone();
	else
		root = nullptr;
	return *this;
}

int type_t::node::cmp( const type_t::node& other ) const {
	if( free_id != other.free_id )
		return free_id - other.free_id;
	if( structure != other.structure )
		return int64_t( structure ) - int64_t( other.structure );
	if( parameters.size() != other.parameters.size() )
		return int64_t( parameters.size() ) - int64_t( other.parameters.size() );
	size_t b = parameters.size();
	for( size_t i = 0; i < b; ++i ) {
		int c = parameters.at( i )->cmp( *other.parameters.at( i ) );
		if( c != 0 )
			return c;
	}
	return 0;
}

bool type_t::operator<( const type_t& other ) const {
	return getRoot()->cmp( *other.getRoot() ) < 0;
}


type_t::type_t( const type_t& t ) : type_t( t.getRoot()->clone() ) {
}

type_t::type_t( type_t&& t ) {
	root = t.root;
	t.root = nullptr;
}

type_t::type_t( type_t::node* n ) {
	root = n;
}

type_t::type_t( int64_t f ) : type_t( new node{f,0} ) {
}

type_t::type_t() : type_t( type_t::free_type_counter++ ) {
}

/*type_t::type_t( structure_t s, std::vector<const type_t*> l ) {
	int64_t f = -2;
	for( size_t i = 0; i < l.size(); ++i )
		if( not l.at(i)->isFinal() )
			f = -1;
	root = new node{ f, s };
	for( size_t i = 0; i < l.size(); ++i )
		root->parameters.push_back( l.at(i)->getRoot()->clone() );
}*/

type_t::type_t( structure_t s, std::vector<type_t> l ) {
	int64_t f = -2;
	for( size_t i = 0; i < l.size(); ++i )
		if( not l.at(i).isFinal() )
			f = -1;
	root = new node{ f, s };
	for( size_t i = 0; i < l.size(); ++i )
		root->parameters.push_back( l.at(i).peel() );
}

type_t::~type_t() {
	if( root )
		delete root;
}

std::ostream& operator<<( std::ostream& os, const type_t& t ) {
	t.getRoot()->print(os);
	return os;
}


type_t typedefTable::getType( symbol_t s, std::vector<type_t> pars ) const {
	auto loc = definitions.find( s );
	if( loc == definitions.end() )
		return ERROR_TYPE;
	const typedefinition& d = loc->second;
	if( d.parameters.size() != pars.size() ) {
		lerr << error_line() << "Incorrect number of arguments to type " + std::to_string( s ) << " (expected " << d.parameters.size() << ", received " << pars.size() << ")" << std::endl;
		return ERROR_TYPE;
	}
	std::vector<type_t> arguments;
	for( const type_t& sub : d.substitutions ) {
		type_t arg = sub;
		size_t i = 0;
		for( const type_t& rule : pars ) {
			arg.applySubstitution( i, rule );
			++i;
		}
		arguments.push_back( arg );
	}
	return type_t( d.base, arguments );
}

bool typedefTable::addType( symbol_t symbol, structure_t structure, std::vector<type_t> par, std::vector<type_t> sub ) {
	if( definitions.count( symbol ) == 1 )
		return false;
	typedefinition t = { structure, symbol, par, sub };
	definitions[symbol] = t;
	return true;
}


symbol_t structureTable::getName( structure_t s ) const {
	if( s >= structures.size() )
		return ERROR_SYMBOL;
	return structures.at(s).symbol;
}

structure_t structureTable::addStructure( symbol_t s, std::vector<type_t> param ) {
	structure_t id = structures.size();
	structures.push_back(structure{ s, id, param });
	return id;
}

structureTable::structureTable() {
	assert( addStructure( ERROR_SYMBOL ) == ERROR_STRUCTURE );
	assert( addStructure( ERROR_SYMBOL ) == FREE_STRUCTURE );
	assert( addStructure( VOID_SYMBOL ) == VOID_STRUCTURE );
	assert( addStructure( INT_SYMBOL ) == INT_STRUCTURE );
	assert( addStructure( FLT_SYMBOL ) == FLT_STRUCTURE );
	assert( addStructure( STR_SYMBOL ) == STR_STRUCTURE );
	assert( addStructure( LST_SYMBOL, {type_t()} ) == LST_STRUCTURE );
	assert( addStructure( SET_SYMBOL, {type_t()} ) == SET_STRUCTURE );
	assert( addStructure( TUP_SYMBOL ) == TUP_STRUCTURE );
	assert( addStructure( FNC_SYMBOL, {type_t(),type_t()} ) == FNC_STRUCTURE );
	assert( ERROR_TYPE == ERROR_TYPE );
	assert( INT_TYPE != ERROR_TYPE );
}

const type_t ERROR_TYPE( ERROR_STRUCTURE, {} );
const type_t VOID_TYPE( VOID_STRUCTURE, {} );
const type_t INT_TYPE( INT_STRUCTURE, {} );
const type_t FLT_TYPE( FLT_STRUCTURE, {} );
const type_t STR_TYPE( STR_STRUCTURE, {} );
const type_t TUP_TYPE( TUP_STRUCTURE, {} );
