	#include "Levels.hpp"

void DrawKineticBlock(float x, float y, float w, float h, float rot, Color col) {
	DrawRectanglePro({ x, y, w, h }, { 0, 0 }, rot, col);
}

void DrawKineticDescend(Vector2 pos, float scale) {
	float time = GetTime();
	float spacing = 85.0f * (scale / 60.0f);
	float thick = 8.0f * (scale / 60.0f);
	float w = scale * 0.7f;
	float h = scale;

	for (int i = 0; i < 7; i++) {
		float wave = sinf(time * 4.0f + i) * 15.0f;
		float jitter = cosf(time * 12.0f + i) * 3.0f;
		unsigned char pulse = 140 + (sinf(time * 8.0f + i) * 110);
		Color col = { pulse, 0, 0, 255 };

		rlPushMatrix();
		rlTranslatef(pos.x + (i * spacing), pos.y + wave, 0);
		rlRotatef(jitter, 0, 0, 1);

		if (i == 0 || i == 6) {
			DrawKineticBlock(0, 0, thick, h, 0, col);
			DrawKineticBlock(thick, 0, w - 2 * thick, thick, 0, col);
			DrawKineticBlock(thick, h - thick, w - 2 * thick, thick, 0, col);
			DrawKineticBlock(w - thick, thick, thick, h - 2 * thick, 0, col);
			DrawKineticBlock(w - thick, thick, thick, thick, -45.0f, col);
			DrawKineticBlock(w - thick, h - thick, thick, thick, 45.0f, col);
		}
		else if (i == 1 || i == 4) {
			DrawKineticBlock(0, 0, thick, h, 0, col);
			DrawKineticBlock(0, 0, w, thick, 0, col);
			DrawKineticBlock(0, h / 2 - thick / 2, w * 0.8f, thick, 0, col);
			DrawKineticBlock(0, h - thick, w, thick, 0, col);
		}
		else if (i == 2) {
			DrawKineticBlock(0, 0, w, thick, 0, col);
			DrawKineticBlock(0, 0, thick, h / 2, 0, col);
			DrawKineticBlock(0, h / 2 - thick / 2, w, thick, 0, col);
			DrawKineticBlock(w - thick, h / 2, thick, h / 2, 0, col);
			DrawKineticBlock(0, h - thick, w, thick, 0, col);
		}
		else if (i == 3) {
			DrawKineticBlock(0, 0, thick, h, 0, col);
			DrawKineticBlock(0, 0, w, thick, 0, col);
			DrawKineticBlock(0, h - thick, w, thick, 0, col);
		}
		else if (i == 5) {
			DrawKineticBlock(0, 0, thick, h, 0, col);
			DrawKineticBlock(w - thick, 0, thick, h, 0, col);
			DrawKineticBlock(thick / 2, 0, thick, h * 1.15f, -32.0f, col);
		}
		rlPopMatrix();
	}
}

	// Main Menu 
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	mainMenu::mainMenu()
	{
		// so initially we are in the waiting stage
		isSeq = false;

		// so initializing the triangles vertices
		tp = { GetScreenWidth() / 2.0f,100.0f };
		bp = { GetScreenWidth() / 2.0f ,400.0f };
		lp = { GetScreenWidth() / 2.0f - 150.0f ,500.0f };
		rp = { GetScreenWidth() / 2.0f + 150.0f ,500.0f };
	}

	void mainMenu::update()
	{
		// when we press enter and while the seq is currently not started
		// or we pressed the enter key for the first time
		if (IsKeyPressed(KEY_ENTER) && !isSeq) {
			// start the ending sequence
			isSeq = true;
			// set the timer to the current time
			timer = GetTime();
			// assuming the game runs at 60 fps
		}

		// so we are updating frames only certain amt of time
		// in raylib the time format is like this x.xxxx or something like that so to get the 0.1 th digit we are multiplying by 10
		// and getting the module so only updating in the 10th cycle
		// or when the unit place changes
		if (isSeq && ( (static_cast<int>(GetTime() - timer)*10)%(10) == 0) ) {
			tp = Vector2Add(tp, { 0.0f,-(static_cast<float>(GetTime() - timer) * 10.0f )});
			bp = Vector2Add(bp, { 0.0f,-(static_cast<float>(GetTime() - timer) * 10.0f) });
			lp = Vector2Add(lp, { 0.0f,-(static_cast<float>(GetTime() - timer) * 10.0f) });
			rp = Vector2Add(rp, { 0.0f,-(static_cast<float>(GetTime() - timer) * 10.0f) });
		}
		// only pushing the triangle in upwards direction
		// also we are adding it as a function of time so that it kind of acceleratres

		// now we add some distortion using the trignometric functions ; this gives the 3d look
		tpd = Vector2Add(tp, { 5.0f * sinf(GetTime()) ,-50.0f * sinf(GetTime()) });
		bpd = Vector2Add(bp, { 5.0f * sinf(GetTime()) ,-50.0f * sinf(GetTime()) });
		lpd = Vector2Add(lp, { 50.0f * cosf(GetTime()) ,10.0f * cosf(GetTime()) });
		rpd = Vector2Add(rp, { 50.0f * cosf(GetTime()) ,10.0f * cosf(GetTime()) });

		/*std::cout << GetTime() << std::endl;*/

		// now when the bottom most point reaches the top we are done with the sequnce
		if (lp.y  <= 0) {
			LevelComplete();
			std::cout << "COMPLETE";
		}
	}

	void mainMenu::draw()
	{
		BeginDrawing();

		ClearBackground(RAYWHITE);

		// this is the background from 0 to half of the screenWidth
		//the gray background
		DrawRectangleGradientH(0, 0, GetScreenWidth(), GetScreenHeight(), { 0,0,0,static_cast<unsigned char>(113 + 112 * cos(0.5 * GetTime())) }, { 0,0,0,static_cast<unsigned char>(113 + 112 * cos(0.5 * GetTime() - PI / 2.0f)) });
		//the red top
		DrawRectangleGradientV(0, 0, GetScreenWidth() , 150, {255,0,0,static_cast<unsigned char>(abs(150*sin(GetTime())))}, { 0, 0, 0, 0 });

		//outlines
		DrawLineEx(tpd, lpd, 7.0f, BLACK);
		DrawLineEx(lpd, rpd, 7.0f, BLACK);
		DrawLineEx(rpd, tpd, 7.0f, BLACK);

	
		//left triangle
		DrawTriangle(tpd, lpd, bpd, YELLOW);
		//right triangle 
		DrawTriangle(tpd, bpd, rpd, { 223,219,0,255 });
		// bottom triangle
		DrawTriangle(bpd, lpd, rpd, {200,188,0,255});


		if (!isSeq) {
			//now the text and the game title
			//DrawText("DESCEND", GetScreenWidth() / 2.0f - 400, GetScreenHeight() / 2.0f, 30, {10,0,0,255});
			DrawKineticDescend({ GetScreenWidth() / 2.0f - 300, GetScreenHeight() / 2.0f }, 60.0f);
			DrawText("Press Enter To Start", GetScreenWidth() / 2.0f - 160, GetScreenHeight() / 2.0f + 220 , 30, RAYWHITE);
		}

		EndDrawing();
	}

	//EndNote : Main Menu is done as far as i can think of .

	// okay so what do i have to do in lvl 1 appart from animations ?
	// okay so the gameplay loop is fully done 
	// cleaning the memory leaks

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// level 1

	level1::level1()
	{

		//init dmg Resolver
		// HANDLED
		damageRslvr = new damageResolver();

		// init events
		//initially the orbs are not collected and the level actually has not started we are in the instruction phase init.
		isOrb1Collected = isOrb2Collected = isOrb3Collected = false;
		isStart = false;

		// init orbs
		orb1 = { 253.45f , 3139.94f };
		orb2 = { 1553.56f , 239.76f };
		orb3 = { 2704.06f , 3174.92f };

		//gun init <---- gonna have to redo this one 
		// HANDLED
		gun = new shooter();

		// camera init
		// HANDLED
		cam = new Camera2D{ 0 };
		cam->offset = { (float)GetScreenWidth() / 2,(float)GetScreenHeight() / 2 };
		cam->rotation = 0.0f;
		cam->zoom = 1.0f;
		cam->target = { 0.0f,0.0f };

		// maze layout init | 0 -> nothing & 1 -> wall
		std::vector<std::vector<int>> m1 = {
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
			{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1},
			{1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1},
			{1,1,0,1,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1},
			{1,1,0,1,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1},
			{1,1,0,1,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1},
			{1,1,0,1,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1},
			{1,1,0,1,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1},
			{1,1,0,1,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1},
			{1,1,0,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1},
			{1,1,0,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1},
			{1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
			{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1},
			{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
			{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
			{1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
			{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1},
			{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
			{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
			{1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			{1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1},
			{1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1},
			{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
			{1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1},
			{1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1},
			{1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		};

		// making a new maze object <-- this also we gonna have to redo some stuff
		// HANDLED
		maze1 = new MazeMaker(100, 35,10 ,m1);

		//HANDLED
		//========================================================================================================

		// player init <-- again gonna have to redo
		Vector2 playerInitPos = { 200.0f,200.0f };
		int playerMxhealth = 10;
		int playerBdyDmg = 2;
		player = new Player(playerInitPos, playerMxhealth, playerBdyDmg, *cam,GameStatus::level_1);
		entityList.push_back(player);

		// the time at which the program started
		timeStart = GetTime();

		// enemies init
		int k = 50;
		while (k >= 0) {
			int x = GetRandomValue(0, 34);
			int y = GetRandomValue(0, 34);

			if (m1[x][y] == 0) {
				k--;
				// spawn the enemies
				enemy1* e = new enemy1({ 0.0f + 100.f * x,0.0f + 100.f * y },12.0f, 2, 1);
				entityList.push_back(e);

			}
			// else go again

		}
		//========================================================================================================
	}

	void level1::update()
	{
		// check if we are on the loading screen
		if (isStart && player->aliveStatus()) {
			// update the player position
			//std::cout << player->collisionData().first.x << " " << player->collisionData().first.y << std::endl;
		
			for (auto it = entityList.begin(); it != entityList.end(); it++) {
				if (it == entityList.begin()) {
					player->update(*gun);
				}
				else if ((*it)->aliveStatus()) {
					(*it)->update();
					damageRslvr->damageResolve(*player, **it, 1.0f, 0.0f);
				}

				// check collision of entities with wall
				if ((*it)->aliveStatus()) {
					maze1->update((**it),1.0f);
				}
			}

			// for the orbs
			if (CheckCollisionCircles(orb1, 10.0f, player->collisionData().first, player->collisionData().second)) { isOrb1Collected = true; };
			if (CheckCollisionCircles(orb2, 10.0f, player->collisionData().first, player->collisionData().second)) { isOrb2Collected = true; };
			if (CheckCollisionCircles(orb3, 10.0f, player->collisionData().first, player->collisionData().second)) { isOrb3Collected = true; };


			// update the shooting mech
			gun->update(this->entityList,*maze1);

			// if all of the orbs are collected
			if (isOrb1Collected && isOrb2Collected && isOrb3Collected) LevelComplete();

		}
		else if (!player->aliveStatus()) {
			if (IsKeyPressed(KEY_ENTER)) {
				GameRestart();
			}

		}
		else {
			if (IsKeyPressed(KEY_ENTER)) {
				isStart = true;
			}
		}
	}

	void level1::draw()
	{
		BeginDrawing();
		ClearBackground(BLACK);

			BeginMode2D(*cam);
			if (isStart && player->aliveStatus()) {
				maze1->draw();
				gun->draw();
				if (!isOrb1Collected) {
					DrawCircleV(orb1, 10.0f, GREEN);
					DrawCircleV(Vector2Add(orb1, { static_cast<float>(3.0f * sin(GetTime()*5)),static_cast<float>(3.0f * cos(GetTime()*5)) }), 10.0f, {0,228,48,200});
					DrawCircleV(Vector2Add(orb1, { static_cast<float>(3.0f * cos(GetTime()*5)),static_cast<float>(3.0f * sin(GetTime()*5)) }), 10.0f, { 0,228,48,160 });
				}
				if (!isOrb2Collected) {
					DrawCircleV(orb2, 10.0f, GREEN);

					DrawCircleV(Vector2Add(orb2, { static_cast<float>(3.0f * sin(GetTime() * 5)),static_cast<float>(3.0f * cos(GetTime() * 5)) }), 10.0f, { 0,228,48,200 });
					DrawCircleV(Vector2Add(orb2, { static_cast<float>(3.0f * cos(GetTime() * 5)),static_cast<float>(3.0f * sin(GetTime() * 5)) }), 10.0f, { 0,228,48,160 });
				}
				if (!isOrb3Collected) {
					DrawCircleV(orb3, 10.0f, GREEN);

					DrawCircleV(Vector2Add(orb3, { static_cast<float>(3.0f * sin(GetTime() * 5)),static_cast<float>(3.0f * cos(GetTime() * 5)) }), 10.0f, { 0,228,48,200 });
					DrawCircleV(Vector2Add(orb3, { static_cast<float>(3.0f * cos(GetTime() * 5)),static_cast<float>(3.0f * sin(GetTime() * 5)) }), 10.0f, { 0,228,48,160 });
				}

				for (auto it = entityList.begin() + 1; it != entityList.end(); it++) {
					if((*it)->aliveStatus())(*it)->draw();
				}

				player->draw();
			}
			EndMode2D();
			if (!isStart) {
				DrawText("Use WASD To Move ;\nUse E to dash ;\nUse Left-Mouse to Shoot \n\nCollect Green Orbs \nto Complete Level \n\n ""You've built a monument \nto yourself out of \nthe people you've stepped \non.""", GetScreenWidth() / 2, GetScreenHeight() / 2 - 150,30 ,YELLOW);
				DrawText("PRIDE AND WRATH", 400, 60, 30, RED);
				// origin
				//288 283
				//std::cout << GetMouseX() << " " << GetMouseY() << std::endl;
				for (int i = 0; i < 400; i++) {
					Rectangle rectInsScrn = { 288.0f + 100*sin((GetTime() + tan(i))) , 283.0f + 100*cos((GetTime() + tan(i))/2), 10.0f,3.0f};
					DrawRectanglePro(rectInsScrn, { rectInsScrn.width / 2 ,rectInsScrn.height / 2 }, RAD2DEG*sin(static_cast<float>(GetTime())), ORANGE);
				}
			}
			else if (!player->aliveStatus()) {
				DrawText("YOUR SOUL WAS NOT WORTHY", 300, 100, 30, ORANGE);
				DrawText("Press Enter to Restart", 450, 200, 16, ORANGE);

			}
		EndDrawing();
	}

	level1::~level1()
	{
		delete gun;
		delete cam;
		delete maze1;
		delete damageRslvr;

		// finally deleting all the entities
		for (int i = 0; i < entityList.size(); i++) {
			delete entityList[i];
		}
	}
	//=================================================================================================================
	// level 2

	// okay so here same as level 1 the gameplay loop is done
	// cleaning memory leaks

	level2::level2()
	{
		//init dmg Resolver
		// HANDLED
		damageRslvr = new damageResolver();

		// init events
		isStart = false;

		// init orbs
		int k = 30;
		while (k >= 0) {
			k--;
			int X = GetRandomValue(13*100, 21*100);
			int Y = GetRandomValue(13*100, 21*100);
			Vector2 v = { X,Y };
			std::pair<Vector2, bool> t = std::make_pair(v, true);
			orbs.push_back(t);
		}

		//gun init
		// HANDLED
		gun = new shooter();

		// camera init
		//HANDLED
		cam = new Camera2D{ 0 };
		cam->offset = { (float)GetScreenWidth() / 2,(float)GetScreenHeight() / 2 };
		cam->rotation = 0.0f;
		cam->zoom = 1.0f;
		cam->target = { 0.0f,0.0f };

		// maze layout init
		std::vector<std::vector<int>> m1 = {
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
			{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
			{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
			{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
			{0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,1,0},
			{0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0},
			{0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0},
			{0,1,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0},
			{0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0},
			{0,1,0,0,0,1,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,1,0},
			{0,1,0,0,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0},
			{0,1,0,0,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0,1,0},
			{0,1,0,0,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0,1,0},
			{0,1,0,0,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0,1,0},
			{0,1,0,0,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0,1,0},
			{0,1,0,0,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0,1,0},
			{0,1,0,0,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0,1,0},
			{0,1,0,0,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0,1,0},
			{0,1,0,0,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0,1,0},
			{0,1,0,0,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0,1,0},
			{0,1,0,0,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0,1,0},
			{0,1,0,0,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0,1,0},
			{0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0,1,0},
			{0,1,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,1,0,0,0,1,0},
			{0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0},
			{0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,1,0},
			{0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0},
			{0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0},
			{0,1,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0},
			{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
			{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
			{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
			{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		};
		maze1 = new MazeMaker(100, 35,10, m1);

		// player init
		Vector2 playerInitPos = { 200.0f,200.0f };
		int playerMxhealth = 10;
		int playerBdyDmg = 2;
		player = new Player(playerInitPos, playerMxhealth, playerBdyDmg, *cam, GameStatus::level_2);
		entityList.push_back(player);

		// the time at which the program started
		timeStart = 0;

		// joe init int k = 150;
		k = 20;
		while (k >= 0) {
			int x = GetRandomValue(2, 32);
			int y = GetRandomValue(2, 32);

			if (m1[x][y] == 0) {
				k--;
				// spawn the enemies
				enemy2* e = new enemy2({ 0.0f + 100.f * x,0.0f + 100.f * y }, 4.0f, 2, 2);
				entityList.push_back(e);

			}
			// else go again

		}

	}

	void level2::update()
	{

		if (isStart && player->aliveStatus()) {
		
			timeStart++;

			if (timeStart > 60*40) {
				timeStart = 0;
				int i = GetRandomValue(1, 4);

				switch (i) {
				case 1:
					player->setPosition({ 300.0f,300.0f });
					break;
				case 2:
					player->setPosition({ 3100.0f,300.0f });
					break;
				case 3:
					player->setPosition({ 300.0f,3100.0f });
					break;
				case 4:
					player->setPosition({ 3100.0f,3100.0f });
					break;
				}
			}

			for (auto it = entityList.begin(); it != entityList.end(); it++) {
				if (it == entityList.begin()) {
					player->update(*gun);
				}
				else if ((*it)->aliveStatus()) {
					static_cast<enemy2*>((*it))->update(player->collisionData().first, *maze1);
					damageRslvr->damageResolve(*player, **it, 1.0f, 0.0f);
				}

				// check collision of entities with wall
				if ((*it)->aliveStatus()) {
					maze1->update((**it), 1.0f);
				}
			}


			for (std::pair<Vector2, bool>& o : orbs) {
				if (o.second && CheckCollisionCircles(player->collisionData().first, player->collisionData().second, o.first, o.second)) {
					o.second = false;
				}
			}

			bool flag = false;

			for (std::pair<Vector2, bool>& o : orbs) {
				flag = flag || o.second;
			}

		

			if (!flag) {
				LevelComplete();
			}


		}
		else if (!player->aliveStatus()) {
			if (IsKeyPressed(KEY_ENTER)) {
				GameRestart();
			}

		}
		else {
			if (IsKeyPressed(KEY_ENTER)) {
				isStart = true;
			}
		}
	}

	void level2::draw()
	{

		BeginDrawing();
		ClearBackground(BLACK);

		BeginMode2D(*cam);
		if (isStart && player->aliveStatus()) {
			maze1->draw();
			gun->draw();
		
			for (std::pair<Vector2, bool> o : orbs) {
				if (o.second) {
					DrawCircleV(o.first, 10.0f, GREEN);
					DrawCircleV(Vector2Add(o.first, { static_cast<float>(3.0f * sin(GetTime() * 5)),static_cast<float>(3.0f * cos(GetTime() * 5)) }), 10.0f, { 0,228,48,200 });
					DrawCircleV(Vector2Add(o.first, { static_cast<float>(3.0f * cos(GetTime() * 5)),static_cast<float>(3.0f * sin(GetTime() * 5)) }), 10.0f, { 0,228,48,160 });
				}
			}

			for (auto it = entityList.begin() + 1; it != entityList.end(); it++) {
				if ((*it)->aliveStatus())(*it)->draw();
			}

			player->draw();
		}
		EndMode2D();
		if (!isStart) {
			DrawText("Use WASD To Move ;\nUse E to dash ; \n\nCollect Green Orbs \nto Complete Level \n\n ""A stomach that never fills, \n hands that never let go. \n You have swallowed the world \n and still you starve.""", GetScreenWidth() / 2, GetScreenHeight() / 2 - 150, 30, YELLOW);			DrawText("GREED AND GLUTTONY", 400, 60, 30, RED);
			// origin
			//288 283
			//std::cout << GetMouseX() << " " << GetMouseY() << std::endl;
			for (int i = 0; i < 400; i++) {
				Rectangle rectInsScrn = { 288.0f + 100 * sin((GetTime() + tan(i))) , 283.0f + 100 * cos((GetTime() + tan(i)) / 2), 10.0f,3.0f };
				DrawRectanglePro(rectInsScrn, { rectInsScrn.width / 2 ,rectInsScrn.height / 2 }, 10*RAD2DEG * sin(static_cast<float>(GetTime())), ORANGE);
			}
		}
		else if (!player->aliveStatus()) {
			DrawText("YOUR SOUL WAS NOT WORTHY", 300, 100, 30, ORANGE);
			DrawText("Press Enter to Restart", 450, 200, 16, ORANGE);

		}
		EndDrawing();
	}

	level2::~level2()
	{
		delete gun;
		delete cam;
		delete maze1;
		delete damageRslvr;

		for (int i = 0; i < entityList.size(); i++) {
			delete entityList[i];
		}
	}

	//=================================================================================================================
	// level 3
	// so how many enemies 
	// so initialy only 1 enemy and the number of enemies inc as you collect orbs
	// gameplay needs some touch ups
	// FUCCKKKKKKK!!!!!!!!!!!!!!!!!!!!!!!!!!!!


	level3::level3()
	{
		//init dmg Resolver
		damageRslvr = new damageResolver();

		// init events
		isStart = false;


		isOrb1Collected = false;
		isOrb2Collected = false;
		isOrb3Collected = false;

		orb1Start = { GetRandomValue(6, 28) * 100.0f, GetRandomValue(6, 28) * 100.0f };
		orb1Target = { GetRandomValue(6, 28) * 100.0f, GetRandomValue(6, 28) * 100.0f };
		orb1Pull = { GetRandomValue(6, 28) * 100.0f, GetRandomValue(6, 28) * 100.0f };

		orb2Start = { GetRandomValue(6, 28) * 100.0f, GetRandomValue(6, 28) * 100.0f };
		orb2Target = { GetRandomValue(6, 28) * 100.0f, GetRandomValue(6, 28) * 100.0f };
		orb2Pull = { GetRandomValue(6, 28) * 100.0f, GetRandomValue(6, 28) * 100.0f };

		orb3Start = { GetRandomValue(6, 28) * 100.0f, GetRandomValue(6, 28) * 100.0f };
		orb3Target = { GetRandomValue(6, 28) * 100.0f, GetRandomValue(6, 28) * 100.0f };
		orb3Pull = { GetRandomValue(6, 28) * 100.0f, GetRandomValue(6, 28) * 100.0f };
		//gun init
		gun = new shooter();

		// camera init
		cam = new Camera2D{ 0 };
		cam->offset = { (float)GetScreenWidth() / 2,(float)GetScreenHeight() / 2 };
		cam->rotation = 0.0f;
		cam->zoom = 1.0f;
		cam->target = { 0.0f,0.0f };

		// maze layout init
		std::vector<std::vector<int>> m1 = {
			// 6 - 28
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
			{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
			{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
			{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
			{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
			{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
			{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
			{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
			{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
			{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
			{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
			{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
			{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
			{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
			{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
			{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
			{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
			{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
			{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
			{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
			{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
			{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
			{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
			{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
			{0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		};

		maze1 = new MazeMaker(100, 35, 10, m1);

		// player init
		Vector2 playerInitPos = { 600.0f,600.0f };
		int playerMxhealth = 10;
		int playerBdyDmg = 2;
		player = new Player(playerInitPos, playerMxhealth, playerBdyDmg, *cam, GameStatus::level_3);
		entityList.push_back(player);

		// the time at which the program started
		timeStart = GetTime();

		// joe init
		// a very slow and shootter enemy

	}

	void level3::update()
	{
		if (isStart && player->aliveStatus()) {
			float duration = 4.0f;
			float elapsed = GetTime() - timeStart;

			if (elapsed >= duration) {
				timeStart = GetTime();
				elapsed = 0;

				orb1Start = orb1Target;
				orb1Target = { GetRandomValue(6, 28) * 100.0f, GetRandomValue(6, 28) * 100.0f };
				orb1Pull = { GetRandomValue(6, 28) * 100.0f, GetRandomValue(6, 28) * 100.0f };

				orb2Start = orb2Target;
				orb2Target = { GetRandomValue(6, 28) * 100.0f, GetRandomValue(6, 28) * 100.0f };
				orb2Pull = { GetRandomValue(6, 28) * 100.0f, GetRandomValue(6, 28) * 100.0f };

				orb3Start = orb3Target;
				orb3Target = { GetRandomValue(6, 28) * 100.0f, GetRandomValue(6, 28) * 100.0f };
				orb3Pull = { GetRandomValue(6, 28) * 100.0f, GetRandomValue(6, 28) * 100.0f };
			}

			float t = elapsed / duration;

			auto GetBezier = [](Vector2 p0, Vector2 p1, Vector2 p2, float t) {
				float invT = 1.0f - t;
				return Vector2{
					(invT * invT * p0.x) + (2 * invT * t * p1.x) + (t * t * p2.x),
					(invT * invT * p0.y) + (2 * invT * t * p1.y) + (t * t * p2.y)
				};
				};

			if (!isOrb1Collected) {
				orb1 = GetBezier(orb1Start, orb1Pull, orb1Target, t);
				if (CheckCollisionCircles(player->collisionData().first, player->collisionData().second, orb1, 10.0f)) {
					isOrb1Collected = true; 
					enemy3* e = new enemy3({ GetRandomValue(6, 28) * 100.0f, GetRandomValue(6, 28) * 100.0f }, 3, 100, 10);
					entityList.push_back(e);
				}
			}

			if (!isOrb2Collected) {
				orb2 = GetBezier(orb2Start, orb2Pull, orb2Target, t);
				if (CheckCollisionCircles(player->collisionData().first, player->collisionData().second, orb2, 10.0f)) {
					isOrb2Collected = true;
					enemy3* e = new enemy3({ GetRandomValue(6, 28) * 100.0f, GetRandomValue(6, 28) * 100.0f }, 3, 100, 10);
					entityList.push_back(e);
				}
			}

			if (!isOrb3Collected) {
				orb3 = GetBezier(orb3Start, orb3Pull, orb3Target, t);
				if (CheckCollisionCircles(player->collisionData().first, player->collisionData().second, orb3, 10.0f)){
					isOrb3Collected = true;
					enemy3* e = new enemy3({ GetRandomValue(6, 28) * 100.0f, GetRandomValue(6, 28) * 100.0f }, 3, 100, 10);
					entityList.push_back(e);
				}

			}

			for (auto it = entityList.begin(); it != entityList.end(); it++) {
				if (it == entityList.begin()) {
					player->update(*gun);
				}
				else if ((*it)->aliveStatus()) {
					static_cast<enemy3*>((*it))->update(player->collisionData().first, *maze1, gun);
					damageRslvr->damageResolve(*player, **it, 1.0f, 0.0f);
				}
				if ((*it)->aliveStatus()) maze1->update((**it), 1.0f);
			}

			if (isOrb1Collected && isOrb2Collected && isOrb3Collected) LevelComplete();
		
			gun->update(this->entityList, *maze1);
		}
		else if (!player->aliveStatus()) {
			if (IsKeyPressed(KEY_ENTER)) GameRestart();
		}
		else {
			if (IsKeyPressed(KEY_ENTER)) isStart = true;
		}
	}

	void level3::draw()
	{
		BeginDrawing();
		ClearBackground(BLACK);

		BeginMode2D(*cam);
		if (isStart && player->aliveStatus()) {
			maze1->draw();
			gun->draw();
			// no other entity so not calling the dmgRslr
			if (!isOrb1Collected) {
				DrawCircleV(orb1, 10.0f, GREEN);
				DrawCircleV(Vector2Add(orb1, { static_cast<float>(3.0f * sin(GetTime() * 5)),static_cast<float>(3.0f * cos(GetTime() * 5)) }), 10.0f, { 0,228,48,200 });
				DrawCircleV(Vector2Add(orb1, { static_cast<float>(3.0f * cos(GetTime() * 5)),static_cast<float>(3.0f * sin(GetTime() * 5)) }), 10.0f, { 0,228,48,160 });
			}
			if (!isOrb2Collected) {
				DrawCircleV(orb2, 10.0f, GREEN);

				DrawCircleV(Vector2Add(orb2, { static_cast<float>(3.0f * sin(GetTime() * 5)),static_cast<float>(3.0f * cos(GetTime() * 5)) }), 10.0f, { 0,228,48,200 });
				DrawCircleV(Vector2Add(orb2, { static_cast<float>(3.0f * cos(GetTime() * 5)),static_cast<float>(3.0f * sin(GetTime() * 5)) }), 10.0f, { 0,228,48,160 });
			}
			if (!isOrb3Collected) {
				DrawCircleV(orb3, 10.0f, GREEN);
				DrawCircleV(orb2, 10.0f, GREEN);

				DrawCircleV(Vector2Add(orb3, { static_cast<float>(3.0f * sin(GetTime() * 5)),static_cast<float>(3.0f * cos(GetTime() * 5)) }), 10.0f, { 0,228,48,200 });
				DrawCircleV(Vector2Add(orb3, { static_cast<float>(3.0f * cos(GetTime() * 5)),static_cast<float>(3.0f * sin(GetTime() * 5)) }), 10.0f, { 0,228,48,160 });
			}

			for (auto it = entityList.begin() + 1; it != entityList.end(); it++) {
				if ((*it)->aliveStatus())(*it)->draw();
			}

			player->draw();
		}
		EndMode2D();
		if (!isStart) {
			DrawText("Use WASD To Move ;\n\nCollect Green Orbs \nto Complete Level \n\n ""A desperate reach for \n what you can never catch. \n Your soul is lead, weighed \n down by the very things \n you crave.""", GetScreenWidth() / 2, GetScreenHeight() / 2 - 150, 30, YELLOW);			DrawText("LUST AND SLOTH", 400, 60, 30, RED);
			// origin
			//288 283
			//std::cout << GetMouseX() << " " << GetMouseY() << std::endl;
			for (int i = 0; i < 400; i++) {
				Rectangle rectInsScrn = { 288.0f + 100 * sin((GetTime() + tan(i))) , 283.0f + 100 * cos((GetTime() + tan(i)) / 2), 10.0f,3.0f };
				DrawRectanglePro(rectInsScrn, { rectInsScrn.width / 2 ,rectInsScrn.height / 2 }, RAD2DEG * tan(static_cast<float>(GetTime())), ORANGE);
			}
		}
		else if (!player->aliveStatus()) {
			DrawText("YOUR SOUL WAS NOT WORTHY", 300, 100, 30, ORANGE);
			DrawText("Press Enter to Restart", 450, 200, 16, ORANGE);

		}
		EndDrawing();
	}

	level3::~level3()
	{
		delete gun;
		delete cam;
		delete maze1;
		delete damageRslvr;


		for (int i = 0; i < entityList.size(); i++) {
			delete entityList[i];
		}

	}


	//=================================================================================================================
	// level 4
	// rework the whole chasing mech 
	// so what do i do ?????????
	// 


	level4::level4()
	{
		//init dmg Resolver
		damageRslvr = new damageResolver();

		// init events
		isStart = false;

		// init orbs

		//gun init
		gun = new shooter();

		// camera init
		cam = new Camera2D{ 0 };
		cam->offset = { (float)GetScreenWidth() / 2,(float)GetScreenHeight() / 2 };
		cam->rotation = 0.0f;
		cam->zoom = 1.0f;
		cam->target = { 0.0f,0.0f };

		// maze layout init
		std::vector<std::vector<int>> m1 = {
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1},
			{1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1},
			{1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
			{1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
			{1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1},
			{1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
			{1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
			{1,0,0,1,0,0,1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,1},
			{1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1},
			{1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1},
			{1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1},
			{1,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1},
			{1,0,0,1,0,0,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,1,0,0,1},
			{1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1},
			{1,0,0,1,0,0,1,0,0,1,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,1,0,0,1},
			{1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1},
			{1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1},
			{1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1},
			{1,0,0,1,0,0,1,0,0,1,0,0,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,0,1,0,0,1},
			{1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1},
			{1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,1,0,0,1},
			{1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1},
			{1,0,0,1,0,0,1,0,0,1,0,0,1,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,0,0,1,0,0,1},
			{1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1},
			{1,0,0,1,0,0,1,0,0,1,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,1},
			{1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
			{1,0,0,1,0,0,1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1},
			{1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
			{1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		};
		maze1 = new MazeMaker(100, 35,10 ,m1);

		// player init
		Vector2 playerInitPos = { 3000.0f,150.0f };
		int playerMxhealth = 10;
		int playerBdyDmg = 2;
		player = new Player(playerInitPos, playerMxhealth, playerBdyDmg, *cam, GameStatus::level_4);

		shadowPos = Vector2Add(player->collisionData().first, {200.0f,0.0f});

		// the time at which the program started
		timeStart = GetTime();

		levelStarted = false;

		orb = { 1500.0f,1400.0f };

	}
	void level4::update()
	{
		if (isStart && player->aliveStatus()) {

			player->update(*gun);
			maze1->update(*player, 0.0f);
			gun->update(this->entityList, *maze1);

			if (CheckCollisionCircles(orb, 10.0f, player->collisionData().first, player->collisionData().second)) { isOrbCollected = true; };
			

			if (levelStarted) {

				playerTrail.push_back(player->collisionData().first);

				if (playerTrail.size() > trailDelay) {
					shadowPos = playerTrail.front();
					playerTrail.pop_front();
				}

				if (CheckCollisionCircles(shadowPos, 150.0f, player->collisionData().first, player->collisionData().second)) {
					player->takeDmg(100);
				}
			}
			else {
				if (IsKeyPressed('W') || IsKeyPressed('A') || IsKeyPressed('S') || IsKeyPressed('D')) {
					levelStarted = true;
				}
			}

			if (isOrbCollected) LevelComplete();

		
		}
		else if (!player->aliveStatus()) {
			if (IsKeyPressed(KEY_ENTER)) GameRestart();
		}
		else {
			if (IsKeyPressed(KEY_ENTER)) isStart = true;
		}
	}
	void level4::draw()
	{
		BeginDrawing();
		ClearBackground(BLACK);

		BeginMode2D(*cam);
		if (isStart) {
			
			float triRadius = 150.0f;

			Vector2 dir = { player->collisionData().first.x - shadowPos.x,
							player->collisionData().first.y - shadowPos.y };
			float shadowAngle = atan2f(dir.y, dir.x);

			Vector2 triPts[3];
			Vector2 triPtsOutline[3];
			for (int i = 0; i < 3; i++) {
				float angle = (i == 0) ? shadowAngle : (i == 1) ? shadowAngle + 2.4f : shadowAngle - 2.4f;
				triPts[i] = { shadowPos.x + triRadius * cosf(angle), shadowPos.y + triRadius * sinf(angle) };
				triPtsOutline[i] = triPts[i]; // same points, same size
			}
			gun->draw();
			maze1->draw();
			DrawTriangle(triPts[0], triPts[2], triPts[1], { 200, 200, 0, 250 });

			if (!isOrbCollected) {
				DrawCircleV(orb, 10.0f, GREEN);
				DrawCircleV(Vector2Add(orb, { static_cast<float>(3.0f * sin(GetTime() * 5)),static_cast<float>(3.0f * cos(GetTime() * 5)) }), 10.0f, { 0,228,48,200 });
				DrawCircleV(Vector2Add(orb, { static_cast<float>(3.0f * cos(GetTime() * 5)),static_cast<float>(3.0f * sin(GetTime() * 5)) }), 10.0f, { 0,228,48,160 });
			}

			player->draw();
			DrawTriangleLines(triPtsOutline[0], triPtsOutline[1], triPtsOutline[2], { 80, 80, 80, 220 });

		}
		EndMode2D();

		if (!isStart) {
			DrawText("Use WASD To Move ;\n\nCollect Green Orbs \nto Complete Level", GetScreenWidth() / 2, GetScreenHeight() / 2 - 100, 30, YELLOW);
			DrawText("PRIDE AND WRATH", 400, 60, 30, RED);
			// origin
			//288 283
			//std::cout << GetMouseX() << " " << GetMouseY() << std::endl;
			for (int i = 0; i < 400; i++) {
				Rectangle rectInsScrn = { 288.0f + 100 * sin((GetTime() + tan(i))) , 283.0f + 100 * cos((GetTime() + tan(i)) / 2), 10.0f,3.0f };
				DrawRectanglePro(rectInsScrn, { rectInsScrn.width / 2 ,rectInsScrn.height / 2 }, RAD2DEG * tan(static_cast<float>(GetTime())*10), ORANGE);
			}
		}
		else if (!player->aliveStatus()) {
			DrawText("YOUR SOUL WAS NOT WORTHY", 300, 100, 30, ORANGE);
			DrawText("Press Enter to Restart", 450, 200, 16, ORANGE);
				
		}
		EndDrawing();
	}

	level4::~level4()
	{
		delete gun;
		delete player;
		delete cam;
		delete maze1;
		delete damageRslvr;
	}

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	endScreen::endScreen()
	{
		// so initially we are in the waiting stage
		isSeq = false;

		// so initializing the triangles vertices
		tp = { GetScreenWidth() / 2.0f,100.0f };
		bp = { GetScreenWidth() / 2.0f ,400.0f };
		lp = { GetScreenWidth() / 2.0f - 150.0f ,500.0f };
		rp = { GetScreenWidth() / 2.0f + 150.0f ,500.0f };
	}

	void endScreen::update()
	{
		// when we press enter and while the seq is currently not started
		// or we pressed the enter key for the first time
		if (IsKeyPressed(KEY_ENTER) && !isSeq) {
			// start the ending sequence
			isSeq = true;
			// set the timer to the current time
			timer = GetTime();
			// assuming the game runs at 60 fps
		}

		// so we are updating frames only certain amt of time
		// in raylib the time format is like this x.xxxx or something like that so to get the 0.1 th digit we are multiplying by 10
		// and getting the module so only updating in the 10th cycle
		// or when the unit place changes
		if (isSeq && ((static_cast<int>(GetTime() - timer) * 10) % (10) == 0)) {
			tp = Vector2Add(tp, { 0.0f,-(static_cast<float>(GetTime() - timer) * 10.0f) });
			bp = Vector2Add(bp, { 0.0f,-(static_cast<float>(GetTime() - timer) * 10.0f) });
			lp = Vector2Add(lp, { 0.0f,-(static_cast<float>(GetTime() - timer) * 10.0f) });
			rp = Vector2Add(rp, { 0.0f,-(static_cast<float>(GetTime() - timer) * 10.0f) });
		}
		// only pushing the triangle in upwards direction
		// also we are adding it as a function of time so that it kind of acceleratres

		// now we add some distortion using the trignometric functions ; this gives the 3d look
		tpd = Vector2Add(tp, { 5.0f * sinf(GetTime()) ,-5.0f * sinf(GetTime()) });
		bpd = Vector2Add(bp, { 5.0f * sinf(GetTime()) ,-5.0f * sinf(GetTime()) });
		lpd = Vector2Add(lp, { 5.0f * cosf(GetTime()) ,1.0f * cosf(GetTime()) });
		rpd = Vector2Add(rp, { 5.0f * cosf(GetTime()) ,1.0f * cosf(GetTime()) });

		/*std::cout << GetTime() << std::endl;*/

		// now when the bottom most point reaches the top we are done with the sequnce
		if (lp.y <= 0) {
			LevelComplete();
			std::cout << "COMPLETE";
		}
	}

	void endScreen::draw()
	{
		BeginDrawing();

		ClearBackground({ 10, 5, 15, 255 });

		// this is the background from 0 to half of the screenWidth
		//the gray background
		DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(),{ 60, 10, 0, 255 },{ 10, 5, 15, 255 });	
		DrawRectangleGradientV(0, 0, GetScreenWidth(), 150, { 255,0,0,static_cast<unsigned char>(abs(150 * sin(GetTime()))) }, { 0, 0, 0, 0 });

		//outlines
		/*DrawLineEx(tpd, lpd, 7.0f, BLACK);
		DrawLineEx(lpd, rpd, 7.0f, BLACK);
		DrawLineEx(rpd, tpd, 7.0f, BLACK);*/


		//left triangle
		DrawTriangle(Vector2Add(tpd, { -10.0f,-40.0f }), Vector2Add(lpd, { -10.0f,-40.0f }), Vector2Add(bpd, { -10.0f,-40.0f }), { 255, 140, 0, 255 });
		//right triangle 
		DrawTriangle(Vector2Add(tpd, { 10.0f,-20.0f }), Vector2Add(bpd, { 10.0f,-20.0f }), Vector2Add(rpd, { 10.0f,-20.0f }), { 223, 100, 0, 255 });
		// bottom triangle
		DrawTriangle(Vector2Add(bpd, {0,10.0f}), Vector2Add(lpd, { 0,10.0f }), Vector2Add(rpd, { 0,10.0f }), { 200, 80, 0, 255 });


		if (!isSeq) {
			//now the text and the game title
			DrawText("DESCEND", GetScreenWidth() / 2.0f - 400, GetScreenHeight() / 2.0f, 30, {200,200,200,255});;
			DrawText("Press ESC to exit \n Thx for playing \n credits : ME \n(FLuroscentCrab) \nPowered by RAYLIB", GetScreenWidth() / 2.0f + 140, GetScreenHeight() / 2.0f - 100, 30, RAYWHITE);
		}

		EndDrawing();
	}
