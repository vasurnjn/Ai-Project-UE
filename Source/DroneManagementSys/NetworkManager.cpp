

#include "NetworkManager.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "IPAddress.h"

ANetworkManager::ANetworkManager()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	FTPServer = TEXT("ftp://test.rebex.net/pub/incoming/");
	LocalFileToUpload = TEXT("C:/Users/DSC/Downloads/23BCE2266.txt");
	FTPUsername = TEXT("demo");
	FTPPassword = TEXT("password");

	//SMTPServer = TEXT("smtp://smtp.gmail.com:587");
	//SenderEmail = TEXT("your_email@gmail.com");
	//SenderPassword = TEXT("your_app_password"); 
	//RecipientEmail = TEXT("recipient@example.com");
	//EmailSubject = TEXT("UE SMTP Test Message");
	//EmailBody = TEXT("Hello! This email was sent from Unreal Engine.");

}

void ANetworkManager::BeginPlay()
{
	Super::BeginPlay();

	DNSReolve(HostToResolve);

	HttpGet(HttpUrl);
	
	FTP();
	
}

void ANetworkManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANetworkManager::DNSReolve(const FString& Hostname)
{

	UE_LOG(LogTemp, Warning, TEXT("[DNS] Resolving host %s"),*Hostname);

	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);

	if (!SocketSubsystem) {
		UE_LOG(LogTemp, Error, TEXT("[DNS] No socket subsystem available"));
		return;
	}

	FAddressInfoResult AddrInfo = SocketSubsystem->GetAddressInfo(
		*Hostname,
		nullptr,
		EAddressInfoFlags::Default,
		NAME_None
	);

	if (AddrInfo.ReturnCode != SE_NO_ERROR || AddrInfo.Results.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("[DNS] Failed to resolve %s (code=%d)"), *Hostname, (int32)AddrInfo.ReturnCode);
		return;
	}

	int32 Index = 0;
	for (const FAddressInfoResultData& It : AddrInfo.Results)
	{
		if (It.Address->IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("[DNS] IP #%d: %s"), Index++, *It.Address->ToString(false));
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("[DNS] Total IPs resolved: %d"), Index);
}



void ANetworkManager::HttpGet(const FString& Url)
{
	UE_LOG(LogTemp, Warning, TEXT("[HTTP] GET %s"), *Url);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb(TEXT("GET"));

	Request->OnProcessRequestComplete().BindLambda(
		[](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bOK)
		{
			if (bOK && Res.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("[HTTP] Status: %d"), Res->GetResponseCode());
				UE_LOG(LogTemp, Warning, TEXT("[HTTP] Content-Length: %d"), Res->GetContentLength());
				UE_LOG(LogTemp, Warning, TEXT("[HTTP] Snippet: %s"),
					*Res->GetContentAsString().Left(200)); 
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[HTTP] Request failed"));
			}
		}
	);

	Request->ProcessRequest();
}

void ANetworkManager::FTP()
{

	UE_LOG(LogTemp, Warning, TEXT("[FTP] Uploading file via FTP..."));

	if (!FPaths::FileExists(LocalFileToUpload))
	{
		UE_LOG(LogTemp, Error, TEXT("[FTP] File does not exist: %s"), *LocalFileToUpload);
		return;
	}

	FString CurlExe = TEXT("C:/Windows/System32/curl.exe");

	FString Args = FString::Printf(
		TEXT("-T \"%s\" \"%s\" --user %s:%s"),
		*LocalFileToUpload,
		*FTPServer,
		*FTPUsername,
		*FTPPassword
	);

	int32 ReturnCode = 0;
	FString StdOut, StdErr;

	FPlatformProcess::ExecProcess(*CurlExe, *Args, &ReturnCode, &StdOut, &StdErr);

	UE_LOG(LogTemp, Warning, TEXT("[FTP] Exit Code: %d"), ReturnCode);
	UE_LOG(LogTemp, Warning, TEXT("[FTP] Output: %s"), *StdOut);
	UE_LOG(LogTemp, Warning, TEXT("[FTP] Error: %s"), *StdErr);


}


