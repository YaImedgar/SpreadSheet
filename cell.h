#pragma once

#include "common.h"
#include "formula.h"
#include <string>
#include <optional>

class Sheet;

class Impl
{
public:
    using Value = CellInterface::Value;
    virtual ~Impl() = default;
    virtual Value GetValue() const = 0;
    virtual std::string GetText() const = 0;
};

class EmptyImpl final : public Impl
{
public:
    Value GetValue() const override;
    std::string GetText() const override;
};

class TextImpl final : public Impl
{
public:
    explicit TextImpl(std::string expression);
    Value GetValue() const override;
    std::string GetText() const override;
private:
    std::string text_;
};

class FormulaImpl final : public Impl
{
public:
    explicit FormulaImpl(const SheetInterface& sheet, std::string expression);
    Value GetValue() const override;
    std::string GetText() const override;
private:
    const SheetInterface& parent_sheet_;
    std::unique_ptr<FormulaInterface> formula_;
};

class Cell final : public CellInterface {
public:
    friend Sheet;

    explicit Cell(std::string text);
    Cell() = default;
    ~Cell() = default;
    void Clear();

    Value GetValue() const override;
    std::string GetText() const override;
    std::vector<Position> GetReferencedCells() const override;
private:
    void Set(std::string text);

    std::optional<SheetInterface*> parent_sheet_;
    std::unique_ptr<Impl> impl_;
};