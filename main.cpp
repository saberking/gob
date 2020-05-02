

#include "includes.hpp"
using namespace std;

sf::Font font;

 sf::RenderWindow window(sf::VideoMode(1200, 700), "Gob");
const int noOfChars=4, noOfEnemies=4;

struct Armour{
    int armour;
    Armour(int a){
        armour=a;
    }
};
struct Weapon{
    int bonus;
    bool twoHands;
    bool sidearm;
    string type;//short, extended, ranged
    Weapon(int b, bool th=false, bool sa=false, string t="short"){
        bonus=b;
    }
};
struct Shield{
    int armour;
    Shield(int a){
        armour=a;
    }
};

struct Being{
    sf::Sprite sprite;
    sf::Texture texture;
    string name;
    int stamina;
    int combat;
    int bow;
    Armour *armour;
    Weapon *sidearm;
    Shield *shield;
    Weapon *weapon;
    int x;
    int y;
    bool hasMoved;
    int fightCount;
    bool friendly;
    char facing;
    int getCombatBonus(){
        return combat+weapon->bonus-2*fightCount;
    }
    int getArmourPercent(){
        if(shield==NULL)
            return armour->armour;
        else
        {
            return armour->armour+shield->armour;
        }
        
    }
    sf::Sprite getSprite(){
        sprite.setPosition(x*64,y*64);
        return sprite;
    }
    Being(string _name, string textureName, bool _friendly=false){
        name=_name;
        texture.loadFromFile("pics/"+textureName+".png");
        sprite.setTexture(texture);

        friendly=_friendly;
        facing='n';
    }
    void takeDamage(int d){
        stamina-=d;
    }
};
Being *chars[noOfChars],*enemies[4];

struct Battle{
    Being * turnOrder[100];
    int currentTurn;
    Battle(){

    };
    void startTurn(){
        list<Being*>combatants;
        for(int i=0;i<noOfChars;i++){
            if(chars[i]->stamina>0)
                combatants.push_back(chars[i]);
        }
        for(int i=0;i<sizeof(enemies)/sizeof(*enemies);i++){
            if(enemies[i]->stamina>0){
                combatants.push_back(enemies[i]);
            }
        }
        int noOfCombatants=combatants.size();
        for(int i=0;i<noOfCombatants;i++){
            int j=rand()%(noOfCombatants-i);
            list<Being*>::iterator it=combatants.begin();
            advance(it, j);
            turnOrder[i]=*it;
            turnOrder[i]->hasMoved=false;
            turnOrder[i]->fightCount=0;
            combatants.remove(*it);
        }
        currentTurn=0;
    }

};
Battle battle;
bool armourSave(Being *a){
        int arm=a->getArmourPercent();
        if(rand()%100<arm){
            return true;
        }
        return false;
}
void fight(Being *a, Being *b){
    int roll=rand()%20+a->getCombatBonus()-rand()%20-b->getCombatBonus();
    if(roll < 0 ){
        if(!armourSave(a)){
            a->takeDamage(-roll);
        }
    }else if(roll>0){
        if(!armourSave(b)){
            b->takeDamage(roll);
        }
    }
}



void draw(){
    for(int i=0;i<sizeof(chars)/sizeof(*chars);i++){
        window.draw(chars[i]->getSprite());
    }
    for(int i=0;i<sizeof(enemies)/sizeof(*enemies);i++){
        window.draw(enemies[i]->getSprite());
    }

}




void setup(){
 
    srand(time(NULL));
        font.loadFromFile("fonts/thestrong.ttf");
    for(int i =0;i<noOfChars;i++){
        chars[i]=new Being("Player "+to_string(i), "player", true);
    }
    for(int i=0;i<noOfEnemies;i++){
        enemies[i]=new Being("Goblin "+to_string(i), "goblin");
    }
  
}

int main(){
    setup();


    sf::Clock clock;
    sf::Time elapsed;
    sf::Event event;

    window.setFramerateLimit(20);
    while (window.isOpen())
    {

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type==sf::Event::MouseButtonPressed){
                sf::Vector2i pos=sf::Mouse::getPosition(window);

            }
        }
        elapsed = clock.getElapsedTime();

    }

    return 0;
}