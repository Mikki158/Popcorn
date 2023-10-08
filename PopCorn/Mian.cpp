// PopCorn.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Main.h"

// AsFrame_DC
// 
AsFrame_DC::AsFrame_DC()
	:Weight(0), Height(0), DC(0), Bitmap(0)
{

}


//
AsFrame_DC::~AsFrame_DC()
{
	if (Bitmap != 0)
		DeleteObject(Bitmap);

	if (DC != 0)
		DeleteObject(DC);
}


//
HDC AsFrame_DC::Get_DC(HWND hwnd, HDC hdc)
{
	RECT rect;
	int dc_weight, dc_height;

	GetClientRect(hwnd, &rect);

	dc_weight = rect.right - rect.left;
	dc_height = rect.bottom - rect.top;

	if (dc_weight != Weight && dc_height != Height)
	{
		if (Bitmap != 0)
			DeleteObject(Bitmap);

		if (DC != 0)
			DeleteObject(DC);

		Weight = dc_weight;
		Height = dc_height;

		DC = CreateCompatibleDC(hdc);
		Bitmap = CreateCompatibleBitmap(hdc, Weight, Height);
		SelectObject(DC, Bitmap);

		rect.right++;
		rect.bottom++;

		AsTools::Rect(DC, rect, AsConfig::BG_Color);
	}

	return DC;
}

// Глобальные переменные:
AsMain_Window Main_Window;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	return Main_Window.Main(hInstance, nCmdShow);
}


// AsMain_Window
AsMain_Window* AsMain_Window::Self = 0;
// 
AsMain_Window::AsMain_Window()
	:Instance(0), szTitle{}, szWindowClass{}
{
	Self = this;
}


//
int APIENTRY AsMain_Window::Main(HINSTANCE instance, int command_show)
{
	MSG msg;
	HACCEL accel_table;

	Instance = instance; // Сохранить маркер экземпляра в глобальной переменной

	LoadStringW(Instance, IDS_APP_TITLE, szTitle, Max_String_Size);
	LoadStringW(Instance, IDC_POPCORN, szWindowClass, Max_String_Size);
	Register_Class();

	// Выполнить инициализацию приложения:
	if (!Init_Instance(command_show))
	{
		return FALSE;
	}

	accel_table = LoadAccelerators(Instance, MAKEINTRESOURCE(IDC_POPCORN));


	// Цикл основного сообщения:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, accel_table, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}


//
ATOM AsMain_Window::Register_Class()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Window_Proc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = Instance;
	wcex.hIcon = LoadIcon(Instance, MAKEINTRESOURCE(IDI_POPCORN));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = AsConfig::BG_Color.Get_Brush();
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_POPCORN);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(Instance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}


//
BOOL AsMain_Window::Init_Instance(int command_show)
{
	RECT window_rect;
	HWND hwnd;

	window_rect.left = 0;
	window_rect.top = 0;
	window_rect.right = 320 * 3;
	window_rect.bottom = 200 * 3;

	AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, TRUE);

	hwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		0, 0, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top, nullptr, nullptr, Instance, nullptr);

	Engine.Init_Engine(hwnd);

	if (hwnd == 0)
	{
		return FALSE;
	}

	ShowWindow(hwnd, command_show);
	UpdateWindow(hwnd);

	return TRUE;
}


//
void AsMain_Window::On_Paint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	HDC frame_dc;

	frame_dc = Frame_DC.Get_DC(hwnd, hdc);
	Engine.Draw_Frame(frame_dc, ps.rcPaint);
	BitBlt(hdc, 0, 0, Frame_DC.Weight, Frame_DC.Height, frame_dc, 0, 0, SRCCOPY);

	EndPaint(hwnd, &ps);
}


