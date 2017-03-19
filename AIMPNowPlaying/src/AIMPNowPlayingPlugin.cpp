
#include "AIMPNowPlaying.h"
#include "AIMPActionEvent.h"
#include <string>
#include <sstream>
#include <vcclr.h>

#include <comdef.h>

using namespace System;
using namespace System::Windows::Interop;
using namespace System::Reflection;

static const wchar_t PluginName[] = L"AIMP NowPlayingÅÙ";
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


		if (!String::IsNullOrEmpty(vm->AccessToken) && !String::IsNullOrEmpty(vm->AccessTokenSecret)) {

			msclr::interop::marshal_context context;

			//IAIMPCore *core = AIMPNowPlayingInstance->GetCore();

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

	void AIMPNowPlayingPlugin::Tweet(String ^text, String  ^jacketPath) {

		AIMPNowPlayingUI::ViewModels::MainWindowViewModel ^vm = static_cast<AIMPNowPlayingUI::ViewModels::MainWindowViewModel^>(ViewModelRoot.Target);

		if (vm->CanTweet) {

			vm->Tweet(text, jacketPath);
		} else {

			MessageBox(NULL, "êÊÇ…ê›íËâÊñ Ç≈îFèÿÇÇµÇƒÇ≠ÇæÇ≥Ç¢ÅB", "AIMP NowPlaying Plugin", MB_OK);
		}

	}

	void AIMPNowPlayingPlugin::UIInitialize() {

		AIMPNowPlayingUI::ViewModels::MainWindowViewModel ^vm = gcnew AIMPNowPlayingUI::ViewModels::MainWindowViewModel();

		ViewModelRoot = GCHandle::Alloc(vm);

		IAIMPString *str;


		if (SUCCEEDED(Config->GetValueAsString(CreateAIMPString(L"AIMPNowPlaying\\AccessToken"), &str))) {

			vm->AccessToken = gcnew String(str->GetData());
		}

		if (SUCCEEDED(Config->GetValueAsString(CreateAIMPString(L"AIMPNowPlaying\\AccessTokenSecret"), &str))) {

			vm->AccessTokenSecret = gcnew String(str->GetData());
			vm->CanTweet = true;
		}
	}

	HRESULT WINAPI AIMPNowPlayingPlugin::Initialize(IAIMPCore *core) {

		Core = core;

		AIMPNowPlayingUI::StartUp::Initialize();

		System::AppDomain::CurrentDomain->AssemblyResolve -= gcnew System::ResolveEventHandler(&OnAssemblyResolve);


		if (FAILED(core->QueryInterface(IID_IAIMPConfig, reinterpret_cast<void**>(&Config)))) {

			//MessageBox(NULL, "èâä˙âªÇ…é∏îsÇµÇ‹ÇµÇΩÅB IAIMPConfig", "AIMPNowPlaying Plugin", MB_OK);
			return E_FAIL;
		}

		UIInitialize();

		if (FAILED(core->QueryInterface(IID_IAIMPServiceMenuManager, reinterpret_cast<void**>(&MenuManager)))) {

			//MessageBox(NULL, "èâä˙âªÇ…é∏îsÇµÇ‹ÇµÇΩÅB IAIMPServiceMenuManager", "AIMPNowPlaying Plugin", MB_OK);
			return E_FAIL;
		}

		if (FAILED(core->QueryInterface(IID_IAIMPServiceActionManager, reinterpret_cast<void**>(&ActionManager)))) {

			//MessageBox(NULL, "èâä˙âªÇ…é∏îsÇµÇ‹ÇµÇΩÅB IAIMPServiceActionManager", "AIMPNowPlaying Plugin", MB_OK);
			return E_FAIL;
		}

		if (FAILED(core->QueryInterface(IID_IAIMPServiceAlbumArt, reinterpret_cast<void**>(&AlbumArt)))) {

			MessageBox(NULL, "èâä˙âªÇ…é∏îsÇµÇ‹ÇµÇΩÅB IAIMPServiceAlbumArt", "AIMPNowPlaying Plugin", MB_OK);
			return E_FAIL;
		}

		if (FAILED(core->QueryInterface(IID_IAIMPServicePlayer, reinterpret_cast<void**>(&Player)))) {

			MessageBox(NULL, "èâä˙âªÇ…é∏îsÇµÇ‹ÇµÇΩÅB IAIMPServicePlayer", "AIMPNowPlaying Plugin", MB_OK);
			return E_FAIL;
		}

		IAIMPMenuItem *playlistMenu;

		if (FAILED(MenuManager->GetBuiltIn(AIMP_MENUID_PLAYER_MAIN_FUNCTIONS, &playlistMenu))) {

			MessageBox(NULL, "èâä˙âªÇ…é∏îsÇµÇ‹ÇµÇΩÅB PlayListMenu", "AIMPNowPlaying Plugin", MB_OK);
			return E_FAIL;
		}

		IAIMPMenuItem *nowPlayingMenu;

		if (FAILED(core->CreateObject(IID_IAIMPMenuItem, reinterpret_cast<void**>(&nowPlayingMenu)))) {

			//MessageBox(NULL, "èâä˙âªÇ…é∏îsÇµÇ‹ÇµÇΩÅB NowPlayingMenu", "AIMPNowPlaying Plugin", MB_OK);
			return E_FAIL;
		}
		
		nowPlayingMenu->SetValueAsObject(AIMP_MENUITEM_PROPID_PARENT, playlistMenu);
		nowPlayingMenu->SetValueAsObject(AIMP_MENUITEM_PROPID_ID, CreateAIMPString(L"AIMPNowPlaying\\Menu"));
		nowPlayingMenu->SetValueAsObject(AIMP_MENUITEM_PROPID_NAME, CreateAIMPString(L"NowPlayingÅÙ"));


		nowPlayingMenu->SetValueAsObject(AIMP_MENUITEM_PROPID_EVENT, new AIMPActionEvent());

		nowPlayingMenu->SetValueAsInt32(AIMP_MENUITEM_PROPID_ENABLED, true);
		nowPlayingMenu->SetValueAsInt32(AIMP_MENUITEM_PROPID_STYLE, AIMP_MENUITEM_STYLE_NORMAL);
		nowPlayingMenu->SetValueAsInt32(AIMP_MENUITEM_PROPID_VISIBLE, true);
		


		core->RegisterExtension(IID_IAIMPServiceMenuManager, nowPlayingMenu);

		return S_OK;
	}

	HRESULT WINAPI AIMPNowPlayingPlugin::Finalize() {

		return S_OK;
	}



	void WINAPI AIMPNowPlayingPlugin::SystemNotification(int NotifyID, IUnknown* Data) {


	}
	void AIMPActionEvent::OnExecute(IUnknown * Data) {

		AIMPNowPlayingPlugin *Plugin = AIMPNowPlayingInstance;


		IAIMPFileInfo *info;

		String ^title;
		String ^artist;
		String ^album;


		if (FAILED(Plugin->GetPlayer()->GetInfo(&info))) {

			MessageBox(NULL, "èâä˙âªÇ…é∏îsÇµÇ‹ÇµÇΩÅB GetInfo", "AIMPNowPlaying Plugin", MB_OK);
			return;
		}

		IAIMPString *str;

		if (SUCCEEDED(info->GetValueAsObject(AIMP_FILEINFO_PROPID_TITLE, IID_IAIMPString, reinterpret_cast<void**>(&str)))) {

			title = gcnew String(str->GetData());
		}

		if (SUCCEEDED(info->GetValueAsObject(AIMP_FILEINFO_PROPID_ARTIST, IID_IAIMPString, reinterpret_cast<void**>(&str)))) {

			artist = gcnew String(str->GetData());
		}

		if (SUCCEEDED(info->GetValueAsObject(AIMP_FILEINFO_PROPID_ALBUM, IID_IAIMPString, reinterpret_cast<void**>(&str)))) {

			album = gcnew String(str->GetData());
		}

		IAIMPImage *image;

		if (SUCCEEDED(info->GetValueAsObject(AIMP_FILEINFO_PROPID_ALBUMART, IID_IAIMPImage, reinterpret_cast<void**>(&image)))) {

			wchar_t buf[256];

			GetTempPathW(256, buf);
			if (FAILED(image->SaveToFile(Plugin->CreateAIMPString(gcnew String(buf) +  "aimp.png"), AIMP_IMAGE_FORMAT_PNG))) {

				MessageBox(NULL, "ÇŸÇºÇÒÇ…ÇµÇ¡ÇœÇ¢", "", MB_OK);
				return;
			}

			Plugin->Tweet("#NowPlaying " + title + " by " + artist, gcnew String(buf) + "aimp.png");
		} else {

			Plugin->Tweet("#NowPlaying " + title + " by " + artist, nullptr);
		}
	}
}

