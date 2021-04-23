#include <SFML/Graphics.hpp>

#include <chrono>
#include <math.h>
#include <vector>
#include <list>
#include <memory>
#include <random>

struct Constants
{
    const static int Width = 1200;
    const static int Height = 800;
    constexpr static float deg_to_rad = 0.017453f;
};

int my_random()
{
    std::default_random_engine dre(static_cast <std::size_t> (std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<> my_random(0, RAND_MAX);
    return my_random(dre);
}

class Animation
{
    public:
        Animation(){}

        Animation(sf::Texture &tex, std::size_t x, std::size_t y, int w, int h, std::size_t number_of_img, float speed):
        m_frame(0), m_speed(speed)
        {
            for(auto i = 0; i < number_of_img; ++i)
            {
                m_frames.push_back(sf::IntRect(x+i*w, y, w, h));
            }
            
            sprite.setTexture(tex);
            sprite.setOrigin(w/2,h/2);
            sprite.setTextureRect(m_frames[0]);
        }

       ~Animation() noexcept = default;
    
    public:
        void update()
        {
            m_frame += m_speed;
            auto n = std::size(m_frames);
            if (m_frame >= n)
                m_frame -= n;
            if (n > 0)
                sprite.setTextureRect(m_frames[static_cast<int>(m_frame)]);
        }

        bool isEnd()
        {
            return m_frame + m_speed >= std::size(m_frames);
        }

    public:
        sf::Sprite sprite;

    private:
        float m_frame;
        float m_speed;
        std::vector<sf::IntRect> m_frames;
        
};


class Entity
{
    public:
        Animation anim;
        float x, y, dx, dy, R, angle;
        bool life;
        std::string name;

        Entity(): life(true)
        {}
    
        void settings(Animation& a, int X, int Y, float Angle = 0, int radius = 1) // ��� float 0.0f
        {
            anim = a;
            x = X;
            y = Y;
            angle = Angle;
            R = radius;
        }

    
        virtual void update() {};

        void draw(sf::RenderWindow & app)
        {
            anim.sprite.setPosition(x, y);
            anim.sprite.setRotation(angle + 90);
            app.draw(anim.sprite);
        }
            

        virtual ~Entity() noexcept = default;
};


class Asteroid : public Entity
{
    public:
        Asteroid()
        {
            dx = my_random() % 8 - 4;
            dy = my_random() % 8 - 4;
            name = "Asteroid";
        }

    private:
        void update()
        {
            x += dx;
            y += dy;

            if (x > Constants::Width) x = 0;
            if (x < 0) x = Constants::Width;
            if (y > Constants::Height) y = 0;
            if (y < 0) y = Constants::Height;
        }

};


class Bullet : public Entity
{
    public:
        Bullet()
        {
            name = "Bullet";
        }
    
    private:
        void  update()
        {
            dx = cos(angle * Constants::deg_to_rad) * 6;
            dy = sin(angle * Constants::deg_to_rad) * 6;
        
            x += dx;
            y += dy;

            if (x > Constants::Width || x < 0 || y > Constants::Height || y < 0)
                    life = 0;
        }

};


class Player : public Entity
{
    public:
        bool thrust;
        std::size_t lives = 3;
        std::size_t score;

        Player()
        {
          
            score = 0;
            name = "Player";
        }

        bool isAlive()
        {
            return (lives > 0);
        }

        void score_increase()
        {
            ++score;
        }

        void lives_decrease()
        {
            --lives;
        }

    private:
        void update() // override ?
        {
            if (thrust)
            {
                dx += std::cos(angle * Constants::deg_to_rad) * 0.2;
                dy += std::sin(angle * Constants::deg_to_rad) * 0.2;
            }
            else
            {
                dx *= 0.99;
                dy *= 0.99;
            }

           
            float speed = sqrt(dx * dx + dy * dy);
            if (speed > maxSpeed)
            {
                dx *= maxSpeed / speed;
                dy *= maxSpeed / speed;
            }

            x += dx;
            y += dy;

            if (x > Constants::Width) x = 0;
            if (x < 0) x = Constants::Width;
            if (y > Constants::Height) y = 0;
            if (y < 0) y = Constants::Height;
        }

    private:
        const int maxSpeed = 15;

};


class Game
{
    public:

    Game(){}

    void run()
    {
        sf::RenderWindow app(sf::VideoMode(Constants::Width, Constants::Height), "ASTEROIDS");
        app.setFramerateLimit(60);
        
        sf::Font font;
        font.loadFromFile("arial.ttf");
        sf::Text text;
        text.setFont(font);
        text.setFillColor(sf::Color::Cyan);
        sf::Texture t1,t2,t3,t4,t5,t6,t7;
        t1.loadFromFile("images/spaceship.png");
        t2.loadFromFile("images/background.jpg");
        t3.loadFromFile("images/explosions/type_C.png");
        t4.loadFromFile("images/rock.png");
        t5.loadFromFile("images/fire_blue.png");
        t6.loadFromFile("images/rock_small.png");
        t7.loadFromFile("images/explosions/type_B.png");

        t1.setSmooth(true);
        t2.setSmooth(true);

        sf::Sprite background(t2);

        Animation sExplosion(t3, 0,0,256,256, 48, 0.5);
        Animation sRock(t4, 0,0,64,64, 16, 0.2);
        Animation sRock_small(t6, 0,0,64,64, 16, 0.2);
        Animation sBullet(t5, 0,0,32,64, 16, 0.8);
        Animation sPlayer(t1, 40,0,40,40, 1, 0);
        Animation sPlayer_go(t1, 40,40,40,40, 1, 0);
        Animation sExplosion_ship(t7, 0,0,192,192, 64, 0.5);
    
        

        creating_asteroids(sRock);
        std::shared_ptr<Player> p = creating_player(sPlayer);

    
        while (app.isOpen() && p->isAlive())
        {
            text.setString("score: " + std::to_string(p->score) + "\nLifes: " + std::to_string(p->lives));
            sf::Event event;
            while (app.pollEvent(event))
            {
                switch(event.type)
                {
                    case sf::Event::Closed:
                        app.close();
                        break;
                    case sf::Event::KeyPressed:
                        if (event.key.code == sf::Keyboard::Space)
                        {
                            std::shared_ptr<Bullet> b(new Bullet);
                            b->settings(sBullet, p->x, p->y, p->angle, 10);
                            entities.push_back(b);
                        }
                            break;
                    default:
                        break;
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) p->angle += 3;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  p->angle -= 3;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) p->thrust = true;
            else p->thrust = false;
        
            for (auto a : entities)
                for (auto b : entities)
                {
                    if (a->name == "Asteroid" && b->name == "Bullet")
                        if (a->anim.sprite.getGlobalBounds().intersects(b->anim.sprite.getGlobalBounds()))
                        {
                            a->life = false;
                            b->life = false;

                            std::shared_ptr<Entity> e(new Entity);
                            e->settings(sExplosion, a->x, a->y);
                            e->name = "Explosion";
                            entities.push_back(e);


                            for ( int i =0; i < 2; ++i)
                            {
                                if (a->R == 15)
                                {
                                    continue;
                                }
                            
                                std::shared_ptr<Entity> e(new Asteroid);
                                e->settings(sRock_small, a->x, a->y, rand() % 360, 15);
                                entities.push_back(e);
                            }

                            p->score_increase();

                        }

                    if (a->name == "Player" && b->name == "Asteroid")
                        if (a->anim.sprite.getGlobalBounds().intersects(b->anim.sprite.getGlobalBounds()))
                        {
                            b->life = false;

                            std::shared_ptr<Entity> e(new Entity);
                            e->settings(sExplosion_ship, a->x, a->y);
                            e->name = "Explosion";
                            entities.push_back(e);

                            p->settings(sPlayer, Constants::Width / 2, Constants::Height / 2, 0, 20);
                            p->dx = 0;
                            p->dy = 0;
                            p->lives_decrease();
                        }
                }


            if (p->thrust)  p->anim = sPlayer_go;
                else   p->anim = sPlayer;


            for (auto e : entities)
                if (e->name == "Explosion")
                    if (e->anim.isEnd()) e->life = 0;

            if (my_random() % 150 == 0)
            {
                std::shared_ptr<Asteroid> a(new Asteroid);
                a->settings(sRock, 0, my_random()% Constants::Height, rand() % 360, 25);
                entities.push_back(a);
            }

            for (auto i = entities.begin(); i != entities.end();)
            {
                std::shared_ptr<Entity> e = *i;

                e->update();
                e->anim.update();

                if (e->life == false)
                {
                    i = entities.erase(i);
                }
                else ++i;
            }

        
            app.draw(background);
            for (auto i : entities)
                i->draw(app);
            app.draw(text);
            app.display();
        }
    }
    private:

        void creating_asteroids(Animation anim)
        {
            for (auto i = 0; i < 15; ++i)
            {
                std::shared_ptr<Asteroid> a(new Asteroid);
                a->settings(anim, my_random() % Constants::Width, my_random() % Constants::Height, my_random() % 360, 25);
                entities.push_back(a);
               
            }
           
        }

        std::shared_ptr<Player> creating_player(Animation anim)
        {
            std::shared_ptr<Player> p(new Player);
            p->settings(anim, 300, 250, 0, 20);
            entities.push_back(p);
            return p;
        }



    private:
        std::list<std::shared_ptr<Entity>> entities;


};


int main()
{
    Game game;
    game.run();

    return 0;
}
