#ifndef UPLOAD_H

#define UPLOAD_H


#include "stdafx.h"

#define BUFFSIZE 500

class uploader
{
public:
	BOOL UseHttpSendReqEx( HINTERNET hConnect, LPCSTR upFile );
	BOOL initUpload();
	LPCSTR ServerName;
	LPCSTR ServerFileDest;
	LPCSTR LocalFile;
	CHAR ErrString[MAX_PATH];
	void WriteErr( char* errVal );
protected:
private:
};


struct struct_UPLOAD
{


	
};

//struct_UPLOAD fUpload;
//------------------------------------------------------------------------

//------------------------------------------------------------------------
#endif
