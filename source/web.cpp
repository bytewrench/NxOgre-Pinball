/** web.cpp
* Implements classes declared in web.h
* Author: Vijay Mathew Pandyalakal
* Date: 18/10/03
**/


#include "stdafx.h"



string URLEncoder::encode(string str) {
	int len = (int)str.length();
	char* buff = new char[len + 1];
	strcpy(buff,str.c_str());
	string ret = "";
	for(int i=0;i<len;i++) {
		if(isOrdinaryChar(buff[i])) {
			ret = ret + buff[i];
		}else if(buff[i] == ' ') {
			ret = ret + "+";
		}else {
			char tmp[6];
			sprintf(tmp,"%%%x",buff[i]);
			ret = ret + tmp;
		}
	}
	delete[] buff;
	return ret;
}

bool URLEncoder::isOrdinaryChar(char c) {
	char ch = tolower(c);
	if(ch == 'a' || ch == 'b' || ch == 'c' || ch == 'd' || ch == 'e' 
		|| ch == 'f' || ch == 'g' || ch == 'h' || ch == 'i' || ch == 'j' 
		|| ch == 'k' || ch == 'l' || ch == 'm' || ch == 'n' || ch == 'o' 
		|| ch == 'p' || ch == 'q' || ch == 'r' || ch == 's' || ch == 't' 
		|| ch == 'u' || ch == 'v' || ch == 'w' || ch == 'x' || ch == 'y' 
		|| ch == 'z' || ch == '0' || ch == '1' || ch == '2' || ch == '3' 
		|| ch == '4' || ch == '5' || ch == '6' || ch == '7' || ch == '8' 
		|| ch == '9') {
			return true;
	}
	return false;
}

string URLDecoder::decode(string str) {
	int len = (int)str.length();
	char* buff = new char[len + 1];
	strcpy(buff,str.c_str());
	string ret = "";
	for(int i=0;i<len;i++) {
		if(buff[i] == '+') {
			ret = ret + " ";
		}else if(buff[i] == '%') {
			char tmp[4];
			char hex[4];			
			hex[0] = buff[++i];
			hex[1] = buff[++i];
			hex[2] = '\0';		
			//int hex_i = atoi(hex);
			sprintf(tmp,"%c",convertToDec(hex));
			ret = ret + tmp;
		}else {
			ret = ret + buff[i];
		}
	}
	delete[] buff;
	return ret;
}

int URLDecoder::convertToDec(const char* hex) {
	char buff[12];
	sprintf(buff,"%s",hex);
	int ret = 0;
	int len = (int)strlen(buff);
	for(int i=0;i<len;i++) {
		char tmp[4];
		tmp[0] = buff[i];
		tmp[1] = '\0';
		getAsDec(tmp);
		int tmp_i = atoi(tmp);
		int rs = 1;
		for(int j=i;j<(len-1);j++) {
			rs *= 16;
		}
		ret += (rs * tmp_i);
	}
	return ret;
}

void URLDecoder::getAsDec(char* hex) {
	char tmp = tolower(hex[0]);
	if(tmp == 'a') {
		strcpy(hex,"10");
	}else if(tmp == 'b') {
		strcpy(hex,"11");
	}else if(tmp == 'c') {
		strcpy(hex,"12");
	}else if(tmp == 'd') {
		strcpy(hex,"13");
	}else if(tmp == 'e') {
		strcpy(hex,"14");
	}else if(tmp == 'f') {
		strcpy(hex,"15");
	}else if(tmp == 'g') {
		strcpy(hex,"16");
	}
}

WebForm::WebForm() {
	string m_strContentType = "Content-Type: application/x-www-form-urlencoded";
	string m_strHostName = "localhost:8080";
	string m_strScriptFile = "";
	HINTERNET m_hSession = NULL;
	HINTERNET m_hConnect = NULL;
	HINTERNET m_hRequest = NULL;
	HINTERNET m_hResponse = NULL;
}
void WebForm::putVariable(const char* var_name,const char* value) {
	if(isDuplicateVar(var_name)) {
		string str = "Duplicate variable name - ";
		str = str + var_name;
		throw WebFormException(str.c_str());
	}else {
		m_vctVars.push_back(var_name);
		m_vctVals.push_back(value);
	}
}

