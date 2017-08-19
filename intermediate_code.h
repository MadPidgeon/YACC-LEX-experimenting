#pragma once
#include <vector>
#include <iostream>
#include "scope_table.h"
#include "syntax_tree.h"
#include "symbol_table.h"

typedef uint64_t label_t;

#define ERROR_LABEL		0

#define IOFR			(1 << 0)
#define IOFA			(1 << 1)
#define IOFB			(1 << 2)
#define IOFL			(1 << 3)
#define IOFF			(1 << 5) // is floating point operation

struct iop_t {
	enum id_t {
		// Arguments to operations are of the form [rabl] where r is the store variable, a and b are the operands, and l is a label.
		// A (W) symbol implies that this variable is written to, which is implicit for r.
		IOP_NOP,		// [----]
		IOP_INT_MOV,	// [ra--]
		IOP_STR_MOV,	// [ra--]
		IOP_LABEL,		// [---l]
		// jumping instructions (3-10)
		IOP_JUMP, 		// [---l]
		IOP_JE, IOP_JN, IOP_JL, IOP_JG, IOP_JLE, IOP_JGE,
						// [-a-l] (integer,label) compares a to 0
		IOP_FJ,			// [r--l] (counter,head)
		// function instructions (11-13)
		IOP_ADD_PARAM,	// [-a--]
		IOP_RESERVE_RETURN, // [-a--]
		IOP_FUNCTION, 	// [r--l]
		IOP_RETURN,		// [-a--]
		// arithmetic instructions (14-22)
		IOP_INT_ADDEQ, IOP_INT_SUBEQ, IOP_INT_MULEQ, 
						// [ra--]
		IOP_INT_MODDIV,	// [rab-] (dividend,divisor,remaider (W))
		IOP_FLT_ADDEQ, IOP_FLT_SUBEQ, IOP_FLT_MULEQ, IOP_FLT_DIVEQ,
						// [ra--]
		IOP_STR_CONEQ,	// [ra--]
		// comparison instructions (23-24)
		IOP_INT_EQ, IOP_INT_NEQ,
						// [rab-]
		// container instructions (25-26)
		IOP_INT_ARR_LOAD, 
						// [rab-] (variable, base, offset)
		IOP_INT_ARR_STORE,
						// [rab-] ((R) base, offset, variable)
		IOP_LIST_ALLOCATE,	
						// [ra--] (memory adress,size in bytes)

		COUNT
	};
	id_t id;
	variable_t r;
	variable_t a, b;
	label_t label;
	union constant_t {
		int64_t integer;
		double floating;
		char* string;
	} c_a, c_b;
	std::vector<variable_t> getReadVariables() const;
	std::vector<variable_t> getWrittenVariables() const;
	std::vector<variable_t> getReadOnlyVariables() const;
	std::vector<variable_t> getWriteOnlyVariables() const;
	void setParameterVariable( int i, variable_t v );
	void setParameterInteger( int i, int64_t x );
	variable_t getParameterVariable( int i ) const;
	int64_t getParameterInteger( int i ) const;
	bool parameterIsVariable( int i ) const;
	bool parameterIsWritten( int i ) const;
	bool parameterIsRead( int i ) const;
	bool isFloating() const;
	bool usesResultParameter() const;
	bool usesReadParameterA() const;
	bool usesReadParameterB() const;
	bool usesLabel() const;
	std::string debugName() const;
	bool operator==( const iop_t& ) const;
};

class intermediateCode {
	friend std::ostream& operator<<( std::ostream&, const intermediateCode& );
public:
	struct function {
		symbol_t symbol;
		function_t id;
		label_t label;
		intermediateCode* parent;
		std::vector<iop_t> operations;
		void translateNode( const syntaxTree::node* );
		void translateAssign( const syntaxTree::node* );
		void translateArguments( const syntaxTree::node* );
		variable_t translateArithmetic( const syntaxTree::node* );
		void translateBranching( const syntaxTree::node* );
		void translateBlock( const syntaxTree::node* );
		variable_t translateConstant( const syntaxTree::node* n );
		variable_t translateContainer( const syntaxTree::node* n );
		variable_t translateExpression( const syntaxTree::node* n );
		variable_t translateFunctionCall( const syntaxTree::node* n );
		variable_t translateFunctionOperation( const syntaxTree::node* n );
		void translateListElements( const syntaxTree::node* n, variable_t, size_t );
		void translateLValue( const syntaxTree::node* n, variable_t value );
		variable_t translateVariable( const syntaxTree::node* n );
		variable_t translateReadIndexing( const syntaxTree::node* n );
		size_t addOperation( iop_t::id_t type, variable_t r = ERROR_VARIABLE, variable_t a = ERROR_VARIABLE, variable_t b = ERROR_VARIABLE, label_t l = ERROR_LABEL, iop_t::constant_t = {.integer=0}, iop_t::constant_t = {.integer=0} );
	};
private:
	std::vector<function> functions;
	size_t label_count;
	scopeTable* scptab;
	void declareFunction( function_t );
public:
	label_t newLabel();
	void defineFunction( function_t, const syntaxTree::node* );
	label_t getFunctionLabel( function_t ) const;
	variable_t newTemporaryVariable( type_t type );
	const function& getFunction( function_t ) const;
	const std::vector<function>& getFunctions() const;
	intermediateCode( scopeTable* );
};

std::ostream& operator<<( std::ostream&, iop_t );
std::ostream& operator<<( std::ostream&, const intermediateCode::function& );
std::ostream& operator<<( std::ostream&, const intermediateCode& );


extern const std::vector<uint8_t> iop_fields;
