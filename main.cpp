

#include "includes.hpp"
using namespace std;

sf::Font font;

 sf::RenderWindow window(sf::VideoMode(1200, 700), "Gob");
const int noOfChars=4, noOfEnemies=4, speed=5;
sf::Texture hightex;
sf::Sprite highlight;
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
        int b=combat;
        if(weapon!=NULL){
            b+=weapon->bonus;
        }
        return b-2*fightCount;
    }
    int getArmourPercent(){
        int a=0;
        if(shield!=NULL)
            a+=shield->armour;
        if(armour!=NULL)
        {
            a+=armour->armour;
        }
        return a;
        
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
        stamina=20;
        weapon=NULL;
        sidearm=NULL;
        shield=NULL;
        armour=NULL;
    }
    void takeDamage(int d){
        stamina-=d;
    }
};
Being *chars[noOfChars],*enemies[4];

struct Battle{
    Being * turnOrder[100];
    int currentTurn;
    int noOfCombatants;
    Battle(){

    };
    void startRound(){
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
        noOfCombatants=combatants.size();
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
    Being* getCurrent(){
        return turnOrder[currentTurn];
    }
    void takeTurn(){
        highlight.setPosition(turnOrder[currentTurn]->x*64,turnOrder[currentTurn]->y*64);
        if(turnOrder[currentTurn]->friendly){

        }else{
            endTurn();
        }
    }
    void endTurn(){
        if(++currentTurn==noOfCombatants){
            startRound();
        }
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
    window.clear();
    window.draw(highlight);
    for(int i=0;i<battle.noOfCombatants;i++){
        window.draw(battle.turnOrder[i]->getSprite());
    }
    window.display();

}

Being *getOccupant(int x, int y){
    for(int i=0;i<battle.noOfCombatants;i++){
        if(battle.turnOrder[i]->x==x&&battle.turnOrder[i]->y==y){
            return battle.turnOrder[i];
        }
    }
    return NULL;
}


void setup(){
 
    srand(time(NULL));
        font.loadFromFile("fonts/thestrong.ttf");
    for(int i =0;i<noOfChars;i++){
        chars[i]=new Being("Player "+to_string(i), "player", true);
        chars[i]->x=i+1;
        chars[i]->y=i+1;
    }
    for(int i=0;i<noOfEnemies;i++){
        enemies[i]=new Being("Goblin "+to_string(i), "goblin");
        enemies[i]->x=i+5;
        enemies[i]->y=i+1;
    }
    hightex.loadFromFile("pics/highlight.png");
    highlight.setTexture(hightex);
}
int dist(int x1, int y1, int x2, int y2){
    return abs(x1-x2)+abs(y1-y2);
}
void click(sf::Vector2i pos){
    int x= (int)pos.x/64;
    int y=(int)pos.y/64;
    Being *current=battle.getCurrent();
    if(current->friendly){
        Being *occupant=getOccupant(x,y);
        if(occupant!=NULL){
            if(dist(current->x,current->y,x,y)==1 &&!occupant->friendly){
                fight(current,occupant);
                battle.endTurn();
            }
        }else{
            if(dist(current->x,current->y,x,y)<=speed){
                current->x=x;
                current->y=y;
                battle.endTurn();
            }
        }
    }
}

int main(){
    setup();


    sf::Clock clock;
    sf::Time elapsed;
    sf::Event event;
    battle.startRound();

    window.setFramerateLimit(20);
    while (window.isOpen())
    {

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type==sf::Event::MouseButtonPressed){
                sf::Vector2i pos=sf::Mouse::getPosition(window);
                    //                 if(event.mouseButton.button==sf::Mouse::Button::Left)leftClick(pos);
                    // else rightClick(pos);
                click(pos);

            }
        }
        elapsed = clock.getElapsedTime();
        battle.takeTurn();
        draw();

    }

    return 0;
}

