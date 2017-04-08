#include "stdafx.h"
#include "tcCurrentWindowInfoGetter.hpp"

#include <oleacc.h>
#include <atlbase.h>
#undef UNICODE
#include <Psapi.h>

#include <iostream>
#include <string>
#include <vector>

namespace
{

std::wstring GetName(IAccessible *pAcc)
{
	CComBSTR bstrName;
	if (!pAcc || FAILED(pAcc->get_accValue(CComVariant((int)CHILDID_SELF), &bstrName)) || !bstrName.m_str)
		return L"";
	return bstrName.m_str;
}

HRESULT WalkTreeWithAccessibleChildren(CComPtr<IAccessible> pAcc, std::wstring& resUrl)
{
	long childCount = 0;
	long returnCount = 0;
	HRESULT hr = pAcc->get_accChildCount(&childCount);
	if (childCount == 0)
		return S_OK;

	CComVariant* pArray = new CComVariant[childCount];
	hr = ::AccessibleChildren(pAcc, 0L, childCount, pArray, &returnCount);
	if (FAILED(hr))
		return hr;
	const std::vector<std::wstring> domens =
	{ L".ru", L".com", L".org", L".ua" , L".info" };
	for (int i = 0; i < returnCount; ++i)
	{
		CComVariant vtChild = pArray[i];
		if (vtChild.vt != VT_DISPATCH)
			continue;

		CComQIPtr<IAccessible> pAccChild = vtChild.pdispVal;
		if (!pAccChild)
			continue;

		const std::wstring name = GetName(pAccChild).data();
		size_t idx = 0;
		while (name.find(domens[idx]) == -1 && idx < domens.size()) { ++idx; }
		if (idx != domens.size())
			resUrl = name;
		else if (WalkTreeWithAccessibleChildren(pAccChild, resUrl) == S_FALSE)
			return S_FALSE;
	}
	delete[] pArray;
	return S_OK;
}

}  // namespace

namespace tcCurrentWindowInfoGetter
{
bool IsBrowserWindow(HWND windowHendler)
{
	wchar_t className[100];
	GetClassName(windowHendler, className, 100);
	return (wcscmp(className, L"Chrome_WidgetWin_1") == 0);
}

std::wstring GetCurrentUrl(HWND windowHendler)
{
	IAccessible* ac = NULL;
	long childCount = 0;
	long returnCount = 0;
	HRESULT hr2 = AccessibleObjectFromWindow(windowHendler, OBJID_CLIENT, IID_IAccessible, (void **)&ac);
	std::wstring res;
	if (WalkTreeWithAccessibleChildren(ac, res) != S_OK)
		return L"unrecognized url";
	return res;
}

std::wstring GetNormalizedUrl(HWND windowHendler)
{
	std::wstring res = GetCurrentUrl(windowHendler);
	size_t found = res.find(L"://");
	if (found != std::wstring::npos)
		res = res.substr(found + 3, res.size());
	found = res.find(L"/");
	if (found != std::wstring::npos)
		return res.substr(0, found);
	return res;
}

std::wstring GetCurrentApplicationName(HWND windowHendler)
{
	DWORD dwPID;
	GetWindowThreadProcessId(windowHendler, &dwPID);
	HANDLE handle = OpenProcess(
		PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
		FALSE,
		dwPID
	);
	if(!handle)
		return L"undefined";
	wchar_t buffer[MAX_PATH];
	if (GetModuleFileNameExW(handle, 0, buffer, MAX_PATH))
	{
		CloseHandle(handle);
		return std::wstring(buffer);
	}
	return L"undefined";
}

std::wstring GetNormalizedCurrentApplicationName(HWND windowHendler)
{
	std::wstring res = GetCurrentApplicationName(windowHendler);
	size_t last = res.find_last_of(L"\\");
	if(last != std::wstring::npos)
		return res.substr(last + 1, res.size());
	return res;
}
}
