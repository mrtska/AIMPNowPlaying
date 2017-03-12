
#include "AIMPNowPlaying.h"
#include "aimp/apiOptions.h"
#include "aimp/apiPlaylists.h"
#include <string>
#include <vcclr.h>
#include <msclr/marshal.h>

#include <comdef.h>

using namespace System;
using namespace System::Windows::Interop;
using namespace System::Reflection;

static const wchar_t PluginName[] = L"AIMP NowPlaying♪";
static const wchar_t Author[] = L"mrtska";
static const wchar_t Description[] = L"AIMP Tweet Plugin";

System::Reflection::Assembly ^ OnAssemblyResolve(System::Object ^sender, System::ResolveEventArgs ^args);

extern AIMPNowPlaying::AIMPNowPlayingPlugin *AIMPNowPlayingInstance;

namespace AIMPNowPlaying {


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


	void OnClosing(System::Object ^sender, System::ComponentModel::CancelEventArgs ^e) {

		AIMPNowPlayingUI::Views::MainWindow ^window = static_cast<AIMPNowPlayingUI::Views::MainWindow^>(sender);
		AIMPNowPlayingUI::ViewModels::MainWindowViewModel ^vm = static_cast<AIMPNowPlayingUI::ViewModels::MainWindowViewModel^>(window->DataContext);

		MessageBox(NULL, "設定を保存する前", "AIMPNowPlaying Plugin", MB_OK);

		if (!String::IsNullOrEmpty(vm->AccessToken) && !String::IsNullOrEmpty(vm->AccessTokenSecret)) {

			msclr::interop::marshal_context context;

			//IAIMPCore *core = AIMPNowPlayingInstance->GetCore();

			MessageBox(NULL, "設定が保存されました。", "AIMPNowPlaying Plugin", MB_OK);
			
			AIMPNowPlayingInstance->GetConfig()->SetValueAsString(AIMPNowPlayingInstance->CreateAIMPString(L"AIMPNowPlaying\\AccessToken"), AIMPNowPlayingInstance->CreateAIMPString(const_cast<wchar_t*>(context.marshal_as<const wchar_t*>(vm->AccessToken))));
			AIMPNowPlayingInstance->GetConfig()->SetValueAsString(AIMPNowPlayingInstance->CreateAIMPString(L"AIMPNowPlaying\\AccessTokenSecret"), AIMPNowPlayingInstance->CreateAIMPString(const_cast<wchar_t*>(context.marshal_as<const wchar_t*>(vm->AccessTokenSecret))));
		}

	}

	void WINAPI AIMPNowPlayingPlugin::Show(HWND ParentWindow) {


		AIMPNowPlayingUI::Views::MainWindow ^window = gcnew AIMPNowPlayingUI::Views::MainWindow();

		window->DataContext = ViewModelRoot.Target;
		WindowInteropHelper ^helper = gcnew WindowInteropHelper(window);
		helper->Owner = IntPtr(ParentWindow);
		window->WindowStartupLocation = System::Windows::WindowStartupLocation::CenterOwner;

		window->Closing += gcnew System::ComponentModel::CancelEventHandler(&OnClosing);

		window->Show();

	}

	void AIMPNowPlayingPlugin::UIInitialize() {

		AIMPNowPlayingUI::ViewModels::MainWindowViewModel ^vm = gcnew AIMPNowPlayingUI::ViewModels::MainWindowViewModel();

		ViewModelRoot = GCHandle::Alloc(vm);

		IAIMPString *str;

		MessageBox(NULL, "設定を読み込む前", "AIMPNowPlaying Plugin", MB_OK);

		if (SUCCEEDED(Config->GetValueAsString(CreateAIMPString(L"AIMPNowPlaying\\AccessToken"), &str))) {

			MessageBox(NULL, "設定読み込み Token", "AIMPNowPlaying Plugin", MB_OK);
			vm->AccessToken = gcnew String(str->GetData());
		}

		if (SUCCEEDED(Config->GetValueAsString(CreateAIMPString(L"AIMPNowPlaying\\AccessTokenSecret"), &str))) {

			MessageBox(NULL, "設定読み込み Secret", "AIMPNowPlaying Plugin", MB_OK);
			vm->AccessTokenSecret = gcnew String(str->GetData());
			vm->CanTweet = true;
		}


	}

	HRESULT WINAPI AIMPNowPlayingPlugin::Initialize(IAIMPCore *core) {

		Core = core;

		AIMPNowPlayingUI::StartUp::Initialize();

		System::AppDomain::CurrentDomain->AssemblyResolve -= gcnew System::ResolveEventHandler(&OnAssemblyResolve);


		if (FAILED(core->QueryInterface(IID_IAIMPConfig, reinterpret_cast<void**>(&Config)))) {

			MessageBox(NULL, "初期化に失敗しました。", "AIMPNowPlaying Plugin", MB_OK);
			return E_FAIL;
		}

		UIInitialize();
		




		return S_OK;
	}

	HRESULT WINAPI AIMPNowPlayingPlugin::Finalize() {

		return S_OK;
	}



	void WINAPI AIMPNowPlayingPlugin::SystemNotification(int NotifyID, IUnknown* Data) {


	}
}

