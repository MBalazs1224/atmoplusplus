 #include "ir_canonical.hh"


std::shared_ptr<IRStatementExpressionList> IRCanonical::nopNull = std::make_shared<IRStatementExpressionList>(
    std::make_shared<IREvaluateExpression>(
        std::make_shared<IRConst>(0)
    ),
    nullptr
);


 bool IRCanonical::IsNop(std::shared_ptr<IRStatement> stmt)
 {
    auto evalExp = std::dynamic_pointer_cast<IREvaluateExpression>(stmt);

    return evalExp && std::dynamic_pointer_cast<IRConst>(evalExp->exp);
 }

 std::shared_ptr<IRStatement> IRCanonical::Seq(std::shared_ptr<IRStatement> a, std::shared_ptr<IRStatement> b)
 {
    if(IsNop(a))
        return b;

    if (IsNop(b))
        return a;

    return std::make_shared<IRSequence>(
        a,
        b
    );    
 }

 bool IRCanonical::Commute(std::shared_ptr<IRStatement> a, std::shared_ptr<IRExpression> b)
 {
    return IsNop(a) || std::dynamic_pointer_cast<IRName>(b) || std::dynamic_pointer_cast<IRConst>(b);
 }

 std::shared_ptr<IRStatement> IRCanonical::DoStatement(std::shared_ptr<IRSequence> s)
 {
    return Seq(
        DoStatement(s->left),
        DoStatement(s->right)
    );
 }

 std::shared_ptr<IRStatement> IRCanonical::DoStatement(std::shared_ptr<IRMove> move)
 {
    auto destTemp = std::dynamic_pointer_cast<IRTemp>(move->destination);
    auto srcCall = std::dynamic_pointer_cast<IRCall>(move->source);

    if(destTemp && srcCall)
    {
        return ReorderStatement(
            std::make_shared<IRMoveCall>(
                destTemp,
                srcCall
            )
        );
    }

    if(auto destEseq = std::dynamic_pointer_cast<IREseq>(move->destination))
    {
        return DoStatement(
            std::make_shared<IRSequence>(
                destEseq->stmt,
                std::make_shared<IRMove>(
                    destEseq->exp, // TODO: The source and destination may need to be switched
                    move->source
                )
            )
        );
    }

    return ReorderStatement(move);


        
 }

 std::shared_ptr<IRStatement> IRCanonical::DoStatement(std::shared_ptr<IREvaluateExpression> evalExp)
 {
    if(auto call = std::dynamic_pointer_cast<IRCall>(evalExp->exp))
    {
        return ReorderStatement(
            std::make_shared<IRExpCall>(
                call
            )
        );
    }

    return ReorderStatement(evalExp);
 }

 std::shared_ptr<IRStatement> IRCanonical::DoStatement(std::shared_ptr<IRStatement> stmt)
 {
    if (auto seq = std::dynamic_pointer_cast<IRSequence>(stmt))
        return DoStatement(seq);

    if (auto move = std::dynamic_pointer_cast<IRMove>(stmt))
        return DoStatement(move);

    if (auto evalExp = std::dynamic_pointer_cast<IREvaluateExpression>(stmt))
        return DoStatement(evalExp);

    return ReorderStatement(stmt);
 }


std::shared_ptr<IRStatement> IRCanonical::ReorderStatement(std::shared_ptr<IRStatement> stmt)
{
    auto x = Reorder(stmt->Kids());

    return Seq(x->statement, stmt->Build(x->expressions));
}

std::shared_ptr<IREseq> IRCanonical::DoExpression(std::shared_ptr<IREseq> eseq)
{
    auto stmts = DoStatement(eseq->stmt);

    auto b = DoExpression(eseq->exp);

    return std::make_shared<IREseq>(
        b->exp,
        Seq(
            stmts, 
            b->stmt
        )
    );
}

std::shared_ptr<IREseq> IRCanonical::DoExpression(std::shared_ptr<IRExpression> exp)
{
    if(auto eseq = std::dynamic_pointer_cast<IREseq>(exp))
    {
        return DoExpression(eseq);
    }

    return ReorderExpression(exp);
}

std::shared_ptr<IREseq> IRCanonical::ReorderExpression(std::shared_ptr<IRExpression> exp)
{
    auto x = Reorder(exp->Kids());

    return std::make_shared<IREseq>(
        exp->Build(x->expressions),
        x->statement
    );
}

std::shared_ptr<IRStatementExpressionList> IRCanonical::Reorder(std::shared_ptr<IRExpressionList> exps)
{
    if(!exps)
        return nopNull;

    auto head = exps->expression;

    if(std::dynamic_pointer_cast<IRCall>(head))
    {
        auto temp = std::make_shared<Temp>();

        auto eseq = std::make_shared<IREseq>(
            std::make_shared<IRTemp>(temp),
            std::make_shared<IRMove>(
                std::make_shared<IRTemp>(temp),
                head
            )
        );

        return Reorder(
            std::make_shared<IRExpressionList>(
                eseq,
                exps->next
            )
        );
    }


    auto aa = DoExpression(head);

    auto bb = Reorder(exps->next);

    if(Commute(bb->statement, aa->exp))
    {
        return std::make_shared<IRStatementExpressionList>(
            Seq(aa->stmt,bb->statement),
            std::make_shared<IRExpressionList>(
                aa->exp,
                bb->expressions
            )
        );
    }

    auto temp = std::make_shared<Temp>();

    return std::make_shared<IRStatementExpressionList>(
        Seq(
            aa->stmt,
            Seq(
                std::make_shared<IRMove>(
                    std::make_shared<IRTemp>(temp),
                    aa->exp
                ),
                bb->statement
            )
        ),
        std::make_shared<IRExpressionList>(
            std::make_shared<IRTemp>(temp),
            bb->expressions
        )
    );

}

std::shared_ptr<IRStatementList> IRCanonical::Linear(std::shared_ptr<IRSequence> seq, std::shared_ptr<IRStatementList> list)
{
    return Linear(seq->left, Linear(seq->right,list));
}

std::shared_ptr<IRStatementList> IRCanonical::Linear(std::shared_ptr<IRStatement> stmt, std::shared_ptr<IRStatementList> list)
{
    if(auto seq = std::dynamic_pointer_cast<IRSequence>(stmt))
        return Linear(seq,list);

    return std::make_shared<IRStatementList>(
        stmt,
        list
    );
}

std::shared_ptr<IRStatementList> IRCanonical::Linearize(std::shared_ptr<IRStatement> stmt)
{
    return  Linear(DoStatement(stmt),nullptr);
}


