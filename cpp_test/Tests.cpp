#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include <sstream>

#include "../Game.h"

// magic from stack overflow that makes it easy to check whats printed in std::cout
class AutoRestoreRdbuf {
    std::ostream& out;
    std::streambuf* old;

public:
    ~AutoRestoreRdbuf()
    {
        out.rdbuf(old);
    }
    AutoRestoreRdbuf(const AutoRestoreRdbuf&) = delete;
    AutoRestoreRdbuf(AutoRestoreRdbuf&&) = delete;

    AutoRestoreRdbuf(std::ostream& out)
        : out { out }
        , old { out.rdbuf() }
    {
    }
};
std::string stringWrittentToStream(std::function<void()> f, std::ostream& out = std::cout)
{
    AutoRestoreRdbuf restore { std::cout };
    std::ostringstream oss;
    std::cout.rdbuf(oss.rdbuf());
    f();
    return oss.str();
}
// end of magic from stack overflow


void setRobotPos(int x, int y, int facing, Board *board, Player *player) 
{
    player->robot->x = x;
    player->robot->y = y;
    player->robot->facing = facing;
    player->robot->dead = false;
    player->robot->damage = 0;
    board->updatePlayerTile(player);
}


TEST_CASE( "test tests", "[test]" ) 
{
    REQUIRE( 1 == 1 );
    REQUIRE( 2 == 2 );
    REQUIRE( 6 == 6 );
    REQUIRE( 3628800 == 3628800 );
}

