#pragma once

#include <Windows.h>

template <class T> class IUnknownInterface : public T {

public:

	IUnknownInterface() : reference_count(0)  {

	}

	virtual ~IUnknownInterface() {
	}

	virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppv_obj) {

		if (!ppv_obj) {

			return E_POINTER;
		}

		if (riid == IID_IUnknown) {

			*ppv_obj = this;
			AddRef();

			return S_OK;
		}
		return E_NOINTERFACE;
	}


	
	virtual unsigned long WINAPI AddRef(void) {

		return reference_count++;
	}

	virtual unsigned long WINAPI Release(void) {

		if (--reference_count == 0) {

			delete this;
		}

		return reference_count;
	}



private:

	//éQè∆Ç≥ÇÍÇƒÇ¢ÇÈêî
	unsigned long reference_count;
	

};


