//
LRESULT CALLBACK AsMain_Window::Window_Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			DialogBox(Self->Instance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
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
		Self->On_Paint(hWnd);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			return Self->Engine.On_Key(EKey_Type::Left, true);

		case VK_RIGHT:
			return Self->Engine.On_Key(EKey_Type::Right, true);

		case VK_SPACE:
			return Self->Engine.On_Key(EKey_Type::Space, true);
		}
		break;

	case WM_KEYUP:
		switch (wParam)
		{
		case VK_LEFT:
			return Self->Engine.On_Key(EKey_Type::Left, false);

		case VK_RIGHT:
			return Self->Engine.On_Key(EKey_Type::Right, false);

		case VK_SPACE:
			return Self->Engine.On_Key(EKey_Type::Space, false);

		case VK_F2:
			return Self->Engine.On_Key(EKey_Type::F, false);
		}
		break;


	case WM_TIMER:
		if (wParam == Self->Engine.TIMER_ID)
		{
			return Self->Engine.On_Timer();
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
INT_PTR CALLBACK AsMain_Window::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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

Отрефакторить код

БАГИ:
При телепортации шарик сбрасывает полученное ускорение
При некоторых телепортациях в телепорте появляется черный круг
Когда шарик получает парашют, он сначала летит наверх
Платформа не растворяется при поражении
После разбиения многоразового кирпича остается 100

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
+ 1.4 М ("Монстры")
+ 1.5 Ж ("Жизнь")

2. Сложные:
+ 2.1 К ("Клей")
+ 2.1.1 Новое состояние платформы
+ 2.1.2 Анимация растворяющегося клея (прямая и обратная)
+ 2.1.3 Фиксация шарика (+ новое состояние шарика)
+ 2.1.4 Пуск шарика пробелом и по таймеру

+ 2.2 Ш ("Шире")
+ 2.2.1 Новое состояние платформы (+ изменение размера)
+ 2.2.2 Анимация расширяющейся платфомы (прямая и обратная)

+ 2.3 П ("Пол")
+ 2.3.1 Отрисовка пола
+ 2.3.2 Шкала пола
+ 2.3.3 Взаимодействие пола с мячом
+ 2.3.4 Отбитие мяча нижней гранью платформы

+ 2.4. Л ("Лазер")
+ 2.4.1 Новое состояние платформы
+ 2.4.2 Анимация превращения в пушку (прямая и обратная)
+ 2.4.3 Стрельба пробелом
+ 2.4.3.1 Обработка пробела особым образом, когда платформа в режиме лазера
+ 2.4.3.2 Объекты лазерных лучей - анимация, перемещение и взаимодействие с другими объектами

+ 2.5 Т ("Три")
+ 2.5.1 Добавить поддержку множества шариков
+ 2.5.2 Вывод множества шариков
+ 2.5.3 Взаимодействие множества мячиков с элементами игры

2.6 + (Переход на следующий уровень)
2.6.1 Анимация досрочного перехода на следующий уровень
2.6.2 Отдельный уровень перехода с особыми стенками (рамкой)


+ Движения мячика
+ 1. Отскок мячика от рамки
+ 2. Попадание в края платфомы
+ 3. Подсечка мяча платформой
+ 4. Попадание в монстра

Попадание в кирпич
+ 1. Попадание в кирпич - состояние кирпича
+ 2. Выбивание падующей буквы
+ 3. Перехват падующей буквы
4. Для каждой буквы - свое действие

+ Гейты
+ 1. Вывод гейтов
+ 2. Анимация открытия и закрытия гейтов
+ 3. Состояние гейтов (закрыт, открывается, закрывается)

Монстры
+ 1. Список монстров (глаз, черепаха, сатурн, голова, бубен, аквариум, зажигалка)
+ 2. для каждого монстра - сделать раскадровку
+ 3. Перемещение, выбор направления + состояние(живой/пауза/мертвый)
+ 4. Взаимодействие с мячом и платформой
+ 5. Выход из гейта
+ 6. Для каждого - анимация

+ Информационная панель
+ 1. Логотип
+ 2. Индикаторы
+ 2.1 Имя игрока
+ 2.2 Текущий счет
+ 3. Шкала монстров и пола
+ 4 Изменение шкал
+ 5. Окно дополнительных жизней
+ 6. Учет игровых действий и отображение на индикаторах

Игра и уровни
1. Список первых 10 уровней
2. Состояние игры 
2.1 заставка
2.2 анимация начала уровня
2.3 играем уровень
2.4 теряем жизнь
2.5 переход на следующий уровень
2.5.1 нормальный 
2.5.2 досрочный
2.6 окончание игры

3. Финальная настройка
3.1 Исправляем 1-й уровень 
3.2 Играем и исправляем ошибки
3.3 Компиляция и релиз

*/
