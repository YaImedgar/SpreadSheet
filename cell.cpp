#include "cell.h"

#include <cassert>
#include <iostream>
#include <string>
#include <optional>

/*void Cell::Set(std::string text)
{
    if (text.empty()) // empty
    {
        impl_ = std::make_unique<EmptyImpl>();
        return;
    }

    if (text.size() > 1 && text[0] == FORMULA_SIGN)
    {
        impl_ = std::make_unique<FormulaImpl>(text);
        return ;
    }

    impl_ = std::make_unique<TextImpl>(text);
}*/

void Cell::Clear()
{
    impl_.release();
}

Cell::Value Cell::GetValue() const
{
    return impl_->GetValue();
}

std::string Cell::GetText() const
{
    return impl_->GetText();
}

Cell::Cell(std::string text)
{
    //Set(std::move(text));
}

Impl::Value EmptyImpl::GetValue() const
{
    return std::string{};
}

std::string EmptyImpl::GetText() const
{
    return std::string();
}

FormulaImpl::FormulaImpl(std::string expression)
{
    formula_ = ParseFormula(expression.substr(1));
}

Impl::Value FormulaImpl::GetValue() const
{
    auto raw_value = formula_->Evaluate();
    if (std::holds_alternative<double>(raw_value))
    {
        return std::get<double>(raw_value);
    }
    return std::get<FormulaError>(raw_value);
}

std::string FormulaImpl::GetText() const
{
    return FORMULA_SIGN + formula_->GetExpression();
}

TextImpl::TextImpl(std::string expression)
    : expression_(std::move(expression))
{}

Impl::Value TextImpl::GetValue() const
{
    if (expression_[0] == ESCAPE_SIGN)
    {
        return expression_.substr(1);
    }

    return expression_;
}

std::string TextImpl::GetText() const
{
    return expression_;
}
