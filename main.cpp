#include "common.h"
#include "gettimeofday.h"
#include "render.h"
#include "texture.h"
#include "gui.h"
#include "math.h"

#include "level.h"
#include "game.h"

#include "ui_funcs.h"

using namespace std;

// keyboard
bool global_keys[GLFW_KEY_LAST];
bool global_keysOnce[GLFW_KEY_LAST];

// mouse
int global_mouse[GLFW_MOUSE_BUTTON_LAST];
float global_mousepos[2];

Render *renderer;
Texturer *texturer;
GUI *gui;
Game *game;

struct timeval tick_otime, tick_ntime, tick_diff, tick_gameStart, tick_totalTime;
float tick_msecs;

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	byte res = 0;

	if (key != GLFW_KEY_UNKNOWN) {
		if (action == GLFW_PRESS || action == GLFW_REPEAT) {
			GUIElement *activeElement;

			global_keys[key] = true;
			// global_keysOnce[key] = true;

			activeElement = gui->GetActiveElement();
			if (activeElement)
				res = activeElement->DoActionKeyPress(key, scancode, mods);

			// TODO: No GUI elements, do something else...
			if (!res) {
				if (key == GLFW_KEY_ESCAPE)
					glfwSetWindowShouldClose(window, 1);
			}
		} else if (action == GLFW_RELEASE) {
			global_keys[key] = false;
		}
	}

//	printf("Key: %i %i Status: %i Mods: %i\n", key, scancode, action, mods);
}

static void character_callback(GLFWwindow* window, unsigned int keycode) {
	byte res = 0;

	GUIElement *activeElement;

	activeElement = gui->GetActiveElement();
	if (activeElement)
		res = activeElement->DoActionCharPress(keycode);

	// TODO: No GUI elements, do something else...
	if (!res) {
	}
}

