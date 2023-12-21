#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#include "imgui/imgui_internal.h"
#include "definition.h"
#include "xor.h"
#include "../krypt/clicker/clicker.h"
#include "font.h"
#include "reach/reach.h"



// Data
static ID3D11Device* g_pd3dDevice = NULL;
static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
static IDXGISwapChain* g_pSwapChain = NULL;
static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

// Helper Function
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void set_position(int x, int y, int cx, int cy, HWND hwnd);
void get_mouse_offset(int& x, int& y, HWND hwnd);

// Vars
int tab = 1;
ImVec4 color;
ImVec4 clear_color = ImColor(47, 142, 48);


void style()
{
	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowTitleAlign = ImVec2(0.5, 0.5);
	style->WindowRounding = 5.0f;
	style->WindowBorderSize = 1.0f;
	style->Alpha = 1.0f;
	style->WindowPadding = ImVec2(10, 10);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 6.0f;
	style->ItemSpacing = ImVec2(5, 5);
	style->ItemInnerSpacing = ImVec2(5, 5);
	style->IndentSpacing = 10.0f;
	style->TouchExtraPadding = ImVec2(5, 5);
	style->ScrollbarSize = 13.0f;
	style->ScrollbarRounding = 15.0f;
	style->GrabMinSize = 15.0f;
	style->GrabRounding = 3.0f;
	style->ColumnsMinSpacing = 10.0f;
	style->ButtonTextAlign = ImVec2(0.5, 0.5);
	style->WindowTitleAlign = ImVec2(0.5, 0.5);
	style->ChildBorderSize = 5.5f;
	style->ChildRounding = 2.0f;
	ImVec4* dark = ImGui::GetStyle().Colors;
	dark[ImGuiCol_TitleBgActive] = ImColor(16, 16, 16);
	dark[ImGuiCol_Text] = clear_color;
	dark[ImGuiCol_WindowBg] = ImColor(10, 10, 10);
	dark[ImGuiCol_Border] = clear_color;
	dark[ImGuiCol_FrameBg] = ImColor(7, 7, 7, 150.0);
	dark[ImGuiCol_FrameBgHovered] = ImColor(8, 8, 8, 150.0);
	dark[ImGuiCol_FrameBgActive] = ImColor(8, 8, 8, 150.0);
	dark[ImGuiCol_Button] = ImColor(122, 129, 223, 100.0);
	dark[ImGuiCol_ButtonHovered] = ImColor(64, 255, 159, 88.8);
	dark[ImGuiCol_ButtonActive] = ImColor(0,0,0,100.0);
	dark[ImGuiCol_PlotHistogram] = ImColor(64, 255, 159, 200.0);
	dark[ImGuiCol_PlotHistogramHovered] = ImColor(64, 255, 159, 200.0);

	dark[ImGuiCol_CheckMark] = clear_color;
	dark[ImGuiCol_SliderGrab] = clear_color;
	dark[ImGuiCol_SliderGrabActive] = clear_color;
	
	dark[ImGuiCol_Separator] = ImColor(255, 255, 255, 120.0);
	dark[ImGuiCol_ChildBg] = ImColor(14, 14, 14);
}

