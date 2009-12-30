#include "stdafx.h"


BOOL uploader::initUpload()
{
	WriteErr("InternetOpen");
	HINTERNET hSession = InternetOpen("HttpSendRequestEx", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	if (!hSession)
	{
		WriteErr("No Internet Session");

		return FALSE;
	}
	WriteErr("InternetConnect");
	HINTERNET hConnect = InternetConnect(hSession, ServerName,    INTERNET_DEFAULT_HTTP_PORT, NULL,
		NULL,     INTERNET_SERVICE_HTTP, NULL,                       NULL);

	if (!hConnect)
	{
		WriteErr("No Internet Session");

		return FALSE;
	}
	else
	{


		if (UseHttpSendReqEx(hConnect, LocalFile))
		{
			WriteErr("Write Completed");
		}

		if (!InternetCloseHandle(hConnect))
		{
			WriteErr("Failed to close Handle (hConnect)");

			return FALSE;
		}
	}

	if (InternetCloseHandle(hSession) == FALSE)
	{
		WriteErr("Failed to close Handle (hSession)");

		return FALSE;
	}

	return TRUE;
}

//------------------------------------------------------------------------
BOOL uploader::UseHttpSendReqEx( HINTERNET hConnect, LPCSTR upFile )
{
	INTERNET_BUFFERS BufferIn = { 0 };

	DWORD dwBytesRead;
	DWORD dwBytesWritten;
	BYTE pBuffer[1024]; // Read from file in 1K chunks
	BOOL bRead, bRet;

	BufferIn.dwStructSize = sizeof(INTERNET_BUFFERS);

	HINTERNET hRequest = HttpOpenRequest(hConnect, "PUT", ServerFileDest, NULL, NULL, NULL, 0, 0);

	if (!hRequest)
	{
		WriteErr("Failed HttpOpenRequest");

		return FALSE;
	}

	HANDLE hFile = CreateFile(upFile,                GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		WriteErr("Failed CreateFile");

		return FALSE;
	}

	BufferIn.dwBufferTotal = GetFileSize(hFile, NULL);

	if (!HttpSendRequestEx(hRequest, &BufferIn, NULL, HSR_INITIATE, 0))
	{
		WriteErr("Failed HttpSendRequestEx");

		return FALSE;
	}

	DWORD sum = 0;

	do
	{
		bRead = ReadFile(hFile, pBuffer, sizeof(pBuffer), &dwBytesRead, NULL);

		if (!bRead)
		{
			WriteErr("break 1");
			break;
		}

		bRet = InternetWriteFile(hRequest, pBuffer, dwBytesRead, &dwBytesWritten);

		if (!bRet)
		{
			char * err;
			sprintf(err,"\nInternetWriteFile failed %lu", GetLastError());
			//printf ("\nInternetWriteFile failed %lu", GetLastError());
			WriteErr(err);
			break;
		}

		sum += dwBytesWritten;
	} while ( dwBytesRead == sizeof(pBuffer) );


	//if (hFile != INVALID_HANDLE_VALUE) {

	char d[50];
	sprintf_s(d, "Actual written bytes: %d\n", sum);
	WriteErr(d);
	CloseHandle(hFile);
	//}

	if (!HttpEndRequest(hRequest, NULL, 0, 0))
	{
		WriteErr("Failed HttpEndRequest");
		return FALSE;
	}

	return TRUE;
}

void uploader::WriteErr( char* errVal )
{
	LogManager::getSingletonPtr()->logMessage(errVal);
	//sprintf_s(ErrString, errVal);
	//OutputDebugString(ErrString);
	//OutputDebugString("\n");
}