TEST_CASE( "Check board elements", "[board]|[player]|[robot]|[tile]") 
{
    Controls controls;
    controls.init();

    Board board = Board(&controls);
    board.setBoard(-1, 0, false);

    Tile defaultTile;

    Player player = Player(board.getFreeRobot(&player), &controls, &defaultTile);

    REQUIRE(player.robot->currentflag == 0);
    setRobotPos(1,1,0,&board,&player);
    REQUIRE(player.robot->x == 1);
    REQUIRE(player.robot->y == 1);
    player.moveInDirection(1);
    REQUIRE(player.robot->x == 1);
    REQUIRE(player.robot->y == 2);
    auto s = stringWrittentToStream([&player](){player.tile->printProperties();});
    REQUIRE(s == "1\n2\nconveyor0000\nexpress0000\nentry0000\nwall0000\npush00000000000000000000\nother000000010\n");

    SECTION( "walls" )
    {
        // from the wall-tile away
        for (int i = 0; i < 4; i++) 
        {
            setRobotPos(2,2 + 2*i,i,&board,&player);
            PlayerAction(new MoveForwardAction(1), &player, &controls, &board).execute();
            REQUIRE(player.robot->x == 2);
            REQUIRE(player.robot->y == 2 + 2*i);
        }

        // from the adjacent tile into the wall-tile
        for (int i = 0; i < 4; i++) 
        {
            setRobotPos(2+utility::dx(i),2 + 2*i+utility::dy(i),(i+2)%4,&board,&player);
            PlayerAction(new MoveForwardAction(1), &player, &controls, &board).execute();
            REQUIRE(player.robot->x == 2 + utility::dx(i));
            REQUIRE(player.robot->y == 2 + 2*i + utility::dy(i));
        }

    }
    SECTION( "holes" )
    {
        // hole at 10, 8

        // drive into the hole
        for (int i = 0; i < 4; i++) 
        {
            setRobotPos(10+utility::dx(i), 8+utility::dy(i), (i+2)%4, &board, &player);
            PlayerAction(new MoveForwardAction(1), &player, &controls, &board).execute();
            REQUIRE(player.robot->dead);
        }

        // try to "jump" over a hole with a 2 forward move
        for (int i = 0; i < 4; i++) 
        {
            setRobotPos(10+utility::dx(i), 8+utility::dy(i), (i+2)%4, &board, &player);
            PlayerAction(new MoveForwardAction(2), &player, &controls, &board).execute();
            REQUIRE(player.robot->dead);
        }

    }
    SECTION( "lasers" )
    {
        // first laser at 3, 10
        setRobotPos(4, 10, 0, &board, &player);
        board.zapLasers();
        REQUIRE(player.robot->damage == 0);

        setRobotPos(3, 10, 0, &board, &player);
        board.zapLasers();
        REQUIRE(player.robot->damage == 1);

        setRobotPos(2, 10, 0, &board, &player);
        board.zapLasers();
        REQUIRE(player.robot->damage == 0);

        setRobotPos(1, 10, 0, &board, &player);
        board.zapLasers();
        REQUIRE(player.robot->damage == 0);

        // second laser at 11, 9
        setRobotPos(11, 8, 0, &board, &player);
        board.zapLasers();
        REQUIRE(player.robot->damage == 0);

        setRobotPos(11, 9, 0, &board, &player);
        board.zapLasers();
        REQUIRE(player.robot->damage == 3);

        setRobotPos(11, 10, 0, &board, &player);
        board.zapLasers();
        REQUIRE(player.robot->damage == 3);

        setRobotPos(11, 11, 0, &board, &player);
        board.zapLasers();
        REQUIRE(player.robot->damage == 3);

        setRobotPos(11, 12, 0, &board, &player);
        board.zapLasers();
        REQUIRE(player.robot->damage == 0);

        // third laser at 11, 4
        setRobotPos(11, 5, 0, &board, &player);
        board.zapLasers();
        REQUIRE(player.robot->damage == 0);

        setRobotPos(11, 4, 0, &board, &player);
        board.zapLasers();
        REQUIRE(player.robot->damage == 1);

        setRobotPos(11, 3, 0, &board, &player);
        board.zapLasers();
        REQUIRE(player.robot->damage == 1);

        setRobotPos(11, 2, 0, &board, &player);
        board.zapLasers();
        REQUIRE(player.robot->damage == 1);

        setRobotPos(11, 1, 0, &board, &player);
        board.zapLasers();
        REQUIRE(player.robot->damage == 0);


        // fourth laser at 10, 6
        setRobotPos(9, 6, 0, &board, &player);
        board.zapLasers();
        REQUIRE(player.robot->damage == 0);

        setRobotPos(10, 6, 0, &board, &player);
        board.zapLasers();
        REQUIRE(player.robot->damage == 1);

        setRobotPos(11, 6, 0, &board, &player);
        board.zapLasers();
        REQUIRE(player.robot->damage == 1);

        setRobotPos(12, 6, 0, &board, &player);
        board.zapLasers();
        REQUIRE(player.robot->damage == 0);

    }
    SECTION( "gears" )
    {
        // clockwise at 3, 3
        setRobotPos(3, 3, 0, &board, &player);
        for (int i = 0; i < 4; i++)
        {
            PlayerAction(player.getTileAction(8, 0), &player, &controls, &board).execute();
            REQUIRE(player.robot->facing == (i+1)%4);
        }

        // counter-clockwise at 3, 6
        setRobotPos(3, 6, 0, &board, &player);
        for (int i = 3; i >= 0; i--)
        {
            PlayerAction(player.getTileAction(8, 0), &player, &controls, &board).execute();
            Action* a = player.getTileAction(8,0);
            std::cout << "Player action rotation: " << a->rotation << "\n";
            delete a;
            player.tile->printProperties();
            REQUIRE(player.robot->facing == i);
        }

    }
    SECTION( "wrenches" )
    {
        // wrench
        setRobotPos(12, 1, 0, &board, &player);
        player.robot->damage = 4;
        for (int i = 0; i < 12; i++) 
        {
            PlayerAction(player.getTileAction(i, 0), &player, &controls, &board).execute();
        }
        REQUIRE(player.robot->damage == 4);
        PlayerAction(player.getTileAction(12, 0), &player, &controls, &board).execute();
        REQUIRE(player.robot->damage == 3);
    }
    SECTION( "flags" )
    {
        player.robot->currentflag = 0;

        setRobotPos(1, 5, 0, &board, &player);
        PlayerAction(player.getTileAction(10, 0), &player, &controls, &board).execute();
        REQUIRE(player.robot->currentflag == 1);

        setRobotPos(1, 7, 0, &board, &player);
        PlayerAction(player.getTileAction(10, 0), &player, &controls, &board).execute();
        REQUIRE(player.robot->currentflag == 1);

        setRobotPos(1, 6, 0, &board, &player);
        PlayerAction(player.getTileAction(10, 0), &player, &controls, &board).execute();
        REQUIRE(player.robot->currentflag == 2);

        setRobotPos(1, 7, 0, &board, &player);
        PlayerAction(player.getTileAction(10, 0), &player, &controls, &board).execute();
        REQUIRE(player.robot->currentflag == 3);

        setRobotPos(1, 6, 0, &board, &player);
        PlayerAction(player.getTileAction(10, 0), &player, &controls, &board).execute();
        REQUIRE(player.robot->currentflag == 3);
    }
    SECTION( "conveyors" )
    {
        int starts[4][3][2] = {
            {{8, 11}, {9, 9}, {9, 11}},
            {{8, 9}, {6, 8}, {8, 8}},
            {{6, 9}, {5, 11}, {5, 9}},
            {{6, 11}, {8, 12}, {6, 12}}
        };

        for (int dir = 0; dir < 4; dir++) {
            for (int i = 0; i < 3; i++) {
                setRobotPos(starts[dir][i][0], starts[dir][i][1], 0, &board, &player);
                PlayerAction(player.getTileAction(6, 0), &player, &controls, &board).execute();
                REQUIRE(player.robot->x == starts[dir][i][0] + utility::dx(dir));
                REQUIRE(player.robot->y == starts[dir][i][1] + utility::dy(dir));
                REQUIRE(player.robot->facing == 0);
            }
        }

        // a conveyor that pushes onto an empty tile
        setRobotPos(1, 9, 0, &board, &player);
        PlayerAction(player.getTileAction(6, 0), &player, &controls, &board).execute();
        REQUIRE(player.robot->y == 10);

    }
    SECTION( "express conveyors" )
    {
        int starts[4][3][2] = {
            {{8, 2}, {9, 5}, {9, 2}},
            {{5, 2}, {8, 1}, {5, 1}},
            {{5, 5}, {4, 2}, {4, 5}},
            {{8, 5}, {5, 6}, {8, 6}}
        };

        for (int dir = 0; dir < 4; dir++) 
        {
            for (int i = 0; i < 3; i++) 
            {
                for (int action = 5; action < 7; action++) 
                {
                    setRobotPos(starts[dir][i][0], starts[dir][i][1], 0, &board, &player);
                    PlayerAction(player.getTileAction(action, 0), &player, &controls, &board).execute();
                    REQUIRE(player.robot->x == starts[dir][i][0] + utility::dx(dir));
                    REQUIRE(player.robot->y == starts[dir][i][1] + utility::dy(dir));
                    REQUIRE(player.robot->facing == 0);
                }
            }
        }

        // a conveyor that pushes onto an empty tile
        for (int action = 5; action < 7; action++) 
        {
            setRobotPos(1, 11, 0, &board, &player);
            PlayerAction(player.getTileAction(action, 0), &player, &controls, &board).execute();
            REQUIRE(player.robot->y == 12);
        }
        
    }
    SECTION( "turning conveyors (counter-clockwise)" )
    {
        int starts[4][2] = {{7, 11}, {8, 10}, {7, 9}, {6, 10}};

        for (int dir = 0; dir < 4; dir++) 
        {
            setRobotPos(starts[dir][0], starts[dir][1], 0, &board, &player);
            PlayerAction(player.getTileAction(6, 0), &player, &controls, &board).execute();
            REQUIRE(player.robot->x == starts[dir][0] + utility::dx(dir));
            REQUIRE(player.robot->y == starts[dir][1] + utility::dy(dir));
            REQUIRE(player.robot->facing == 3);
        }

    }
    SECTION( "turning express conveyors (clockwise)" )
    {
        int starts[4][2] = {{6, 2}, {5, 4}, {7, 5}, {8, 3}};

        for (int dir = 0; dir < 4; dir++) 
        {
            for (int action = 5; action < 7; action++) 
            {
                setRobotPos(starts[dir][0], starts[dir][1], 0, &board, &player);
                PlayerAction(player.getTileAction(6, 0), &player, &controls, &board).execute();
                REQUIRE(player.robot->x == starts[dir][0] + utility::dx(dir));
                REQUIRE(player.robot->y == starts[dir][1] + utility::dy(dir));
                REQUIRE(player.robot->facing == 1);
            }
        }
        
    }
    SECTION( "hammer-and-wrenches" )
    {
        
    }
    SECTION( "pushers" )
    {

    }

}

