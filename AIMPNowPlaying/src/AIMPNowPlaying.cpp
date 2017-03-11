
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

		return Assembly::LoadFile(directory + "\\" + name->Name + ".dll");
	} else {

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




