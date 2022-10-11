#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include "ResourcePath.hpp"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

using namespace sf;
float calculatePaddle(float x1,float y1, float x2,float y2,float targetX);

int main(){
    srand((int)time(0));
    int width = 1200, height = 800;
    int score1 = 0,score2 = 0;
    float circleRadius = 20;
    sf::RenderWindow window(sf::VideoMode(width, height), "Pong!");
    sf::CircleShape shape(circleRadius);
    shape.setFillColor(sf::Color::White);
    shape.setPosition(width/4+(rand()%width/2), height/4+(rand()%height/2));
    shape.setOrigin(circleRadius, circleRadius);
    float xSpeed = .4,ySpeed;
    ySpeed = xSpeed;
    float paddleSpeed = .6;
    const int barrierLength = height/40;
    RectangleShape barrier[barrierLength];
    for(int i = 0;i<barrierLength;++i){
        barrier[i].setSize(Vector2f(20,20));
        barrier[i].setOrigin(10, 10);
        barrier[i].setPosition(width/2, i*40+20);
        barrier[i].setFillColor(sf::Color::Blue);
    }

    int paddleWidth = 10,paddleLength =80;
    sf::RectangleShape paddles[2];
    paddles[0].setSize(Vector2f(paddleWidth,paddleLength));
    paddles[0].setPosition(20, 100);
    paddles[0].setOrigin(paddleWidth/2, paddleLength/2);
    paddles[1].setSize(Vector2f(paddleWidth,paddleLength));
    paddles[1].setPosition(width-20, 100);
    paddles[1].setOrigin(paddleWidth/2, paddleLength/2);
    window.setKeyRepeatEnabled(false);
    Text scoreTexts[2];
    int scores[2];
    Font font;
    font.loadFromFile(resourcePath() + "sansation.ttf");
    scoreTexts[0].setFont(font);
    scoreTexts[1].setFont(font);
    scoreTexts[0].setCharacterSize(100);
    scoreTexts[1].setCharacterSize(100);
    scoreTexts[0].setFillColor(Color::White);
    scoreTexts[1].setFillColor(Color::White);
    scoreTexts[0].setPosition(width/4, 100);
    scoreTexts[1].setPosition(3*width/4, 100);
    scoreTexts[0].setString("0");
    scoreTexts[1].setString("0");
    scores[0] = 0; scores[1] = 0;
    float prevX = 0, prevY = 0;
        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }
            if(Keyboard::isKeyPressed(Keyboard::Return)){
                scores[0] = 0; scores[1] = 0;
                shape.setPosition(width/4+(rand()%width/2), height/4+(rand()%height/2));
                continue;
            }
            float circleX = shape.getPosition().x, circleY = shape.getPosition().y;
            float paddle1X = paddles[0].getPosition().x, paddle1Y = paddles[0].getPosition().y;
                if(Keyboard::isKeyPressed(Keyboard::W)&&paddle1Y-paddleLength/2>=0){
                    paddle1Y -= paddleSpeed;
                    paddles[0].setPosition(paddle1X, paddle1Y);
                    // event.key.code = Keyboard::Unknown;
                }
                if(Keyboard::isKeyPressed(sf::Keyboard::S)&&paddle1Y+paddleLength/2<=height){
                    paddle1Y+= paddleSpeed;
                    paddles[0].setPosition(paddle1X, paddle1Y);
                    // event.key.code = Keyboard::Unknown;
                }
            float paddle2X = paddles[1].getPosition().x, paddle2Y = paddles[1].getPosition().y;
//            if(Keyboard::isKeyPressed(Keyboard::Up)&&paddle2Y-paddleLength/2>=0){
//                paddle2Y -= paddleSpeed;
//                paddles[1].setPosition(paddle2X, paddle2Y);
//            }
//            if(Keyboard::isKeyPressed(Keyboard::Down)&&paddle2Y+paddleLength/2<=height){
//                paddle2Y+=paddleSpeed;
//                paddles[1].setPosition(paddle2X, paddle2Y);
//            }
//
            if(circleX - circleRadius<=paddle1X+paddleWidth/2 && circleY-circleRadius<=paddle1Y+paddleLength/2
               && circleY+circleRadius>=paddle1Y-paddleLength/2){
                xSpeed *= -1;
            }
            if(circleX + circleRadius>=paddle2X-paddleWidth/2 && circleY-circleRadius<=paddle2Y+paddleLength/2
               && circleY+circleRadius>=paddle2Y-paddleLength/2){
                xSpeed *= -1;
            }
            std::stringstream s1,s2;
            if(circleX + circleRadius>=width) {
                //Hits Right side
                
                s1<<++scores[0];
                
                xSpeed*=-1;
                shape.setPosition(width/4+(rand()%width/2), height/4+(rand()%height/2));
                circleX = shape.getPosition().x;circleY = shape.getPosition().y;
                sleep(1);
            }else if(circleX-circleRadius<=0){
                //Hits left side
                
                s2<<++scores[1];
                
                xSpeed*=-1;
                shape.setPosition(width/4+(rand()%width/2), height/4+(rand()%height/2));
                circleX = shape.getPosition().x;circleY = shape.getPosition().y;
                sleep(1);
            }else{
                s1<<scores[0];
                s2<<scores[1];
            }
            
            scoreTexts[1].setString(s2.str());
            scoreTexts[0].setString(s1.str());
            
            if(circleY+circleRadius>=height||circleY-circleRadius<=0){
                ySpeed*=-1;
                
                prevX = circleX; prevY=circleY;
            }
            if(xSpeed>0){
                //Adjusts the opposite paddle to match the ball's trajectory
               float targetY = calculatePaddle(circleX,circleY,prevX,prevY,paddle2X);
 //               float targetY = circleY;
                if(targetY>=0&&targetY<=height){
                    if(paddle2Y>targetY) paddle2Y-=paddleSpeed;
                    else if(paddle2Y<targetY) paddle2Y+=paddleSpeed;
                    
                    paddles[1].setPosition(paddle2X,paddle2Y);
                }}
//            else if(xSpeed<0){
//               float targetY = calculatePaddle(circleX,circleY,prevX,prevY,paddle1X);
// //               float targetY = circleY;
//                if(targetY>=0&&targetY<=height){
//                    paddles[0].setPosition(paddle1X,targetY);
//                    paddle1Y = targetY;
//                }}
            shape.setPosition(circleX+xSpeed, circleY+ySpeed);
            window.clear();
            for(RectangleShape block:barrier) window.draw(block);
            for(Text scoreText: scoreTexts) window.draw(scoreText);
            window.draw(shape);
            for(RectangleShape paddle:paddles) window.draw(paddle);
            window.display();
        }
    return 0;
}

float calculatePaddle(float x1,float y1, float x2,float y2,float targetX){
    float slope = (y2-y1)/(x2-x1);
    return slope*(targetX-x1)+y1;
}
