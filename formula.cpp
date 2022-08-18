#include "formula.h"

#include "FormulaAST.h"

#include <algorithm>
#include <sstream>

using namespace std::literals;

std::ostream &operator<<(std::ostream &output, FormulaError fe)
{
    return output << "#DIV/0!";
}

namespace
{
    class Formula : public FormulaInterface
    {
    public:
        explicit Formula(std::string expression)
            : ast_(ParseFormulaAST(expression))
        {}

        Value Evaluate(const SheetInterface &sheet) const override
        {
            Value val;
            try
            {
                val = ast_.Execute(sheet);
            }
            catch (const FormulaError &fe)
            {
                val = fe;
            }

            return val;
        }

        std::string GetExpression() const override
        {
            std::stringstream ss;

            ss.clear();
            ast_.PrintFormula(ss);

            return ss.str();
        }

        std::vector<Position> GetReferencedCells() const override
        {
            return {ast_.GetCells().begin(), ast_.GetCells().end()};
        }
    private:
        FormulaAST ast_;
    };
}  // namespace

std::unique_ptr<FormulaInterface> ParseFormula(std::string expression)
{
    return std::make_unique<Formula>(std::move(expression));
}
