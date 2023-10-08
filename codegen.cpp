#include "keywords.h"
#include "node.h"
#include "codegen.h"
#include "parser.hpp"

using namespace std;

/* Compile the AST into a module */
void CodeGenContext::generateCode(NodeBlock& root) {
	std::cout << "Generating code...\n";
	
	/* Create the top level interpreter function to call as entry */
	vector<Type*> argTypes;
	FunctionType *ftype = FunctionType::get(Type::getVoidTy(MyContext), makeArrayRef(argTypes), false);
	mainFunction = Function::Create(ftype, GlobalValue::InternalLinkage, "main", module);
	BasicBlock *bblock = BasicBlock::Create(MyContext, "entry", mainFunction, 0);
	
	/* Push a new variable/block context */
	pushBlock(bblock);
	root.codeGen(*this); /* emit bytecode for the toplevel block */
	ReturnInst::Create(MyContext, bblock);
	popBlock();
	
	/* Print the bytecode in a human-readable format 
	   to see if our program compiled properly
	 */
	std::cout << "Code is generated.\n";
	// module->dump();

	legacy::PassManager pm;
	// TODO:
	pm.add(createPrintModulePass(outs()));
	pm.run(*module);
}

/* Executes the AST by running the main function */
GenericValue CodeGenContext::runCode() {
	std::cout << "Running code...\n";
	ExecutionEngine *ee = EngineBuilder( unique_ptr<Module>(module) ).create();
	ee->finalizeObject();
	vector<GenericValue> noargs;
	GenericValue v = ee->runFunction(mainFunction, noargs);
	std::cout << "Code was run.\n";
	return v;
}


/* Returns an LLVM type based on the identifier */
static Type *typeOf(const std::string type) {

	if (type.name.compare("integer") == 0) {
		return Type::getInt64Ty(MyContext);
	}
	else if (type.name.compare("real") == 0) {
		return Type::getDoubleTy(MyContext);
	}

	return Type::getVoidTy(MyContext);
}

/* -- Code Generation -- */

Value* NodeInteger::codeGen(CodeGenContext& context){
	std::cout << "Creating integer: " << value << endl;
	return ConstantInt::get(Type::getInt64Ty(MyContext), value, true);
}

Value* NodeDouble::codeGen(CodeGenContext& context){
	std::cout << "Creating double: " << value << endl;
	return ConstantFP::get(Type::getDoubleTy(MyContext), value);
}

Value* NodeIdentifier::codeGen(CodeGenContext& context){
	std::cout << "Creating identifier reference: " << name << endl;


    llvm::Value* valueFound = context.locals();

	if (valueFound == NULL) {
		std::cerr << "Undeclared variable " << name << endl;
		return NULL;
	}

	// return nullptr;  
	return new LoadInst(
            valueFound->getType(),
            valueFound,
            name,
            false,
            context.currentBlock()
    );
}

Value* NodeMethodCall::codeGen(CodeGenContext& context) {
	Function *function = context.module->getFunction(id.name.c_str());
	if (function == NULL) {
		std::cerr << "no such function " << id.name << endl;
	}

	std::vector<Value*> args;
	ExpressionList::const_iterator it;
	for (it = arguments.begin(); it != arguments.end(); it++) {
		args.push_back((**it).codeGen(context));
	}

	CallInst *call = CallInst::Create(function, makeArrayRef(args), "", context.currentBlock());
	std::cout << "Creating method call: " << id.name << endl;
	return call;
}



Value* NodeBinaryOperator::codeGen(CodeGenContext& context)
{

	std::cout << "Creating binary operation " << op << endl;
	Instruction::BinaryOps instr;
	switch (op) {
		case TOKEN_BOP_ADD: 	
            instr = Instruction::Add;
            return BinaryOperator::Create(
                instr,
                lhs.codeGen(context),
                rhs.codeGen(context),
                "",
                context.currentBlock()
            );
		case TOKEN_BOP_SUB: 	
            instr = Instruction::Sub;
            return BinaryOperator::Create(
                instr,
                lhs.codeGen(context),
                rhs.codeGen(context),
                "",
                context.currentBlock()
            );

		case TOKEN_BOP_MUL: 		
            instr = Instruction::Mul;
            return BinaryOperator::Create(
                instr,
                lhs.codeGen(context),
                rhs.codeGen(context),
                "",
                context.currentBlock()
            );

        case TOKEN_BOP_DIV: 		
            instr = Instruction::SDiv;
            return BinaryOperator::Create(
                instr,
                lhs.codeGen(context),
                rhs.codeGen(context),
                "",
                context.currentBlock()
            );
        case default:
            std::cout << "DUDE WTF HOW DID YOU FUCK UP" << std::endl;
            break;
	}
	return NULL;
}


