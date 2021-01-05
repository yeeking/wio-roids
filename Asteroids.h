#include <math.h>      
#include <stdlib.h> 
#include <time.h> 

//#include <iostream>

class Vec2D{
    public:
        Vec2D(const float x,
              const float y): x{x}, y{y} {}
        static Vec2D solveTriangle(const double angle, const double hypD)
        {
            double oppY, adjX;
            oppY = -1 * cos(angle) * hypD;
            adjX = sin(angle) * hypD;
            
            //printf("Vec2D::solveTri:: hyp %f theta %f oppY %f adjX %f\n", hypD, angle, oppY, adjX);
            return Vec2D{(float)adjX, (float)oppY};
        }
        float x;
        float y;
};

/** base class for things that move providing 
 * update and set dx/y, rotate functions etc.
*/
class MovingThing
{
  public:
    /** checks if the sent items are within mimDist of eachother*/
    static bool thingsCollide(Vec2D* p1, Vec2D* p2, double minDist)
    {
        double dist, dx, dy; 
        dx = fabs(p1->x - p2->x);
        dy = fabs(p1->y - p2->y);
        dist = (dx*dx) + (dy*dy);
        dist = sqrt(dist);
        //printf("dx %f, dy %f, dist %f\n", dx, dy, dist);
        if (dist < minDist) return true;
        else return false; 
    }
  /** x and y are the starting position  */
    MovingThing(const Vec2D pos)  
          : pos{pos}, dPos{Vec2D{0, 0}}, angle{0},visible{false}
    {

    }

    virtual void update(float screenWidth, float screenHeight) 
    {;
        pos.x = fmod((pos.x + dPos.x),  screenWidth);
        if (pos.x < 0) pos.x = screenWidth + pos.x;
        pos.y = fmod((pos.y + dPos.y),  screenHeight);
        if (pos.y < 0) pos.y = screenHeight + pos.y;
        
    }
    void setDX(float  dx)
    {
        dPos.x = dx;
    }
    void setDY(float  dy)
    {
        dPos.y = dy;
    }
    void rotateLeft(float byAngle = 0.05)
    {
        angle -= byAngle;
    }
    void rotateRight(float byAngle = 0.05)
    {
        angle += byAngle;
    }
    /** directly set the angle.*/
    void setAngle(double angle)
    {
        this->angle = angle; 
    }
    /** return a pointer to the pos variable (for speed) which should not be edited */
    Vec2D* getPos()
    {
        return &pos;
    }
    /** return a pointer to the pos variable (for speed) which should not be edited */
    Vec2D* getDPos()
    {
        return &dPos;
    }

    /** set the position of this thing. Will make a copy of the incoming pos and store it 
     * so not super efficient probably.
     */
    void setPos(const Vec2D pos)
    {
        this->pos = pos;
    }
    
    double getAngle() const 
    {
        return this->angle;
    }
    bool getVisible() const
    {
        return this->visible;
    }
    void setVisible(bool visible)
    {
        this->visible = visible; 
    }
  private:
    Vec2D pos;
    Vec2D dPos;
    double angle;
    bool visible;
};

class Bullet : public MovingThing
{
public:
    Bullet(const Vec2D pos) : MovingThing(pos)
    {

    }
};

/** represents ... an asteroid. Spawns at a random position with a random velocity */
class Asteroid : public MovingThing
{
    public:
    /** create an asteroid with random position and velocity*/
    Asteroid(float screenWidth, float screenHeight, float maxVel = 10) 
    : MovingThing(
        Vec2D{0, 0})
    {
        setPos(Vec2D{ 
            (float) fmod(rand(), maxVel) - maxVel/2, 
            (float) fmod(rand(), maxVel) - maxVel/2
            });
        setDX( fmod(rand(), maxVel) - maxVel/2); 
        setDY( fmod(rand(), maxVel) - maxVel/2); 
    }
    /**
     *  Sets the position and velocity to random
     */
    void randomise()
    {

    }
    private:
};


class Ship : public MovingThing
{
    public:
    /** create a shiup at the sent position*/
    Ship(const Vec2D pos) : 
    MovingThing(pos), bulletInd{0}, thrust{false}
    {
        bulletCount = sizeof(bullets) / sizeof(Bullet*);
        for (auto i=0;i<bulletCount; ++i)
        {
            bullets[i] = new Bullet(Vec2D{getPos()->x, getPos()->y});
            bullets[i]->setVisible(false);
        }
    }

    ~Ship()
    {
        for (auto i=0;i<bulletCount; ++i)
        {
            delete bullets[i];
        }
    }

