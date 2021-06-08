#include "Form.h"

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR nCmdLine, int nCmdShow)
{
    LPCTSTR windowClass = TEXT("WinApp");
    LPCTSTR windowTitle = TEXT("Interpolator");
    WNDCLASSEX wcex;
    HWND hWnd;

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;

    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    wcex.cbClsExtra = 0;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.cbWndExtra = 0;
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hInstance = hInstance;
    wcex.lpfnWndProc = WndProc;
    wcex.lpszClassName = windowClass;
    wcex.lpszMenuName = NULL;
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL, TEXT("RegisterClassEx Failed!"), TEXT("Error"),
            MB_ICONERROR);
        return EXIT_FAILURE;
    }

    if (!(hWnd = CreateWindow(windowClass, windowTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, NULL, NULL, hInstance, NULL)))
    {
        MessageBox(NULL, TEXT("CreateWindow Failed!"), TEXT("Error"), MB_ICONERROR);
        return EXIT_FAILURE;
    }

    InitInterface(hWnd);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);

    return EXIT_SUCCESS;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(EXIT_SUCCESS);
        return 0;
    case WM_COMMAND:
        if (LOWORD(wParam) == SEARCH_BTN_ID)
            SearchButtonClick(hWnd);
        else if (LOWORD(wParam) == DRAW_BTN_ID)
            DrawButtonClick(hWnd);
        else if (LOWORD(wParam) == INCDIM_BTN_ID)
            IncBtnClick(hWnd);
        else if(LOWORD(wParam) == DECDIM_BTN_ID)
            DecBtnClick(hWnd);
        return 0;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return FALSE;
}

void InitInterface(HWND hWnd) {

    HWND hWndEdit = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(""),
        WS_CHILD | WS_VISIBLE, EditBX, EditBY, EditBW,
        EditBH, hWnd, (HMENU)EDIT_ID, NULL, NULL);

    HWND hWndDisplay = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT("Press calculate to get equation"),
        WS_CHILD | WS_VISIBLE | ES_READONLY | ES_AUTOVSCROLL | ES_MULTILINE, DisplayBX, DisplayBY, DisplayBW,
        DisplayBH, hWnd, (HMENU)DISPLAY_ID, NULL, NULL);
    if (!hWndDisplay)
        ErrorExit(L"CreateWindowEx");

    HWND hwndButton = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        SearchBText,      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        SearchBX,         // x position 
        SearchBY,         // y position 
        SearchBW,        // Button width
        SearchBH,        // Button height
        hWnd,     // Parent window
        (HMENU)SEARCH_BTN_ID,       // Control Id
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.
    HWND hwndButtonDraw = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        DrawBtnText,      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        DrawButtonX,         // x position 
        DrawButtonY,         // y position 
        DrawButtonW,        // Button width
        DrawButtonH,        // Button height
        hWnd,     // Parent window
        (HMENU)DRAW_BTN_ID,       // Control Id
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.

    HWND IncDim = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        IncDimBtnText,      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        IncDimX,         // x position 
        IncDimY,         // y position 
        IncDimW,        // Button width
        IncDimH,        // Button height
        hWnd,     // Parent window
        (HMENU)INCDIM_BTN_ID,       // Control Id
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.

    HWND DecDim = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        DecDimBtnText,      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        DecDimX,         // x position 
        DecDimY,         // y position 
        DecDimW,        // Button width
        DecDimH,        // Button height
        hWnd,     // Parent window
        (HMENU)DECDIM_BTN_ID,       // Control Id
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.

}

VOID DecBtnClick(HWND hWind) {
    HDC          hdc;
    hdc = GetDC(hWind);
    stepDim -= stepDim*(1.0/3);

    if (n > 0 && n < 1000)
        GraphicDraw(hdc, n,number);
    ReleaseDC(hWind, hdc);
}

VOID IncBtnClick(HWND hWind) {
    HDC          hdc;
    hdc = GetDC(hWind);
    stepDim += stepDim * (1.0/2);

    if (n > 0 && n < 1000)
        GraphicDraw(hdc, n,number);
    ReleaseDC(hWind, hdc);
}


VOID DrawButtonClick(HWND hWind) {
    LPWSTR lpEditBoxText;
    char* temp;
    HDC          hdc;
    hdc = GetDC(hWind);

    HWND hWndEdit;
    if (!(hWndEdit = GetDlgItem(hWind, EDIT_ID)))
        MessageBox(0, TEXT("Dig Item Error"), TEXT(""), 0);

    lpEditBoxText = new wchar_t[EditBSize];
    temp = new char[EditBSize];

    if (!GetWindowText(hWndEdit, lpEditBoxText, EditBSize)) {
        //MessageBox(0, TEXT("Edit box read error"), TEXT(""), 0);
        return;
    }

    size_t* PtNumOfCharConverted = NULL;
    wcstombs_s(PtNumOfCharConverted, temp, EditBSize, lpEditBoxText, EditBSize);
    int h = 1, x0 = 0, i = 0;
    n = atof(temp, i);
    number = atof(temp, i);

    delete[] lpEditBoxText;
    delete[] temp;
    GraphicDraw(hdc, n,number);
    ReleaseDC(hWind, hdc);
}

