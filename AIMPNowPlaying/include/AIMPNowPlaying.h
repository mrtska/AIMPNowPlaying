#pragma once


#include <Windows.h>

#include "IUnknownHelper.h"

#include "aimp/apiPlugin.h"

using namespace System::Runtime::InteropServices;


namespace AIMPNowPlaying {

	class AIMPNowPlayingPlugin : public IUnknownInterface<IAIMPPlugin>, public IAIMPExternalSettingsDialog {

	private:

		IAIMPCore *Core;

		IAIMPConfig *Config;

		GCHandle ViewModelRoot;
		

	public:

		typedef IUnknownInterface<IAIMPPlugin> base;


		// Information about the plugin
		PWCHAR WINAPI InfoGet(int Index);
		DWORD WINAPI InfoGetCategories();
		// Initialization / Finalization
		void UIInitialize();

		HRESULT WINAPI Initialize(IAIMPCore* Core);
		HRESULT WINAPI Finalize();
		// System Notifications
		void WINAPI SystemNotification(int NotifyID, IUnknown* Data);
		virtual void WINAPI Show(HWND ParentWindow);

		virtual unsigned long WINAPI AddRef(void) {

			return base::AddRef();
		}

		virtual unsigned long WINAPI Release(void) {

			return base::Release();
		}

		AIMPNowPlayingPlugin() {

			AddRef();
		}

		virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppvObj) {

			if (!ppvObj) return S_OK;

			if (riid == IID_IAIMPExternalSettingsDialog) {

				*ppvObj = static_cast<IAIMPExternalSettingsDialog*>(this);
				AddRef();
				return S_OK;
			}

			return E_NOINTERFACE;
		}

		IAIMPString *CreateAIMPString(wchar_t *str) {

			IAIMPString *aimpStr;

			if (Core) {

				if (SUCCEEDED(Core->CreateObject(IID_IAIMPString, reinterpret_cast<void**>(&aimpStr)))) {

					aimpStr->SetData(str, wcslen(str));
					return aimpStr;
				}
			}

			
			return nullptr;
		}

		IAIMPCore *GetCore() {

			return Core;
		}

		IAIMPConfig *GetConfig() {

			return Config;
		}

	};
}