Value* NodeAssignment::codeGen(CodeGenContext& context)
{
	std::cout << "Creating assignment for " << lhs.name << endl;


    llvm::Value* valueFound = context.locals(lhs.name);

	if ( valueFound ) {
		std::cerr << "undeclared variable " << lhs.name << endl;
		return NULL;
	}
	return new StoreInst(
        rhs.codeGen(context),
        valueFound,
        false,
        context.currentBlock()
    );
}


Value* NodeBlock::codeGen(CodeGenContext& context)
{
	StatementList::const_iterator it;
	Value *last = NULL;
	for (it = statements.begin(); it != statements.end(); it++) {
		std::cout << "Generating code for " << typeid(**it).name() << endl;
		last = (**it).codeGen(context);
	}
	std::cout << "Creating block" << endl;
	return last;
}



Value* NodeExpressionStatement::codeGen(CodeGenContext& context) {
	std::cout << "Generating code for " << typeid(expression).name() << endl;
	return expression.codeGen(context);
}

Value* NodeReturnStatement::codeGen(CodeGenContext& context) {
	std::cout << "Generating return code for " << typeid(expression).name() << endl;
	Value *returnValue = expression.codeGen(context);
	context.setCurrentReturnValue(returnValue);
	return returnValue;
}

Value* NodeVariableDeclaration::codeGen(CodeGenContext& context)
{
	std::cout << "Creating variable declaration " << type.name << " " << id.name << endl;
	AllocaInst *alloc = new AllocaInst(typeOf(type),4, id.name.c_str(), context.currentBlock());
	context.locals()[id.name] = alloc;
	if (assignmentExpr != NULL) {
		NAssignment assn(id, *assignmentExpr);
		assn.codeGen(context);
	}
	return alloc;
}

Value* NodeExternDeclaration::codeGen(CodeGenContext& context)
{
    vector<Type*> argTypes;
    VariableList::const_iterator it;
    for (it = arguments.begin(); it != arguments.end(); it++) {
        argTypes.push_back(typeOf((**it).type));
    }
    FunctionType *ftype = FunctionType::get(typeOf(type), makeArrayRef(argTypes), false);
    Function *function = Function::Create(ftype, GlobalValue::ExternalLinkage, id.name.c_str(), context.module);
    return function;
}

Value* NodeFunctionDeclaration::codeGen(CodeGenContext& context)
{
	vector<Type*> argTypes;
	VariableList::const_iterator it;
	for (it = arguments.begin(); it != arguments.end(); it++) {
		argTypes.push_back(typeOf((**it).type));
	}
	FunctionType *ftype = FunctionType::get(typeOf(type), makeArrayRef(argTypes), false);
	Function *function = Function::Create(ftype, GlobalValue::InternalLinkage, id.name.c_str(), context.module);
	BasicBlock *bblock = BasicBlock::Create(MyContext, "entry", function, 0);

	context.pushBlock(bblock);

	Function::arg_iterator argsValues = function->arg_begin();
    Value* argumentValue;

	for (it = arguments.begin(); it != arguments.end(); it++) {
		(**it).codeGen(context);
		
		argumentValue = &*argsValues++;
		argumentValue->setName((*it)->id.name.c_str());
		StoreInst *inst = new StoreInst(argumentValue, context.locals()[(*it)->id.name], false, bblock);
	}
	
	block.codeGen(context);
	ReturnInst::Create(MyContext, context.getCurrentReturnValue(), bblock);

	context.popBlock();
	std::cout << "Creating function: " << id.name << endl;
	return function;
}

