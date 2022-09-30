/*******************************************************************************************
*
*   raylib game template
*
*   <Game title>
*   <Game description>
*
*   This game has been created using raylib (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2021 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

float version = 0.0;
std::string title;
int num_textures = 0;
std::map<std::string, std::string> textures;

std::map<std::string, Texture2D> level_textures;


std::string** background;
int background_w = 0;
int background_h = 0;

std::string** foreground;
int foreground_w = 0;
int foreground_h = 0;

std::string** objects;
int objects_w = 0;
int objects_h = 0;

const std::string COLLISION = "X";
const std::string DESTRUCTIBLE = "D";

bool is_passable(int x, int z) {
	return objects[z][x] == "0";
}

struct Bombs {
	int pos_x;
	int pos_y;
	float time;
};

struct Player {
	std::string num = "0";
	int pos_x = 0.0;
	int pos_y = 1.0;
	int pos_z = 0.0;
	Color color;
	int num_bombs = 0;
	int max_bombs = 1;
	std::vector<Bombs> bombs;
};

std::vector<Player>players;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int draw(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera mode");

	

	std::map<std::string, std::string>::iterator it;

	for (it = textures.begin(); it != textures.end(); it++) {
		
		Image image = LoadImage(it->second.c_str());
		level_textures[it->first] = LoadTextureFromImage(image);

		std::cout << it->first << " " << it->second << std::endl;
	}

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = { 0.0f, 10.0f, 10.0f };  // Camera position
    camera.target = { 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type


    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
	float BOX_SIZE = 1.0;

	
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
		//Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
		float x_margin = background_w % 2 == 0 ? .5 : 0;
		float z_margin = background_h % 2 == 0 ? .5 : 0;

		float x_offset = background_w / 2;
		float z_offset = background_h / 2;

		Vector3 cubePosition = { 
			-x_offset+ x_margin, 
			0.0f,
			-z_offset + z_margin 
		};

		for (int i = 0; i < background_h; i++) {
			for (int j = 0; j < background_w; j++) {
				Vector3 pos = {
					cubePosition.x + float(i),
					cubePosition.y,
					cubePosition.z + float(j)
				};
				std::string t = background[i][j];
				if (t == "0")
					continue;

				DrawCubeTexture(level_textures[t], cubePosition, BOX_SIZE, BOX_SIZE,BOX_SIZE, WHITE);
				DrawCubeWires(cubePosition, BOX_SIZE, BOX_SIZE, BOX_SIZE, WHITE);
			}
		}
		x_margin = foreground_w % 2 == 0 ? .5 : 0;
		z_margin = foreground_h % 2 == 0 ? .5 : 0;
				   
		x_offset = foreground_w / 2;
		z_offset = foreground_h / 2;

		  cubePosition = {
			-x_offset + x_margin,
			1.0f,
			-z_offset + z_margin 
		  };

		for (int i = 0; i < foreground_h; i++) {
			for (int j = 0; j < foreground_w; j++) {
				 Vector3 pos = {
					cubePosition.x + float(i),
					cubePosition.y,
					cubePosition.z + float(j)
				};
				std::string t = foreground[i][j];
				if (t == "0")
					continue;

				DrawCubeTexture(level_textures[t], cubePosition, BOX_SIZE, BOX_SIZE, BOX_SIZE, WHITE);
				DrawCubeWires(cubePosition, BOX_SIZE, BOX_SIZE, BOX_SIZE, WHITE);
			}
		}
		x_margin = objects_w % 2 == 0 ? .5 : 0;
		z_margin = objects_h % 2 == 0 ? .5 : 0;
		
		x_offset = objects_w / 2;
		z_offset = objects_h / 2;
		Vector3 playerPosition = {
			-x_offset + x_margin,
			2.0f,
			-z_offset + z_margin
		};

		for (int j = 0; j < objects_h; j++) {
			for (int i = 0; i < objects_w; i++) {
				if (objects[j][i] == "1" || objects[j][i] == "2" || objects[j][i] == "3" || objects[j][i] == "4") {
					
					if (objects[j][i] == "1") {


						int x_inc = 0;
						int z_inc = 0;
						if (IsKeyPressed(KEY_RIGHT)) {
							if (i + 1 < objects_w) {
								if (is_passable(i+1,j)){
									x_inc = 1;
								}

							}
						}
						else if (IsKeyPressed(KEY_LEFT)) {
							if (i - 1 >= 0) {
								if (is_passable(i - 1, j)) {
									x_inc = -1;
								}
							}
						}
						else if (IsKeyPressed(KEY_DOWN)) {
							if (i - 1 >= 0) {
								if (is_passable(i, j+1)) {
									z_inc = 1;
								}
							}
						}
						else if (IsKeyPressed(KEY_UP)) {
							if (i - 1 >= 0) {
								if (is_passable(i, j-1)) {
									z_inc = -1;
								}
							}
						}
						Vector3 pos = {
						playerPosition.x + float(i),
						playerPosition.y,
						playerPosition.z + float(j)
						};
						Color c = WHITE;
						if (objects[j][i] == "2") {
							c = BLACK;
						}
						if (objects[j][i] == "3") {
							c = RED;
						}
						if (objects[j][i] == "4") {
							c = YELLOW;
						}
						DrawSphere(pos, .5, c);
					}
				}

			}
		}
		
        DrawGrid(10, 1.0f);

        EndMode3D();

        DrawText("Welcome to the third dimension!", 10, 40, 20, DARKGRAY);

        DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
int main() {

	std::string temp;

	std::ifstream level("level.sus");

	//Abrir archivo
	if (!level.is_open()) {
		std::cout << "Error al abrir el archivo\n" << std::endl;
		return 1;
	}

	getline(level, temp, ';');

	std::cout << temp << std::endl;


	if (temp != "SUS") {
		std::cout << "Error: Cabecera mal formada" << std::endl;
		return 2;
	}

	//Cogemos version del archivo y cambiamos la nuestra
	getline(level, temp, ';');


	std::cout << temp << std::endl;


	version = stof(temp);

	//Para coger la siguiente linea
	getline(level, temp);

	getline(level, temp, ';');
	//
	std::cout << temp << std::endl;

	//Comprobamos titulo y lo substituimos
	if (temp != "TITLE") {
		std::cout << "Error: Titulo incorrecto" << std::endl;
		return 3;
	}
	getline(level, temp, ';');


	std::cout << temp << std::endl;

	title = temp;

	getline(level, temp);

	getline(level, temp, ';');

	//Comprobamos texturas y lo substituimos
	if (temp != "TEXTURES") {
		std::cout << "Error: Texturas mal cargadas" << std::endl;
		return 4;
	}

	std::cout << temp << std::endl;

	getline(level, temp, ';');

	num_textures = stoi(temp);

	std::cout << temp << std::endl;

	getline(level, temp);

	for (int i = 0; i < num_textures; i++) {
		std::string key;
		getline(level, key, ';');

		std::string value;
		getline(level, value, ';');

		textures[key] = value;

		std::cout << key << ";" << value << std::endl;

		getline(level, temp);
	}

	getline(level, temp, ';');

	if (temp != "BACKGROUND") {
		std::cout << "Error: Background incorrecto" << std::endl;
		return 5;
	}

	std::cout << temp << std::endl;

	getline(level, temp, ';');

	std::cout << temp << std::endl;

	background_w = stoi(temp);

	getline(level, temp, ';');
	std::cout << temp << std::endl;

	background_h = stoi(temp);

	getline(level, temp);

	//Dibujando background

	background = new std::string * [background_h];

	for (int i = 0; i < background_h; i++) {
		background[i] = new std::string[background_w];

	}

	for (int i = 0; i < background_h; i++) {
		for (int j = 0; j < background_w; j++) {
			getline(level, temp, ';');
			std::cout << temp;

			background[i][j] = temp;
		}
		std::cout << std::endl;

		getline(level, temp);

	}

	//Cogiendo foreground

	getline(level, temp, ';');

	if (temp != "FOREGROUND") {
		std::cout << "Error: Foreground incorrecto" << std::endl;
		return 6;
	}

	std::cout << temp << std::endl;

	getline(level, temp, ';');

	std::cout << temp << std::endl;

	foreground_w = stoi(temp);

	getline(level, temp, ';');
	std::cout << temp << std::endl;

	foreground_h = stoi(temp);

	getline(level, temp);

	//Dibujando foreground

	foreground = new std::string * [foreground_h];

	for (int i = 0; i < foreground_h; i++) {
		foreground[i] = new std::string[foreground_w];

	}

	for (int i = 0; i < foreground_h; i++) {
		for (int j = 0; j < foreground_w; j++) {
			getline(level, temp, ';');
			std::cout << temp;

			foreground[i][j] = temp;
		}
		std::cout << std::endl;

		getline(level, temp);

	}

	//Cogiendo objects
	getline(level, temp, ';');

	if (temp != "OBJECTS") {
		std::cout << "Error: Objetos incorrecto" << std::endl;
		return 7;
	}

	std::cout << temp << std::endl;

	getline(level, temp, ';');

	std::cout << temp << std::endl;

	objects_w = stoi(temp);

	getline(level, temp, ';');
	std::cout << temp << std::endl;

	objects_h = stoi(temp);

	getline(level, temp);



	//Dibujando objetos

	objects = new std::string * [objects_h];

	for (int i = 0; i < objects_h; i++) {
		objects[i] = new std::string[objects_w];

	}

	for (int i = 0; i < objects_h; i++) {
		for (int j = 0; j < objects_w; j++) {
			getline(level, temp, ';');
			std::cout << temp;

			objects[i][j] = temp;
		}
		std::cout << std::endl;

		getline(level, temp);

	}
	draw();

	return 0;
}