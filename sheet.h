#pragma once

#include "cell.h"

#include <functional>
#include <unordered_map>
#include <deque>
#include <map>

class Sheet : public SheetInterface {
public:
    Sheet() = default;
    ~Sheet() = default;

    void SetCell(Position pos, std::string text) override;

    const CellInterface* GetCell(Position pos) const override;
    CellInterface* GetCell(Position pos) override;

    void ClearCell(Position pos) override;

    Size GetPrintableSize() const override;

    void PrintValues(std::ostream& output) const override;
    void PrintTexts(std::ostream& output) const override;
private:
    enum class TypeOfCell
    {
        NONE,
        TEXT,
        VALUE
    };

    void RemoveOneFrom(std::map<int, int> &map, int row);
    void PrintRowTo(std::ostream &ostream, int row, TypeOfCell type) const;
    void PrintTo(std::ostream &output, TypeOfCell type) const;

    std::unordered_map<Position, Cell, PositionHasher> sheet_;
    std::map<Position, Cell*> sheet_map_;
    // row, number of rows
    std::map<int, int> row_count_;
    // col, number of cols
    std::map<int, int> col_count_;
};