void menu()
{

	//main threads for clicker/reach and checking if inv open
	CreateThread(nullptr, 4096, reinterpret_cast<LPTHREAD_START_ROUTINE>(c::hook), nullptr, 0, nullptr);
	CreateThread(nullptr, 4096, reinterpret_cast<LPTHREAD_START_ROUTINE>(c::left_thread), nullptr, 0, nullptr);
	CreateThread(nullptr, 4096, reinterpret_cast<LPTHREAD_START_ROUTINE>(c::right_thread), nullptr, 0, nullptr);
	CreateThread(nullptr, 4096, reinterpret_cast<LPTHREAD_START_ROUTINE>(c::inventory_thread), nullptr, 0, nullptr);

	//hides the console window *SWITCH HIDE TO SHOW FOR DEBUG*
	ShowWindow(GetConsoleWindow(), SW_HIDE);



	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, " ", NULL};
	::RegisterClassEx(&wc);
	HWND hwnd;
	hwnd = ::CreateWindow(wc.lpszClassName, _xor_(" "), WS_POPUP, 100, 100, 800, 500, NULL, NULL, wc.hInstance, NULL);
	if (!CreateDeviceD3D(hwnd))
	{
		CleanupDeviceD3D();
		::UnregisterClass(wc.lpszClassName, wc.hInstance);
		return;
	}
	::ShowWindow(hwnd, SW_SHOWDEFAULT);
	::UpdateWindow(hwnd);
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
	
	const auto main = io.Fonts->AddFontFromMemoryCompressedTTF(mainfont, mainfont_size, 12);

	// Main
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			continue;
		}
		style();
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		{
			static int x = 0, y = 0;
			ImGui::PushFont(main);
			ImGui::SetCursorPos(ImVec2(0, 20));
			ImGui::Begin("Krypt | Free Clicker | github.com/ignVegas | xveggie", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
			ImGui::SetWindowPos(ImVec2(0, 0));
			ImGui::SetWindowSize(ImVec2(800, 500));

			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				get_mouse_offset(x, y, hwnd);

			if (y >= 0 && y <= (ImGui::GetTextLineHeight() + ImGui::GetStyle().FramePadding.y * 4) && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
				set_position(x, y, 800, 500, hwnd);

			//Tab Bar
			ImGui::SetCursorPos(ImVec2(0, 27.5));
			ImGui::PushStyleColor(ImGuiCol_Border, clear_color);
			//ImGui::PushStyleColor(ImGuiCol_ButtonActive, clear_color);
			ImGui::BeginChild("TabBar", ImVec2(75, 0));
			{

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0.05, 0.05, 0.05, 1.f)));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(0, 0, 0)));
				//ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(ImColor(31, 32, 54)));
				{
					ImGui::SetCursorPos(ImVec2(7.5, 75));
					if (ImGui::Button("Clicker", ImVec2(65, 30)))
					{
						tab = 1;
					}
					ImGui::SetCursorPos(ImVec2(7.5, 110));
					if (ImGui::Button("Misc", ImVec2(65, 30)))
					{
						tab = 2;
					}

					ImGui::SetCursorPos(ImVec2(10, 440));
					ImGui::Text("krypt 2.0");

				}
			}
			ImGui::EndChild();

			// Name

			//simple tab system
			if (tab == 1)
			{
				//ImGui::Separator();
				ImGui::SetCursorPos(ImVec2(80, 27.5));
				ImGui::BeginChild("lmbclickertab", ImVec2(350, 462.5));
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255)));


					ImGui::SetCursorPos(ImVec2(5, 5));
					ImGui::Checkbox("Left Clicker", &v::left_toggle);
					ImGui::SameLine(); ImGui::KeyBindButton("  " , &v::left_bind, ImVec2(43,25));	

					ImGui::PushItemWidth(320);
					ImGui::SetCursorPos(ImVec2(5, 35));
					ImGui::SliderFloat("###min", &v::left_min, 5.f, v::fBlatant, "%.1f");
					if (ImGui::IsItemActive())
					{
						if (v::left_min > v::left_max)
						{
							v::left_max = v::left_min;
						}
					}
					ImGui::PushItemWidth(320);
					ImGui::SetCursorPos(ImVec2(5, 65));
					ImGui::SliderFloat("###max", &v::left_max, 5.f, v::fBlatant, "%.1f");
					if (ImGui::IsItemActive())
					{
						if (v::left_max < v::left_min)
						{
							v::left_min = v::left_max;
						}
					}


					ImGui::SetCursorPos(ImVec2(5, 95));
					ImGui::Checkbox("Blatant", &v::blatant);
					if (v::blatant)
					{
						v::fBlatant = 80.f;

					}
					else if (!v::blatant)
					{
						v::fBlatant = 20.f;

						if (v::left_min > 20.f || v::left_max > 20.f)
						{
							v::left_min = 20.f;
							v::left_max = 20.f;
						}

					}
					ImGui::SetCursorPos(ImVec2(5, 120));
					ImGui::Checkbox("Inventory", &v::inventory);
					ImGui::PopStyleColor(1);
				}
				ImGui::EndChild();
				ImGui::SetCursorPos(ImVec2(435, 27.5));
				ImGui::BeginChild("rmbclickertab", ImVec2(350, 462.5));
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255)));


					ImGui::SetCursorPos(ImVec2(5, 5));
					ImGui::Checkbox("Right Clicker", &v::right_toggle);

					ImGui::PushItemWidth(320);
					ImGui::SetCursorPos(ImVec2(5, 35));
					ImGui::SliderFloat("###min", &v::right_min, 5.f, 30.f, "%.1f");
					if (ImGui::IsItemActive())
					{
						if (v::right_min > v::right_max)
						{
							v::right_max = v::right_min;
						}
					}

					ImGui::PushItemWidth(320);
					ImGui::SetCursorPos(ImVec2(5, 65));
					ImGui::SliderFloat("###max", &v::right_max, 5.f, 30.f, "%.1f");
					if (ImGui::IsItemActive())
					{
						if (v::right_max < v::right_min)
						{
							v::right_min = v::right_max;
						}
					}

					ImGui::PopStyleColor(1);
				}
				ImGui::EndChild();
			}

			if (tab == 2)
			{
				ImGui::SetCursorPos(ImVec2(80, 27.5));
				ImGui::BeginChild("misctab", ImVec2(340, 462.5));
				{
					ImGui::SetCursorPos(ImVec2(5, 5));
					ImGui::ColorPicker4("##picker", (float*)&clear_color, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
				}
				ImGui::EndChild();

				ImGui::SetCursorPos(ImVec2(435, 27.5));
				ImGui::BeginChild("Config", ImVec2(350, 462.5));
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255)));


					ImGui::SetCursorPos(ImVec2(5, 5));
					ImGui::Checkbox("Custom Randomization", &v::customRandomization);

					ImGui::PushItemWidth(320);
					ImGui::SetCursorPos(ImVec2(5, 35));
					ImGui::SliderFloat("###min", &v::customFloatMin, 0.1f, 2.f, "%.2f");
					if (ImGui::IsItemActive())
					{
						if (v::customFloatMin > v::customFloatMax)
						{
							v::customFloatMax = v::customFloatMin;
						}
					}

					ImGui::PushItemWidth(320);
					ImGui::SetCursorPos(ImVec2(5, 65));
					ImGui::SliderFloat("###max", &v::customFloatMax, 0.1f, 2.f, "%.2f");
					if (ImGui::IsItemActive())
					{
						if (v::customFloatMax < v::customFloatMin)
						{
							v::customFloatMin = v::customFloatMax;
						}
					}

					ImGui::PopStyleColor(1);
				}
				ImGui::EndChild();
			}
			ImGui::End();

		}
		ImGui::Render();
		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
		g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, (float*)&clear_color);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		static UINT presentFlags = 0;
		if (g_pSwapChain->Present(1, presentFlags) == DXGI_STATUS_OCCLUDED) {
			presentFlags = DXGI_PRESENT_TEST;
			Sleep(20);
		}
		else {
			presentFlags = 0;
		}
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();
	::DestroyWindow(hwnd);
	::UnregisterClass(wc.lpszClassName, wc.hInstance);

	exit(0);
}

