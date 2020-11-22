#include <iostream>
#include "Core/Field.h"
int main(){
    Game::Core::Field field(20, 20, 40);

    while(true) {
        //system("clear");
        for (int i = 0; i < 20; ++i) {
            for (int j = 0; j < 20; ++j) {
                auto optcell = field.get_cell(j, i);
                if (optcell.has_value()) {
                    const Game::Core::Cell cell = *optcell;
                    if(cell.is_mine){
                        std::cout<<"* ";
                    }else{
                        std::cout<<(int)cell.mines_around<<" ";
                    }
                }
                else{
                    std::cout<<"ERROR"<<std::endl;
                }
            }
            std::cout << std::endl;
        }
        std::cout<<std::endl;
        for (int i = 0; i < 20; ++i) {
            for (int j = 0; j < 20; ++j) {
                auto optcell = field.get_cell(j, i);
                if (optcell.has_value()) {
                    const Game::Core::Cell cell = *optcell;
                    if(cell.is_opened){
                        uint8_t mc = cell.mines_around;
                        std::cout<<(int)mc<<" ";
                    }else{
                        std::cout<<"* ";
                    }
                }
                else{
                    std::cout<<"ERROR"<<std::endl;
                }
            }
            std::cout << std::endl;
        }
        uint16_t x, y;
        std::cin >> x;
        std::cin >> y;
        auto opcell = field.get_cell(x,y);
        if (opcell.has_value()){
            Game::Core::Cell c = *opcell;
            std::cout<<c.x<<" "<<c.y<<std::endl;
            Game::Core::Cell::Type t = *field.open_cell(x, y);
            if (t == Game::Core::Cell::Type::O_MINE) {
                break;
            }
        }
    }
    return 0;
}