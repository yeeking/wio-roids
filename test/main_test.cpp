#include "../Asteroids.h"
#include <string>
#include <iostream>
#include <stdlib.h> 
#include <time.h> 

bool testCreate()
{
    MovingThing thing{Vec2D{0, 0}};
    Ship ship{Vec2D{0, 0}};
    Bullet bullet{Vec2D{0, 0}};
    Asteroid asteroid{500, 500};
    return true; 
}

bool testUpdate()
{
    MovingThing thing{Vec2D{0, 0}};
    thing.setDX(10);
    thing.update(100, 100);
    Vec2D* pos = thing.getPos();
    return (pos->x == 10 && pos->y == 0);    
}

bool testCanFire()
{
    Ship ship{Vec2D{0, 0}};
    ship.fire();
    return true;
}

bool testBulletMoves()
{
    Ship ship{Vec2D{0, 0}};
    for (auto i=0; i<10; i++) ship.rotateLeft();// point slightly off top
    Bullet* b = ship.fire();
    int x1 = b->getPos()->x;
    ship.update(100, 100);
    //b->update(100, 100);
    int x2 = b->getPos()->x;
    printf("x1 %i, x2 %i \n", x1, x2);
    return x1 != x2;
}

bool testLotsaBullets()
{
     Ship ship{Vec2D{0, 0}};
    Bullet* b;
    Bullet* lastb;
    bool res = true;
    for (auto i=0; i<5; i++)
    {
        ship.rotateLeft();// point slightly off top
        b = ship.fire();
        ship.update(100, 100);// all bullets should move 
        if (i > 0) // verify bullets are moving
        {
            if (b->getPos()->y == lastb->getPos()->y) res = false; 
        }
        lastb = b;

    }
    return res;
    
}

bool testWrapPositionPos()
{    
    MovingThing t{Vec2D{99, 0}};
    t.setDX(2);
    t.update(100, 100);
    return (t.getPos()->x == 1);
}


bool testWrapPositionNeg()
{    
    MovingThing t{Vec2D{0, 0}};
    t.setDX(-2);
    t.update(100, 100);
   // printf("pos %i ", t.getPos()->x);
    return (t.getPos()->x == 98);
}


bool testBulletMovesRightUp()
{
    Ship ship{Vec2D{50, 50}};
    ship.setAngle(1.0); // radians
    Bullet* b = ship.fire();
    int x1 = b->getPos()->x;
    int y1 = b->getPos()->y;
    b->update(100, 100);
    int x2 = b->getPos()->x;
    int y2 = b->getPos()->y;
    return (x2 > x1 && y2 < y1);
}


bool testBulletMovesLeftUp()
{
    Ship s {Vec2D{50, 50}};
    s.setAngle(5.0);
    Bullet* b = s.fire();
    int x1 = b->getPos()->x;
    int y1 = b->getPos()->y;
    b->update(100, 100);
    int x2 = b->getPos()->x;
    int y2 = b->getPos()->y;
    return (x2 < x1 && y2 < y1);

}



bool testBulletMovesRightDown()
{
    Ship ship{Vec2D{50, 50}};
    ship.setAngle(2.8);
    Bullet* b = ship.fire();
    int x1 = b->getPos()->x;
    int y1 = b->getPos()->y;
    ship.update(200, 200);    
    int x2 = b->getPos()->x;
    int y2 = b->getPos()->y;
    return (x2 > x1 && y2 > y1);
}


bool testBulletMovesLeftDown()
{
    Ship ship{Vec2D{50, 50}};
    ship.setAngle(4.0);
    Bullet* b = ship.fire();
    int x1 = b->getPos()->x;
    int y1 = b->getPos()->y;
    ship.update(100, 100);
    int x2 = b->getPos()->x;
    int y2 = b->getPos()->y;
    printf("x1 %i x2 %i y1 %i y2 %i\n", x1, x2, y1, y2);
    return (x2 < x1 && y2 > y1);
}

bool testAsteroidsPlaceAtRandom()
{
    srand (time(NULL));
    Asteroid a1 {500, 500};
    Asteroid a2 {500, 500};
    if (a1.getPos()->x == a2.getPos()->x) return false;
    return true;
}

bool testAsteroidsMoveAtRandom()
{
    srand (time(NULL));
    Asteroid a1 {500, 500};
    Asteroid a2 {500, 500};
    if (a1.getDPos()->x == a2.getDPos()->x) return false;
    return true;
}

bool testCreateGame()
{
    AsteroidsGame game{500, 500};
    return true;   
}

bool testThingsCollide()
{
    Vec2D p1{100, 100};
    Vec2D p2{100, 100};
    return MovingThing::thingsCollide(&p1, &p2, 10);
}

bool testThingsDontCollide()
{
    Vec2D p1{100, 100};
    Vec2D p2{10, 25};
    return !MovingThing::thingsCollide(&p1, &p2, 10);
}

bool testGameUpdate()
{
    AsteroidsGame game{500, 500};
    game.update();
    return true;
}

int doTest(std::string what, bool res)
{
    std::cout << what << " res: " << res << std::endl;
    return res;
}

void runTests()
{
    int total = 0;
    int passed = 0;

//     passed += doTest("testCreate", testCreate());
//     total ++;
//     passed += doTest("testUpdate", testUpdate());
//     total ++;
//     passed += doTest("testCanFire", testCanFire());
//     total ++;
//     passed += doTest("testBulletMoves", testBulletMoves());
//     total ++;
//     passed += doTest("testWrapPositionPos", testWrapPositionPos());
//     total ++;
//     passed += doTest("testWrapPositionNeg", testWrapPositionNeg());
//     total ++;
//    passed += doTest("testLotsaBullets", testLotsaBullets());
//     total ++;
    
//     passed += doTest("testBulletMovesRightUp", testBulletMovesRightUp());
//     total ++;
//     passed += doTest("testBulletMovesLeftUp", testBulletMovesLeftUp());
//     total ++;
    
//     passed += doTest("testBulletMovesRightDown", testBulletMovesRightDown());
//     total ++;
//     passed += doTest("testBulletMovesLeftDown", testBulletMovesLeftDown());
//     total ++;

//     passed += doTest("testAsteroidsPlaceAtRandom", testAsteroidsPlaceAtRandom());
//     total ++;
//     passed += doTest("testAsteroidsMoveAtRandom", testAsteroidsMoveAtRandom());
//     total ++;
    
//     passed += doTest("testCreateGame", testCreateGame());
//     total ++;

//     passed += doTest("testThingsCollide", testThingsCollide());
//     total ++;
//     passed += doTest("testThingsDontCollide", testThingsDontCollide());
//     total ++;

    passed += doTest("testGameUpdate", testGameUpdate());
    total ++;
    

    std::cout << "Passed: " << passed << " of "<< total << std::endl;

}
int main()
{
    runTests();
}