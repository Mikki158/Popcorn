// PopCorn.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Main.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
AsEngine Engine;
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Разместите код здесь.


	// Инициализация глобальных строк
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_POPCORN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_POPCORN));

	MSG msg;

	// Цикл основного сообщения:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_POPCORN));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = AsConfig::BG_Color.Get_Brush();
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_POPCORN);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной


	RECT window_rect;
	window_rect.left = 0;
	window_rect.top = 0;
	window_rect.right = 320 * 3;
	window_rect.bottom = 200 * 3;

	AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, TRUE);

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		0, 0, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top, nullptr, nullptr, hInstance, nullptr);

	Engine.Init_Engine(hWnd);

	if (hWnd == 0)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Добавьте сюда любой код прорисовки, использующий HDC...

		Engine.Draw_Frame(hdc, ps.rcPaint);

		EndPaint(hWnd, &ps);
	}
	break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			return Engine.On_Key(EKT_Left, true);

		case VK_RIGHT:
			return Engine.On_Key(EKT_Right, true);

		case VK_SPACE:
			return Engine.On_Key(EKT_Space, true);
		}
		break;

	case WM_KEYUP:
		switch (wParam)
		{
		case VK_LEFT:
			return Engine.On_Key(EKT_Left, false);

		case VK_RIGHT:
			return Engine.On_Key(EKT_Right, false);

		case VK_SPACE:
			return Engine.On_Key(EKT_Space, false);

		case VK_F2:
			return Engine.On_Key(EKT_F, false);
		}
		break;


	case WM_TIMER:
		if (wParam == TIMER_ID)
		{
			return Engine.On_Timer();
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{

	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}

	return (INT_PTR)FALSE;
}

/*

+ Конец уровня при потере мяча
+ 1. Анимация пасплавления платформы
+ 2. Анимация выкатывания новой платформы
+ 3. Анимация расширающейся платформы
+ 4. Пуск мячика пробелом и состояние мяча

+ Кирпичи
+ 1. Обычные кирпичи(синие или красные)
+ 1.1 Могут быть с буквой или без
+ 1.2 При попадании в кирпич он исчезает из уровня сразу, и если кирпич был:
+ 1.2.1 Без буквы - создается активный кирпич
+ 1.2.2 С буквой - создается падующая буква

+ 2. Неразрушаемый кирпич
+ 2.1 Анимация при попадании

+ 3. Многоразовый кирпич
+ 3.1 4 состояния кирпича (1-4 удара до разрушения)
+ 3.2 Переход по состояниям
+ 3.3 Анимация приза при разрушении (кирпич исчезает, анимация - остается)

+ 4. Кирпич с парашютом
+ 4.1 Анимация парашюта - как вариант падающей буквы

+ 5. Кирпич телепортации
+ 5.1 Анимация при захвате и выпуске мяча

+ 6. Кирпич рекламы
+ 6.1 Анимация фрагмента "рекламы", скрывающейся за кирпичём


+ Взаимодействие особых кирпичей
+ 1. Падающая буква - взаимодействие только с платформой
+ 2. Активный кирпич - взаимодействует только с шариком


Действия букв
1. Простые:
1.1 О ("Отмена") — отмена действия символов К, Ш, П, Л и М
+ 1.2 И ("Инверсия")
+ 1.3 С ("Скорость")
1.4 М ("Монстры")
+ 1.5 Ж ("Жизнь")

2. Сложные:
2.1 К ("Клей")
2.1.1 Новое состояние платформы
2.1.2 Анимация растворяющегося клея (прямая и обратная)
2.1.3 Фиксация шарика (+ новое состояние шарика)
2.1.4 Пуск шарика пробелом и по таймеру

2.2 Ш ("Шире")
2.2.1 Новое состояние платформы (+ изменение размера)
2.2.2 Анимация расширяющейся платфомы (прямая и обратная)

2.3 П ("Пол")
2.3.1 Отрисовка пола
2.3.2 Шкала пола
2.3.3 Взаимодействие пола с мячом
2.3.4 Отбитие мяча нижней гранью платформы

2.4. Л ("Лазер")
2.4.1 Новое состояние платформы
2.4.2 Анимация превращения в пушку (прямая и обратная)
2.4.3 Стрельба пробелом
2.4.3.1 Обработка пробела особым образом, когда платформа в режиме лазера
2.4.3.2 Объекты лазерных лучей - анимация, перемещение и взаимодействие с другими объектами

+ 2.5 Т ("Три")
+ 2.5.1 Добавить поддержку множества шариков
+ 2.5.2 Вывод множества шариков
+ 2.5.3 Взаимодействие множества мячиков с элементами игры

2.6 + (Переход на следующий уровень)
2.6.1 Анимация досрочного перехода на следующий уровень
2.6.2 Отдельный уровень перехода с особыми стенками (рамкой)


Движения мячика
1. Отскок мячика от рамки
+ 2. Попадание в края платфомы
3. Подсечка мяча платформой
4. Попадание в монстра

Попадание в кирпич
1. Попадание в кирпич - состояние кирпича
2. Выбивание падующей буквы
+ 3. Перехват падующей буквы
4. Для каждой буквы - свое действие

Гейты
1. Вывод гейтов
2. Анимация открытия и закрытия гейтов
3. Состояние гейтов (закрыт, открывается, закрывается)

Монстры
1. Список монстров (глаз, черепаха, сатурн, голова, бубен, аквариум, зажигалка)
2. для каждого монстра - сделать раскадровку
3. Перемещение, выбор направления + состояние(живой/пауза/мертвый)
4. Взаимодействие с мячом и платформой
5. Выход из гейта
6. Для каждого - анимация

Информационная панель
1. Логотип
2. Индикаторы
2.1 Имя игрока
2.2 Текущий счет
3. Шкала монстров и пола
4 Изменение шкал
5. Окно дополнительных жизней
6. Учет игровых действий и отображение на индикаторах

Игра и уровни
1. Список первых 10 уровней
2. Состояние игры (заставка, анимация начала уровня, играем уровень, теряем жизнь, 
переход на следующий уровень(нормальный и досрочный), окончание игры)

Кирпичи
1. Для каждого - вывод изображения


*/