void SearchButtonClick(HWND hWind) {
    LPWSTR lpEditBoxText;
    char* temp;

    HWND hWndEdit;
    HWND hWndDisplay;

    if (!(hWndEdit = GetDlgItem(hWind, EDIT_ID)))
        MessageBox(0, TEXT("Dig Item Error"), TEXT(""), 0);

    if (!(hWndDisplay = GetDlgItem(hWind, DISPLAY_ID)))
        MessageBox(0, TEXT("Dig Item Error"), TEXT(""), 0);

    lpEditBoxText = new wchar_t[EditBSize];
    temp = new char[EditBSize];

    if (!GetWindowText(hWndEdit, lpEditBoxText, EditBSize)) {
        //MessageBox(0, TEXT("Edit box read error"), TEXT(""), 0);
        return;
    }

    size_t* PtNumOfCharConverted = NULL;
    wcstombs_s(PtNumOfCharConverted, temp, EditBSize, lpEditBoxText, EditBSize);
    int h = 1, x0 = 0, i = 0;
    n = atof(temp, i);
    number = atof(temp, i);


    LPTSTR polin;
    if ((polin = Interpolation::BuildPolinom(n, h, x0,number)) == NULL) {
        MessageBox(0, TEXT("BuildPolinom returned null array"), TEXT("Error"), 0);
        return;
    }
    FILE* fLog;
    fLog = _wfopen(L"out.txt", L"w");
    if (fLog != NULL)
    {
        fwrite(polin, sizeof(WCHAR), wcslen(polin), fLog);
        fclose(fLog);
    }

    SetWindowText(hWndDisplay,polin);
    free(polin);
}

VOID OnPaint(HDC hdc)
{
    Graphics graphics(hdc);
    Pen      pen(Color(255, 0, 0, 0));
    pen.SetWidth(5);
    graphics.DrawLine(&pen, 0, 0, 200, 200);
}

VOID GraphicDraw(HDC hdc, int n, int number)
{
    Graphics graphics(hdc);
    HBRUSH brsh = CreateSolidBrush(0x00000000);
    Pen      pen(Color(255, 0, 0, 0));
    pen.SetWidth(5);
    PointF      pointF(GraphicX, GraphicY);

    Image* graphBackground = Image::FromFile(TEXT(FILE_NAME));

    graphics.DrawImage(graphBackground, pointF);
    const float X = GraphicX + OxYx;
    const float Y = GraphicY + OxYy;

    DrawAxisVals(hdc);

    for (double x = 0; x < stepDim * OxStepCount; x += stepDim / 100) {
        float y = Interpolation::GetValue(n, 1, 0, x, number);
        //float y = (x - 1) * (x - 1) - Interpolation::sine(2 * x);
        RECT cor = { X + (x * Ostep) / stepDim,Y - (y * Ostep) / stepDim,X + (x * Ostep) / stepDim + 2,Y - (y * Ostep) / stepDim + 2 };
        if (cor.left > 440 && cor.left < 840 && cor.top > 75 && cor.top < (330 + 75))
            FillRect(hdc, &cor, brsh);
    }

}

VOID GDIWrite(HDC hdc, double x, double y, wchar_t* str, int FontSize )
{
    Graphics    graphics(hdc);
    SolidBrush  brush(Color(255, 0, 0, 0));
    FontFamily  fontFamily(L"Times New Roman");
    Font        font(&fontFamily, FontSize, FontStyleRegular, UnitPixel);
    PointF      pointF(x, y);

    graphics.DrawString(str, -1, &font, pointF, &brush);
}

void ErrorExit(LPCTSTR lpszFunction)
{
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);

    // Display the error message and exit the process

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf,
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"),
        lpszFunction, dw, lpMsgBuf);
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(dw);
}

LPTSTR DoublecutZeros(double val) {
    std::stringstream ss;
    ss << val;
    std::string sDenom = ss.str();
    ss.str("");
    sDenom = sDenom.substr(0, sDenom.find_last_not_of('0') + 1);
    ss << sDenom;
    
    ss.seekg(0, std::ios::end);
    int size = ss.tellg();
    char* temp = (char*)malloc((size + 1) * sizeof(char));
    LPTSTR strOut = (wchar_t*)malloc((size + 1) * sizeof(wchar_t));
    size_t* PtNumOfCharConverted = NULL;
    auto str = ss.str();
    std::copy(str.begin(), str.end(), temp);
    mbstowcs_s(PtNumOfCharConverted, strOut, size + 1, temp, size);
    free(temp);

    return strOut;
}

VOID DrawAxisVals(HDC hdc) {
    double cx = OYx;
    double cy = OXy;
    LPTSTR dVal;
    for (int stepY = 0; stepY <= OyStepCount; stepY++) {
        double temp = -(8 - stepY) * stepDim;
        dVal = DoublecutZeros(temp);
        GDIWrite(hdc, GraphicX + OYx, GraphicY + OXy - (Ostep * stepY) - 7, dVal, 10);
        free(dVal);
    }
    for (int stepX = 0; stepX <= OxStepCount; stepX++) {
        double temp = stepX * stepDim;
        dVal = DoublecutZeros(temp);
        GDIWrite(hdc, GraphicX + OYx + stepX * Ostep - 7, GraphicY + OxYy + 5, dVal, 10);
        free(dVal);
    }
}