int main()
{
	SetThreadPriority(std::this_thread::get_id, THREAD_PRIORITY_TIME_CRITICAL);
	SetConsoleTitleA(" ");
	std::cout << "Welcome To Krypt!" << std::endl;
	Sleep(1000);
	menu();

}

void set_position(int x, int y, int cx, int cy, HWND hwnd)
{
	POINT point; GetCursorPos(&point);

	auto flags = SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE;
	if (x != 0 && y != 0)
	{
		x = point.x - x;
		y = point.y - y;
		flags &= ~SWP_NOMOVE;
	}

	if (cx != 0 && cy != 0)
		flags &= ~SWP_NOSIZE;

	SetWindowPos(hwnd, nullptr, x, y, cx, cy, flags);
}

void get_mouse_offset(int& x, int& y, HWND hwnd)
{
	POINT point; RECT rect;

	GetCursorPos(&point);
	GetWindowRect(hwnd, &rect);

	x = point.x - rect.left;
	y = point.y - rect.top;
}

bool CreateDeviceD3D(HWND hWnd)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 30;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
		return false;

	CreateRenderTarget();
	return true;
}

void CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
	pBackBuffer->Release();
}

void CleanupRenderTarget()
{
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			CleanupRenderTarget();
			g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			CreateRenderTarget();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