TEST_CASE( "Check death", "[board][playeraction][controls][tile]")
{
    Controls controls;
    controls.init();

    Board board = Board(&controls);
    board.setBoard(-1, 0, false); // hole at 10, 8

    Tile defaultTile;

    Player player = Player(board.getFreeRobot(&player), &controls, &defaultTile);

    SECTION( "real holes " )
    {
        controls.hasRealHoles = true;
        setRobotPos(9, 8, 2, &board, &player);
        PlayerAction(new MoveForwardAction(1), &player, &controls, &board).execute();
        REQUIRE(player.tile->y == 13);
        REQUIRE(player.tile->x == 1);
    }
    SECTION( "not real holes" )
    {
        setRobotPos(9, 8, 2, &board, &player);
        PlayerAction(new MoveForwardAction(1), &player, &controls, &board).execute();
        REQUIRE(player.tile->y == 13);
        REQUIRE(player.tile->x == 1);
    }
    SECTION( "lasered to death" )
    {
        // laser at 3, 10
        setRobotPos(3, 10, 0, &board, &player);
        for (int i = 0; i < 9; i++)
        {
            board.zapLasers();
        }
        REQUIRE(!player.isDead());
        REQUIRE(player.robot->damage == 9);
        REQUIRE(player.getX() == 3);
        REQUIRE(player.getY() == 10);
        board.zapLasers();
        REQUIRE(player.isDead());
        REQUIRE(player.getY() == 13);
        REQUIRE(player.getX() == 1);
    }
}

