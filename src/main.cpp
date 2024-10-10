/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

For a C++ project simply rename the file to .cpp and re-run the build script 

-- Copyright (c) 2020-2024 Jeffery Myers
--
--This software is provided "as-is", without any express or implied warranty. In no event 
--will the authors be held liable for any damages arising from the use of this software.

--Permission is granted to anyone to use this software for any purpose, including commercial 
--applications, and to alter it and redistribute it freely, subject to the following restrictions:

--  1. The origin of this software must not be misrepresented; you must not claim that you 
--  wrote the original software. If you use this software in a product, an acknowledgment 
--  in the product documentation would be appreciated but is not required.
--
--  2. Altered source versions must be plainly marked as such, and must not be misrepresented
--  as being the original software.
--
--  3. This notice may not be removed or altered from any source distribution.

*/

#include "raylib.h"
#include<iostream>
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
//#define YELLOW     (Color){ 253, 249, 0, 255 }
//#define GREEN      (Color){ 0, 228, 48, 255 }
//#define BLACK      (Color){ 0, 0, 0, 255 }
//#define WHITE      (Color){ 255, 255, 255, 255 }


int playerScore=0;
int cpuScore=0;

class Paddle;
class cpuPaddle;

class Ball{
	public:
		float x,y;
		int speed_x,speed_y;
		int radius;
		Ball(){
			x=GetScreenWidth()/2;
			y=GetScreenHeight()/2;
			speed_x=7;
			speed_y=7;
			radius=20;
		}
		int getBall_y(){
			return (int)y;
		}
		void Draw(){
			DrawCircle(x,y,radius,YELLOW);
		}

		void resetBall(){
			x=GetScreenWidth()/2;
			y=GetScreenHeight()/2;
			int speeds[2]={-1,1};
			speed_x*=speeds[GetRandomValue(0,1)];
			speed_y*=speeds[GetRandomValue(0,1)];
		}
		void Update(){
			x+=speed_x;
			y+=speed_y;
			if((y+radius>=GetScreenHeight())||(y-radius<=0)){
				speed_y=speed_y*-1;
			}

			if(x+radius>=GetScreenWidth()){
				cpuScore++;
				resetBall();
			}

			if(x-radius<=0){
				playerScore++;
				resetBall();
			}

		}
		friend void collisionPad(Ball ball,Paddle player);
		friend void collisionCpu(Ball ball,cpuPaddle cpu);
};

class Paddle{
	protected:
		void limitMovement(){
			if(y<=0){
				y=0;
			}
			if(y+height>=GetScreenHeight()){
				y=GetScreenHeight()-height;
			}
		}
		public:
		float x,y;
		float width,height;
		int speed;
		Paddle(){
			y=GetScreenHeight()/2;
			width=25;
			height=120;
			x=GetScreenWidth()-width-10;
			speed=7;
		}
		void Draw(){
			DrawRectangleRec(Rectangle{x,y,width,height},DARKGREEN);
		}
		void Update(){
			if(IsKeyDown(KEY_UP)){
				y-=speed;
			}
			if(IsKeyDown(KEY_DOWN)){
				y+=speed;
			}
			limitMovement();
		}
		friend void collisionPad(Ball ball,Paddle player);
};


class cpuPaddle: public Paddle{
	protected:
		void limitMovement(){
			if(y<=0){
				y=0;
			}
			if(y+height>=GetScreenHeight()){
				y=GetScreenHeight()-height;
			}
		}
	public:

		/*float x,y;
		float width,height;
		int speed*/;
		cpuPaddle(){
			y=GetScreenHeight()/2;
			width=25;
			height=120;
			x=10;
			speed=4;
		}
		void Draw(){
			DrawRectangleRec(Rectangle{x,y,width,height},GREEN);
		}
		void Update(int ball_y){
			if(y+height/2>ball_y){
				y=y-speed;
			}
			if(y+height/2<ball_y){
				y=y+speed;
			}

			limitMovement();
		}
		friend void collisionCpu(Ball ball,cpuPaddle cpu);
};

void collisionPad(Ball ball, Paddle player){
	if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {player.x, player.y, player.width, player.height})) {
        ball.speed_x =ball.speed_x= -1;}
}

void collisionCpu(Ball ball,cpuPaddle cpu){
	if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {cpu.x, cpu.y, cpu.width, cpu.height})) {
            ball.speed_x *= -1;
        }
}

int main ()
{	
	bool check=true;
	// Tell the window to use vysnc and work on high DPI displays
	//SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Pong");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	//SearchAndSetResourceDir("resources");
	SetTargetFPS(60);
	Ball ball;
	Paddle player;
	cpuPaddle cpu;
	// Load a texture from the resources directory
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{	
		// drawing
		BeginDrawing();
		ball.Update();
		player.Update();
		cpu.Update(ball.getBall_y());
		//collisionPad(ball,player);
		//collisionCpu(ball, cpu);
		if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {player.x, player.y, player.width, player.height})) {
            ball.speed_x *= -1;
        }
		int y=0;
        if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {cpu.x, cpu.y, cpu.width, cpu.height})) {
            ball.speed_x *= -1;
        }
		ClearBackground(BLACK);
		for(int i=0;i<24;i++){
			if(i%2==0){
			DrawRectangleRec(Rectangle{1280/2,(float)y,10,50},BLACK);}
			else{
				DrawRectangleRec(Rectangle{1280/2,(float)y,10,50},WHITE);
			}
			y+=50;
		}
		ball.Draw();
		cpu.Draw();
		player.Draw();
		DrawText(TextFormat("%i",cpuScore),1280/4-20,20,80,WHITE);
		DrawText(TextFormat("%i",playerScore),3*1280/4-20,20,80,WHITE);
		if(playerScore>=3){
			DrawText(TextFormat("%i",playerScore),3*1280/4-20,20,80,WHITE);
			DrawText("Player Wins",GetScreenWidth()/2,GetScreenHeight()/2,30,WHITE);
			check=false;
		}
		if(cpuScore>=3){
			DrawText(TextFormat("%i",cpuScore),1280/4-20,20,80,WHITE);
			DrawText("CPU Wins",GetScreenWidth()/2,GetScreenHeight()/2,30,WHITE);
			check=false;
		}
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
		if(!check){
			WaitTime(3);
			CloseWindow();
		}
	}

	// cleanup
	// unload our texture so it can be cleaned up
	// destory the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}