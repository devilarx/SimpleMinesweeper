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

#include <optional>
#include <iostream>
#include <vector>
#include <list>
#include <random>
#include "Field.h"

namespace Game::Core{

        Cell::Type Cell::get_state() const {
            if (is_opened)
                if (is_mine)
                    return Type::O_MINE;
                else
                    return Type::O_EMPTY;
            else
                if (is_flagged)
                    return Type::C_FLAGGED;
                else if (is_marked)
                    return Type::C_MARKED;
                else
                    return Type::C_EMPTY;
        }

        Field::Field(uint16_t width, uint16_t height, uint32_t mines_count)
        {
            this->restart(width, height, mines_count);
        }


        std::optional<Cell::Type> Field::open_cell(uint16_t x, uint16_t y) {
            if(x>=this->_width || y>=this->_height){
                return std::nullopt;
            }
            Cell& c = this->_field[y][x];
            if(!c.is_opened && !c.is_marked && !c.is_flagged){
                this->open_cell_with_neighbours(x,y);
            }
            return c.get_state();
        }

        std::optional<Cell::Type> Field::change_state(uint16_t x, uint16_t y){
            if(x>=this->_width || y>=this->_height){
                return std::nullopt;
            }
            Cell& c = this->_field[y][x];
            if(!c.is_opened){
                if (c.is_flagged) {
                    c.is_marked = true;
                    c.is_flagged = false;
                }else if(c.is_marked){
                    c.is_marked = false;
                }else{
                    c.is_flagged = true;
                }
            }
            return c.get_state();
        }

        std::optional<const Cell> Field::get_cell(uint16_t x, uint16_t y) {
            if (x>=this->_width || y>=this->_height){
                return std::nullopt;
            }
            return this->_field[y][x];
        }

        std::pair<uint16_t, uint16_t> Field::get_field_size() {
            return {this->_width, this->_height};
        }

        uint32_t Field::get_mines_count() {
            return this->_mines_count;
        }

        void Field::restart(uint16_t width, uint16_t height, uint32_t mines_count){
            this->_width = width > 10 ? width : 10;
            this->_height = height > 10 ? height : 10;
            this->_mines_count = mines_count > this->_width*this->_height - 2 ? this->_width*this->_height - 2 : mines_count;
            this->_mines_count = this->_mines_count >2 ? this->_mines_count : 2;
            this->_field = std::vector<CellRow>(this->_height, std::vector<Cell>(this->_width));
            this->restart();
        }

        void Field::restart() {
            uint32_t count = this->_mines_count;
            std::vector<std::pair<uint16_t, uint16_t>> cells;
            std::random_device rd;
            std::mt19937 generator(rd());
            for (uint16_t x = 0; x< this->_width; ++x)
                for (uint16_t y = 0; y<this->_height; ++y) {
                    this->_field[y][x].x = x;
                    this->_field[y][x].y = y;
                    cells.push_back({x, y});
                }
            while(count){
                std::shuffle(cells.begin(), cells.end(), generator);
                auto [x,y] = *(cells.end()-1);
                cells.pop_back();
                count-=1;
                this->_field[y][x].is_mine = true;
            }
            for (uint16_t x = 0; x< this->_width; ++x){
                for (uint16_t y = 0;  y<this->_height; ++y){
                    uint8_t mines_around = 0;
                    if (x>0 && y>0) mines_around += this->_field[y-1][x-1].is_mine;
                    if (x>0) mines_around += this->_field[y][x-1].is_mine;
                    if (x>0 && y<this->_height-1) mines_around+=this->_field[y+1][x-1].is_mine;
                    if (y>0) mines_around+=this->_field[y-1][x].is_mine;
                    if (y<this->_height-1) mines_around+=this->_field[y+1][x].is_mine;
                    if (x<this->_width-1 && y>0) mines_around += this->_field[y-1][x+1].is_mine;
                    if (x<this->_width-1) mines_around += this->_field[y][x+1].is_mine;
                    if (x<this->_width-1 && y<this->_height-1) mines_around+=this->_field[y+1][x+1].is_mine;
                    this->_field[y][x].mines_around = mines_around;
                }
            }
        }

        uint32_t Field::get_count_of_flags() {
            uint32_t count = 0;
            for (const auto& row : this->_field)
                for (const auto& cell : row)
                    if (!cell.is_opened && cell.is_flagged)
                        count += 1;
            return count;
        }

        void Field::open_cell_with_neighbours(uint16_t x, uint16_t y){
            Cell& cell = this->_field[y][x];
            if (cell.is_opened){
                return;
            }
            cell.is_opened = true;
            cell.is_flagged = false;
            cell.is_marked = false;
            if (cell.mines_around==0){
                if (x>0 && y>0) this->open_cell_with_neighbours(x-1, y-1);
                if (x>0)this->open_cell_with_neighbours(x-1, y);
                if (x>0 && y<this->_height-1) this->open_cell_with_neighbours(x-1, y+1);
                if (y>0) this->open_cell_with_neighbours(x, y-1);
                if (y<this->_height-1) this->open_cell_with_neighbours(x, y+1);
                if (x<this->_width-1 && y>0) this->open_cell_with_neighbours(x+1, y-1);
                if (x<this->_width-1) this->open_cell_with_neighbours(x+1, y);
                if (x<this->_width-1 && y<this->_height-1) this->open_cell_with_neighbours(x+1, y+1);
            }
        }
    }