string WebForm::getVariable(const char* var_name) const {
	for(int i=0;i<m_vctVars.size();i++) {
		if(strcmpi(var_name,m_vctVars[i].c_str()) == 0) {
			return m_vctVals[i];
		}
	}
	string str = "Variable not found - ";
	str = str + var_name;
	throw WebFormException(str.c_str());
}

void WebForm::setHost(const char* host) {
	m_strHostName = host;
}

void WebForm::setScriptFile(const char* sf) {
	m_strScriptFile = sf;
}

string WebForm::getHost() const {
	return m_strHostName;
}

string WebForm::getScriptFile() const {
	return m_strScriptFile;
}

bool WebForm::sendRequest() {
	string host = m_strHostName;
	string form_action = host + m_strScriptFile + "?";
	int sz = (int)m_vctVars.size();
	for(int i=0;i<sz;i++) {
		string var = m_vctVars[i];
		string enc_var = URLEncoder::encode(var);
		string val = m_vctVals[i];
		string enc_val = URLEncoder::encode(val);
		form_action = form_action + enc_var;
		form_action = form_action + "=";
		form_action = form_action + enc_val;
		if(i != (sz-1)) {
			form_action = form_action + "&";
		}
	}   
	
	m_hSession = InternetOpen("WebForm 1",
		PRE_CONFIG_INTERNET_ACCESS,
		NULL,
		INTERNET_INVALID_PORT_NUMBER,
		0);
	if(m_hSession == NULL) {
		return false;
	}
	OutputDebugStr("\n");
	OutputDebugStr(form_action.c_str());
	OutputDebugStr("\n");

	m_hRequest = InternetOpenUrl(m_hSession,
		form_action.c_str(),
		NULL,
		0,
		INTERNET_FLAG_RELOAD,
		0);
	
	if(m_hRequest == NULL) {
		return false;
	}
	return true;
}

bool WebForm::getResponse(char* buff,int sz) {
	
	if(m_hRequest == NULL) {
		return false;
	}	
	if(InternetReadFile(m_hRequest,buff,sz,&m_lBytesRead)) {
		if((int)m_lBytesRead > sz) {
			return false;
		}
		buff[m_lBytesRead] = 0;
		return true;
	}
	
	return false;
}

bool WebForm::isDuplicateVar(const char* var_name) {
	for(int i=0;i<(int)m_vctVars.size();i++) {
		if(strcmpi(var_name,m_vctVars[i].c_str()) == 0) {
			return true;
		}
	}
	return false;
}

WebForm::~WebForm() {
	if(m_hSession != NULL) InternetCloseHandle(m_hSession);
	if(m_hRequest != NULL) InternetCloseHandle(m_hRequest);	
};


//==-----------------------------------------------------------------------------


webManager::webManager(){

}
webManager * webManager::getWebmanager(){
	static webManager * webman;
	if (webman == 0)
	{
		webman = new webManager();
	}
	return webman;
}
//==------------------------------------------------------------
string  webManager::postScore(int score, string initials, string version){
	char tmp[50];
	WebForm wf;
	
	wf.setHost(URL_HOST);
	wf.setScriptFile("/getdata.asp");
	wf.putVariable("NAME", "VALUE");

	
	char response[MAX_PATH];
	
	if ( wf.getResponse(response, MAX_PATH - 1) )
	{
		string retVal(response);
		return retVal;
	}
	
	string retVal("ERROR");
	return retVal;

}
//==------------------------------------------------------------
string  webManager::getStandings(string version){
	WebForm wf;
	
	wf.setHost(URL_HOST);
	wf.setScriptFile("/yourwebpagegoeshere.extension");
	wf.putVariable("NAME", "VALUE");
	
	if (!wf.sendRequest())
	{
		return "ERROR";
	}
	
	char response[MAX_PATH];
	if ( wf.getResponse(response, MAX_PATH - 1) ){
		string retVal(response);
		return retVal;
	}
	
	string retVal("ERROR");
	return retVal;

}
//==------------------------------------------------------------

void webManager::testInet(){
	WebForm wf;

	wf.setHost(URL_HOST);
	wf.setScriptFile("/yourwebpagegoeshere.extension");
	wf.putVariable("NAME", "VALUE");

	if (!wf.sendRequest())
	{
		return;
	}
	char response[MAX_PATH];
	if ( wf.getResponse(response, MAX_PATH - 1) ){
		string retVal(response);
		return ;
	}
}