#include <bangtal.h>
#include <iostream>
#include <ctime>

using namespace bangtal;
using namespace std;


//객체 생성
class Board
{
private:
	ObjectPtr front;
	ObjectPtr back;
public:
	void getPuzzle(string filenameF, string filenameB, ScenePtr scene, int i) {
		front = Object::create(filenameF, scene, 400 + (i % 3) * 160, 500 - (i / 3) * 160);
		back = Object::create(filenameB, scene, 400 + (i % 3) * 160, 500 - (i / 3) * 160, false);
	}
	ObjectPtr getFront() {
		return front;
	}
	ObjectPtr getBack() {
		return back;
	}
	ObjectPtr& setFront() { return front; };
	ObjectPtr& setBack() { return back; };
};


int main() {
	//랜덤 초기화
	srand((unsigned int)time(NULL));
	time_t start_time;
	auto timer = Timer::create(0.1f);

	// 불필요한 부분 삭제
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	//장면은 두개로 진행합니다.
	ScenePtr scene1 = Scene::create("퍼즐 맞추기", "images/scene.png");
	ScenePtr scene2 = Scene::create("퍼즐 맞추기", "images/scene.png");

	//INTRO SCREEN - scene1
	auto paint1 = Object::create("images/full_a.png", scene1, 100, 200);
	auto paint2 = Object::create("images/full_b.png", scene1, 700, 200);
	auto logo = Object::create("images/logo.png", scene1, 250, 50);
	auto start = Object::create("images/start.png", scene1, 800, 50);

	start->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		scene2->enter();
		timer->start();
		start_time = time(NULL);
		return true;
		});

	// Start - scene2
	auto logo2 = Object::create("images/logo.png", scene2, 250, 50);
	auto end = Object::create("images/end.png", scene2, 800, 50, false);
	end->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		endGame();
		return true;
		});
	ObjectPtr init_board[9];
	int blank = 8;
	Board board[9];

	for (int i = 0; i < 9; i++) {		string filenameA = "images/" + to_string(i + 1) + "a.png";
		string filenameB = "images/" + to_string(i + 1) + "b.png";
		board[i].getPuzzle(filenameA, filenameB, scene2, i);

		init_board[i] = board[i].getFront();
		/// 앞면일때
			board[i].getFront()->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
				int j;
				for (j = 0; j < 9; j++) {
					if (board[j].getFront() == object)
						break;
				}

				if ((j % 3 > 0 && j - 1 == blank) ||
					(j % 3 < 2 && j + 1 == blank) ||
					(j < 6 && j + 3 == blank) ||
					(j > 2 && j - 3 == blank)) {
					board[j].getFront()->locate(scene2, 400 + (blank % 3) * 160, 500 - (blank / 3) * 160);
					board[blank].getFront()->locate(scene2, 400 + (j % 3) * 160, 500 - (j / 3) * 160);
					board[j].getBack()->locate(scene2, 400 + (blank % 3) * 160, 500 - (blank / 3) * 160);
					board[blank].getBack()->locate(scene2, 400 + (j % 3) * 160, 500 - (j / 3) * 160);


					board[j].setFront() = board[blank].getFront();
					board[blank].setFront() = object;
					board[j].setBack() = board[blank].getBack();
					board[blank].setBack() = object;

					blank = j;

					for (int x = 0; x < 9; x++) {
							board[x].getFront()->hide();
							board[x].getBack()->show();
					}

					board[j].getBack()->hide();

				

					int k;
					for (k = 0; k < 9; k++) {
						if (board[k].getFront() != init_board[k]) break;
					}
					if (k == 9) {
						auto end_time = time(NULL);
						string buf = to_string((int)difftime(end_time, start_time)) + "초 걸렸습니다! 축하드립니다.";
						showMessage(buf.c_str());
						end->show();
					}
				}
				
				return true;
				});
		/// 뒷면일때
		board[i].getBack()->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
			int j;
			for (j = 0; j < 9; j++) {
				if (board[j].getBack() == object)
					break;
			}

			if ((j % 3 > 0 && j - 1 == blank) ||
				(j % 3 < 2 && j + 1 == blank) ||
				(j < 6 && j + 3 == blank) ||
				(j > 2 && j - 3 == blank)) {
				board[j].getBack()->locate(scene2, 400 + (blank % 3) * 160, 500 - (blank / 3) * 160);
				board[blank].getBack()->locate(scene2, 400 + (j % 3) * 160, 500 - (j / 3) * 160);
				board[j].getFront()->locate(scene2, 400 + (blank % 3) * 160, 500 - (blank / 3) * 160);
				board[blank].getFront()->locate(scene2, 400 + (j % 3) * 160, 500 - (j / 3) * 160);


				board[j].setBack() = board[blank].getBack();
				board[blank].setBack() = object;
				board[j].setFront() = board[blank].getFront();
				board[blank].setFront() = object;

				blank = j;


				for (int x = 0; x < 9; x++) {
						board[x].getBack()->hide();
						board[x].getFront()->show();
				}
				board[j].getFront()->hide();


				int k;
				for (k = 0; k < 9; k++) {
					if (board[k].getBack() != init_board[k]) break;
				}
				if (k == 9) {
					auto end_time = time(NULL);
					string buf = to_string((int)difftime(end_time, start_time)) + "초 걸렸습니다! 축하드립니다.";
					showMessage(buf.c_str());
					end->show();
				}
			}
			
			return true;
			});		
	}
	board[blank].getFront()->hide();
	board[blank].getBack()->hide();

	auto count = 0;
	timer->setOnTimerCallback([&](TimerPtr t)->bool {
		//Random하게 blank 움직이기
		int j = 0;
			switch (rand() % 4) {
			case 0:j = blank - 1; break;
			case 1:j = blank + 1; break;
			case 2:j = blank - 3; break;
			case 3:j = blank + 3; break;
			}
			if ((j>0&&j<9)&&((j % 3 > 0 && j - 1 == blank) ||
				(j % 3 < 2 && j + 1 == blank) ||
				(j < 6 && j + 3 == blank) ||
				(j > 2 && j - 3 == blank)))
			{
				board[j].getFront()->locate(scene2, 400 + (blank % 3) * 160, 500 - (blank / 3) * 160);
				board[blank].getFront()->locate(scene2, 400 + (j % 3) * 160, 500 - (j / 3) * 160);
				board[j].getBack()->locate(scene2, 400 + (blank % 3) * 160, 500 - (blank / 3) * 160);
				board[blank].getBack()->locate(scene2, 400 + (j % 3) * 160, 500 - (j / 3) * 160);

				//앞면
				auto tmp= board[j].getFront();
				board[j].setFront() = board[blank].getFront();
				board[blank].setFront() = tmp;
				// 뒷면
				tmp = board[j].getBack();
				board[j].setBack() = board[blank].getBack();
				board[blank].setBack() = tmp;
				blank = j;
				count++;
			}

		if (count < 2) {
			t->set(0.01f);
			t->start();
		}
		return true;
	});

	//게임 시작
	startGame(scene1);
	return 0;
}