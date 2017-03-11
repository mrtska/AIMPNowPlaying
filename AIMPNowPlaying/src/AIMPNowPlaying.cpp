
#include "AIMPNowPlaying.h"
#include <vcclr.h>
#include <msclr/marshal.h>

using namespace System;
using namespace System::Reflection;

AIMPNowPlaying::AIMPNowPlayingPlugin *AIMPNowPlayingInstance;

//
System::Reflection::Assembly ^ OnAssemblyResolve(System::Object ^sender, System::ResolveEventArgs ^args) {


	msclr::interop::marshal_context context;

	Assembly ^current = Assembly::GetExecutingAssembly();

	String ^directory = IO::Path::GetDirectoryName(current->Location);


	AssemblyName ^name = gcnew AssemblyName(args->Name);

	if (name->Name + ".dll" == "AIMPNowPlayingUI.dll") {

		MessageBox(NULL, context.marshal_as<const TCHAR*>(directory + "\\" + name->Name + ".dll"), "UI DLL�����I", MB_OK);
		return Assembly::LoadFile(directory + "\\" + name->Name + ".dll");
	} else {

		MessageBox(NULL, "UL ���Ă炢�� DLL �܂��I", "�肼���", MB_OK);

		for each(System::String ^str in Assembly::LoadFile(directory + "\\" + name->Name + ".dll")->GetManifestResourceNames()) {

			MessageBox(NULL, context.marshal_as<const TCHAR*>(str), "�肼���", MB_OK);
		}


		MessageBox(NULL, "UL ���Ă炢�� DLL ���ƁI", "�肼���", MB_OK);
		return nullptr;
	}
}

HRESULT __declspec(dllexport) WINAPI AIMPPluginGetHeader(IAIMPPlugin **Header) {

	System::AppDomain::CurrentDomain->AssemblyResolve += gcnew System::ResolveEventHandler(&OnAssemblyResolve);

	AIMPNowPlayingInstance = new AIMPNowPlaying::AIMPNowPlayingPlugin;
	AIMPNowPlayingInstance->AddRef();
	*Header = AIMPNowPlayingInstance;

	return S_OK;
}




