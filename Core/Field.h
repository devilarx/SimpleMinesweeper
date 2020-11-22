/* This file is part of Simple Minesweeper.
 *
 * Simple Minesweeper is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Simple Minesweeper is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Simple Minesweeper.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef SIMPLEMINESWEEPER_FIELD_H
#define SIMPLEMINESWEEPER_FIELD_H

#include <cstdint>
#include <vector>
#include <optional>

namespace Game::Core{

        // Simple structure describing one Cell at mine field
        struct Cell{
            // Enum for cell type Empty, Flagged, Marked and opened types
            enum Type : uint8_t {C_EMPTY, C_FLAGGED, C_MARKED, O_EMPTY, O_MINE};

            //Simple cell constructor
            Cell():is_opened(false), is_flagged(false), is_marked(false), is_mine(false), x(0), y(0){}
            // This method return Type of cell based on its status flags
            Type get_state() const;

            // Flags of cell state
            bool        is_opened;
            bool        is_flagged;
            bool        is_marked;
            bool        is_mine;
            // X position
            uint16_t    x;
            // Y position
            uint16_t    y;
            // Count of mines around this cell
            uint8_t     mines_around;
        };
        // Type for field row of cells
        typedef std::vector<Cell> CellRow;

        // Class describing game field at the current time
        class Field{
        public:
            // This constructor allows you to set the initial dimensions
            // of the field and the number of mines
            Field(uint16_t width, uint16_t height, uint32_t mines_count);

            // This method provides the ability to open the cell.
            // Returns the new state of the cell if it can be opened
            // and old state if it cannot
            // return std::nullopt if cell no exists
            std::optional<Cell::Type> open_cell(uint16_t x, uint16_t y);

            // This function changes the current label of cell with coordinates x and y
            // empty -> flagged -> marked -> empty
            // Return state of cell or std::nullopt
            std::optional<Cell::Type> change_state(uint16_t x, uint16_t y);

            // This method allows to get constant cell located by x and y coordinates
            // If cell doesn't exist the method will return std::nullopt
            std::optional<const Cell> get_cell(uint16_t x, uint16_t y);

            // This method returns pair with x and y coordinates
            std::pair<uint16_t, uint16_t> get_field_size();

            // This method returns count of mines at field
            uint32_t get_mines_count();

            // This method regenerates game field with new parameters
            void restart(uint16_t width, uint16_t height, uint32_t mines_count);

            // This method regenerates game field with old parameters
            void restart();

            // This method return count of flagged cells of field
            uint32_t get_count_of_flags();
        private:
            void open_cell_with_neighbours(uint16_t x, uint16_t y);

            std::vector<CellRow>    _field;
            uint16_t                _width;
            uint16_t                _height;
            uint32_t               _mines_count;
        };

    }

#endif //SIMPLEMINESWEEPER_FIELD_H