    void update(float screenWidth, float screenHeight) override
    {
        MovingThing::update(screenWidth, screenHeight);
        // update the bullets
        Vec2D* pos;
        for (auto i = 0; i<bulletCount; ++i)
        {
            bullets[i]->update(screenWidth, screenHeight);
            pos = bullets[i]->getPos();
            if (pos->x > screenWidth 
                || pos->x < 0
                || pos->y > screenHeight 
                || pos->y < 0)
                {
                    bullets[i]->setVisible(false);
                }
        }
    }
    void applyThrust(float speed = 1)
    {
        Vec2D boost = Vec2D::solveTriangle(getAngle(), speed);
        Vec2D* now = getDPos();
        now->x += boost.x;
        now->y += boost.y;
    }
    /** Fire a bullet in the current direction of the ship*/
    Bullet* fire(float speed = 2)
    {
        Bullet* bullet = bullets[bulletInd];
        bullet->setPos(Vec2D{getPos()->x, getPos()->y}); 
        Vec2D dPos = Vec2D::solveTriangle(getAngle(), speed);
        bullet->setDX(dPos.x);
        bullet->setDY(dPos.y);
        bullet->setVisible(true);
        
        bulletInd = (bulletInd + 1 ) % bulletCount;
        return bullet;
    }
    bool bulletsHitAsteroid(Asteroid* asteroid, double astSize)
    {
        for (auto i = 0; i<bulletCount; ++i)
        {
            if (asteroid->getVisible()
               &&
               MovingThing::thingsCollide(bullets[i]->getPos(),
                                          asteroid->getPos(), 
                                          astSize))
            {
                bullets[i]->setVisible(false); // switch off the bullet
                bullets[i]->setPos(Vec2D{this->getPos()->x, this->getPos()->y});
                return true;
            }
        }
        return false; 
    }

    /** get direct access to the bullet pointer*/
    Bullet* bullets[2]; 
    short bulletCount;

private:
    
    bool thrust;
    short bulletInd;
};


class AsteroidsGame 
{
    public:
    AsteroidsGame(float screenWidth, float screenHeight) 
    : screenWidth{screenWidth}, screenHeight{screenHeight}, score{0}, lives{3}
    { 
        //printf("Aseroind coiunt %i ", asteroidCount);
        asteroidCount = sizeof(asteroids) / sizeof(Asteroid*);
        for (auto i=0; i<asteroidCount; ++i)
        {
            asteroids[i] = new Asteroid(screenWidth, screenHeight, 5);
            asteroids[i]->setVisible(false);
        }
        level = 1;
        resetLevel(level);
        ship = new Ship(Vec2D{(float) (screenWidth/2), (float) (screenHeight/2)});
        gameOver = false;
    }
    ~AsteroidsGame()
    {
       for (auto i=0; i<10; ++i)
        {
            delete asteroids[i];
        }
        delete ship;
    }

    void resetLevel(int level)
    {
        // level 1 has 5 asteroids
        int visibleAsteroids = level + 3;
        if (visibleAsteroids > asteroidCount) visibleAsteroids = asteroidCount;

        for (auto i=0; i<visibleAsteroids; ++i)
        {
            asteroids[i]->setVisible(true);
        }        
    }

    void resetShipPosition()
    {

    }

    void update()
    {
        double asteroidDiameter = 10;
        int aliveAsteroids = 0; 
        // check for collisions
         
        for (auto i=0; i<asteroidCount; ++i)
        {
            if (asteroids[i]->getVisible()) aliveAsteroids ++;

            // bullets
            if (asteroids[i]->getVisible() 
               && ship->bulletsHitAsteroid(asteroids[i], asteroidDiameter))
            {
                score ++;
                asteroids[i]->setVisible(false);
            }
            // ship

            if (asteroids[i]->getVisible() 
                && MovingThing::thingsCollide(ship->getPos(), 
                                          asteroids[i]->getPos(), 
                                          asteroidDiameter + 3))
            {
                // kill the ship
                lives --; 
                ship->setPos(Vec2D{(float) (screenWidth/2), (float) (screenHeight/2)});

                if (lives == 0)
                {
                    // dead! 
                    lives = 3;
                    gameOver = true;
                }
            }
        }
        
        if(aliveAsteroids == 0)
        {
            // reset the game
            level ++;
            resetLevel(level);
        }

        for (auto i=0; i<asteroidCount; ++i)
        {
            asteroids[i]->update(screenWidth, screenHeight);
        }
        ship->update(screenWidth, screenHeight);
    }
    // void moveLeft()
    // {
    //     ship->rotateLeft();
    // }
    // void moveRight()
    // {
    //     ship->rotateRight();
    // }
    // void fire()
    // {
    //     ship->fire();
    // }
    
    bool gameOver;
    float screenWidth;
    float screenHeight;
    int score;
    int lives;
    int level; 
    int asteroidCount;

    Ship* ship;
    Asteroid* asteroids[20];
    
};



