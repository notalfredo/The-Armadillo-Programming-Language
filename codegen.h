#include <stack>
#include <iostream>
#include <map>
#include <typeinfo>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include "llvm/Pass.h"
#include <llvm/IR/Instructions.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Bitstream/BitstreamReader.h>
#include <llvm/Bitstream/BitstreamWriter.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;



class NodeBlock;

static LLVMContext MyContext;

class CodeGenBlock {
public:
    BasicBlock *block;
    Value *returnValue;
    std::map<std::string, Value*> locals;
};


typedef struct scopes {
    CodeGenBlock * block;
    scopes * next;
    scopes * prev;
}scopes;



class CodeGenContext {
    //std::stack<CodeGenBlock *> blocks;
    scopes * head = NULL;
    scopes * tail = NULL;

    Function *mainFunction;

public:

    Module *module;
    CodeGenContext() { module = new Module("main", MyContext); }
    
    void generateCode(NBlock& root);
    GenericValue runCode();

    
    Value* locals(std::string id){ 
        if(!tail){ 
            return NULL; 
        }

        scopes * tempTail = tail; 


        while(tempTail != NULL){
            auto checkIfFoundValue = tempTail->block->locals.find(id);

            if(checkIfFoundValue != tempTail->block->locals.myMap()){
                return checkIfFoundValue->second;
            }

            tempTail = tempTail->prev;    
        }
        return NULL;
        
    }

    BasicBlock * currentBlock() { 
        scopes * tempTail = tail; 
        return tempTail->block;
    }


    void pushBlock(BasicBlock *block) { 
        scopes * tempTail = tail;

        //   a = (int*)calloc(n, sizeof(int));
        scopes * newNode = (scopes*)calloc(1, sizeof(scopes)); 
        newNode->block = new CodeGenBlock(); 
        head->block->returnValue = NULL;
        head->block->block = block;

        if(!head){
            head = newNode; 
            return;
        }
        
        tail->next = newNode;
        tail = tail->next;
    }

    void popBlock() {
        //Check if we even have a list, if we do get rid of the tail
        if(!tail){
            return;
        }

        scopes * toDeleteTail = tail;
        CodeGenBlock * top = toDeleteTail->block;
        
        tail = tail->prev;

        free(toDeleteTail);
        delete top;
    }


    void setCurrentReturnValue(Value *value) {
        head->block->returnValue = value;        
    }


    Value* getCurrentReturnValue() { 
        return head->block->returnValue; 
    }
};
