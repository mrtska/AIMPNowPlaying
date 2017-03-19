#pragma once


#include "aimp/apiActions.h"


namespace AIMPNowPlaying {

	class  AIMPActionEvent : public IUnknownInterface<IAIMPActionEvent> {

	public:
		virtual void WINAPI OnExecute(IUnknown *Data);

		virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppv_obj) {

			if (!ppv_obj) {

				return E_POINTER;
			}

			if (riid == IID_IAIMPActionEvent) {

				*ppv_obj = static_cast<IAIMPActionEvent*>(this);
				AddRef();

				return S_OK;
			}
			return E_NOINTERFACE;
		}
	};
}

















