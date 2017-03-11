
#include "AIMPNowPlaying.h"
#include "aimp/apiOptions.h"
#include <string>
#include <vcclr.h>
#include <msclr/marshal.h>

#include <comdef.h>

using namespace System;
using namespace System::Windows;
using namespace System::Windows::Interop;
using namespace System::Reflection;

static const wchar_t PluginName[] = L"AIMP NowPlayingÅÙ";
static const wchar_t Author[] = L"mrtska";
static const wchar_t Description[] = L"AIMP Tweet Plugin";

System::Reflection::Assembly ^ OnAssemblyResolve(System::Object ^sender, System::ResolveEventArgs ^args);

namespace AIMPNowPlaying {

	extern AIMPNowPlayingPlugin *AIMPNowPlayingInstance;

	PWCHAR WINAPI AIMPNowPlayingPlugin::InfoGet(int Index) {

		switch (Index) {
		case AIMP_PLUGIN_INFO_NAME:
			return const_cast<PWCHAR>(PluginName);
		case AIMP_PLUGIN_INFO_AUTHOR:
			return const_cast<PWCHAR>(Author);
		case AIMP_PLUGIN_INFO_SHORT_DESCRIPTION:
			return const_cast<PWCHAR>(Description);
		}

		return L"";
	}

	DWORD WINAPI AIMPNowPlayingPlugin::InfoGetCategories() {

		return AIMP_PLUGIN_CATEGORY_ADDONS;
	}

	void WINAPI AIMPNowPlayingPlugin::Show(HWND ParentWindow) {


		AIMPNowPlayingUI::Views::MainWindow ^window = gcnew AIMPNowPlayingUI::Views::MainWindow();

		
		WindowInteropHelper ^helper = gcnew WindowInteropHelper(window);
		helper->Owner = IntPtr(ParentWindow);
		window->WindowStartupLocation = WindowStartupLocation::CenterOwner;

		window->Show();

	}


	HRESULT WINAPI AIMPNowPlayingPlugin::Initialize(IAIMPCore *core) {

		Core = core;

		AIMPNowPlayingUI::StartUp::Initialize();

		System::AppDomain::CurrentDomain->AssemblyResolve -= gcnew System::ResolveEventHandler(&OnAssemblyResolve);

		return S_OK;
	}
	HRESULT WINAPI AIMPNowPlayingPlugin::Finalize() {

		return S_OK;
	}



	void WINAPI AIMPNowPlayingPlugin::SystemNotification(int NotifyID, IUnknown* Data) {


	}
}

