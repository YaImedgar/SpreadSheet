#include "cell.h"

#include <cassert>
#include <iostream>
#include <string>
#include <optional>

void Cell::Set(std::string text)
{
    if (text.empty()) // empty
    {
        impl_ = std::make_unique<EmptyImpl>();
        return;
    }

    if (text.size() > 1 && text[0] == FORMULA_SIGN)
    {
        impl_ = std::make_unique<FormulaImpl>(*parent_sheet_.value(), text);
        return ;
    }

    impl_ = std::make_unique<TextImpl>(text);
}

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
    Set(std::move(text));
}

std::vector<Position> Cell::GetReferencedCells() const
{
    // TODO
    assert(false);
    return {};
}

Impl::Value EmptyImpl::GetValue() const
{
    return std::string{};
}

std::string EmptyImpl::GetText() const
{
    return std::string();
}

FormulaImpl::FormulaImpl(const SheetInterface& sheet, std::string expression)
    : parent_sheet_(sheet)
{
    formula_ = ParseFormula(expression.substr(1));
}

Impl::Value FormulaImpl::GetValue() const
{
    auto raw_value = formula_->Evaluate(parent_sheet_);
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
    : text_(std::move(expression))
{}

Impl::Value TextImpl::GetValue() const
{
    if (text_[0] == ESCAPE_SIGN)
    {
        return text_.substr(1);
    }

    return text_;
}

std::string TextImpl::GetText() const
{
    return text_;
}
