#include "array_subscript.hh"

ArraySubscriptExpression::ArraySubscriptExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc) : TwoOperandExpression(left_in,right_in,loc)
{
}

std::shared_ptr<Type> ArraySubscriptExpression::GetType()
{
    // At this point the Check funtion checked that the left expression is an array so we can just cast it

    auto array = std::dynamic_pointer_cast<Array>(left->GetType());

    return array->inner_type;
}

bool ArraySubscriptExpression::Check()
{
    if(!left->Check())
    {
        return false;
    }

    if(!right->Check())
    {
        return false;
    }

    auto arrayType = left->GetType();
    if (!std::dynamic_pointer_cast<Array>(arrayType))
    {
        Error::ShowError(Helper::FormatString("Only array types can appear on the left side of array subscript expression! (received '%s')",arrayType->ToString().c_str()),left->location);
        return false;
    }

    auto intType = right->GetType();
    if (!std::dynamic_pointer_cast<TypeInteger>(intType))
    {
        Error::ShowError(Helper::FormatString("Only integer types can appear on the right side of array subscript expression! (received '%s')",intType->ToString().c_str()),right->location);
        return false;
    }

    return true;
}




std::shared_ptr<IRExpression> ArraySubscriptExpression::TranslateIndexingIntoIR( std::shared_ptr<Temp> tempForIndex
)
{
    // (array_address) + (data_size * index) + 4 is the algorithm used to index into an array
    // The first element is the size of the array, so we need to skip that, that's why + 4

    // The boundary check already put the index value into one of the registers

    auto indexExpression = std::make_shared<IRTemp>(tempForIndex);


    auto locationOfArray = left->TranslateExpressionToIr()->ToValueExpression();

    // The semantic analyzer already checked that the left expression is of array type

    auto leftAsArray = std::dynamic_pointer_cast<Array>(left->GetType());


    int sizeOfElementsInTheArray = leftAsArray->inner_type->GetSize();

    // (data_size * index)
    auto multipliedIndexValue = std::make_shared<IRBinaryOperator>(
        BinaryOperator::MULTIPLY,
        std::make_shared<IRConst>(sizeOfElementsInTheArray),
        indexExpression
    );

    auto adjustedIndex = std::make_shared<IRBinaryOperator>(
        BinaryOperator::PLUS,
        multipliedIndexValue,
        std::make_shared<IRConst>(4)
    );


    auto offsetIntoArray = std::make_shared<IRBinaryOperator>(
        BinaryOperator::PLUS,
        locationOfArray,
        adjustedIndex
    );

    auto memExpression = std::make_shared<IRMem>(
        offsetIntoArray
    );

    return memExpression;
}

std::shared_ptr<IRStatement> ArraySubscriptExpression::TranslateBoundaryCheckIntoIR(std::shared_ptr<Label> labelForIndexing, std::shared_ptr<Temp> tempForIndex)
{
    auto labelForIndexLessThanSize = std::make_shared<Label>();

    auto labelForAutoExit = std::make_shared<Label>();

    // ----------- INITIALIZE EXIT -----------

    // 60 in RAX represents the sys_exit system call on linux

    auto move60IntoRax = std::make_shared<IRMove>(
        std::make_shared<IRTemp>(ReservedIrRegisters::RAX),
        std::make_shared<IRConst>(60)
    );

    // The value inside RDI will be the exit code, now I chose 255 (the max value) to indicate something is wrong

    auto move255IntoRDI = std::make_shared<IRMove>(
        std::make_shared<IRTemp>(ReservedIrRegisters::RDI),
        std::make_shared<IRConst>(255)
    );

    auto initializeRegs = std::make_shared<IRSequence>(
        move60IntoRax,
        move255IntoRDI
    );

    auto initRegsThanSysCall = std::make_shared<IRSequence>(
        initializeRegs,
        std::make_shared<IRSysCall>()
    );

    auto printExitLabelThenExit = std::make_shared<IRSequence>(
        std::make_shared<IRLabel>(labelForAutoExit),
        initRegsThanSysCall
    );

    // ----------- GENERATE BOUNDARY CHECK -----------
    
    auto indexExpression = right->TranslateExpressionToIr()->ToValueExpression();

    // Move the index into a register (to prevent it being executed multiple times during boundary check and actually indexing, this would be a problem for function calls, because in that case it would be called twice)

    auto moveIndexIntoTemp = std::make_shared<IRMove>(
        std::make_shared<IRTemp>(tempForIndex),
        indexExpression
    );


    
    auto indexGreaterThanZero = std::make_shared<IRCJump>(
        RelationalOperator::GreaterThanOrEqual,
        std::make_shared<IRTemp>(tempForIndex),
        std::make_shared<IRConst>(0),
        labelForIndexLessThanSize, // If true
        labelForAutoExit // If false
    );

    // The size of the array is the first element inside the array

    auto numberOfElements = left->TranslateExpressionToIr()->ToValueExpression();


    auto indexLessThanSizeOfArray = std::make_shared<IRCJump>(
        RelationalOperator::LessThan,
        std::make_shared<IRTemp>(tempForIndex),
        numberOfElements,
        labelForIndexing, // If true
        labelForAutoExit // If false

    );

    // ----------- GENERATE CORRECT SEQUENCES -----------

    auto lessThanLabelAndExpression = std::make_shared<IRSequence>(
        std::make_shared<IRLabel>(labelForIndexLessThanSize),
        indexLessThanSizeOfArray
    );

    auto moveIndexIntoTempAndIndexGreaterThanZero = std::make_shared<IRSequence>(
        moveIndexIntoTemp,
        indexGreaterThanZero
    );

    auto indexMoveAndBoudnaryChecks = std::make_shared<IRSequence>(
        moveIndexIntoTempAndIndexGreaterThanZero,
        lessThanLabelAndExpression
    );

    auto boundaryCheckAndExit = std::make_shared<IRSequence>(
        indexMoveAndBoudnaryChecks,
        printExitLabelThenExit
    );

    return boundaryCheckAndExit;
    

}

std::shared_ptr<TranslateExpression> ArraySubscriptExpression::TranslateExpressionToIr()
{
    // For better safety, the compiler should generate code that checks if the given index is inside the bounds of the array (index > 0 && index < SIZE_OF_ARRAY)

    // Label where the actual indexing happens, this is needed so the boundary check knows where to jump if the boundary check returns that the index is correct

    auto indexLabel = std::make_shared<Label>();

    // To prevetnt multiple evaluations of the index, first move the index value into a reg and then do the needed calcualtions on that reg (without this functions would be called at boundary check and actual indexing as well)

    auto tempForindex = std::make_shared<Temp>();

    auto boundaryCheckExpression = TranslateBoundaryCheckIntoIR(indexLabel,tempForindex);


    auto indexIntoArrayExpression = TranslateIndexingIntoIR(tempForindex);

    auto indexLabelThenExpression = std::make_shared<IREseq>(
        indexIntoArrayExpression,
        std::make_shared<IRLabel>(indexLabel)
    );

    // The boundary check should be executed first, then the indexing should be taken as the real value of this expression

    auto finalEseq = std::make_shared<IREseq>(
        indexLabelThenExpression,
        boundaryCheckExpression
    );

    return std::make_shared<TranslateValueExpression>(finalEseq);
}

std::shared_ptr<IRStatement> ArraySubscriptExpression::TranslateToIR()
{
    //TODO: "Implement array subscript to ir"
    return nullptr;
}