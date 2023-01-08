#include "chessgameHandler.hpp"

// by default white
bool orientation = true;

int main()
{
    struct tm birthdate;
    birthdate.tm_year = 1998;
    birthdate.tm_mon = 9; // 0 - 11
    birthdate.tm_mday = 14;

    User u = User("emigcm98", "password", birthdate);

    User u2 = User("eve", "password", birthdate);

    ChessGameHandler* cgh = new ChessGameHandler(&u, &u2);
    //ChessGameHandler cgh = ChessGameHandler(&u);

    int ret = -1;
    while (ret != 0){
        if (ret == 1){
            delete cgh;
            cgh = new ChessGameHandler(&u, &u2);
        }
        ret = cgh->start();
    }

    return 0;
}