static void cursor_position_callback(GLFWwindow* window, double x, double y) {
	byte res = 0;

	global_mousepos[0] = (float)x;
	global_mousepos[1] = (float)y;

	if (global_mouse[0]) {
		if (gui->GetHalfActiveElement()) {
			res = gui->GetHalfActiveElement()->DoActionHold((int)global_mousepos[0], (int)global_mousepos[1]);
		}

		// TODO: No GUI elements, do something else...
		if (!res) {

		}
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	byte res = 0;

	global_mouse[button] = action == GLFW_PRESS ? true : false;

	if (button == 0) {
		GUIElement *clickElement = NULL;
		GUIElement *prevElement = NULL;

		if (action == GLFW_PRESS) {
			prevElement = gui->GetActiveElement();
			clickElement = gui->FindElementByCoords(gui->GetRootElement(), global_mousepos[0], global_mousepos[1]);
			if (prevElement && prevElement != clickElement)
				prevElement->DoActionLoseFocus();
			gui->SetActiveElement(NULL);
			gui->SetHalfActiveElement(clickElement);
			if (clickElement)
				res = clickElement->DoActionHold((int)global_mousepos[0], (int)global_mousepos[1]);
		} else {
			clickElement = gui->FindElementByCoords(gui->GetRootElement(), global_mousepos[0], global_mousepos[1]);
			if (gui->GetHalfActiveElement())
				gui->GetHalfActiveElement()->DoActionStopHold((int)global_mousepos[0], (int)global_mousepos[1]);
			if (clickElement && clickElement == gui->GetHalfActiveElement()) {
				gui->SetActiveElement(clickElement);
				gui->SetHalfActiveElement(NULL);
				res = clickElement->DoActionClick((int)global_mousepos[0], (int)global_mousepos[1]);
			}
		}
		// TODO: No GUI elements, do something else...
		if (!res) {

		}

	}

//	printf("Mouse: %i %i %i\n", button, action, mods);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	GUIElement *clickElement = NULL;

	clickElement = gui->FindElementByCoords(gui->GetRootElement(), global_mousepos[0], global_mousepos[1]);
	if (clickElement)
		clickElement->DoActionScroll((float)xoffset, (float)-yoffset*10.0f);
}

void size_callback(GLFWwindow* window, int x, int y) {
	renderer->Reshape(x, y);
}

void doTick() {

	/*
	if (global_keys[GLFW_KEY_A])
		cameraPos[0] += overall_msecs / 1000.0f;
	else if (global_keys[GLFW_KEY_D])
		cameraPos[0] -= overall_msecs / 1000.0f;
	if (global_keys[GLFW_KEY_W])
		cameraPos[1] += overall_msecs / 1000.0f;
	else if (global_keys[GLFW_KEY_S])
		cameraPos[1] -= overall_msecs / 1000.0f;
	*/

}

int main(void) {
	srand(time(NULL));

	GLFWwindow* window;

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	//glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

    window = glfwCreateWindow(1920, 1080, "Combat2D", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

	glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCharCallback(window, character_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	//glfwSetWindowSizeCallback(window, size_callback);

	if (glewInit() != GLEW_OK) {
		printf("Failed to initialize GLEW");
		exit(EXIT_FAILURE);
	}

	renderer = new Render();
	renderer->Init(1920, 1080);

	texturer = new Texturer();
	texturer->LoadTexturesList();

	game = new Game();

	gui = new GUI(texturer, renderer);

	{
		int i;

		// clear
		for (i = 0; i < GLFW_KEY_LAST; i++)
			global_keys[i] = false;
		for (i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
			global_mouse[i] = false;
	}
	
	gettimeofday(&tick_otime, NULL);
	/*
	{
		Level *level;
		Pathfinder *pf;
		list<uint *> *foundPath;

		level = new Level(10, 5);
		pf = new Pathfinder(level);

		level->GetTile(0, 2)->SetFloor(1);
		level->GetTile(1, 2)->SetFloor(1);
		level->GetTile(2, 2)->SetFloor(1);
		level->GetTile(3, 2)->SetFloor(1);
		level->GetTile(4, 2)->SetFloor(1);
		level->GetTile(5, 2)->SetFloor(1);
		level->GetTile(6, 2)->SetFloor(1);
		level->GetTile(7, 2)->SetFloor(1);

		
		foundPath = pf->GetPath(1, 0, 3, 3, 2, -1);

		if (foundPath && foundPath->size() > 0) {
			list<uint *>::iterator foundPathI;
			uint *tile;

			printf("PATH!\n");
			foundPathI = foundPath->begin();
			while (foundPathI != foundPath->end()) {
				ushort x, y;

				tile = *foundPathI;

				pf->MakeCoords(*tile, &x, &y);
				printf("%i %i\n", x, y);

				foundPathI++;
			}
		} else {
			printf("No path\n");
		}
		

		ObjectPattern *op = new ObjectPattern(4, 2);
		op->SetPattern(0, 0, 1);
		op->SetPattern(1, 0, 0);
		op->SetPattern(2, 0, 0);
		op->SetPattern(3, 0, 1);
		op->SetPattern(0, 1, 1);
		op->SetPattern(1, 1, 1);
		op->SetPattern(2, 1, 0);
		op->SetPattern(3, 1, 0);

		Object *obj1 = new Object();
		Object *obj2 = new Object();
		Object *obj3 = new Object();
		Object *obj4 = new Object();

		obj1->SetPattern(op);
		
		obj2->SetPattern(op);
		obj2->SetAngle(1);
		
		obj3->SetPattern(op);
		obj3->SetAngle(2);
		
		obj4->SetPattern(op);
		obj4->SetAngle(3);

		byte x, y;
		byte sx, sy;
		
		obj1->GetSize(&sx, &sy);
		for (y = 0; y < sy; y++) {
			for (x = 0; x < sx; x++) {
				printf("%i", obj1->GetPatternCell(x, y));
			}
			printf("\n");
		}

		printf("---\n");
		obj2->GetSize(&sx, &sy);
		for (y = 0; y < sy; y++) {
			for (x = 0; x < sx; x++) {
				printf("%i", obj2->GetPatternCell(x, y));
			}
			printf("\n");
		}
		printf("---\n");
		obj3->GetSize(&sx, &sy);
		for (y = 0; y < sy; y++) {
			for (x = 0; x < sx; x++) {
				printf("%i", obj3->GetPatternCell(x, y));
			}
			printf("\n");
		}
		printf("---\n");
		obj4->GetSize(&sx, &sy);
		for (y = 0; y < sy; y++) {
			for (x = 0; x < sx; x++) {
				printf("%i", obj4->GetPatternCell(x, y));
			}
			printf("\n");
		}
	}
	*/

	{
		GUIElementWindow *wnd1;
		GUIElementText *txt1;
		GUIElementButton *btn1;
		GUIElementMultilineText *mt1;
		GUIElementEdit *ed1;
		GUIElementCheckbox *cb1;
		GUIElementRadio *rb1;

		wnd1 = new GUIElementWindow(texturer, renderer, L"mainMenu", 0, 0, 50, 50, GUIElement::GUIElementMeasureType_PercentSizeX | GUIElement::GUIElementMeasureType_PercentSizeY,
			GUIElement::GUIElementAlign_HorCenter | GUIElement::GUIElementAlign_VertCenter, 1, 0.8f, gui->GetRootElement());

		txt1 = new GUIElementText(texturer, renderer, L"mainMenuTitle", L"Main Menu", Render::FontSize_Huge, 0, 0, 0, 0, GUIElement::GUIElementMeasureType_ContentSizeX | GUIElement::GUIElementMeasureType_ContentSizeY,
			GUIElement::GUIElementAlign_HorCenter, 1, (GUIElement *)wnd1);

		txt1 = new GUIElementText(texturer, renderer, L"mainMenuEditLabel", L"Edit: ", Render::FontSize_Medium, 10, 12, 0, 0,
			GUIElement::GUIElementMeasureType_PercentPosX | GUIElement::GUIElementMeasureType_PercentPosY | GUIElement::GUIElementMeasureType_ContentSizeX | GUIElement::GUIElementMeasureType_ContentSizeY,
			0, 1, (GUIElement *)wnd1);
		ed1 = new GUIElementEdit(texturer, renderer, L"mainMenuEdit", L"", Render::FontSize_Medium, 20, 12, 50, 0,
			GUIElement::GUIElementMeasureType_PercentPosX | GUIElement::GUIElementMeasureType_PercentPosY | GUIElement::GUIElementMeasureType_PercentSizeX | GUIElement::GUIElementMeasureType_ContentSizeY,
			0, 1, (GUIElement *)wnd1);

		cb1 = new GUIElementCheckbox(texturer, renderer, L"mainMenuCheck", 0, L"Checkbox!", Render::FontSize_Medium, 10, 20, 0, 0,
			GUIElement::GUIElementMeasureType_PercentPosX | GUIElement::GUIElementMeasureType_PercentPosY | GUIElement::GUIElementMeasureType_ContentSizeX | GUIElement::GUIElementMeasureType_ContentSizeY,
			0, 1, (GUIElement *)wnd1);

		rb1 = new GUIElementRadio(texturer, renderer, L"mainMenuRadioGroup1", L"mainMenuRadio1-1", 0, L"Radio1", Render::FontSize_Medium, 50, 20, 0, 0,
			GUIElement::GUIElementMeasureType_PercentPosX | GUIElement::GUIElementMeasureType_PercentPosY | GUIElement::GUIElementMeasureType_ContentSizeX | GUIElement::GUIElementMeasureType_ContentSizeY,
			0, 1, (GUIElement *)wnd1);
		rb1 = new GUIElementRadio(texturer, renderer, L"mainMenuRadioGroup1", L"mainMenuRadio1-2", 0, L"Radio2", Render::FontSize_Medium, 50, 25, 0, 0,
			GUIElement::GUIElementMeasureType_PercentPosX | GUIElement::GUIElementMeasureType_PercentPosY | GUIElement::GUIElementMeasureType_ContentSizeX | GUIElement::GUIElementMeasureType_ContentSizeY,
			0, 1, (GUIElement *)wnd1);

		mt1 = new GUIElementMultilineText(texturer, renderer, L"mainMenuMultilineText",
			L"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\
			Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\
			Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\
			Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\
			Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."
			, Render::FontSize_Small, 10, 40, 80, 40,
			GUIElement::GUIElementMeasureType_PercentPosX | GUIElement::GUIElementMeasureType_PercentPosY | GUIElement::GUIElementMeasureType_PercentSizeX | GUIElement::GUIElementMeasureType_PercentSizeY,
			0, 1, (GUIElement *)wnd1);

		btn1 = new GUIElementButton(texturer, renderer, L"mainMenuExit", L"EXIT", Render::FontSize_Medium, 0, 0, 50, 12,
			GUIElement::GUIElementMeasureType_PercentSizeY | GUIElement::GUIElementMeasureType_PercentSizeX | GUIElement::GUIElementMeasureType_PercentPosY,
			GUIElement::GUIElementAlign_HorCenter | GUIElement::GUIElementAlign_Bottom, 1, (GUIElement *)wnd1);
		btn1->SetActionClick(&GameExitButtonClick);
	}

	gui->ResizeElements(gui->GetRootElement());

	tick_totalTime.tv_sec = 0;
	tick_totalTime.tv_usec = 0;
	while ( !glfwWindowShouldClose(window) && !game->GetExitGame()) {
		float ox, oy, oz;

		glfwPollEvents();

		gettimeofday(&tick_ntime, NULL);
		getdifftime(tick_ntime, tick_otime, &tick_diff);
		tick_msecs = tick_diff.tv_sec * 1000 + tick_diff.tv_usec / 1000.0f;
		tick_otime = tick_ntime;
		getaddtime(tick_totalTime, tick_diff, &tick_totalTime);
		renderer->SetTickMsecs(tick_msecs);
		renderer->SetTotalTime(tick_totalTime);

		doTick();

		texturer->Bind(0);
		renderer->Begin();

		renderer->SetTextureMode();

		renderer->ReadCoordsUnderCursor((int)global_mousepos[0], (int)global_mousepos[1], &ox, &oy, &oz);

		renderer->GetScreenWidth();
		
		renderer->Set2DMode();
		// GUI
		glDisable(GL_DEPTH_TEST);

		texturer->Bind(texturer->GetWhite());
		renderer->DrawRect(0.0f, (float)renderer->GetScreenHeight(), (float)renderer->GetScreenWidth(), (float)renderer->GetScreenHeight(), Color(1.0f, 1.0f, 0.0f, 1.0f));
		// TODO: combine functions into one?
		gui->SetHoverElement(gui->FindElementByCoords(gui->GetRootElement(), global_mousepos[0], global_mousepos[1]));
		gui->RenderElements(gui->GetRootElement(), global_mousepos[0], global_mousepos[1]);
		/*
		//renderer->DrawRect(100.0f, 668.0f, 100.0f, 32.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		//renderer->DrawBorder(100.0f, 668.0f, 100.0f, 32.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		//gui->DrawString(102.0f, 668.0f, 16, L"arial.ttf", L"����!");
		renderer->DrawRect(100.0f, 300.0f, 100.0f, 32.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		renderer->DrawBorder(100.0f, 300.0f, 100.0f, 32.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		gui->DrawString(102.0f, 300.0f, GUI::FontSize_Small, L"arial.ttf", L"����!");

		//renderer->DrawRect(300.0f, 668.0f, 100.0f, 32.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		//renderer->DrawBorder(300.0f, 668.0f, 100.0f, 32.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		//gui->DrawString(300.0f, 668.0f, 16, L"arial.ttf", L"FreeType!");
		renderer->DrawRect(300.0f, 100.0f, 100.0f, 32.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		renderer->DrawBorder(300.0f, 100.0f, 100.0f, 32.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		gui->DrawString(300.0f, 100.0f, GUI::FontSize_Small, L"arial.ttf", L"FreeType!");
		*/
		/*
		{
			wchar_t buf[1024];

			swprintf(buf, L"OBJ: %f %f %f (%i %i)", ox, oy, oz,(int) global_mousepos[0], (int)global_mousepos[1]);
			//gui->DrawString(0.0f, 768.0f, 16, L"arial.ttf", buf);
			//wsprintf(buf, L"OBJ: %i %i", (int)global_mousepos[0], (int)global_mousepos[1]);
			gui->DrawString(0.0f, 768.0f, GUI::FontSize_Small, L"arial.ttf", buf);

		}
		gui->DrawString(0.0f, 750.0f, GUI::FontSize_Tiny, L"arial.ttf", L"Test text (Tiny)");
		gui->DrawString(0.0f, 700.0f, GUI::FontSize_Small, L"arial.ttf", L"Test text (Small)");
		gui->DrawString(0.0f, 650.0f, GUI::FontSize_Medium, L"arial.ttf", L"Test text (Medium)");
		gui->DrawString(0.0f, 600.0f, GUI::FontSize_Large, L"arial.ttf", L"Test text (Large)");
		gui->DrawString(0.0f, 550.0f, GUI::FontSize_Huge, L"arial.ttf", L"Test text (Huge)");
		*/
		gui->DrawCursor(global_mousepos[0], global_mousepos[1], 0);

		renderer->End2DMode();
		renderer->End();

		glfwSwapBuffers(window);       
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
