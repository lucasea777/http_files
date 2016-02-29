//http://forums.codeguru.com/showthread.php?406873-Download-a-file-using-http-amp-winsock
#include <string>
#include <stdio.h>
#include <winsock2.h>
#define BUFFER_LEN (4096)
using std::string;
int main(int argc, char **argv)
{
    HANDLE fhand;
    string request;
    int sendret;
    int iRecv;
    int iResponseLength=0;
    int offset;
    DWORD dw;
    string res2;
    char recvBuffer[BUFFER_LEN]={0};
    string response;
    const char lb[]="\r\n\r\n";
    const char http[]="http\x3a//";
    const char snsn[]="%s\n";
    bool error1=false;
    bool error2=false;
    bool error3=false;
    //int len3=strlen(argv[3]);
    printf(snsn,"\n-=[  httpget v1.0 by Avery Tarasov");
    //printf(snsn,"-=[  Email: c0ldshadow@deeptide.com");
    //printf(snsn,"-=[  Web: www.DeepTide.com");
    //printf(snsn,"-=[  Dedicated to my fiance, Ostine!\n");
    printf(snsn,"Example usage: httpget theserver.com /somefolder/somefile.zip C:\\savehere.zip");
    if(argc!=4) {
        printf(snsn,"\nInvalid usage");
        goto cleanup;
    }
    WSADATA wsaData; //(structure to hold "winsocket implementation info") https://msdn.microsoft.com/en-us/library/windows/desktop/ms741563(v=vs.85).aspx
    if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0) {//The WSAStartup function initiates use of the Winsock DLL by a process.
        printf(snsn,"\nError initializing Winsock 2.2");
        goto cleanup;
    }
    error1=true;
    if(LOBYTE(wsaData.wVersion)!=2||HIBYTE(wsaData.wVersion)!=2)
    {
        printf(snsn,"\nWinsock 2.2 not available");
        goto cleanup;
    }
    printf(snsn,"\nWinsock 2.2 initialized via wsa2_32.dll");
    struct hostent *h;
    struct sockaddr_in sa;
    SOCKET server1;
    h=gethostbyname(argv[1]);
    if(h==0)
    {
        printf(snsn,"\ngethostbyname() failed");
        goto cleanup;
    }
    printf("%s","\nHost lookup succeeded for ");
    printf(snsn,argv[1]);
    memcpy((char *)&sa.sin_addr,(char *)h->h_addr,sizeof(sa.sin_addr));
    sa.sin_family=h->h_addrtype;
    sa.sin_port=htons(80);
    server1=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(server1==INVALID_SOCKET)
    {
        printf(snsn,"\nsocket() failed");
        goto cleanup;
    }
    error1=false;
    error2=true;
    if(connect(server1,(struct sockaddr *)&sa,sizeof(sa))<0)
    {
        printf(snsn,"\nconnect() failed");
        goto cleanup;
    }
    printf("%s","\nNow connected to ");
    printf("%s",argv[1]);
    printf(snsn," via port 80");
    request+="GET ";
    request+=argv[2];
    request+=" HTTP/1.0";
    request+=&lb[2];
    request+="Host: ";
    request+=argv[1];
    request+=lb;
    printf(snsn,"\nHTTP request constructed successfully:\n");
    printf(snsn,request.c_str());
    sendret=send(server1,request.c_str(),request.length(),0);
    if(sendret==-1)
    {
        printf(snsn,"send() failed");
        goto cleanup;
    }
    printf(snsn,"Successfully sent HTTP request to the server");
    printf(snsn,"\nWaiting for download to complete");
	fhand=CreateFile(argv[3],GENERIC_WRITE,FILE_SHARE_READ,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
	if(fhand==INVALID_HANDLE_VALUE)
	{
        printf(snsn,"\nCreateFile() failed");
        goto cleanup;
	}
    error2=false;
    error3=true;
    while((iRecv=recv(server1,recvBuffer,BUFFER_LEN-1,0))>0)
    {
        /*
        char hex[5];
        string packet;
        packet.reserve(5*iRecv);
        printf(snsn,"\n");
        printf("%s","Receiving ");
        printf("%d",iRecv);
        printf(snsn," byte packet:\n");
        for(int i=0;i<iRecv;++i)
        {
            wsprintf(hex,"%02x",(unsigned char)recvBuffer[i]);
            packet.append(hex);
            printf("%s ",hex);
        }
        */
        response.append(recvBuffer,iRecv);
        iResponseLength+=iRecv;
        ZeroMemory(recvBuffer,BUFFER_LEN);
    }
    if(iRecv==SOCKET_ERROR)
    {
        printf(snsn,"\n\nrecv() failed");
    }
    offset=response.find(lb)+4;
    if(offset!=string::npos)
    {
        printf("%s","\n\nFile starts at offset ");
        printf("%d\n",offset);
        printf(snsn,"\nInitial response from server:\n");
        for(int j=0;j<offset;++j)
        {
            printf("%c",response[j]);
        }
        res2.assign(response,offset,response.size());
        if(WriteFile(fhand,res2.data(),res2.size(),&dw,0)==0)
        {
            printf(snsn,"\nWriteFile() failed");
            goto cleanup;
        }
        else
        {
            printf("%s","\nFile successfully downloaded and saved to ");
            printf(snsn,argv[3]);
        }
    }
    cleanup:
    if(error1 || error2 || error3)
        WSACleanup();
    if(error2 || error3)
        closesocket(server1);
    if(error3)
        CloseHandle(fhand);
    return 0;
}
