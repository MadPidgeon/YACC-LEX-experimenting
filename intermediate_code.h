#pragma once
#include <vector>
#include <iostream>
#include <utility>
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
#define IOFS			(1 << 6) // sets r without reading (take care with ARR_STORE )
#define IOFJ			(1 << 7) // is jumping operation
#define IOFE			(1 << 8) // operation has side effect besides assigning to r (only in combination with IOFR)

struct iop_t {
	enum id_t {
		// Arguments to operations are of the form [rabl] where r is the store variable, a and b are the operands, and l is a label.
		// A (W) symbol implies that this variable is written to, which is implicit for r.
		IOP_NOP,		// [----]
		IOP_GARBAGE,	// [r---]
		IOP_INT_MOV,	// [ra--]
		IOP_STR_MOV,	// [ra--]
		IOP_FLT_MOV,	// [ra--]
		IOP_LABEL,		// [---l]
		// jumping instructions (4-14)
		IOP_JUMP, 		// [---l]
		IOP_JT, IOP_JF, // [-a-l] jumps on thruth and falsehood respectively
		IOP_JE, IOP_JN, IOP_JL, IOP_JG, IOP_JLE, IOP_JGE,
						// [-abl]
		IOP_FJ,			// [r--l] (counter,head)
		// function instructions (15-18)
		IOP_INT_ADD_PARAM,	// [-a--]
		IOP_FLT_ADD_PARAM,	// [-a--]
		IOP_INT_POP_RETURN, // [r---]
		IOP_FLT_POP_RETURN, // [r---]
		IOP_RESERVE_RETURN, // [-a--]
		IOP_FUNCTION, 	    // [-a-l]  (*(a+l))( param_1, param_2, ... )
		IOP_LABEL_TO_PTR,   // [r--l]
		IOP_RETURN,		    // [-a--]
		// arithmetic instructions (19-31)
		IOP_INT_ANDEQ, IOP_INT_OREQ, IOP_INT_XOREQ, IOP_INT_ANDNEQ,
		IOP_INT_ADDEQ, IOP_INT_SUBEQ, IOP_INT_MULEQ, 
						// [ra--]
		IOP_INT_MODDIV,	// [rab-] (dividend,divisor,remaider (W))
		IOP_FLT_ADDEQ, IOP_FLT_SUBEQ, IOP_FLT_MULEQ, IOP_FLT_DIVEQ,
						// [ra--]
		IOP_FLT_ADD, IOP_FLT_SUB, IOP_FLT_MUL, IOP_FLT_DIV,
						// [rab-]
		IOP_STR_CONEQ,	// [ra--]
		// comparison instructions (32-37)
		IOP_INT_EQ, IOP_INT_NEQ, IOP_INT_L, IOP_INT_G, IOP_INT_LE, IOP_INT_GE,
						// [rab-]
		// container instructions (38-40)
		IOP_INT_ARR_LOAD, 
						// [rab-] (variable, base, offset) r = *( a + offset )
		IOP_INT_ARR_STORE,
						// [rab-] ((R) base, offset, variable)
		IOP_FLT_ARR_LOAD,
		IOP_FLT_ARR_STORE,
		IOP_INT_TUP_LOAD,
						// [rab-] (variable, base, offset)  r = *( (&a) + offset )
		IOP_INT_TUP_STORE,
						// [rab-] ((R) base, offset, variable)
		IOP_FLT_TUP_LOAD,
		IOP_FLT_TUP_STORE,
		IOP_LIST_ALLOCATE,	
						// [ra--] (memory adress,size in bytes)
		IOP_LIST_SIZE,  // [ra--]

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
	static const std::vector<uint16_t> iop_fields;
	std::vector<variable_t> getReadVariables() const;
	std::vector<variable_t> getWrittenVariables() const;
	std::vector<variable_t> getReadOnlyVariables() const;
	std::vector<variable_t> getWriteOnlyVariables() const;
	void setParameterVariable( int i, variable_t v );
	void setParameterInteger( int i, int64_t x );
	void setParameterFloating( int i, double x );
	variable_t getParameterVariable( int i ) const;
	int64_t getParameterInteger( int i ) const;
	double getParameterFloating( int i ) const;
	bool parameterIsVariable( int i ) const;
	bool parameterIsWritten( int i ) const;
	bool parameterIsRead( int i ) const;
	bool isFloating() const;
	bool usesResultParameter() const;
	bool usesReadParameterA() const;
	bool usesReadParameterB() const;
	bool hasSideEffects() const;
	bool reduceStrength();
	bool reduceStrengthEq( int64_t i );
	bool isJump() const;
	bool usesLabel() const;
	void negateCompare();
	void rotateCompare();
	void legalizeCompare();
	std::string debugName() const;
	bool operator==( const iop_t& ) const;
};

class intermediateCode {
	friend std::ostream& operator<<( std::ostream&, const intermediateCode& );
	typedef std::vector<std::pair<label_t,label_t>> loop_stack_t;
	typedef syntaxTree::node::id_t SN;
	typedef iop_t::id_t IOP;
public:
	struct function {
		symbol_t symbol;
		function_t id;
		label_t label;
		intermediateCode* parent;
		std::vector<iop_t> operations;
		type_t getReturnType() const;
		type_t getArgumentType( size_t i ) const;
		const std::vector<variable_t>& getArguments() const;
		void translateNode( const syntaxTree::node*, loop_stack_t& );
		void translateAssign( const syntaxTree::node* );
		void translateArguments( const syntaxTree::node* );
		variable_t translateArithmetic( const syntaxTree::node* );
		void translateBranching( const syntaxTree::node*, loop_stack_t& );
		void translateBlock( const syntaxTree::node*, loop_stack_t& );
		variable_t translateComparisonChain( const syntaxTree::node* );
		std::pair<variable_t,variable_t> translateComparison( const syntaxTree::node* );
		variable_t translateConstant( const syntaxTree::node* n );
		variable_t translateContainer( const syntaxTree::node* n );
		variable_t translateFunctionDefinition( const syntaxTree::node* n );
		void translateGarbage( const syntaxTree::node* n );
		void translateControlFlow( const syntaxTree::node* n, loop_stack_t& );
		variable_t translateExpression( const syntaxTree::node* n );
		variable_t translateFunctionCall( const syntaxTree::node* n );
		variable_t translateFunctionOperation( const syntaxTree::node* n );
		void translateListElements( const syntaxTree::node* n, variable_t, variable_t );
		size_t translateTupleAssignment( variable_t target, variable_t source, size_t offset, type_t t );
		void translateTupleLValue( const syntaxTree::node* n, variable_t value, variable_t offset );
		void translateLValue( const syntaxTree::node* n, variable_t value );
		variable_t translateVariable( const syntaxTree::node* n );
		variable_t translateReadIndexing( const syntaxTree::node* n );
		void translateSequentialBlock( const syntaxTree::node* n, loop_stack_t& );
		void translateTupleElements( const syntaxTree::node* n, variable_t, size_t );
		void translatePopReturn( variable_t );
		void translateAssignToListElementWeak( variable_t, variable_t, variable_t );
		void translateAssignFromListElementWeak( variable_t, variable_t, variable_t );
		void translateListToListAssignmentWeak( variable_t, variable_t, variable_t, variable_t );
		void translateAssignToTupleElementWeak( variable_t, variable_t, variable_t );
		void translateAssignFromTupleElementWeak( variable_t, variable_t, variable_t );
		void translateTupleToTupleAssignmentWeak( variable_t, variable_t, variable_t, variable_t );
		void translateGeneralAssignment( variable_t, variable_t );

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