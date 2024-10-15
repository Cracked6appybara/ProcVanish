#pragma once
PVOID CopyMemoryEx(_Inout_ PVOID Destination, _In_ CONST PVOID Source, _In_ SIZE_T Length);
INT StringCompareA(_In_ LPCSTR String1, _In_ LPCSTR String2);
INT StringCompareW(_In_ LPCWSTR String1, _In_ LPCWSTR String2);
VOID IatCamouflage();