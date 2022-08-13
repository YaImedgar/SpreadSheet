#pragma once

#include "common.h"
#include "formula.h"

#include <string>

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
    std::string expression_;
};

class FormulaImpl final : public Impl
{
public:
    explicit FormulaImpl(std::string expression);
    Value GetValue() const override;
    std::string GetText() const override;
private:
    std::unique_ptr<FormulaInterface> formula_;
};

class Cell final : public CellInterface {
public:
    Cell() = default;
    ~Cell() = default;
    Cell(std::string text);
    void Set(std::string text) override;
    void Clear();

    Value GetValue() const override;
    std::string GetText() const override;
private:
    std::unique_ptr<Impl> impl_;
};