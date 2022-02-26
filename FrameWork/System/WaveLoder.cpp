//=============================================================================
//
//  Waveì«Ç›çûÇ› [WaveLoder.cpp]
//  Date   : 2021/11/2
//
//=============================================================================
#include <Windows.h>
#include "waveLoder.h"


HRESULT WaveLoader::FindChunk(HANDLE hFile, DWORD fourcc, DWORD &dwChunkSize, DWORD &dwChunkPosition)
{
	HRESULT hr = S_OK;

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN)) return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;


	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		switch (dwChunkType)
		{
		case FOURCCRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;

		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
				return HRESULT_FROM_WIN32(GetLastError());
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) return S_FALSE;

	}

	return S_OK;
}


HRESULT WaveLoader:: ReadChunk(HANDLE hFile, void* buffer, DWORD bufferSize, DWORD bufferOffset)
{
	HRESULT hr = S_OK;

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferOffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, bufferSize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());

	return hr;
}


HRESULT WaveLoader::ReadRiff(HANDLE hFile, DWORD &dwChunkSize, DWORD &dwChunkPosition)
{
	HRESULT hr = FindChunk(hFile, FOURCCRIFF, dwChunkSize, dwChunkPosition);

	if (FAILED(hr))
	{
		return hr;
	}

	DWORD filetype;
	hr = ReadChunk(hFile, &filetype, sizeof(DWORD), dwChunkPosition);

	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

HRESULT WaveLoader::ReadWfx(HANDLE hFile, WAVEFORMATEX &Wfx, DWORD &dwChunkSize, DWORD &dwChunkPosition)
{
	HRESULT hr = FindChunk(hFile, FOURCCFMT, dwChunkSize, dwChunkPosition);

	if (FAILED(hr))
	{
		return hr;
	}

	hr = ReadChunk(hFile, &Wfx, dwChunkSize, dwChunkPosition);

	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

HRESULT WaveLoader::ReadData(HANDLE hFile, BYTE* &buffer, DWORD &dwChunkSize, DWORD &dwChunkPosition)
{
	HRESULT hr = FindChunk(hFile, FOURCCDATA, dwChunkSize, dwChunkPosition);

	if (FAILED(hr))
	{
		return hr;
	}

	BYTE* pDataBuffer = new BYTE[dwChunkSize];
	hr = ReadChunk(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);
	buffer = pDataBuffer;

	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}